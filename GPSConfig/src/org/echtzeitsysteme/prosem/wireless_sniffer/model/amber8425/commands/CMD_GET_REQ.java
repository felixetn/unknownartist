package org.echtzeitsysteme.prosem.wireless_sniffer.model.amber8425.commands;

import org.echtzeitsysteme.prosem.wireless_sniffer.model.amber8425.ENonVolatileAddresses;


public class CMD_GET_REQ extends Command {
	private final static int COMMAND = 0x0A;

	private ENonVolatileAddresses address;
	private int numBytes;

	protected CMD_GET_REQ(boolean out, ENonVolatileAddresses address, int numBytes) {
		super(out, COMMAND);
		setData(address, numBytes);
	}

	protected void setData(ENonVolatileAddresses address, int numBytes) {
		this.address = address;
		this.numBytes = numBytes;
		int[] pkg = new int[2];
		pkg[0] = address.getAddress() & 0xff;
		pkg[1] = numBytes & 0xff;
		super.setData(pkg);
	}

	public static CMD_GET_REQ getReadSourceIdCommand() {
		return new CMD_GET_REQ(true, ENonVolatileAddresses.MAC_SourceAddrLSB, 1);
	}
	
	public static CMD_GET_REQ getReadChannelCommand() {
		return new CMD_GET_REQ(true, ENonVolatileAddresses.PHY_DefaultChannel, 1);
	}
	
	public ENonVolatileAddresses getAddress() {
		return address;
	}
	
	public int getNumBytes() {
		return numBytes;
	}
}
