package org.echtzeitsysteme.prosem.wireless_sniffer.view.messagetable;

import javax.swing.RowFilter;

import org.echtzeitsysteme.prosem.messages.Message;
import org.echtzeitsysteme.prosem.wireless_sniffer.model.filters.Filter;

public class PackageTableFilter extends RowFilter<PackageTableModel, Integer> {

	private boolean showAll = true;
	
	private PackageTableModel model;
	
	public PackageTableFilter(PackageTableModel model, boolean showAll) {
		this.model = model;
		this.showAll = showAll;
	}

	@Override
	public boolean include(
			javax.swing.RowFilter.Entry<? extends PackageTableModel, ? extends Integer> entry) {
		Message msg = (Message) entry.getModel().getValueAt(
				entry.getIdentifier(), -1);

		if (showAll)
			return true;

		Filter f = model.getModel().getFilterQueue().getMatchingFilter(msg);
		if (f != null) {
			return true;
		}
		
		return false;
	}

}
