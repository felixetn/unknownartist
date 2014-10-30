package org.echtzeitsysteme.prosem.gps;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JPopupMenu;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;
import javax.swing.JTable;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.ListSelectionModel;
import javax.swing.UIManager;
import javax.swing.border.TitledBorder;
import javax.swing.table.DefaultTableModel;

import org.echtzeitsysteme.prosem.messages.bluetooth.GPSControlMessage;
import org.echtzeitsysteme.prosem.messages.bluetooth.GPSControlMessage.EGPSControlMessageType;
import org.echtzeitsysteme.prosem.messages.bluetooth.GPSUpdateCoordMessage;
import org.echtzeitsysteme.prosem.tuio.Point;
import org.echtzeitsysteme.prosem.tuio.TuioObjectListener;
import org.echtzeitsysteme.prosem.tuio.TuioReceiver;
import org.echtzeitsysteme.prosem.wireless_sniffer.Sniffer;

import TUIO.TuioObject;

/**
 * The ConfigGUI is used to control the TUIO Client. It is just an frontend.
 * 
 * @author Jonas Schönichen, Nicolas Weber
 * 
 */
public class ConfigGUI extends JFrame implements ActionListener, MouseListener,
		FocusListener, TuioObjectListener {
	private static final long serialVersionUID = 3772460482889183076L;

	// Definitions
	private final static String CONFIG_FILE = "config.cfg";
	private final static String CLUSTER_FILE = "cluster.cfg";
	private final static String CLIENTS_FILE = "clients.cfg";
	private final static String NEWLINE = System.getProperty("line.separator");

	// Global Vars

	private DefaultTableModel clusterModel, configModel, clientsModel;

	private JTextArea textArea;
	private JButton buttonStartSzenario, buttonStopSzenario, buttonStart,
			buttonStop, buttonMehrwege, buttonOffroadFix, buttonOffroadLive,
			buttonCartography, buttonSniffer, buttonSLAM;
	private JTextField setIDField;
	private JLabel setIDLabel;

	private File fconfig, fclients, fcluster, dir;

	private JPopupMenu popup;

	private Sniffer sniff;

	private JFrame mapWindow;

	private TuioReceiver tuioRec = null;

	private int targetId = 12;

	/**
	 * Default Constructor. Initializes the GUI and checks if the TUIO Client is
	 * in the same directory. Otherwise it will prombt the user to define the
	 * location.
	 */
	public ConfigGUI() {
		super("GPS Config V1.0");

		try {
			// UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
			UIManager
					.setLookAndFeel("com.sun.java.swing.plaf.nimbus.NimbusLookAndFeel");
		} catch (Exception e) {
		}

		initGUIObjects();
		initMenuBar();
		initPopupMenu();

		buttonStart.setEnabled(false);
		buttonStop.setEnabled(false);

		this.setDefaultCloseOperation(EXIT_ON_CLOSE);
		this.pack();
		this.setLocationRelativeTo(null);
		this.setVisible(true);
	}

	/**
	 * Quick function to create a JTable with a DefaultTableModel
	 * 
	 * @param names
	 * @param name
	 * @param tooltip
	 * @param panel
	 * @return
	 */
	private DefaultTableModel createTable(String[] names, String name,
			String tooltip, JPanel panel) {
		DefaultTableModel model = new DefaultTableModel(names, 0);
		JTable table = new JTable(model);
		table.addMouseListener(this);
		table.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);

		JScrollPane scroll = new JScrollPane(table);
		scroll.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
		scroll.setBorder(new TitledBorder(name));
		scroll.addMouseListener(this);
		scroll.setToolTipText(tooltip);

		panel.add(scroll);

		return model;
	}

	/**
	 * Initializes nearly all GUI objects of the application
	 */
	private void initGUIObjects() {
		// CONFIG VIEW
		JPanel configPane = new JPanel();
		configPane.setLayout(new GridLayout(3, 1));

		// CONFIG TABLES
		String cluster_description = new String(
				"<html>A cluster is:<br>"
						+ "a UDP port, which receives the TUIO Messages of 1 camera<br>"
						+ "3 points 0, 1, 2 with their respective X, Y coordinates in world system<br>"
						+ "IMPORTANT: 0 to 1 spans the x-axis");
		String clients_description = new String(
				"<html>A client is 1 car and contains:<br>"
						+ "the fiducial symbol ID of the car<br>"
						+ "the COM port the car is connected to<br>"
						+ "the fiducial symbol ID the car should follow.");
		String config_description = new String(
				"<html>The config must contain an option followed by a value:<br>"
						+ "example: option: \"Timeout\" value: \"500\"<br>"
						+ "this sends all TUIO Messages every 500 milliseconds");

		clusterModel = createTable(new String[] { "UDP Port", "X0", "Y0", "X1",
				"Y1", "X2", "Y2" }, "Cluster", cluster_description, configPane);
		clientsModel = createTable(new String[] { "Car Symbol Id", "COM Port",
				"Symbol Id to follow" }, "Clients", clients_description,
				configPane);
		configModel = createTable(new String[] { "Option", "Value" }, "Config",
				config_description, configPane);

		// TUIO Client View
		JPanel pane = new JPanel();
		pane.setLayout(new BorderLayout());

		// BUTTON PANEL
		JPanel buttonPane = new JPanel();

		buttonPane.setLayout(new GridLayout(3, 5));
		buttonPane.setBorder(new TitledBorder("TUIO Client Control"));
		buttonPane.setPreferredSize(new Dimension(10, 150));

		buttonStart = createButton("Start TUIO Client", CMD.START_TUIO.name(),
				buttonPane);
		buttonStop = createButton("Stop TUIO Client", CMD.STOP_TUIO.name(),
				buttonPane);
		buttonStopSzenario = createButton("Stop Szenario", CMD.CAR_STOP.name(),
				buttonPane);
		buttonStartSzenario = createButton("Starte Szenario",
				CMD.CAR_START.name(), buttonPane);
		buttonMehrwege = createButton("Mehrwegenavigation",
				CMD.CAR_MEHRWEGE.name(), buttonPane);
		buttonOffroadFix = createButton("Offroad fixed",
				CMD.CAR_OFFROADFIX.name(), buttonPane);
		buttonOffroadLive = createButton("Offroad live",
				CMD.CAR_OFFROADLIVE.name(), buttonPane);
		buttonCartography = createButton("Kartographie", CMD.CAR_MAP.name(),
				buttonPane);
		createButton("Show map", CMD.CAR_SHOWMAP.name(), buttonPane);
		buttonSniffer = createButton("Start Funk Modul", CMD.CAR_FUNK.name(),
				buttonPane);
		buttonSLAM = createButton("SLAM", CMD.CAR_SLAM.name(), buttonPane);

		enableButtons();

		setIDLabel = new JLabel();
		setIDLabel.setText("Set ID of RFModul:");
		buttonPane.add(setIDLabel);

		setIDField = new JTextField();
		setIDField.setEditable(true);
		setIDField.setText(String.valueOf(targetId));
		setIDField.addFocusListener(this);
		setIDField.setName("rfId");
		buttonPane.add(setIDField);

		// TEXT AREA
		textArea = new JTextArea();
		textArea.setEditable(false);
		textArea.setLineWrap(true);
		textArea.setWrapStyleWord(true);

		JScrollPane textAreaScrollPane = new JScrollPane(textArea);
		textAreaScrollPane.setBorder(new TitledBorder("TUIO Client Output"));
		textAreaScrollPane
				.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);

		// ADD ALL PANES TO WINDOW
		pane.add(textAreaScrollPane, BorderLayout.CENTER);
		// pane.add(setID, BorderLayout.PAGE_END);
		pane.add(buttonPane, BorderLayout.SOUTH);

		// TabbedPane
		JTabbedPane tab = new JTabbedPane();
		tab.setPreferredSize(new Dimension(800, 600));

		tab.add(pane, "TUIO Client");
		tab.add(configPane, "Config");

		this.setContentPane(tab);
	}

	/**
	 * Initializes the menubar
	 */
	private void initMenuBar() {
		JMenu file = new JMenu("Datei");

		JMenuItem save = new JMenuItem("Speichern");
		save.addActionListener(this);
		save.setActionCommand(CMD.SAVE_CONFIG.name());

		JMenuItem exit = new JMenuItem("Exit");
		exit.addActionListener(this);
		exit.setActionCommand(CMD.EXIT.name());

		file.add(save);
		file.addSeparator();
		file.add(exit);

		JMenuBar bar = new JMenuBar();
		bar.add(file);

		this.setJMenuBar(bar);
	}

	/**
	 * Initializes the PopupMenu
	 */
	private void initPopupMenu() {
		popup = new JPopupMenu();

		JMenuItem add = new JMenuItem("Neu");
		add.addActionListener(this);
		add.setActionCommand(CMD.ADD.name());

		JMenuItem del = new JMenuItem("Löschen");
		del.addActionListener(this);
		del.setActionCommand(CMD.DEL.name());

		popup.add(add);
		popup.add(del);
	}

	private void enableButtons() {
		buttonStopSzenario.setEnabled(sniff != null);
		buttonStartSzenario.setEnabled(sniff != null);
		buttonMehrwege.setEnabled(sniff != null);
		buttonOffroadFix.setEnabled(sniff != null);
		buttonOffroadLive.setEnabled(sniff != null);
		buttonCartography.setEnabled(sniff != null);
		buttonSLAM.setEnabled(sniff != null);
	}

	/**
	 * quick function to create a button, add the ActionListener, ...
	 * 
	 * @param title
	 * @param action
	 * @param panel
	 * @return
	 */
	private JButton createButton(String title, String action, JPanel panel) {
		JButton tmp = new JButton(title);
		tmp.addActionListener(this);
		tmp.setActionCommand(action);

		if (panel != null)
			panel.add(tmp);

		return tmp;
	}

	/**
	 * Saves all config files
	 */
	private void saveFiles() {
		if (fconfig != null && fclients != null && fcluster != null) {
			try {
				saveFile(fconfig, configModel, " = ", 2);
				saveFile(fclients, clientsModel, " ", 3);
				saveFile(fcluster, clusterModel, " ", 7);
			} catch (IOException e) {
				JOptionPane.showMessageDialog(this, e.getMessage(), "Error",
						JOptionPane.ERROR_MESSAGE);
			}
		}
	}

	/**
	 * Saves a specific config file
	 * 
	 * @param file
	 * @param model
	 * @param split
	 * @param len
	 * @throws IOException
	 */
	private void saveFile(File file, DefaultTableModel model, String split,
			int len) throws IOException {
		FileWriter writer = new FileWriter(file);

		for (int r = 0; r < model.getRowCount(); r++) {
			String out = "";

			for (int c = 0; c < model.getColumnCount(); c++) {
				Object o = model.getValueAt(r, c);

				if (o == null) {
					out = null;
					break;
				}

				String s = ((String) o).trim();

				if (s.equals("")) {
					out = null;
					break;
				}

				if (c > 0)
					out += split;

				out += s;
			}

			if (out != null) {
				writer.write(out + NEWLINE);
			}
		}

		writer.close();
	}

	/**
	 * loads all config files
	 */
	private void loadFiles() {
		fconfig = new File(dir.getAbsolutePath() + File.separator + CONFIG_FILE);
		fclients = new File(dir.getAbsolutePath() + File.separator
				+ CLIENTS_FILE);
		fcluster = new File(dir.getAbsolutePath() + File.separator
				+ CLUSTER_FILE);

		try {
			loadFile(fconfig, configModel, "=", 2);
			loadFile(fclients, clientsModel, " ", 3);
			loadFile(fcluster, clusterModel, " ", 7);
		} catch (IOException e) {
			JOptionPane.showMessageDialog(this, e.getMessage(), "Error",
					JOptionPane.ERROR_MESSAGE);
		}
	}

	/**
	 * loads a specific config file
	 * 
	 * @param file
	 * @param model
	 * @param split
	 * @param len
	 * @throws IOException
	 */
	private void loadFile(File file, DefaultTableModel model, String split,
			int len) throws IOException {
		model.setRowCount(0); // Clear Table

		if (file.exists()) {
			BufferedReader reader = new BufferedReader(new FileReader(file));

			String str;
			while ((str = reader.readLine()) != null) {
				String[] tmp = str.split(split);

				if (tmp.length == len) {
					for (int i = 0; i < tmp.length; i++)
						tmp[i] = tmp[i].trim();

					model.addRow(tmp);
				}
			}
			reader.close();
		}
	}

	/**
	 * start the TUIO Client
	 */
	private void startTuioClient() {
		tuioRec = new TuioReceiver();
		tuioRec.addMonitoredArea(3333);
		tuioRec.addListener(this);

		buttonStart.setEnabled(false);
		buttonStop.setEnabled(true);
	}

	/**
	 * stops the TUIO Client
	 */
	private void stopTuioClient() {
		if (tuioRec != null) {
			tuioRec.closeAllMonitoredAreas();
			tuioRec.stop();
			tuioRec = null;

			buttonStart.setEnabled(true);
			buttonStop.setEnabled(false);
		}
	}

	/**
	 * deletes or adds an row
	 * 
	 * @param cmd
	 */
	private void deleteOrAddRow(CMD cmd) {
		JTable table = null;

		if (popup.getInvoker() instanceof JScrollPane)
			table = (JTable) ((JScrollPane) popup.getInvoker()).getViewport()
					.getComponent(0);
		else if (popup.getInvoker() instanceof JTable)
			table = (JTable) popup.getInvoker();

		if (table != null) {
			DefaultTableModel model = (DefaultTableModel) table.getModel();

			if (cmd == CMD.DEL)
				model.removeRow(table.getSelectedRow());
			else
				model.addRow(new String[model.getColumnCount()]);
		}
	}

	/**
	 * sends the request to start the szenario
	 */
	private void carStart() {
		sniff.send(new GPSControlMessage(true, sniff.getAmber8425()
				.getChannel(), targetId, sniff.getAmber8425().getRfId(), 1, 0,
				EGPSControlMessageType.CONTROL_START, 9/* Config->idToFollow */));
	}

	/**
	 * sends the request stop the szenario
	 */
	private void carStop() {
		sniff.send(new GPSControlMessage(true, sniff.getAmber8425()
				.getChannel(), targetId, sniff.getAmber8425().getRfId(), 1, 0,
				EGPSControlMessageType.CONTROL_STOP, 0));
	}

	/**
	 * sends the request to start the szenario Mehrwege
	 */
	private void carMehrwege() {
		sniff.send(new GPSControlMessage(true, sniff.getAmber8425()
				.getChannel(), targetId, sniff.getAmber8425().getRfId(), 1, 0,
				EGPSControlMessageType.CONTROL_FOLLOW_LOWEST_ID, 254));
	}

	/**
	 * sends the request to start the szenario Offroad Fix
	 */
	private void carOffroadFix() {
		sniff.send(new GPSControlMessage(true, sniff.getAmber8425()
				.getChannel(), targetId, sniff.getAmber8425().getRfId(), 1, 0,
				EGPSControlMessageType.CONTROL_FOLLOW_OFFROAD, 9/*
																 * TODO:
																 * Config->
																 * idToFollow
																 */));
	}

	/**
	 * sends the request to start the szenario Offroad Live
	 */
	private void carOffroadLive() {
		sniff.send(new GPSControlMessage(true, sniff.getAmber8425()
				.getChannel(), targetId, sniff.getAmber8425().getRfId(), 1, 0,
				EGPSControlMessageType.CONTROL_FOLLOW_OFFROAD_DYN, 254));
	}

	/**
	 * sends the request to start the szenario Offroad Live
	 */
	private void carMap() {
		sniff.send(new GPSControlMessage(true, sniff.getAmber8425()
				.getChannel(), targetId, sniff.getAmber8425().getRfId(), 1, 0,
				EGPSControlMessageType.CONTROL_CARTOGRAPHY, 9/*
															 * Config->idToFollow
															 */));
	}

	private void carSLAM() {
		sniff.send(new GPSControlMessage(true, sniff.getAmber8425()
				.getChannel(), targetId, sniff.getAmber8425().getRfId(), 1, 0,
				EGPSControlMessageType.CONTROL_SLAM, 9/* Config->idToFollow */));
	}

	/**
	 * @brief starts the Packet Sniffer
	 */
	private void carSniffer() {
		buttonStart.setEnabled(true);
		buttonSniffer.setEnabled(false);
		sniff = new Sniffer();
		enableButtons();
	}

	/**
	 * @brief visualizes the received map
	 */
	private void carShowMap() {
		// hier neues Fenster erstellen mit aktueller Karte
		try {
			mapWindow = new JFrame("Aktuelle Karte");
			DrawPanel draw = new DrawPanel();
			mapWindow.getContentPane().add(draw);
			mapWindow.pack();
			mapWindow.setSize(800, 800);
			mapWindow.setDefaultCloseOperation(DISPOSE_ON_CLOSE);
			mapWindow.setVisible(true);
		} catch (FileNotFoundException e) {
			JOptionPane.showMessageDialog(this,
					"Bisher wurde keine Map-Datei angelegt!",
					"Keine Map gefunden", JOptionPane.WARNING_MESSAGE);
		}
	}

	private void rfID() {
		this.targetId = Integer.parseInt(setIDField.getText());
	}

	/**
	 * ActionListener implementation. Used for all event handling in the GUI
	 */
	public void actionPerformed(ActionEvent e) {
		CMD cmd = CMD.valueOf(e.getActionCommand());

		switch (cmd) {
			case EXIT:
				System.exit(0);
				break;
			case DEL:
			case ADD:
				deleteOrAddRow(cmd);
				break;
			case STOP_TUIO:
				stopTuioClient();
				break;
			case SAVE_CONFIG:
				saveFiles();
				break;
			case START_TUIO:
				startTuioClient();
				break;
			case CAR_START:
				carStart();
				break;
			case CAR_STOP:
				carStop();
				break;
			case CAR_MEHRWEGE:
				carMehrwege();
				break;
			case CAR_OFFROADFIX:
				carOffroadFix();
				break;
			case CAR_OFFROADLIVE:
				carOffroadLive();
				break;
			case CAR_MAP:
				carMap();
				break;
			case CAR_FUNK:
				carSniffer();
				break;
			case CAR_SHOWMAP:
				carShowMap();
				break;
			case CAR_SLAM:
				carSLAM();
				break;
		}
	}

	/**
	 * invoked if application is started
	 * 
	 * @param args
	 *            arguments
	 */
	public static void main(String args[]) {
		new ConfigGUI();
	}

	/**
	 * MouseListener for the popup menu
	 */
	@Override
	public void mouseClicked(MouseEvent e) {
		if (e.getSource() instanceof JTable) {
			if (e.getButton() == MouseEvent.BUTTON3) {
				JTable table = (JTable) e.getSource();

				int row = table.rowAtPoint(e.getPoint());

				table.setRowSelectionInterval(row, row);

				popup.getComponent(1).setEnabled(true);
				popup.show(table, e.getX(), e.getY());
			}
		} else if (e.getSource() instanceof JScrollPane) {
			if (e.getButton() == MouseEvent.BUTTON3) {
				popup.getComponent(1).setEnabled(false);
				popup.show((JComponent) e.getSource(), e.getX(), e.getY());
			}
		}
	}

	/**
	 * Possible events, which are interpreted by the ActionListener
	 * 
	 * @author Jonas Sch�nichen, Nicolas Weber
	 * 
	 */
	enum CMD {
		SAVE_CONFIG,
		START_TUIO,
		STOP_TUIO,
		EXIT,
		ADD,
		DEL,
		CAR_START,
		CAR_STOP,
		CAR_MEHRWEGE,
		CAR_OFFROADFIX,
		CAR_OFFROADLIVE,
		CAR_MAP,
		CAR_FUNK,
		CAR_SHOWMAP,
		CAR_SLAM
	}

	@Override
	public void mouseEntered(MouseEvent arg0) {
	}

	@Override
	public void mouseExited(MouseEvent arg0) {
	}

	@Override
	public void mousePressed(MouseEvent arg0) {
	}

	@Override
	public void mouseReleased(MouseEvent arg0) {
	}

	@Override
	public void focusGained(FocusEvent arg0) {
	}

	@Override
	public void focusLost(FocusEvent e) {
		String sourceName = ((JComponent) e.getSource()).getName();

		if (sourceName.equals("rfId"))
			rfID();
	}

	@Override
	public void tuioObjectData(TuioObject object, Point point, int camPort) {
		GPSUpdateCoordMessage msg = new GPSUpdateCoordMessage(true, sniff
				.getAmber8425().getChannel(), targetId, sniff.getAmber8425()
				.getRfId(), 1, 0, object, point, camPort);
		sniff.send(msg);
	}

}
