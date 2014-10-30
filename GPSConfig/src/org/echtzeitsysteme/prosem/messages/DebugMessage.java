package org.echtzeitsysteme.prosem.messages;

import org.echtzeitsysteme.prosem.Util;

public class DebugMessage extends Message {
	private final static int MSG_TYPE = 0;

	private String message = null;

	public DebugMessage(boolean out, int channel, int destId, int srcId,
			int priority, int feldstaerke, String message) {
		super(out, channel, destId, srcId, MSG_TYPE, priority, feldstaerke,
				new int[0]);
		setData(message);
	}

	protected void setData(String msg) {
		this.message = msg;
		int[] data = Util.byteArrayToIntArray(msg.getBytes());
		super.setData(data);
	}

	public static Message parseRaw(int srcId, int feldstaerke, int destId,
			int priority, int[] data) {
		StringBuffer dataASCII = new StringBuffer(data.length);
		for (int d : data) {
			dataASCII.append((char) d);
		}

		DebugMessage debMsg = new DebugMessage(false, 0, destId, srcId,
				priority, feldstaerke, dataASCII.toString());
		return debMsg;
	};

	@Override
	public void printCommand() {
		super.printCommand();
		System.out.println("DebugMessage:");
		System.out.println("  Message: " + message);
		System.out.println();
	}
}
