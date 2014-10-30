package org.echtzeitsysteme.prosem.wireless_sniffer.model.amber8425.commands;

import org.echtzeitsysteme.prosem.wireless_sniffer.model.amber8425.ENonVolatileAddresses;

public class CMD_SET_REQ extends Command {
	private final static int COMMAND = 0x09;

	private ENonVolatileAddresses address;
	private int[] values;

	protected CMD_SET_REQ(boolean out, ENonVolatileAddresses address,
			int[] values) {
		super(out, COMMAND);
		this.address = address;
		this.values = values;
		setData(values);
	}

	@Override
	protected void setData(int[] data) {
		int[] payload = new int[data.length + 2];
		payload[0] = address.getAddress();
		payload[1] = data.length;
		System.arraycopy(data, 0, payload, 2, data.length);
		super.setData(payload);
	}

	public static CMD_SET_REQ parseRaw(int commandId, int[] payload) {
		int address = payload[0];
		int[] data = new int[payload.length - 2];
		System.arraycopy(payload, 2, data, 0, data.length);
		return new CMD_SET_REQ(false, ENonVolatileAddresses.getItem(address),
				data);
	}

	public static CMD_SET_REQ getSetSourceIdCommand(int id) {
		return new CMD_SET_REQ(true, ENonVolatileAddresses.MAC_SourceAddrLSB,
				new int[] { id });
	}

	public static CMD_SET_REQ getSetChannelCommand(int channel) {
		return new CMD_SET_REQ(true, ENonVolatileAddresses.PHY_DefaultChannel,
				new int[] { channel });
	}

	public ENonVolatileAddresses getAddress() {
		return address;
	}

	public int[] getValues() {
		return values;
	}

	@Override
	public void printCommand() {
		System.out.println();
		System.out.println("CMD_SET_REQ / GET_REPLY:");
		System.out.print("  Data:   ");
		for (int i : values) {
			System.out.print(String.format("%02x:", i));
		}
		System.out.println();
		super.printCommand();
	}
}
