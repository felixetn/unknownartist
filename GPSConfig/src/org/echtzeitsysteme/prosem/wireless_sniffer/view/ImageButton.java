package org.echtzeitsysteme.prosem.wireless_sniffer.view;

import java.awt.Color;
import java.awt.Cursor;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.net.URL;

import javax.swing.BorderFactory;
import javax.swing.ImageIcon;
import javax.swing.JButton;

public class ImageButton extends JButton {
	private static final long serialVersionUID = 8223954950682535091L;

	public ImageButton(URL url, ActionListener listener, String actionName) {
		super();
		ImageIcon upIcon = new ImageIcon(url);
		setIcon(upIcon);
		setName(actionName);
		addActionListener(listener);
		setBorder(BorderFactory.createEtchedBorder());
		setCursor(new Cursor(Cursor.HAND_CURSOR));
		addMouseListener(new ImageButtonColoring());
	}

	private static class ImageButtonColoring extends MouseAdapter {
		public void mousePressed(MouseEvent evt) {
			evt.getComponent().setBackground(Color.gray);
		}

		public void mouseReleased(MouseEvent evt) {
			evt.getComponent().setBackground(Color.white);
		}
	}

}
