package org.echtzeitsysteme.prosem.wireless_sniffer.model;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashSet;

import org.echtzeitsysteme.prosem.Paths;
import org.echtzeitsysteme.prosem.messages.Message;
import org.echtzeitsysteme.prosem.wireless_sniffer.model.filters.FilterQueue;

public class Model {

	private ArrayList<Message> messageStack = new ArrayList<Message>();
	private FilterQueue filterQueue = new FilterQueue();

	private HashSet<ModelChangeListener> listeners = new HashSet<ModelChangeListener>();

	public Model() {
	}

	public void addListener(ModelChangeListener l) {
		listeners.add(l);
	}

	public void removeListener(ModelChangeListener l) {
		listeners.remove(l);
	}

	private void notifyAdded(Message msg) {
		int pos = messageStack.indexOf(msg);
		for (ModelChangeListener l : listeners)
			l.messageAdded(this, msg, pos);
	}

	private void notifyUpdated(Message msg) {
		int pos = messageStack.indexOf(msg);
		for (ModelChangeListener l : listeners)
			l.messageUpdated(this, msg, pos);
	}

	private void notifyCleared() {
		for (ModelChangeListener l : listeners)
			l.messagesCleared(this);
	}

	/**
	 * @brief is called when a new Message is coming. It appends the message to
	 *        the message stack without any modifications
	 * @param msg
	 *            object
	 */
	public void addMessage(Message msg) {
		int firstelement, amount, amountmax;
		int longelement;

		messageStack.add(msg);
		notifyAdded(msg);
		
		amount = 0;
		amountmax = 5;

		// Schreibe die Informationen in die Textdatei
		try {
			BufferedWriter writer = new BufferedWriter(new FileWriter(new File(
					Paths.getProgramPath(), "test.txt"), true));
			String str = ""; // msg.msgDataInt.toString();

			if (msg.getData().length > 1) {
				firstelement = msg.getData()[0];

				// Block wurde gesendet
				if (firstelement == 22) {
					for (int i = 1; i < 15; i++) {
						// siehe message = (gps_reducedData_t*)packet->userdata;
						if (amount < amountmax) {
							if (msg.getData()[i] == 1) { // Startzeichen, ein
															// weiteres Zeichen
															// ist sicher evtl
															// drei (stuffing)
								longelement = msg.getData()[++i];
								System.out.println("long =: " + longelement);
								if (longelement == 1) {
									if (++i < msg.getData().length) {
										if (msg.getData()[i] == 1)
											longelement = msg.getData()[++i];
										else {
											str = str + longelement + ",";
											amount++;
											continue;
										}
									}
								}
								// longelement = msg.msgDataInt[++i];
								longelement = (longelement << 8);
								System.out.println("longshift =: "
										+ longelement);
								longelement |= msg.getData()[++i];
								if ((longelement >> 8) == 1)
									i++;
								if (longelement > 32768)
									longelement = (65536 - longelement) * (-1);
								str = str + longelement + ",";
								amount++;
								amountmax++;
							} else {
								amount++;
								str = str + (byte) msg.getData()[i] + ",";
							}
						}
					}
					str = str + "\n";
					// Schreiben in die .txt
					writer.write(str);
				}
			}
			writer.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public void updateMessage(Message msg) {
		notifyUpdated(msg);
	}

	/**
	 * @breif returns the current filter queue
	 * @return filterqueue
	 */
	public FilterQueue getFilterQueue() {
		return filterQueue;
	}

	/**
	 * @brief reset fo the message stack
	 */
	public void resetMessageTable() {
		this.messageStack = new ArrayList<Message>();
		notifyCleared();
	}

	// ##################### Getter and Setter ######################

	public int getMessageStackSize() {
		return messageStack.size();
	}

	public Message getMessage(int index) {
		return messageStack.get(index);
	}

}
