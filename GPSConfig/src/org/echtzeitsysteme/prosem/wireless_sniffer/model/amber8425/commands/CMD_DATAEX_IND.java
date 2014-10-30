package org.echtzeitsysteme.prosem.wireless_sniffer.model.amber8425.commands;

import org.echtzeitsysteme.prosem.messages.Message;

public class CMD_DATAEX_IND extends Command {
	private final static int COMMAND = 0x81;

	protected CMD_DATAEX_IND() {
		super(false, COMMAND);
	}

	public static Message parseRaw(int commandId, int[] payload) {
		int srcId = payload[0];
		int feldstaerke = payload[payload.length-1];
		int[] data = new int[payload.length-2];
		System.arraycopy(payload, 1, data, 0, data.length);
		return Message.parseRaw(srcId, feldstaerke, data);
	}

}
