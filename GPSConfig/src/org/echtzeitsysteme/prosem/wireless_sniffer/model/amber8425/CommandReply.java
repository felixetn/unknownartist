package org.echtzeitsysteme.prosem.wireless_sniffer.model.amber8425;

import java.util.Date;

public class CommandReply {
	int[] msgDataInt;
	int replyCode;

	Date date;

	public CommandReply(int[] rawData) {
		this.date = new Date();

		msgDataInt = new int[rawData.length - 4];

		this.replyCode = rawData[1];
		System.arraycopy(rawData, 3, msgDataInt, 0, msgDataInt.length);
	}
	
	public int getReplyCode() {
		return replyCode;
	}
	
	public int[] getReplyData() {
		return msgDataInt;
	}
	
	public Date getDate() {
		return date;
	}

}
