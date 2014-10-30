package org.echtzeitsysteme.prosem.wireless_sniffer.model.amber8425.commands;

public class CMD_DATA_REPLY extends Command {
	private final static int COMMAND = 0x40;

	private int ackState;

	protected CMD_DATA_REPLY(int ackState) {
		super(false, COMMAND);
		this.ackState = ackState;
		setData(new int[0]);
	}

	@Override
	protected void setData(int[] data) {
		int[] payload = new int[1];
		payload[0] = ackState;
		super.setData(payload);
	}

	public static CMD_DATA_REPLY parseRaw(int commandId, int[] payload) {
		return new CMD_DATA_REPLY(payload[0]);
	}

	public boolean hasAck() {
		return (ackState == 0 ? true : false);
	}

	@Override
	public void printCommand() {
		System.out.println();
		System.out.println("CMD_DATA_REPLY:");
		System.out.println("  Ack:     " + (1 - ackState));
		System.out.println();
		super.printCommand();
	}
}
