package org.echtzeitsysteme.prosem.wireless_sniffer.model.amber8425;

import org.echtzeitsysteme.prosem.wireless_sniffer.model.amber8425.commands.Command;

public interface CommandListener {
	public void messageReceived(Amber8425 src, Command cmd);
	public void messageUpdated(Amber8425 src, Command cmd);
}
