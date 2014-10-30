package org.echtzeitsysteme.prosem.wireless_sniffer.model.amber8425;

import java.util.LinkedList;

import org.echtzeitsysteme.prosem.wireless_sniffer.model.amber8425.commands.Command;

class ReceiveStateMachine {
	private enum EState {
		IDLE,
		OPCODE,
		MESSAGELENGTH,
		DATA,
		CS
	};

	private EState state = EState.IDLE;
	private LinkedList<Integer> cmdData = new LinkedList<Integer>();
	private int dataLength;
	private int received;

	private LinkedList<Integer[]> commands = new LinkedList<Integer[]>();

	private void resetMachine() {
		state = EState.IDLE;
		received = 0;
		cmdData.clear();
	}

	public boolean isIdle() {
		return state == EState.IDLE;
	}

	boolean bytesReceived(byte[] rec) {
		boolean messageDone = false;
		for (byte bb : rec) {
			int b = bb & 0xff;
			switch (state) {
				case IDLE:
					if (b == Command.START_BYTE)
						state = EState.OPCODE;
					break;
				case OPCODE:
					state = EState.MESSAGELENGTH;
					break;
				case MESSAGELENGTH:
					dataLength = b;
					if (dataLength > 0)
						state = EState.DATA;
					else
						state = EState.CS;
					break;
				case DATA:
					received++;
					if (received >= dataLength) {
						state = EState.CS;
					}
					break;
				case CS:
					cmdData.add(b);
					commands.add(cmdData.toArray(new Integer[0]));
					resetMachine();
					messageDone = true;
					break;
				default:
					break;
			}
			if (state != EState.IDLE)
				cmdData.add(b);
		}
		return messageDone;
	}

	public Command getCommand() {
		if (commands.size() == 0)
			return null;

		Integer[] obj = commands.removeFirst();
		int[] basic = new int[obj.length];
		for (int i = 0; i < obj.length; i++)
			basic[i] = obj[i];
		
		return Command.parseCommand(basic);
	}
}
