package org.echtzeitsysteme.prosem.wireless_sniffer.model;

import org.echtzeitsysteme.prosem.messages.Message;

public interface ModelChangeListener {
	public void messageAdded(Model src, Message msg, int index);

	public void messageUpdated(Model src, Message msg, int index);

	public void messagesCleared(Model src);
}
