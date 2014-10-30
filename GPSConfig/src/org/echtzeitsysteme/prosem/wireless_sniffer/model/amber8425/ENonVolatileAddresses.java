package org.echtzeitsysteme.prosem.wireless_sniffer.model.amber8425;


public enum ENonVolatileAddresses {
	MAC_SourceAddrLSB(29),
	PHY_DefaultChannel(42);

	private int address;

	private ENonVolatileAddresses(int address) {
		this.address = address;
	}

	public int getAddress() {
		return address;
	}

	public static ENonVolatileAddresses getItem(int id) {
		for (ENonVolatileAddresses t : ENonVolatileAddresses.values())
			if (t.getAddress() == id)
				return t;
		return null;
	}
}
