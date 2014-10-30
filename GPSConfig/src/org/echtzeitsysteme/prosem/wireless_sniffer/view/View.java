package org.echtzeitsysteme.prosem.wireless_sniffer.view;

import java.awt.Color;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.util.Vector;

import javax.swing.BorderFactory;
import javax.swing.DefaultComboBoxModel;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.JComponent;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextField;

import net.miginfocom.swing.MigLayout;

import org.echtzeitsysteme.prosem.wireless_sniffer.controller.Controller;
import org.echtzeitsysteme.prosem.wireless_sniffer.model.Model;
import org.echtzeitsysteme.prosem.wireless_sniffer.model.amber8425.Amber8425;
import org.echtzeitsysteme.prosem.wireless_sniffer.model.amber8425.SettingChangeListener;
import org.echtzeitsysteme.prosem.wireless_sniffer.view.filters.FilterPanel;
import org.echtzeitsysteme.prosem.wireless_sniffer.view.messagetable.PackageTable;

public class View extends JFrame implements ActionListener,
		SettingChangeListener, FocusListener {
	private static final long serialVersionUID = -2433896815801080043L;

	private Controller controller;
	private Model model;

	private PackageTable dataTable;
	private FilterPanel filterPanel;

	private JTextField ownId;
	private JTextField channel;

	private JLabel comPortLabel;
	private JLabel statusLabel;

	private JComboBox comportComboBox;

	public View(Model m, Controller c) {
		super("PS-ES Packet-Sniffer v2");

		this.model = m;
		this.controller = c;
		controller.getAmber8425().addSettingListener(this);

		// to close window on X
		setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);

		this.initGrid();
		pack();
		setVisible(true);
	}

	/**
	 * @brief Initiates the basic gridLayout
	 */
	private void initGrid() {
		setLayout(new MigLayout("fill", "", "[grow][min][min][min][min]"));
		JLabel tempLabel;

		/*
		 * Row 1: Table
		 */
		dataTable = new PackageTable(model);
		JScrollPane scrollTable = new JScrollPane(dataTable,
				JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
				JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
		add(scrollTable, "grow, wrap");

		/*
		 * Row 2: Table control
		 */
		JPanel tableControll = new JPanel(new MigLayout());
		JLabel labelDetails = new JLabel("Einstellungen:");
		tableControll.add(labelDetails);

		JCheckBox checkboxAutoscroll = new JCheckBox("AutoScroll", true);
		checkboxAutoscroll.addActionListener(this);
		checkboxAutoscroll.setName("autoScroll");
		tableControll.add(checkboxAutoscroll);

		JCheckBox checkboxShowAllMessages = new JCheckBox("Zeige alle Pakete",
				true);
		checkboxShowAllMessages.addActionListener(this);
		checkboxShowAllMessages.setName("showAllMessages");
		tableControll.add(checkboxShowAllMessages);

		add(tableControll, "right, wrap");

		/*
		 * Row 3: Filter
		 */
		filterPanel = new FilterPanel(controller);
		add(filterPanel, "grow, wrap");

		/*
		 * Row 4: Sniffer control
		 */
		JPanel snifferControlPanel = new JPanel(new MigLayout("fill",
				"[min]push[min]", ""));

		tempLabel = new JLabel("Eigene Id:");
		snifferControlPanel.add(tempLabel, "split 4");
		ownId = new JTextField();
		ownId.addFocusListener(this);
		ownId.setName("ownId");
		snifferControlPanel.add(ownId, "w 50!");

		tempLabel = new JLabel("Kanal:");
		snifferControlPanel.add(tempLabel, "gap unrelated");
		channel = new JTextField();
		channel.addFocusListener(this);
		channel.setName("channel");
		snifferControlPanel.add(channel, "w 50!");

		JButton startSniffer = new JButton("Start");
		startSniffer.setName("startSniffer");
		startSniffer.addActionListener(this);
		snifferControlPanel.add(startSniffer, "split 6");

		JButton stopSniffer = new JButton("Stop");
		stopSniffer.setName("stopSniffer");
		stopSniffer.addActionListener(this);
		snifferControlPanel.add(stopSniffer);

		JButton clearSniffer = new JButton("Liste löschen");
		clearSniffer.setName("clearSniffer");
		clearSniffer.addActionListener(this);
		snifferControlPanel.add(clearSniffer);

		JLabel labelComport = new JLabel("ComPorts:");
		snifferControlPanel.add(labelComport);
		comportComboBox = new JComboBox();
		snifferControlPanel.add(comportComboBox, "gap unrelated");

		JButton refreshComportList = new ImageButton(this.getClass()
				.getResource("/icons/vp04_reload_BlOnWh.gif"), this,
				"refreshComportList");
		snifferControlPanel.add(refreshComportList);

		add(snifferControlPanel, "grow, wrap");

		/*
		 * Row 5: Footer
		 */
		JPanel statusPanel = new JPanel(new MigLayout());
		statusPanel.setBorder(BorderFactory.createLoweredBevelBorder());
		tempLabel = new JLabel("Com Port:");
		statusPanel.add(tempLabel);
		comPortLabel = new JLabel("---");
		statusPanel.add(comPortLabel);

		tempLabel = new JLabel("Status:");
		statusPanel.add(tempLabel, "gap unrelated");
		statusLabel = new JLabel("---");
		statusPanel.add(statusLabel);

		add(statusPanel, "dock south");

		// controller.updateFooterInfoPanel("---", "---", Color.black);
	}

	/**
	 * @brief updates the program controll panel
	 * @param comPorts
	 *            available
	 */
	public void updateComPortList(Vector<String> comPorts) {
		comportComboBox.setModel(new DefaultComboBoxModel(comPorts));
	}

	public String getSelectedPort() {
		return (String) comportComboBox.getSelectedItem();
	}

	/**
	 * @brief updates the footer information panel
	 * @param status
	 *            New status
	 * @param portName
	 *            New port name
	 * @param c
	 *            Color to use
	 */
	public void updateFooterInfoPanel(String status, String portName, Color c) {
		if (portName == null) {
			portName = "---";
		}
		comPortLabel.setText(portName);
		statusLabel.setText(status);

		statusLabel.setForeground(c);
	}

	private void enableAutoScroll(Boolean enableAutoScroll) {
		dataTable.setAutoScroll(enableAutoScroll);
	}

	private void showAllMessages(Boolean showAllMessages) {
		dataTable.setFilter(showAllMessages);
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		String sourceName = ((JComponent) e.getSource()).getName();

		if (sourceName.equals("autoScroll")) {
			enableAutoScroll(((JCheckBox) e.getSource()).isSelected());
		} else if (sourceName.equals("showAllMessages")) {
			showAllMessages(((JCheckBox) e.getSource()).isSelected());
		} else if (sourceName.equals("startSniffer")) {
			controller.startSniffing(controller.getSelectedComPort());
		} else if (sourceName.equals("stopSniffer")) {
			controller.stopSniffing();
		} else if (sourceName.equals("clearSniffer")) {
			controller.resetMessageTable();
		} else if (sourceName.equals("refreshComportList")) {
			controller.updateComPortList();
		}
	}

	@Override
	public void wirelessSettingsChanged(Amber8425 src, int rfId, int channel) {
		ownId.setText(String.valueOf(src.getRfId()));
		this.channel.setText(String.valueOf(src.getChannel()));
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
	public void focusGained(FocusEvent e) {
	}

	@Override
	public void focusLost(FocusEvent e) {
		String sourceClass = e.getSource().getClass().getSimpleName();
		String sourceName = ((JComponent) e.getSource()).getName();

		if (sourceName.equals("ownId")) {
			controller.getAmber8425().setRfId(getTextFieldInt((JTextField) e.getSource()));
		} else if (sourceName.equals("channel")) {
			controller.getAmber8425().setChannel(getTextFieldInt((JTextField) e.getSource()));
		} else {
			System.out.println("Uncaught focus event: Class=" + sourceClass
					+ " - Name=" + sourceName);
		}
	}

}
