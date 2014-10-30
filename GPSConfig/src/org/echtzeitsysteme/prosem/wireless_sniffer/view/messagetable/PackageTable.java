package org.echtzeitsysteme.prosem.wireless_sniffer.view.messagetable;

import java.awt.Color;
import java.awt.Component;
import java.awt.Rectangle;
import java.util.ArrayList;
import java.util.List;

import javax.swing.JTable;
import javax.swing.JViewport;
import javax.swing.ListSelectionModel;
import javax.swing.RowSorter;
import javax.swing.SortOrder;
import javax.swing.UIManager;
import javax.swing.event.TableModelEvent;
import javax.swing.table.TableCellRenderer;
import javax.swing.table.TableRowSorter;

import org.echtzeitsysteme.prosem.messages.Message;
import org.echtzeitsysteme.prosem.wireless_sniffer.model.Model;
import org.echtzeitsysteme.prosem.wireless_sniffer.model.filters.Filter;
import org.echtzeitsysteme.prosem.wireless_sniffer.view.TableColors;

public class PackageTable extends JTable {
	private static final long serialVersionUID = 530167834059924752L;

	private Color normal = UIManager.getColor("Table.background");

	private PackageTableModel model;
	private TableRowSorter<PackageTableModel> sorter;

	private boolean autoScroll = true;

	public PackageTable(Model mvcModel) {
		super();

		setSelectionMode(ListSelectionModel.SINGLE_SELECTION);

		model = new PackageTableModel(mvcModel);

		setModel(this.model);

		sorter = new TableRowSorter<PackageTableModel>(model);
		setRowSorter(sorter);

		setFilter(true);

		List<RowSorter.SortKey> sortKeys = new ArrayList<RowSorter.SortKey>();
		sortKeys.add(new RowSorter.SortKey(0, SortOrder.ASCENDING));
		sorter.setSortKeys(sortKeys);

		for (int i = 0; i < model.getColumnCount(); i++) {
			model.setColumnConstraints(i, getColumnModel().getColumn(i));
		}
	}

	public void setAutoScroll(boolean autoscroll) {
		this.autoScroll = autoscroll;
	}

	@Override
	public void tableChanged(TableModelEvent e) {
		super.tableChanged(e);

		if ((e.getType() == TableModelEvent.INSERT) && autoScroll) {
			if (model.getRowCount() > 0)
				goToRow(convertRowIndexToView(model.getRowCount() - 1));
		}
	}

	public void setFilter(boolean showAll) {
		sorter.setRowFilter(new PackageTableFilter(model, showAll));
	}

	@Override
	public Component prepareRenderer(TableCellRenderer renderer, int row,
			int col) {
		Component comp = super.prepareRenderer(renderer, row, col);
		Message msg = (Message) model.getValueAt(row, -1);

		if (row != getSelectedRow()) {
			Filter f = model.getModel().getFilterQueue().getMatchingFilter(msg);
			if (f != null) {
				Color c = f.getColor();
				if (row % 2 == 0) {
					comp.setBackground(TableColors.getInstance()
							.getAlternateColor(c));
				} else {
					comp.setBackground(c);
				}
			} else {
				if (row % 2 == 0) {
					comp.setBackground(TableColors.getInstance()
							.getAlternateColor(normal));
				} else {
					comp.setBackground(normal);
				}
			}
		}

		return comp;
	}

	private void goToRow(int row) {
		JViewport viewport = (JViewport) getParent();
		Rectangle rect = getCellRect(row, 0, true);
		Rectangle r2 = viewport.getVisibleRect();
		scrollRectToVisible(new Rectangle(rect.x, rect.y, (int) r2.getWidth(),
				(int) r2.getHeight()));
	}

}
