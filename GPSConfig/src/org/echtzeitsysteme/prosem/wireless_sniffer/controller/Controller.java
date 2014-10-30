package org.echtzeitsysteme.prosem.wireless_sniffer.controller;

import gnu.io.NoSuchPortException;
import gnu.io.PortInUseException;

import java.awt.Color;

import org.echtzeitsysteme.prosem.messages.Message;
import org.echtzeitsysteme.prosem.wireless_sniffer.model.Model;
import org.echtzeitsysteme.prosem.wireless_sniffer.model.amber8425.Amber8425;
import org.echtzeitsysteme.prosem.wireless_sniffer.model.amber8425.CommandListener;
import org.echtzeitsysteme.prosem.wireless_sniffer.model.amber8425.commands.Command;
import org.echtzeitsysteme.prosem.wireless_sniffer.model.filters.FilterQueue;
import org.echtzeitsysteme.prosem.wireless_sniffer.view.View;

public class Controller implements CommandListener {
	private Model model;
	private View view = null;
	private Amber8425 amber8425;

	public Controller(Model m) {
		this.model = m;
	}

	/**
	 * @brief tells the view to update the program control panel
	 */
	public void updateComPortList() {
		this.view.updateComPortList(this.amber8425.getPortNames());
	}

	public void updateFooterInfoPanel(String status, String portName, Color c) {
		this.view.updateFooterInfoPanel(status, portName, c);
	}
	
	public FilterQueue getFilterQueue() {
		return model.getFilterQueue();
	}

	/**
	 * @param port
	 *            To sniff on
	 * @brief starts sniffing on the selected comport
	 */
	public void startSniffing(String port) {
		this.resetMessageTable();
		try {
			this.amber8425.startOnPort(port);
			updateFooterInfoPanel("Verbindung hergestellt!", port, Color.black);
		} catch (NoSuchPortException e) {
			updateFooterInfoPanel(
					"Konnte keine Verbindung mit ComPort herstellen!", port,
					Color.red);
			e.printStackTrace();
		} catch (PortInUseException e) {
			updateFooterInfoPanel(
					"ComPort ist verbunden mit einem anderen Programm!", port,
					Color.red);
			e.printStackTrace();
		}
	}

	/**
	 * @brief stop sniffing
	 */
	public void stopSniffing() {
		this.amber8425.stop();
		updateFooterInfoPanel("---", "---", Color.black);
	}

	/**
	 * @brief tells the model and view to reset the message stack and remove all
	 *        messages from the gui
	 */
	public void resetMessageTable() {
		this.model.resetMessageTable();
	}

	// ##################### Getter and Setter ######################

	public Amber8425 getAmber8425() {
		return amber8425;
	}

	public void setAmber8425(Amber8425 amber8425) {
		this.amber8425 = amber8425;
		amber8425.addCommandListener(this);
	}

	public void setView(View view) {
		this.view = view;
	}

	public void sendMessage(Message msg) {
		model.addMessage(msg);
		amber8425.sendMessage(msg);
	}

	public String getSelectedComPort() {
		return view.getSelectedPort();
	}

	@Override
	public void messageReceived(Amber8425 src, Command cmd) {
		if (cmd instanceof Message)
			model.addMessage((Message) cmd);
	}

	@Override
	public void messageUpdated(Amber8425 src, Command cmd) {
		if (cmd instanceof Message)
			model.updateMessage((Message) cmd);
	}

}
