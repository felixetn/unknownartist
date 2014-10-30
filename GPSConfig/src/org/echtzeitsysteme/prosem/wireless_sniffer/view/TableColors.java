package org.echtzeitsysteme.prosem.wireless_sniffer.view;

import java.awt.Color;
import java.util.LinkedHashMap;
import java.util.Vector;

public class TableColors {
	private static TableColors instance = new TableColors();

	private LinkedHashMap<Color, Color> colors = new LinkedHashMap<Color, Color>();

	private TableColors() {
		addColor(Color.WHITE);
		addColor(Color.CYAN);
		addColor(Color.GRAY);
		addColor(Color.GREEN);
		addColor(Color.LIGHT_GRAY);
		addColor(Color.MAGENTA);
		addColor(Color.ORANGE);
		addColor(Color.PINK);
		addColor(Color.RED);
		addColor(Color.YELLOW);
	}

	private void addColor(Color c) {
		colors.put(c, getShadedColor(c));
	}

	private Color getShadedColor(Color c) {
		return new Color(Math.max(c.getRed() - 20, 0), Math.max(
				c.getGreen() - 20, 0), Math.max(c.getBlue() - 20, 0));
	}

	public static TableColors getInstance() {
		return instance;
	}

	public Vector<Color> getBaseColors() {
		Vector<Color> cols = new Vector<Color>();
		cols.addAll(colors.keySet());
		return cols;
	}

	public Color getAlternateColor(Color c) {
		return colors.get(c);
	}
}
