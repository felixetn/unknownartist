package org.echtzeitsysteme.prosem.wireless_sniffer.view.messagetable;

import java.awt.FontMetrics;
import java.text.SimpleDateFormat;

import javax.swing.JLabel;
import javax.swing.table.AbstractTableModel;
import javax.swing.table.TableColumn;

import org.echtzeitsysteme.prosem.messages.Message;
import org.echtzeitsysteme.prosem.wireless_sniffer.model.Model;
import org.echtzeitsysteme.prosem.wireless_sniffer.model.ModelChangeListener;
import org.echtzeitsysteme.prosem.wireless_sniffer.model.filters.Filter;
import org.echtzeitsysteme.prosem.wireless_sniffer.model.filters.FilterChangeListener;

public class PackageTableModel extends AbstractTableModel implements
		ModelChangeListener, FilterChangeListener {
	private static final long serialVersionUID = 4345745670042114727L;

	private enum ETableColumns {
		ID,
		DIRECTION,
		STATE,
		TIME,
		SRCID,
		DESTID,
		TYPE,
		ClASS,
		LENGTH,
		DATA;

		public static ETableColumns valueOf(int pos) {
			for (ETableColumns col : ETableColumns.values()) {
				if (col.ordinal() == pos)
					return col;
			}
			return null;
		}

		public static int getElementCount() {
			return ETableColumns.values().length;
		}
	};

	private Model model;

	private SimpleDateFormat dateFormatter = new SimpleDateFormat("HH:mm:ss");

	public PackageTableModel(Model model) {
		this.model = model;
		model.addListener(this);
		model.getFilterQueue().addListener(this);
	}

	public Model getModel() {
		return model;
	}

	@Override
	public Class<?> getColumnClass(int columnIndex) {
		switch (ETableColumns.valueOf(columnIndex)) {
			case ID:
				return Integer.class;
			case DIRECTION:
				return String.class;
			case TIME:
				return String.class;
			case SRCID:
				return Integer.class;
			case DESTID:
				return Integer.class;
			case TYPE:
				return Integer.class;
			case ClASS:
				return String.class;
			case LENGTH:
				return Integer.class;
			case DATA:
				return String.class;
			case STATE:
				return String.class;
		}
		return Object.class;
	}

	@Override
	public int getColumnCount() {
		return ETableColumns.getElementCount();
	}

	@Override
	public String getColumnName(int column) {
		switch (ETableColumns.valueOf(column)) {
			case ID:
				return "#";
			case DIRECTION:
				return "Dir";
			case TIME:
				return "Time";
			case SRCID:
				return "Src-Id";
			case DESTID:
				return "Dest-Id";
			case TYPE:
				return "Type";
			case ClASS:
				return "Class";
			case LENGTH:
				return "Len";
			case DATA:
				return "Data";
			case STATE:
				return "State";
		}
		return "";
	}

	@Override
	public int getRowCount() {
		return model.getMessageStackSize();
	}

	@Override
	public Object getValueAt(int row, int col) {
		Message msg = model.getMessage(row);
		ETableColumns column = ETableColumns.valueOf(col);
		if (column == null)
			return msg;

		switch (column) {
			case ID:
				return new Integer(row + 1);
			case DIRECTION:
				return (msg.isDirectionOut() ? "Out" : "In");
			case TIME:
				return dateFormatter.format(msg.getDate());
			case SRCID:
				return new Integer(msg.getSourceId());
			case DESTID:
				return new Integer(msg.getDestinationId());
			case TYPE:
				return new Integer(msg.getMessageType());
			case ClASS:
				return msg.getClass().getSimpleName();
			case LENGTH:
				return new Integer(msg.getData().length);
			case DATA:
				Filter f = model.getFilterQueue().getMatchingFilter(msg);
				if (f == null || f.getPrintAsHex())
					return msg.getDataAsHex();
				else
					return msg.getDataAsASCII();
			case STATE:
				switch (msg.getMessageState()) {
					case RECV:
						return "Recv";
					case NOT_SENT:
						return "!Sent";
					case TIMEOUT:
						return "Timeout";
					case NO_ACK:
						return "!Ack";
					case ACK:
						return "Ack";
				}
				return "";
		}
		return null;
	}

	/**
	 * Set the constraints on the columns size for the given column
	 * 
	 * @param colNum
	 *            Column number
	 * @param col
	 *            Column object
	 */
	void setColumnConstraints(int colNum, TableColumn col) {
		int w = -1;
		final int sizeBonus = 28;
		JLabel jl = new JLabel();
		FontMetrics fm = jl.getFontMetrics(jl.getFont());
		switch (ETableColumns.valueOf(colNum)) {
			case ID:
				w = fm.stringWidth("12345") + 12;
				break;
			case DIRECTION:
				w = fm.stringWidth("Dir") + sizeBonus;
				break;
			case TIME:
				w = fm.stringWidth("HH:mm:ss") + 12;
				break;
			case SRCID:
				w = fm.stringWidth("Dest-Id") + sizeBonus;
				break;
			case DESTID:
				w = fm.stringWidth("Dest-Id") + sizeBonus;
				break;
			case TYPE:
				w = fm.stringWidth("Type") + sizeBonus;
				break;
			case ClASS:
				col.setPreferredWidth(75);
				break;
			case LENGTH:
				w = fm.stringWidth("Type") + sizeBonus;
				break;
			case DATA:
				col.setPreferredWidth(100);
				break;
			case STATE:
				w = fm.stringWidth("Timeout") + sizeBonus;
				break;
		}
		if (w > 0) {
			col.setPreferredWidth(w);
			col.setMinWidth(w);
			col.setMaxWidth(w);
		}
	}

	@Override
	public void messageAdded(Model src, Message msg, int index) {
		fireTableRowsInserted(index, index);
	}

	@Override
	public void messageUpdated(Model src, Message msg, int index) {
		fireTableRowsUpdated(index, index);
	}

	@Override
	public void messagesCleared(Model src) {
		fireTableDataChanged();
	}

	@Override
	public void filtersUpdated() {
		fireTableDataChanged();
	}
}
