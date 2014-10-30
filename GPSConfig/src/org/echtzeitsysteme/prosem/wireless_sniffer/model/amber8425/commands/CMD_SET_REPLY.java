package org.echtzeitsysteme.prosem.wireless_sniffer.model.amber8425.commands;


public class CMD_SET_REPLY extends Command {
	private final static int COMMAND = 0x49;

	private boolean success;

	protected CMD_SET_REPLY(boolean success) {
		super(false, COMMAND);
		this.success = success;
		setData(new int[0]);
	}

	@Override
	protected void setData(int[] data) {
		int[] payload = new int[1];
		payload[0] = (success ? 0 : 1);
		super.setData(payload);
	}

	public static CMD_SET_REPLY parseRaw(int commandId, int[] payload) {
		int successInt = payload[0];
		return new CMD_SET_REPLY(successInt == 0 ? true : false);
	}

	public boolean isSuccess() {
		return success;
	}

	@Override
	public void printCommand() {
		System.out.println();
		System.out.println("CMD_SET_REPLY:");
		System.out.println("  Success: " + success);
		System.out.println();
		super.printCommand();
	}
}
