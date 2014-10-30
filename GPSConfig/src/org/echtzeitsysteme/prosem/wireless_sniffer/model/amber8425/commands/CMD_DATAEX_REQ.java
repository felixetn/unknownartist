package org.echtzeitsysteme.prosem.wireless_sniffer.model.amber8425.commands;

public class CMD_DATAEX_REQ extends Command {
	private final static int COMMAND = 0x01;

	private int channel;
	private int destAddr;

	protected CMD_DATAEX_REQ(boolean out, int channel, int destAddr) {
		super(out, COMMAND);
		this.channel = channel;
		this.destAddr = destAddr;
	}

	protected void setData(int[] data) {
		int[] packed = new int[data.length + 2];
		packed[0] = channel & 0xff;
		packed[1] = destAddr & 0xff;
		System.arraycopy(data, 0, packed, 2, data.length);
		super.setData(packed);
	}
}
