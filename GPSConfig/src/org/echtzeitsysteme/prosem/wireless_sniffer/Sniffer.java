package org.echtzeitsysteme.prosem.wireless_sniffer;

import org.echtzeitsysteme.prosem.messages.DebugMessage;
import org.echtzeitsysteme.prosem.messages.Message;
import org.echtzeitsysteme.prosem.wireless_sniffer.controller.Controller;
import org.echtzeitsysteme.prosem.wireless_sniffer.model.Model;
import org.echtzeitsysteme.prosem.wireless_sniffer.model.amber8425.Amber8425;
import org.echtzeitsysteme.prosem.wireless_sniffer.model.filters.Filter;
import org.echtzeitsysteme.prosem.wireless_sniffer.view.View;

public class Sniffer {
	private Model model;
	private View view;
	private Controller controller;
	private Amber8425 amber8425;

	public Sniffer() {
		amber8425 = new Amber8425();
		model = new Model();
		controller = new Controller(model);
		controller.setAmber8425(amber8425);

		Filter fDebug = model.getFilterQueue().createFilter();
		fDebug.setFilterName("Debug messages");
		fDebug.setEnabled(true);
		fDebug.setMessageType(0);
		fDebug.setMessageClass(DebugMessage.class);
		fDebug.setPrintAsHex(false);
		model.getFilterQueue().createFilter().setFilterName("Filter2");

		view = new View(model, controller);
		controller.setView(view);
		controller.updateComPortList();
	}

	public void send(Message msg) {
		controller.sendMessage(msg);
	}

	public Amber8425 getAmber8425() {
		return amber8425;
	}

}
