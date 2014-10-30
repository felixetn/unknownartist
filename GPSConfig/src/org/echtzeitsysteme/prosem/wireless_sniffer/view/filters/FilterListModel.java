package org.echtzeitsysteme.prosem.wireless_sniffer.view.filters;

import javax.swing.AbstractListModel;

import org.echtzeitsysteme.prosem.wireless_sniffer.model.filters.FilterQueue;

public class FilterListModel extends AbstractListModel {
	private static final long serialVersionUID = 5055550928781967882L;

	private FilterQueue queue;

	public FilterListModel(FilterQueue queue) {
		this.queue = queue;
	}

	@Override
	public Object getElementAt(int ind) {
		if (ind >= getSize())
			return null;
		return queue.get(ind);
	}

	@Override
	public int getSize() {
		return queue.getQueueSize();
	}

	public void updateData() {
		fireContentsChanged(this, 0, getSize());
	}

}
