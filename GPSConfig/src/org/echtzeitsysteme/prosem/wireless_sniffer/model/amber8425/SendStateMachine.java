package org.echtzeitsysteme.prosem.wireless_sniffer.model.amber8425;

import java.io.IOException;
import java.util.Date;
import java.util.concurrent.ConcurrentLinkedQueue;

import org.echtzeitsysteme.prosem.wireless_sniffer.model.amber8425.commands.Command;

class SendStateMachine implements Runnable {
	private ConcurrentLinkedQueue<Command> sendQueue = new ConcurrentLinkedQueue<Command>();
	private Command commandAwaitingReply = null;
	private long waitingSince = -1;

	private Amber8425 parent;
	private ReceiveStateMachine recvSM;

	private Thread t = null;
	private boolean run = true;

	public SendStateMachine(Amber8425 parent, ReceiveStateMachine recvSM) {
		this.parent = parent;
		this.recvSM = recvSM;
		t = new Thread(this);
		t.start();
	}

	synchronized void addCommandToQueue(Command msg) {
		sendQueue.add(msg);
	}

	public void stop() {
		if (t != null) {
			run = false;
			try {
				t.join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			t = null;
		}
	}

	void messageReceived() {
		// TODO: check if we were waiting for a reply: resend!
	}

	Command replyReceived(Command repCmd) {
		Command cmd = getWaitingCommand();
		cmd.setReply(repCmd);
		parent.notifyListenersMessageUpdate(cmd);
		return cmd;
	}

	public synchronized Command getWaitingCommand() {
		Command msg = commandAwaitingReply;
		commandAwaitingReply = null;
		return msg;
	}

	@Override
	public void run() {
		try {
			while (run) {
				if (commandAwaitingReply != null) {
					long now = new Date().getTime();
					if ((now - waitingSince) > 250) {
						replyReceived(null);
					}
				} else {
					if (parent.isConnected() && recvSM.isIdle()
							&& sendQueue.size() > 0) {
						commandAwaitingReply = sendQueue.poll();
						waitingSince = new Date().getTime();
						sendCommand(commandAwaitingReply);
					}
				}
				Thread.sleep(50);
			}
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}

	private void sendCommand(Command cmd) {
		byte[] cmdData = new byte[cmd.getCommandData().length];
		for (int i = 0; i < cmdData.length; i++) {
			cmdData[i] = (byte) (cmd.getCommandData()[i] & 0xff);
		}

		try {
			if (parent.getOutStream() != null) {
				System.out.print("Sending: ");
				for (byte b : cmdData)
					System.out.format("%02x ", b);
				System.out.println();
				parent.getOutStream().write(cmdData);
			} else
				System.err.println("outStream not initialized");
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

}
