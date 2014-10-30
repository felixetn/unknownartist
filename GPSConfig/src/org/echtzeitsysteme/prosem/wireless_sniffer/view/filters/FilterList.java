package org.echtzeitsysteme.prosem.wireless_sniffer.view.filters;

import java.awt.Component;
import java.awt.Font;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

import javax.swing.DefaultListCellRenderer;
import javax.swing.JCheckBox;
import javax.swing.JList;

import org.echtzeitsysteme.prosem.wireless_sniffer.model.filters.Filter;
import org.echtzeitsysteme.prosem.wireless_sniffer.model.filters.FilterQueue;

public class FilterList extends JList {
	private static final long serialVersionUID = -6730471416009822107L;

	private FilterListModel model;

	public FilterList(FilterQueue queue) {
		super();
		model = new FilterListModel(queue);
		setModel(model);

		setCellRenderer(new FilterListRenderer());

		addMouseListener(new MouseAdapter() {
			@Override
			public void mousePressed(MouseEvent e) {
				int index = locationToIndex(e.getPoint());

				if (index != -1) {
					if (e.getPoint().getX() < 20) {
						Filter f = (Filter) getModel().getElementAt(index);
						f.setEnabled(!f.isEnabled());
					}
				}
			}
		});
	}

	private static class FilterListRenderer extends DefaultListCellRenderer {
		private static final long serialVersionUID = 7811357859053366583L;

		@Override
		public Component getListCellRendererComponent(JList list, Object value,
				int index, boolean isSelected, boolean cellHasFocus) {
			Filter f = (Filter) list.getModel().getElementAt(index);
			JCheckBox chk = new JCheckBox(f.getFilterName());
			chk.setOpaque(true);
			chk.setBackground(f.getColor());
			if (isSelected)
				chk.setFont(chk.getFont().deriveFont(Font.BOLD));
			chk.setSelected(f.isEnabled());
			return chk;
		}
	}

	public void updateData() {
		model.updateData();
	}

}
