package org.echtzeitsysteme.prosem.wireless_sniffer.view.filters;

import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

import javax.swing.BorderFactory;
import javax.swing.ButtonGroup;
import javax.swing.DefaultListCellRenderer;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JScrollPane;
import javax.swing.JTextField;
import javax.swing.ListCellRenderer;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.text.AttributeSet;
import javax.swing.text.BadLocationException;
import javax.swing.text.DocumentFilter;
import javax.swing.text.PlainDocument;

import net.miginfocom.swing.MigLayout;

import org.echtzeitsysteme.prosem.messages.Message;
import org.echtzeitsysteme.prosem.messages.MessageClassList;
import org.echtzeitsysteme.prosem.wireless_sniffer.controller.Controller;
import org.echtzeitsysteme.prosem.wireless_sniffer.model.filters.Filter;
import org.echtzeitsysteme.prosem.wireless_sniffer.model.filters.FilterChangeListener;
import org.echtzeitsysteme.prosem.wireless_sniffer.model.filters.Filter.EDirection;
import org.echtzeitsysteme.prosem.wireless_sniffer.view.ImageButton;
import org.echtzeitsysteme.prosem.wireless_sniffer.view.TableColors;

public class FilterPanel extends JPanel implements ListSelectionListener,
		FilterChangeListener, ActionListener, KeyListener, ItemListener {
	private static final long serialVersionUID = 2352252806127618983L;

	private Controller controller;

	private FilterList filterList;
	private JButton moveFilterItemUp;
	private JButton moveFilterItemDown;
	private JButton newFilter;
	private JButton deleteFilter;

	private JPanel filterSettingsPanel;
	private JTextField filterName;
	private JCheckBox filterDirection;
	private JRadioButton filterDirectionOut;
	private JRadioButton filterDirectionIn;
	private JCheckBox filterSource;
	private JTextField filterSourceVal;
	private JCheckBox filterDestination;
	private JTextField filterDestinationVal;
	private JCheckBox filterType;
	private JTextField filterTypeVal;
	private JCheckBox filterPriority;
	private JTextField filterPriorityVal;
	private JCheckBox filterLength;
	private JTextField filterLengthVal;
	private JCheckBox filterClass;
	private JComboBox filterClassVal;

	private JPanel filterVisuPanel;
	private JRadioButton filterOutputHex;
	private JRadioButton filterOutputASCII;
	private JComboBox filterOutputColorBox;
	private JPanel filterOutputColor;

	private boolean isAdjustingByCode = false;

	public FilterPanel(Controller controller) {
		super(new MigLayout("fill", "[min][min][min][grow]", ""));
		setBorder(BorderFactory.createTitledBorder("Filter"));

		this.controller = controller;

		controller.getFilterQueue().addListener(this);

		initComponents();
	}

	private void initComponents() {
		JLabel tempLabel;

		/*
		 * Col 1: Filter list and controls
		 */
		JPanel filterPanel = new JPanel(new MigLayout("fill", "[200px!]",
				"[grow][min][min]"));

		filterList = new FilterList(controller.getFilterQueue());
		filterList.addListSelectionListener(this);
		JScrollPane scrollFilters = new JScrollPane(filterList,
				JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
				JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
		filterPanel.add(scrollFilters, "grow, wrap");

		// move up Button
		moveFilterItemUp = new ImageButton(this.getClass().getResource(
				"/icons/vp04_top_BlOnWh.gif"), this, "moveFilterItemUp");
		filterPanel.add(moveFilterItemUp, "split 4");

		// move down Button
		moveFilterItemDown = new ImageButton(this.getClass().getResource(
				"/icons/vp04_down_BlOnWh.gif"), this, "moveFilterItemDown");
		filterPanel.add(moveFilterItemDown);

		// new filter Button
		newFilter = new ImageButton(this.getClass().getResource(
				"/icons/vp04_new_BlOnWh.gif"), this, "createNewFilter");
		filterPanel.add(newFilter);

		// delete filter Button
		deleteFilter = new ImageButton(this.getClass().getResource(
				"/icons/vp04_del_BlOnWh.gif"), this, "deleteFilter");
		filterPanel.add(deleteFilter);

		add(filterPanel);

		/*
		 * Col 2: Filter settings
		 */
		filterSettingsPanel = new JPanel(new MigLayout("fill",
				"[][200px:pref][][70px:pref]", ""));

		tempLabel = new JLabel("Name:");
		filterSettingsPanel.add(tempLabel);
		filterName = new JTextField();
		filterName.setName("name");
		filterName.addKeyListener(this);
		filterSettingsPanel.add(filterName, "grow");

		filterSource = new JCheckBox("Quell-Id:");
		filterSource.setName("sourceId");
		filterSource.addActionListener(this);
		filterSettingsPanel.add(filterSource);
		filterSourceVal = new JTextField();
		filterSourceVal.setDocument(new DigitFilter());
		filterSourceVal.setName("sourceIdVal");
		filterSourceVal.addKeyListener(this);
		filterSettingsPanel.add(filterSourceVal, "grow, wrap");

		filterClass = new JCheckBox("Klasse:");
		filterClass.setName("class");
		filterClass.addActionListener(this);
		filterSettingsPanel.add(filterClass);
		filterClassVal = new JComboBox(MessageClassList.getMessageClassNames()
				.toArray(new String[0]));
		filterClassVal.setName("classVal");
		filterClassVal.addItemListener(this);
		filterSettingsPanel.add(filterClassVal, "grow");

		filterDestination = new JCheckBox("Ziel-Id:");
		filterDestination.setName("destinationId");
		filterDestination.addActionListener(this);
		filterSettingsPanel.add(filterDestination);
		filterDestinationVal = new JTextField();
		filterDestinationVal.setDocument(new DigitFilter());
		filterDestinationVal.setName("destinationIdVal");
		filterDestinationVal.addKeyListener(this);
		filterSettingsPanel.add(filterDestinationVal, "grow, wrap");

		filterDirection = new JCheckBox("Richtung");
		filterDirection.setName("direction");
		filterDirection.addActionListener(this);
		filterSettingsPanel.add(filterDirection);
		filterDirectionOut = new JRadioButton("Out");
		filterDirectionOut.setName("directionOut");
		filterDirectionOut.addActionListener(this);
		filterSettingsPanel.add(filterDirectionOut, "split 2");
		filterDirectionIn = new JRadioButton("In");
		filterDirectionIn.setName("directionIn");
		filterDirectionIn.addActionListener(this);
		filterSettingsPanel.add(filterDirectionIn);
		ButtonGroup bg = new ButtonGroup();
		bg.add(filterDirectionOut);
		bg.add(filterDirectionIn);

		filterType = new JCheckBox("Typ:");
		filterType.setName("messageType");
		filterType.addActionListener(this);
		filterSettingsPanel.add(filterType);
		filterTypeVal = new JTextField();
		filterTypeVal.setDocument(new DigitFilter());
		filterTypeVal.setName("messageTypeVal");
		filterTypeVal.addKeyListener(this);
		filterSettingsPanel.add(filterTypeVal, "grow, wrap");

		filterPriority = new JCheckBox("Priorität:");
		filterPriority.setName("priority");
		filterPriority.addActionListener(this);
		filterSettingsPanel.add(filterPriority);
		filterPriorityVal = new JTextField();
		filterPriorityVal.setDocument(new DigitFilter());
		filterPriorityVal.setName("priorityVal");
		filterPriorityVal.addKeyListener(this);
		filterSettingsPanel.add(filterPriorityVal, "grow");

		filterLength = new JCheckBox("Datenlänge:");
		filterLength.setName("length");
		filterLength.addActionListener(this);
		filterSettingsPanel.add(filterLength);
		filterLengthVal = new JTextField();
		filterLengthVal.setDocument(new DigitFilter());
		filterLengthVal.setName("lengthVal");
		filterLengthVal.addKeyListener(this);
		filterSettingsPanel.add(filterLengthVal, "grow");

		add(filterSettingsPanel);

		/*
		 * Col 3: Filter visualization
		 */
		filterVisuPanel = new JPanel(new MigLayout("fill", "[][70px!][20px!]",
				""));

		filterOutputHex = new JRadioButton("Ausgabe in Hex");
		filterOutputHex.setName("printHex");
		filterOutputHex.addActionListener(this);
		filterVisuPanel.add(filterOutputHex, "span, wrap");
		filterOutputASCII = new JRadioButton("Ausgabe in ASCII");
		filterOutputASCII.setName("printASCII");
		filterOutputASCII.addActionListener(this);
		filterVisuPanel.add(filterOutputASCII, "span, wrap");
		bg = new ButtonGroup();
		bg.add(filterOutputHex);
		bg.add(filterOutputASCII);

		tempLabel = new JLabel("Farbe:");
		filterVisuPanel.add(tempLabel);
		filterOutputColorBox = new JComboBox(TableColors.getInstance()
				.getBaseColors());
		filterOutputColorBox.setRenderer(new ColorCellRenderer());
		filterOutputColorBox.addActionListener(this);
		filterOutputColorBox.setName("color");
		filterVisuPanel.add(filterOutputColorBox, "grow");
		filterOutputColor = new JPanel();
		filterVisuPanel.add(filterOutputColor, "grow");

		add(filterVisuPanel);

		updateFilterDetails();
	}

	private void updateFilterDetails() {
		Filter f = (Filter) filterList.getSelectedValue();

		enableFilterComponents(f != null);
		if (f != null) {
			filterName.setText(f.getFilterName());
			filterDirection.setSelected(f.getDirection() != EDirection.IGNORE);
			filterDirectionOut
					.setEnabled(f.getDirection() != EDirection.IGNORE);
			filterDirectionOut.setSelected(f.getDirection() == EDirection.OUT);
			filterDirectionIn.setEnabled(f.getDirection() != EDirection.IGNORE);
			filterDirectionIn.setSelected(f.getDirection() == EDirection.IN);
			filterSource.setSelected(f.getSourceId() >= 0);
			filterSourceVal.setEnabled(f.getSourceId() >= 0);
			filterSourceVal.setText(String.valueOf(f.getSourceId()));
			filterDestination.setSelected(f.getDestinationId() >= 0);
			filterDestinationVal.setEnabled(f.getDestinationId() >= 0);
			filterDestinationVal.setText(String.valueOf(f.getDestinationId()));
			filterType.setSelected(f.getMessageType() >= 0);
			filterTypeVal.setEnabled(f.getMessageType() >= 0);
			filterTypeVal.setText(String.valueOf(f.getMessageType()));
			filterPriority.setSelected(f.getPriority() >= 0);
			filterPriorityVal.setEnabled(f.getPriority() >= 0);
			filterPriorityVal.setText(String.valueOf(f.getPriority()));
			filterLength.setSelected(f.getDataLength() >= 0);
			filterLengthVal.setEnabled(f.getDataLength() >= 0);
			filterLengthVal.setText(String.valueOf(f.getDataLength()));
			filterClass.setSelected(f.getMessageClass() != null);
			filterClassVal.setEnabled(f.getMessageClass() != null);
			isAdjustingByCode = true;
			if (f.getMessageClass() != null) {
				filterClassVal.setSelectedItem(MessageClassList
						.getMessageClassNameByClass(f.getMessageClass()));
			} else
				filterClassVal.setSelectedIndex(0);
			isAdjustingByCode = false;
			filterOutputHex.setSelected(f.getPrintAsHex());
			filterOutputASCII.setSelected(!f.getPrintAsHex());
			filterOutputColorBox.setSelectedItem(f.getColor());
			filterOutputColor.setBackground((Color) filterOutputColorBox
					.getSelectedItem());
			moveFilterItemUp.setEnabled(filterList.getSelectedIndex() > 0);
			moveFilterItemDown
					.setEnabled(filterList.getSelectedIndex() < (filterList
							.getModel().getSize() - 1));
		} else {
			filterName.setText("");
			filterDirection.setSelected(false);
			filterDirectionOut.setSelected(true);
			filterSource.setSelected(false);
			filterSourceVal.setText("0");
			filterDestination.setSelected(false);
			filterDestinationVal.setText("0");
			filterType.setSelected(false);
			filterTypeVal.setText("0");
			filterPriority.setSelected(false);
			filterPriorityVal.setText("0");
			filterLength.setSelected(false);
			filterLengthVal.setText("0");
			filterClass.setSelected(false);
			filterClassVal.setSelectedIndex(0);
			filterOutputHex.setSelected(true);
			filterOutputColorBox.setSelectedIndex(0);
			filterOutputColor.setBackground((Color) filterOutputColorBox
					.getSelectedItem());
		}
	}

	private void enableFilterComponents(boolean enable) {
		for (Component c : filterSettingsPanel.getComponents())
			c.setEnabled(enable);
		for (Component c : filterVisuPanel.getComponents())
			c.setEnabled(enable);
		deleteFilter.setEnabled(enable);
		moveFilterItemDown.setEnabled(enable);
		moveFilterItemUp.setEnabled(enable);
	}

	public Filter getSelectedFilter() {
		return (Filter) filterList.getSelectedValue();
	}

	private static class ColorCellRenderer implements ListCellRenderer {
		private DefaultListCellRenderer defaultRenderer = new DefaultListCellRenderer();

		// width doesn't matter as combobox will size
		private final static Dimension preferredSize = new Dimension(40, 20);

		public Component getListCellRendererComponent(JList list, Object value,
				int index, boolean isSelected, boolean cellHasFocus) {
			JLabel renderer = (JLabel) defaultRenderer
					.getListCellRendererComponent(list, "", index, isSelected,
							cellHasFocus);
			renderer.setOpaque(true);
			if (value instanceof Color) {
				renderer.setBackground((Color) value);
			}
			renderer.setPreferredSize(preferredSize);
			return renderer;
		}
	}

	private static class DigitFilter extends PlainDocument {
		private static final long serialVersionUID = 8359163786460120488L;

		public DigitFilter() {
			super();
			setDocumentFilter(new DocumentFilter() {
				@Override
				public void insertString(FilterBypass fb, int offset,
						String string, AttributeSet attr)
						throws BadLocationException {
					fb.insertString(offset, string.replaceAll("\\D++", ""),
							attr);
				}

				@Override
				public void replace(FilterBypass fb, int offset, int length,
						String text, AttributeSet attrs)
						throws BadLocationException {
					fb.replace(offset, length, text.replaceAll("\\D++", ""),
							attrs);
				}
			});
		}
	}

	@Override
	public void filtersUpdated() {
		filterList.updateData();
		updateFilterDetails();
	}

	@Override
	public void valueChanged(ListSelectionEvent e) {
		updateFilterDetails();
	}

	private int getTextFieldInt(JTextField txt) {
		int val = 0;
		try {
			val = Integer.valueOf(txt.getText());
		} catch (NumberFormatException ex) {
		}
		return val;
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		String sourceClass = e.getSource().getClass().getSimpleName();
		String sourceName = ((JComponent) e.getSource()).getName();

		if (sourceName.equals("createNewFilter")) {
			controller.getFilterQueue().createFilter();
			return;
		}

		if (getSelectedFilter() == null)
			return;

		if (sourceName.equals("class")) {
			if (filterClass.isSelected()) {
				String className = (String) filterClassVal.getSelectedItem();
				Class<? extends Message> cls = MessageClassList
						.getMessageClassByName(className);
				getSelectedFilter().setMessageClass(cls);
			} else
				getSelectedFilter().setMessageClass(null);
		} else if (sourceName.equals("direction")
				|| sourceName.equals("directionOut")
				|| sourceName.equals("directionIn")) {
			if (filterDirection.isSelected())
				getSelectedFilter().setDirection(
						filterDirectionOut.isSelected() ? EDirection.OUT
								: EDirection.IN);
			else
				getSelectedFilter().setDirection(EDirection.IGNORE);
		} else if (sourceName.equals("priority")) {
			if (filterPriority.isSelected())
				getSelectedFilter().setPriority(
						getTextFieldInt(filterPriorityVal));
			else
				getSelectedFilter().setPriority(-1);
		} else if (sourceName.equals("sourceId")) {
			if (filterSource.isSelected())
				getSelectedFilter().setSourceId(
						getTextFieldInt(filterSourceVal));
			else
				getSelectedFilter().setSourceId(-1);
		} else if (sourceName.equals("destinationId")) {
			if (filterDestination.isSelected())
				getSelectedFilter().setDestinationId(
						getTextFieldInt(filterDestinationVal));
			else
				getSelectedFilter().setDestinationId(-1);
		} else if (sourceName.equals("messageType")) {
			if (filterType.isSelected())
				getSelectedFilter().setMessageType(
						getTextFieldInt(filterTypeVal));
			else
				getSelectedFilter().setMessageType(-1);
		} else if (sourceName.equals("length")) {
			if (filterLength.isSelected())
				getSelectedFilter().setDataLength(
						getTextFieldInt(filterLengthVal));
			else
				getSelectedFilter().setDataLength(-1);
		} else if (sourceName.equals("printHex")) {
			getSelectedFilter().setPrintAsHex(true);
		} else if (sourceName.equals("printASCII")) {
			getSelectedFilter().setPrintAsHex(false);
		} else if (sourceName.equals("color")) {
			getSelectedFilter().setColor(
					(Color) filterOutputColorBox.getSelectedItem());
		} else if (sourceName.equals("moveFilterItemUp")) {
			getSelectedFilter().moveUp();
			filterList.setSelectedIndex(filterList.getSelectedIndex() - 1);
		} else if (sourceName.equals("moveFilterItemDown")) {
			getSelectedFilter().moveDown();
			filterList.setSelectedIndex(filterList.getSelectedIndex() + 1);
		} else if (sourceName.equals("deleteFilter")) {
			getSelectedFilter().delete();
		} else {
			System.out.println("Uncaught action event: Class=" + sourceClass
					+ " - Name=" + sourceName + " - Cmd="
					+ e.getActionCommand());
		}
	}

	@Override
	public void keyPressed(KeyEvent e) {
	}

	@Override
	public void keyReleased(KeyEvent e) {
		String sourceClass = e.getSource().getClass().getSimpleName();
		String sourceName = ((JComponent) e.getSource()).getName();
		JTextField src = (JTextField) e.getSource();

		if (sourceName.equals("name")) {
			getSelectedFilter().setFilterName(src.getText());
		} else {

			if (sourceName.equals("priorityVal")) {
				getSelectedFilter().setPriority(getTextFieldInt(src));
			} else if (sourceName.equals("sourceIdVal")) {
				getSelectedFilter().setSourceId(getTextFieldInt(src));
			} else if (sourceName.equals("destinationIdVal")) {
				getSelectedFilter().setDestinationId(getTextFieldInt(src));
			} else if (sourceName.equals("messageTypeVal")) {
				getSelectedFilter().setMessageType(getTextFieldInt(src));
			} else if (sourceName.equals("lengthVal")) {
				getSelectedFilter().setDataLength(getTextFieldInt(src));
			} else {
				System.out.println("Uncaught key event: Class=" + sourceClass
						+ " - Name=" + sourceName);
			}
		}
	}

	@Override
	public void keyTyped(KeyEvent e) {
	}

	@Override
	public void itemStateChanged(ItemEvent e) {
		String sourceClass = e.getSource().getClass().getSimpleName();
		String sourceName = ((JComponent) e.getSource()).getName();

		if (isAdjustingByCode)
			return;

		if (sourceName.equals("classVal")) {
			if (filterClass.isSelected()) {
				String className = (String) filterClassVal.getSelectedItem();
				Class<? extends Message> cls = MessageClassList
						.getMessageClassByName(className);
				getSelectedFilter().setMessageClass(cls);
			}
		} else {
			System.out.println("Uncaught item event: Class=" + sourceClass
					+ " - Name=" + sourceName);
		}
	}

}
