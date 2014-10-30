package org.echtzeitsysteme.prosem.gps;

import java.awt.Color;
import java.awt.Graphics;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

import javax.swing.JPanel;

import org.echtzeitsysteme.prosem.Paths;

public class DrawPanel extends JPanel {
	private static final long serialVersionUID = -6991857032282632553L;

	private boolean mapcomplete;

	DrawPanel() throws FileNotFoundException {
	}

	/**
	 * @brief Zeichnen des Koordinatensystems
	 * @param x
	 * @param y
	 * @param txt
	 * @param g
	 * @param rechts
	 *            : true wenn Pfeil nach rechts
	 */
	private void arrow(int x, int y, String txt, Graphics g, boolean rechts) {
		if (rechts) {
			g.fillRect(x - 1, y, 1, 1);
			g.fillRect(x - 2, y + 1, 1, 1);
			g.fillRect(x - 3, y + 2, 1, 1);
			g.fillRect(x - 4, y + 3, 1, 1);
			g.fillRect(x - 5, y + 4, 1, 1);
			g.fillRect(x - 1, y, 1, 1);
			g.fillRect(x - 2, y - 1, 1, 1);
			g.fillRect(x - 3, y - 2, 1, 1);
			g.fillRect(x - 4, y - 3, 1, 1);
			g.fillRect(x - 5, y - 4, 1, 1);
			g.drawString(txt, x + 10, y);
		} else {
			g.fillRect(x, y + 1, 1, 1);
			g.fillRect(x + 1, y + 2, 1, 1);
			g.fillRect(x + 2, y + 3, 1, 1);
			g.fillRect(x + 3, y + 4, 1, 1);
			g.fillRect(x + 4, y + 5, 1, 1);
			g.fillRect(x, y + 1, 1, 1);
			g.fillRect(x - 1, y + 2, 1, 1);
			g.fillRect(x - 2, y + 3, 1, 1);
			g.fillRect(x - 3, y + 4, 1, 1);
			g.fillRect(x - 4, y + 5, 1, 1);
			g.drawString(txt, x - 20, y - 2);
		}

	}

	/**
	 * auslesen der .txt und Darstellung der Rechtecke
	 */
	@Override
	protected void paintComponent(Graphics g) {
		int diffx, diffy;
		super.paintComponent(g);
		// Txt lesen
		String[] data;

		mapcomplete = false;
		try {
			BufferedReader fileReader = new BufferedReader(new FileReader(
					new File(Paths.getProgramPath(), "test.txt")));

			String zeile = null;
			while (mapcomplete != true) {
				// Rechteck symbolisiert komplette Karte. Beide Achsen gehen von
				// -327 bis 327.
				g.fillRect(10, 10, 654, 654);
				g.setColor(Color.gray);
				g.drawLine(337, 10, 337, 664);
				arrow(664, 337, "x - Achse", g, true);
				g.drawLine(10, 337, 664, 337);
				arrow(337, 10, "y - Achse", g, false);
				g.setColor(Color.white);
				while ((zeile = fileReader.readLine()) != null) {
					data = zeile.split(",");
					if (data.length == 5) { // Kann komplette Quadrate setzen.
											// Vermindert Anzahl der
											// �bertragungen.
						int directionx = 1, directiony = 1;
						if (Integer.parseInt(data[0]) > Integer
								.parseInt(data[2]))
							directionx = -1;
						if (Integer.parseInt(data[1]) > Integer
								.parseInt(data[3]))
							directiony = -1;
						diffx = Math.abs(Integer.parseInt(data[0])
								- Integer.parseInt(data[2]));
						diffy = Math.abs(Integer.parseInt(data[1])
								- Integer.parseInt(data[3]));
						int a;
						int i, j;

						// Hindernisse darstellen
						if (Integer.parseInt(data[4]) == 13) {
							Color original = g.getColor();
							g.setColor(Color.RED);
							g.fillRect(337 + Integer.parseInt(data[2]) - 2,
									337 - Integer.parseInt(data[3]) - 2, 4, 4);
							g.setColor(original);
							// break;
						}

						// Freie Fl�che darstellen (selbe Strukur wie beim
						// Speichern auf dem Auto)
						// Karte hat andere Aufl�sung als die des Autos ->
						// gr��eren Abst�nden
						if ((diffx - diffy) >= 6 || (diffy <= 3 && diffx >= 3)) {
							if (diffy <= 3)
								directiony = directiony * (-1);
							if (Integer.parseInt(data[4]) == 12)
								diffy = 5;
							else
								diffy = 9;
							g.fillRect(337 + Integer.parseInt(data[0]), 337
									- Integer.parseInt(data[1])
									- (directiony * diffy) / 2, diffx
									* directionx, diffy * directiony);
							continue;
						}

						if ((diffx - diffy) >= -6 || (diffx <= 3 && diffy >= 3)) {
							if (diffx <= 3)
								directiony = directiony * (-1);
							if (Integer.parseInt(data[4]) == 12)
								diffx = 5;
							else
								diffx = 9;
							g.fillRect(337 + Integer.parseInt(data[0])
									- (directionx * diffx) / 2,
									337 - Integer.parseInt(data[1]), diffx
											* directionx, diffy * directiony);
							continue;
						}

						if (Integer.parseInt(data[4]) == 12) {
							a = 5;
							if (diffx > diffy)
								diffy = diffx;
							else
								diffx = diffy;
							for (i = 0; i < diffx; i++) {
								for (j = 0; j < diffy; j++) {
									if ((j > 5 && j - i - a < 1 + i && i < 6)
											|| (i > 5 && j - 5 + a > -6 && j
													- 5 + a < 6)
											|| (j < 6 && i < 6))
										g.fillRect(
												337 + Integer.parseInt(data[0])
														+ directionx * i,
												337 - Integer.parseInt(data[1])
														- directiony * j, 1, 1);
								}
								a--;
							}
						} else {
							a = 9;
							if (diffx > diffy)
								diffy = diffx;
							else
								diffx = diffy;
							for (i = 0; i < diffx; i++) {
								for (j = 0; j < diffy; j++) {
									if ((j > 9 && j - i - a < 1 + i && i < 10)
											|| (i > 9 && j - 9 + a > -10 && j
													- 9 + a < 10)
											|| (j < 10 && i < 10))
										g.fillRect(
												337 + Integer.parseInt(data[0])
														+ directionx * i,
												337 - Integer.parseInt(data[1])
														- directiony * j, 1, 1);
								}
								a--;
							}
						}

					} else
						g.fillRect(327 + Integer.parseInt(data[0]),
								327 - Integer.parseInt(data[1]), 1, 1); // KS
																		// beachten!
																		// x-Achse
																		// normal.
																		// y-Achse
																		// Sonderfall
					System.out.println("Gelesene Zeile: " + zeile);
				}

				mapcomplete = true;
				System.out.println("fertig");
			}
			
			fileReader.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

}
