package org.echtzeitsysteme.prosem.wireless_sniffer.model.filters;

import java.awt.Color;

import org.echtzeitsysteme.prosem.messages.Message;
import org.echtzeitsysteme.prosem.wireless_sniffer.view.TableColors;

public class Filter {
	public enum EDirection {
		IGNORE,
		IN,
		OUT
	};

	private FilterQueue queue;

	private String filterName;
	private boolean isEnabled = false;

	private int sourceId = -1;
	private int destinationId = -1;
	private int messageType = -1;
	private int priority = -1;
	private int dataLength = -1;
	private EDirection direction = EDirection.IGNORE;
	private Class<? extends Message> messageClass = null;

	private boolean printAsHex = true;
	private Color color = TableColors.getInstance().getBaseColors()
			.firstElement();

	Filter(FilterQueue filterQueue) {
		this.queue = filterQueue;
		this.filterName = "Neuer Filter";
	}

	@Override
	public String toString() {
		return filterName;
	}

	public void delete() {
		queue.remove(this);
	}

	public void moveUp() {
		queue.moveUp(this);
	}

	public void moveDown() {
		queue.moveDown(this);
	}

	public boolean matchesMessage(Message msg) {
		if ((sourceId >= 0) && (sourceId != msg.getSourceId()))
			return false;
		if ((destinationId >= 0) && (destinationId != msg.getDestinationId()))
			return false;
		if ((messageType >= 0) && (messageType != msg.getMessageType()))
			return false;
		if ((priority >= 0) && (priority != msg.getPriority()))
			return false;
		if ((dataLength >= 0) && (dataLength != msg.getData().length))
			return false;
		if (direction != EDirection.IGNORE) {
			if ((direction == EDirection.IN) && (msg.isDirectionOut()))
				return false;
			if ((direction == EDirection.OUT) && (!msg.isDirectionOut()))
				return false;
		}
		if ((messageClass != null) && (messageClass != msg.getClass()))
			return false;

		return true;
	}

	// ################### Getter and Setter #########################

	public String getFilterName() {
		return filterName;
	}

	public void setFilterName(String filterName) {
		this.filterName = filterName;
		queue.notifyUpdated();
	}

	public Color getColor() {
		return color;
	}

	public void setColor(Color color) {
		this.color = color;
		queue.notifyUpdated();
	}

	public boolean isEnabled() {
		return isEnabled;
	}

	public void setEnabled(boolean bool) {
		isEnabled = bool;
		queue.notifyUpdated();
	}

	public int getDestinationId() {
		return destinationId;
	}

	public void setDestinationId(int destinationId) {
		this.destinationId = destinationId;
		queue.notifyUpdated();
	}

	public int getDataLength() {
		return dataLength;
	}

	public void setDataLength(int dataLength) {
		this.dataLength = dataLength;
		queue.notifyUpdated();
	}

	public int getMessageType() {
		return messageType;
	}

	public void setMessageType(int messageType) {
		this.messageType = messageType;
		queue.notifyUpdated();
	}

	public int getPriority() {
		return priority;
	}

	public void setPriority(int priority) {
		this.priority = priority;
		queue.notifyUpdated();
	}

	public int getSourceId() {
		return sourceId;
	}

	public void setSourceId(int sourceId) {
		this.sourceId = sourceId;
		queue.notifyUpdated();
	}

	/**
	 * @return the direction
	 */
	public EDirection getDirection() {
		return direction;
	}

	/**
	 * @param direction
	 *            the direction to set
	 */
	public void setDirection(EDirection direction) {
		this.direction = direction;
		queue.notifyUpdated();
	}

	/**
	 * @return the messageClass
	 */
	public Class<? extends Message> getMessageClass() {
		return messageClass;
	}

	/**
	 * @param messageClass
	 *            the messageClass to set
	 */
	public void setMessageClass(Class<? extends Message> messageClass) {
		this.messageClass = messageClass;
		queue.notifyUpdated();
	}

	public void setPrintAsHex(boolean asHex) {
		this.printAsHex = asHex;
		queue.notifyUpdated();
	}

	public boolean getPrintAsHex() {
		return this.printAsHex;
	}

}
