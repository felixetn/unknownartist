/**
 * 
 */
package org.echtzeitsysteme.prosem.wireless_sniffer.model.amber8425;

import gnu.io.CommPortIdentifier;
import gnu.io.NoSuchPortException;
import gnu.io.PortInUseException;
import gnu.io.SerialPort;
import gnu.io.SerialPortEvent;
import gnu.io.SerialPortEventListener;
import gnu.io.UnsupportedCommOperationException;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Arrays;
import java.util.Enumeration;
import java.util.HashSet;
import java.util.TooManyListenersException;
import java.util.Vector;

import org.echtzeitsysteme.prosem.messages.Message;
import org.echtzeitsysteme.prosem.wireless_sniffer.model.amber8425.commands.CMD_GET_REQ;
import org.echtzeitsysteme.prosem.wireless_sniffer.model.amber8425.commands.CMD_SET_REQ;
import org.echtzeitsysteme.prosem.wireless_sniffer.model.amber8425.commands.Command;

/**
 * Provides driver functionality for Amber8425 wireless USB stick
 */
public class Amber8425 implements SerialPortEventListener {
	private final int BAUD_RATE = 115200;
	private final int DATABITS = 8;
	private final int PARITY = 0;
	private final int STOPBITS = 1;

	private SerialPort serialPort;
	private InputStream inputStream;
	private OutputStream outStream;

	private HashSet<CommandListener> commandListeners = new HashSet<CommandListener>();
	private HashSet<SettingChangeListener> settingListeners = new HashSet<SettingChangeListener>();

	private ReceiveStateMachine receiveSM;
	private SendStateMachine sendSM;

	private int rfId = -1;
	private int channel = -1;

	public Amber8425() {
		receiveSM = new ReceiveStateMachine();
		sendSM = new SendStateMachine(this, receiveSM);

		serialPort = null;
	}

	public void addCommandListener(CommandListener l) {
		commandListeners.add(l);
	}

	public void removeCommandListener(CommandListener l) {
		commandListeners.remove(l);
	}

	public void addSettingListener(SettingChangeListener l) {
		settingListeners.add(l);
	}

	public void removeSettingListener(SettingChangeListener l) {
		settingListeners.remove(l);
	}

	private void notifyListenersMessageReceived(Command cmd) {
		for (CommandListener l : commandListeners)
			l.messageReceived(this, cmd);
	}

	void notifyListenersMessageUpdate(Command cmd) {
		for (CommandListener l : commandListeners)
			l.messageUpdated(this, cmd);
	}

	void notifyListenersSettingsChange() {
		for (SettingChangeListener l : settingListeners)
			l.wirelessSettingsChanged(this, rfId, channel);
	}

	public Vector<String> getPortNames() {
		@SuppressWarnings("unchecked")
		Enumeration<CommPortIdentifier> portEnum = CommPortIdentifier
				.getPortIdentifiers();
		Vector<String> ports = new Vector<String>();

		while (portEnum.hasMoreElements()) {
			CommPortIdentifier portIdentifier = portEnum.nextElement();
			if (portIdentifier.getPortType() == CommPortIdentifier.PORT_SERIAL) {
				ports.add(portIdentifier.getName());
			}
		}
		return ports;
	}

	public void startOnPort(String portName) throws NoSuchPortException,
			PortInUseException {
		if (!getPortNames().contains(portName))
			throw new NoSuchPortException();

		if (serialPort != null) { // close any currently open port
			serialPort.notifyOnDataAvailable(false);
			try {
				inputStream.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
			serialPort.close();
			serialPort.removeEventListener();
		}
		serialPort = null; // clear the portOpen flag

		CommPortIdentifier cpi = CommPortIdentifier.getPortIdentifier(portName);
		serialPort = (SerialPort) cpi.open("CommUtil", 2000);

		// if we succeeded in opening the port, set it up for operation
		try {
			// set the port parameters
			serialPort.setSerialPortParams(BAUD_RATE, DATABITS, STOPBITS,
					PARITY);

			inputStream = serialPort.getInputStream();
			outStream = serialPort.getOutputStream(); //

			serialPort.addEventListener(this);
		} catch (TooManyListenersException e) {
			e.printStackTrace();
		} catch (UnsupportedCommOperationException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}

		// activate the DATA_AVAILABLE notifier
		serialPort.notifyOnDataAvailable(true);

		sendSM.addCommandToQueue(CMD_GET_REQ.getReadSourceIdCommand());
		sendSM.addCommandToQueue(CMD_GET_REQ.getReadChannelCommand());
	}

	public void stop() {
		if (serialPort != null) {
			serialPort.close();
			serialPort = null;
		}
	}

	public boolean isConnected() {
		return serialPort != null;
	}

	/**
	 * @return the rfId
	 */
	public int getRfId() {
		return rfId;
	}

	/**
	 * @param rfId
	 *            the rfId to set
	 */
	public void setRfId(int rfId) {
		if (rfId != this.rfId)
			sendSM.addCommandToQueue(CMD_SET_REQ.getSetSourceIdCommand(rfId));
		sendSM.addCommandToQueue(CMD_GET_REQ.getReadSourceIdCommand());
	}

	/**
	 * @return the channel
	 */
	public int getChannel() {
		return channel;
	}

	/**
	 * @param channel
	 *            the channel to set
	 */
	public void setChannel(int channel) {
		if (channel != this.channel)
			sendSM.addCommandToQueue(CMD_SET_REQ.getSetChannelCommand(channel));
		sendSM.addCommandToQueue(CMD_GET_REQ.getReadChannelCommand());
	}

	OutputStream getOutStream() {
		return outStream;
	}

	public void sendMessage(Message msg) {
		sendSM.addCommandToQueue(msg);
	}

	public void serialEvent(SerialPortEvent event) {
		switch (event.getEventType()) {
			case SerialPortEvent.BI:
			case SerialPortEvent.OE:
			case SerialPortEvent.FE:
			case SerialPortEvent.PE:
			case SerialPortEvent.CD:
			case SerialPortEvent.CTS:
			case SerialPortEvent.DSR:
			case SerialPortEvent.RI:
			case SerialPortEvent.OUTPUT_BUFFER_EMPTY:
				break;
			case SerialPortEvent.DATA_AVAILABLE:

				byte[] readBuffer = new byte[128];
				try {
					// read data
					int numBytes = inputStream.read(readBuffer);
					byte[] msgData = Arrays.copyOf(readBuffer, numBytes);

					if (receiveSM.bytesReceived(msgData)) {
						Command cmd = receiveSM.getCommand();
						while (cmd != null) {
							if (cmd instanceof Message) {
								notifyListenersMessageReceived(cmd);
								sendSM.messageReceived();
							} else {
								sendSM.replyReceived(cmd);
								if (cmd instanceof CMD_SET_REQ) {
									CMD_SET_REQ getRep = (CMD_SET_REQ) cmd;
									switch (getRep.getAddress()) {
										case MAC_SourceAddrLSB:
											rfId = getRep.getValues()[0];
											notifyListenersSettingsChange();
											break;
										case PHY_DefaultChannel:
											channel = getRep.getValues()[0];
											notifyListenersSettingsChange();
											break;
										default:
											break;
									}
								}
							}

							cmd = receiveSM.getCommand();
						}
					}
				} catch (IOException e) {
					e.printStackTrace();
				}
				break;
		}
	}

}
