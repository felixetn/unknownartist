/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file map.c
 * @author Team GPS
 * @brief Dieses Modul stellt eine statische Karte mit Hiflsmethoden sowie eine Methode zur Kollisionsberechnung zur Verfügung
 **/

#include "mapMatrix.h"
//#include "math.h"
#include "GPSmessagetypes.h"
#include <stdlib.h>
#include "../sdcard/sdcard.h"
#include "BitwiseMap.h"
#include "../cartography/create_map.h"
#include "math.h"

#include "../api/wireless/wireless.h"

//static uint8_t map[50][50]; //[x][y]
static uint8_t norm = 32 / 10;
static activeSensor = 0;

static int16_t fileID = 0, fileID_obs = 0;
uint16_t fileID_Cart;

obsLeft obs_left;
obsRight obs_right;

/*
 * karte auf 0 initialisieren
 */
void map_init(void) {
	int16_t i = 0, j = 0;
	norm = MAPSIZE / 10; // geteilt durch 10 da WK max 10 Einheiten
//	for(i=0;i<MAPSIZE;i++){
//		for(j=0;j<MAPSIZE;j++){
//			map[i][j] = 0;
//		}
//	}
}

/*
 * Erstellt einen Initialring um den Nullpunkt
 */
void map_init_Cart() {
	uint8_t i = 0, j = 0;
	fileID_Cart = 1;
	norm = MAPSIZE / 10; // geteilt durch 10 da WK max 10 Einheiten
//	for(i=0;i<MAPSIZE;i++){
//		for(j=0;j<MAPSIZE;j++){
//			map[i][j] = 0;
//		}
//	}
}

/**
 * karte definiert einen Bereich von -10 bis 10 in Weltkoordinaten. Gespeichert werden Hindernisse.
 */
void map_init_values() {
	uint8_t i;
	map_init();
	//Eine Linie von Punkt [5][10] bis [25][10] = 1,2 bis 5,2

	/*//Senkrecht
	 for (i = 5; i < 21; i++){
	 set_coord(15,i);
	 set_coord_obstacle(16,i);
	 } */

	for (i = 5; i < 20; i++) {
		set_coord_driven(i, 10);
		//	set_coord(i,11);
	}

	/*
	 set_coord(5,13);
	 set_coord(5,14);
	 set_coord(5,15);
	 set_coord(6,13);
	 set_coord(6,14);
	 set_coord(6,15);
	 set_coord(7,13);
	 set_coord(7,14);
	 set_coord(7,15);
	 */
	/*
	 //Zweites Hindernis (senkrecht)
	 */

}

/**
 * Berechnet die FileID (Kartenabschnitt) entsprechend der Koordinaten
 */
int32_t get_FileID(int16_t coord_x, int16_t coord_y) {
	int32_t id = 0;
	id = GRIDSIZE * 2 * GRIDSIZE - GRIDSIZE; //Nullpunkt bestimmen
	id = id + coord_x / MAPSIZE - (coord_y / MAPSIZE) * GRIDSIZE * 2;
	//Korrekturen bei negativen Koordinaten
	if ((coord_x < 0) && ((coord_x % MAPSIZE) != 0))
		id--;
	if ((coord_y < 0) && ((coord_y % MAPSIZE) != 0))
		id = id + GRIDSIZE * 2;
	return id;
}

/**
 * Struktur zum Zwischenspeichern von Hindernissen initialisieren
 */
void init_obstacles(void) {
	int8_t i;
	for (i = 0; i < DRIVELENGTH / 5 - 1; i++) {
		obs_left.x[i] = 0;
		obs_left.y[i] = 0;
		obs_left.dist[i] = 0;
		obs_left.angle[i] = 0;
		obs_right.x[i] = 0;
		obs_right.y[i] = 0;
		obs_right.dist[i] = 0;
		obs_right.angle[i] = 0;
	}
}

/**
 * Zwischenspeichern von Hindernissen die während der Fahrt durch den linken Sensor erkannt werden
 */
void set_obstacleLeft(int16_t x, int16_t y, uint16_t direction, uint16_t dist) {
	uint8_t i;

	for (i = 0; i < DRIVELENGTH / 5 - 1; i++) {
		if (obs_left.dist != 0) {
			obs_left.x[i] = x;
			obs_left.y[i] = y;
			obs_left.dist[i] = dist;
			obs_left.angle[i] = direction;
		}
	}
}

/**
 * Zwischenspeichern von Hindernissen die während der Fahrt durch den rechten Sensor erkannt werden
 */
void set_obstacleRight(int16_t x, int16_t y, uint16_t direction, uint16_t dist) {
	uint8_t i;

	for (i = 0; i < DRIVELENGTH / 5 - 1; i++) {
		if (obs_right.dist != 0) {
			obs_right.x[i] = x;
			obs_right.y[i] = y;
			obs_right.dist[i] = dist;
			obs_right.angle[i] = direction;
		}
	}
}

/**
 * Lädt einen Abschnitt der Karte abhängig von den gesuchten Koordinaten
 * @param x-Koordinate
 * @param coord_y = y-Koordinate
 *  type = 1/2 wenn BefahrenerBereich/Hinderiskarte
 */
void load_map(int16_t coord_x, int16_t coord_y, uint8_t type) {
	int32_t fileID_new;
	fileID_new = get_FileID(coord_x, coord_y);

	//Anhand der Koordinaten berechnen welche Abschnitte geladen werden müssen
	if (((fileID_new != fileID && type == 1) || (fileID_new != fileID_obs && type == 2)) && fileID_new != -1) { //Wenn das array bereits eingelesen ist kann direkt weitergearbeitet werden
		close_file();
		if (FileExists(fileID_new) == 0) { //Datei existiert
			if (type == 1)
				WriteToFile(fileID, (char*) &DrivenArea_activePlaces, sizeof(DrivenArea_activePlaces), 1); //abspeichern der aktuell geöffneten Karte

			if (type == 2)
				WriteToFile(fileID_obs, (char*) &Obstacle_activePlaces, sizeof(Obstacle_activePlaces), 2);
			close_file();

			if (type == 1) {
				fileID = fileID_new;
				ReadFromFile(fileID, (char*) &DrivenArea_activePlaces, 0, sizeof(DrivenArea_activePlaces), 1);
			}

			if (type == 2) {
				fileID_obs = fileID_new;
				ReadFromFile(fileID_obs, (char*) &Obstacle_activePlaces, 0, sizeof(Obstacle_activePlaces), 2);
			}

		} else { //Segment wurde noch nicht initialisiert
			if (type == 1) {
				WriteToFile(fileID, (char*) &DrivenArea_activePlaces, sizeof(DrivenArea_activePlaces), 1); //abspeichern der aktuell geöffneten Karte
				fileID = fileID_new;
			}

			if (type == 2) {
				WriteToFile(fileID_obs, (char*) &Obstacle_activePlaces, sizeof(Obstacle_activePlaces), 2);
				fileID_obs = fileID_new;
			}

			BitwiseMap_init();
		}
	}
}

/**
 * Speichert die aktuell geöffnete Karte
 */
void store_Map(uint8_t type) {
	if (type == 1)
		WriteToFile(fileID, (char*) &DrivenArea_activePlaces, sizeof(DrivenArea_activePlaces), 1);
	if (type == 2)
		WriteToFile(fileID_obs, (char*) &Obstacle_activePlaces, sizeof(Obstacle_activePlaces), 2);
}

/**
 * löscht alle Kartenabschnitte auf der SD-Karte
 */
void delete_map() {
	uint16_t id_delete;
	for (id_delete = 1; id_delete < GRIDSIZE * GRIDSIZE; id_delete++) {
		delete_file(id_delete, 1);
		os_wait(100);
		delete_file(id_delete, 2);
	}
}

/**
 * Methode zum Abspeichern schräger Hindernisse
 */
void draw_line(int16_t x, int16_t y, int16_t xnew, int16_t ynew) {
	int16_t diffy, diffx, faktorx = 0, faktory = 0, i = 0, j = 0, k;
	int8_t directionx = 1, directiony = 1, toggle = 0;

	diffx = abs16(xnew - x);
	diffy = abs16(ynew - y);

	if (activeSensor != FRONT_SENSOR) {
		if (diffx > RANGESIDE / 5)
			diffx = RANGESIDE / 5;
		if (diffy > RANGESIDE / 5)
			diffy = RANGESIDE / 5;
	} else {
		if (diffx > RANGEFRONT / 5)
			diffx = RANGEFRONT / 5;
		if (diffy > RANGEFRONT / 5)
			diffy = RANGEFRONT / 5;
	}

	if (xnew < x)
		directionx = -1;
	if (ynew < y)
		directiony = -1;

	xnew = abs16(xnew);
	ynew = abs16(ynew);

	if (diffx >= diffy && diffy != 0) {
		faktorx = diffx / diffy;
		if ((diffx % diffy) > diffx / 2)
			toggle = 1; //genauigkeit des faktors erhöhen
	} else if (diffx != 0) {
		faktory = diffy / diffx;
		if ((diffy % diffx) > diffy / 2)
			toggle = 1;
	}

	if (faktorx != 0) {
		while (abs16(x + directionx * i) < xnew || abs16(y + directiony * j) < ynew) {
			for (k = 0; k < faktorx; k++) {
				int16_t tempX = x + directionx * (i + k);
				int16_t tempY = y + directiony * j;
				set_coord_driven(tempX, tempY);
				set_coord_driven(tempX, tempY + directiony);
				set_coord_driven(tempX, tempY - directiony);
				//Wenn nicht FrontSensor dann muss ein breiteres Rechteck gespeichert werden
				if (activeSensor != FRONT_SENSOR) {
					set_coord_driven(tempX, tempY + directiony * 2);
					set_coord_driven(tempX, tempY - directiony * 2);
					set_coord_driven(tempX, tempY + directiony * 3);
					set_coord_driven(tempX, tempY - directiony * 3);
					set_coord_driven(tempX, tempY + directiony * 4);
					set_coord_driven(tempX, tempY - directiony * 4);
				}
				diffx--;
			}
			if (toggle != 0) {
				if (toggle == 1) {
					faktorx++;
					toggle = 2;
				} else {
					toggle = 1;
					faktorx--;
				}
			}
			diffy--;
			i = i + faktorx;
			j++;
		}
	} else {
		while (abs16(x + directionx * i) < xnew || abs16(y + directiony * j) < ynew) {
			for (k = 0; k < faktory; k++) {
				int16_t tempX = x + directionx * j;
				int16_t tempY = y + directiony * (i + k);
				set_coord_driven(tempX, tempY);
				set_coord_driven(tempX + directionx, tempY);
				set_coord_driven(tempX - directionx, tempY);
				if (activeSensor != FRONT_SENSOR) {
					set_coord_driven(tempX + directionx * 2, tempY);
					set_coord_driven(tempX - directionx * 2, tempY);
					set_coord_driven(tempX + directionx * 3, tempY);
					set_coord_driven(tempX - directionx * 3, tempY);
					set_coord_driven(tempX + directionx * 4, tempY);
					set_coord_driven(tempX - directionx * 4, tempY);
				}
				diffy--;
			}
			if (toggle != 0) {
				if (toggle == 1) {
					faktory++;
					toggle = 2;
				} else {
					toggle = 1;
					faktory--;
				}
			}
			diffx--;
			i++;
			j = j + faktory;
		}
	}
}

/**
 * Abspeichern der bereits befahrenen Orte und senden der Sensorwerte an TUIO-Client
 * direction: 1: left, 2: right, 3: front, 4: rear
 * Annahme: Mittelpunkt des Tags befindet sich im Mittelpunkt des Autos. Auto befindet sich nicht in einem Hindernis
 */
void set_free_area(int16_t x, int16_t y, int16_t dist, int8_t direction, uint16_t angle, int8_t type) {
	uint16_t i = 0, j = 0;
	int8_t directionx = 1, directiony = 1;
	int16_t diffx, diffy, startx, starty, endx, endy;
	int32_t xnew, ynew, xhilfe, yhilfe;
	wirelessMessage_t msg;

	msg.destinationId = 5;
	msg.messageType = 12;
	msg.priority = 1;

	activeSensor = direction;
	if (dist == 0)
		return;

	switch (direction) {
		case (RIGHT_SENSOR):
		case (LEFT_SENSOR):

			//+20 als Sonderfall um Lücken in der Karte zu vermeiden
			if (dist > RANGESIDE + 20 && type == DRIVEN_AREA)
				dist = RANGESIDE;

			dist = dist / 5; // 5 Abschnitte pro Einheit, abhängig von der Auflösung der Karte

			if (direction == LEFT_SENSOR) {
				xnew = (dist * cos_Taylor(uint16DegreeRelativeToX(AnglePlus(angle, 16383)))) / 100 + x; // + 90°
				ynew = (dist * sin_Taylor(uint16DegreeRelativeToX(AnglePlus(angle, 16383)))) / 100 + y;
			}
			if (direction == RIGHT_SENSOR) {
				xnew = (dist * cos_Taylor(uint16DegreeRelativeToX(AngleMinus(angle, 16383)))) / 100 + x; // -90°
				ynew = (dist * sin_Taylor(uint16DegreeRelativeToX(AngleMinus(angle, 16383)))) / 100 + y;
			}

			diffx = abs(xnew - x);
			diffy = abs(ynew - y);
			if (diffx > dist)
				diffx = dist;
			if (diffy > dist)
				diffy = dist;

			//Abspeichern der Hindernisse (Karte2)
			if (dist < RANGESIDE / 5 || type == OBSTACLE) {
				set_coord_obstacle(xnew, ynew);
				store_Map(2);
			}

			if (type == DRIVEN_AREA) {
				//Abspeichern der Bereiche die nicht mehr befahren werden dürfen
				//Bereich zwischen new und old auffüllen
				if (xnew < x)
					directionx = -1;
				if (ynew < y)
					directiony = -1;

				if ((angle > 13000 && angle < 19000) || (angle > 46000 && angle < 52000)) {
					//Auto steht quer -> Standardbreite auf der x-Achse (7*5 = Länge des Autos)
					diffx = 7;
					for (i = 0; i <= diffx * 2; i++) {
						for (j = 0; j <= diffy; j++) {
							set_coord_driven(x + directionx * i - directionx * diffx, y + directiony * j);
						}
					}
					//letzte Aktualisierung abspeichern (Für den Fall das set_free_area nicht mehr aufgerufen wird
					store_Map(1);

				} else if ((angle > 29000 && angle < 36000) || angle < 3000 || angle > 62000) {
					//Auto steht längs -> Standardbreite auf der y-Achse
					diffy = 7;
					for (i = 0; i < diffx; i++) {
						for (j = 0; j < diffy * 2; j++) {
							set_coord_driven(x + directionx * i, y + directiony * j - directiony * diffy);
						}
					}
					store_Map(1);

				} else {
					//Speichern von Hindernissen die schräg im Raum liegen
					startx = x;
					starty = y;
					endx = xnew;
					endy = ynew;

					if (direction == LEFT_SENSOR) {
						x = (3 * cos_Taylor(uint16DegreeRelativeToX(AnglePlus(AnglePlus(angle, 16383), 16383)))) / 100
								+ x;
						y = (3 * sin_Taylor(uint16DegreeRelativeToX(AnglePlus(AnglePlus(angle, 16383), 16383)))) / 100
								+ y;

						xnew = (3 * cos_Taylor(uint16DegreeRelativeToX(AnglePlus(AnglePlus(angle, 16383), 16383))))
								/ 100 + xnew;
						ynew = (3 * sin_Taylor(uint16DegreeRelativeToX(AnglePlus(AnglePlus(angle, 16383), 16383))))
								/ 100 + ynew;

						draw_line(x, y, xnew, ynew);

						xhilfe = (6 * cos_Taylor(uint16DegreeRelativeToX(AngleMinus(AnglePlus(angle, 16383), 16383))))
								/ 100 + x;
						yhilfe = (6 * sin_Taylor(uint16DegreeRelativeToX(AngleMinus(AnglePlus(angle, 16383), 16383))))
								/ 100 + y;

						x = xhilfe;
						y = yhilfe;
						xhilfe = (6 * cos_Taylor(uint16DegreeRelativeToX(AngleMinus(AnglePlus(angle, 16383), 16383))))
								/ 100 + xnew;
						yhilfe = (6 * sin_Taylor(uint16DegreeRelativeToX(AngleMinus(AnglePlus(angle, 16383), 16383))))
								/ 100 + ynew;
					} else {
						x = (3 * cos_Taylor(uint16DegreeRelativeToX(AnglePlus(AngleMinus(angle, 16383), 16383)))) / 100
								+ x;
						y = (3 * sin_Taylor(uint16DegreeRelativeToX(AnglePlus(AngleMinus(angle, 16383), 16383)))) / 100
								+ y;

						xnew = (3 * cos_Taylor(uint16DegreeRelativeToX(AnglePlus(AngleMinus(angle, 16383), 16383))))
								/ 100 + xnew;
						ynew = (3 * sin_Taylor(uint16DegreeRelativeToX(AnglePlus(AngleMinus(angle, 16383), 16383))))
								/ 100 + ynew;

						draw_line(x, y, xnew, ynew);

						xhilfe = (6 * cos_Taylor(uint16DegreeRelativeToX(AngleMinus(AngleMinus(angle, 16383), 16383))))
								/ 100 + x;
						yhilfe = (6 * sin_Taylor(uint16DegreeRelativeToX(AngleMinus(AngleMinus(angle, 16383), 16383))))
								/ 100 + y;

						x = xhilfe;
						y = yhilfe;
						xhilfe = (6 * cos_Taylor(uint16DegreeRelativeToX(AngleMinus(AngleMinus(angle, 16383), 16383))))
								/ 100 + xnew;
						yhilfe = (6 * sin_Taylor(uint16DegreeRelativeToX(AngleMinus(AngleMinus(angle, 16383), 16383))))
								/ 100 + ynew;
					}

					draw_line(x, y, xhilfe, yhilfe);
					store_Map(1);
				}

				//Senden der Informationen per RF an den TUIO-Client
#ifdef SEND_MAP
				msg.dataLength = 6;
				msg.data[0] = 22;
				msg.data[1] = x;
				msg.data[2] = y;
				msg.data[3] = xnew;
				msg.data[4] = ynew;
				if (type == OBSTACLE)
					msg.data[5] = 13;
				else
					msg.data[5] = direction + 10;
				wirelessSend(&msg);
#endif
				break;

				case (FRONT_SENSOR):
				if (dist > RANGEFRONT && type == DRIVEN_AREA)
					dist = RANGEFRONT;
				dist = dist / 5; // 5 abschnitte pro Einheit

				xnew = (dist * cos_Taylor(uint16DegreeRelativeToX(angle))) / 100 + x;
				ynew = (dist * sin_Taylor(uint16DegreeRelativeToX(angle))) / 100 + y;

				diffx = abs(xnew - x);
				diffy = abs(ynew - y);
				if (diffx > dist)
					diffx = dist - 1;
				if (diffy > dist)
					diffy = dist - 1;
				if (dist < RANGEFRONT / 5 || type == OBSTACLE) {
					set_coord_obstacle(xnew, ynew);
					store_Map(2);
				}

				if (type == DRIVEN_AREA) {
					if (xnew < x)
						directionx = -1;
					if (ynew < y)
						directiony = -1;

					if ((angle > 13000 && angle < 19000) || (angle > 46000 && angle < 52000)) {
						//Auto steht quer -> Standardbreite auf der y-Achse
						diffy = 4;
						for (i = 0; i <= diffx; i++) {
							for (j = 0; j <= diffy * 2; j++) {
								set_coord_driven(x + directionx * i, y + directiony * j - directiony * diffy);
							}
						}
						store_Map(1);

					} else if ((angle > 29000 && angle < 36000) || angle < 3000 || angle > 62000) {
						//Auto steht längs -> Standardbreite auf der x-Achse
						diffx = 4;
						for (i = 0; i < diffx * 2; i++) {
							for (j = 0; j < diffy; j++) {
								set_coord_driven(x + directionx * i - directionx * diffx, y + directiony * j);
							}
						}
						store_Map(1);

					} else {
						startx = x;
						starty = y;
						endx = xnew;
						endy = ynew;

						x = (2 * cos_Taylor(uint16DegreeRelativeToX(AnglePlus(angle, 16383)))) / 100 + x;
						y = (2 * sin_Taylor(uint16DegreeRelativeToX(AnglePlus(angle, 16383)))) / 100 + y;

						xnew = (2 * cos_Taylor(uint16DegreeRelativeToX(AnglePlus(angle, 16383)))) / 100 + xnew;
						ynew = (2 * sin_Taylor(uint16DegreeRelativeToX(AnglePlus(angle, 16383)))) / 100 + ynew;

						draw_line(x, y, xnew, ynew);

						xhilfe = (3 * cos_Taylor(uint16DegreeRelativeToX(AngleMinus(angle, 16383)))) / 100 + x;
						yhilfe = (3 * sin_Taylor(uint16DegreeRelativeToX(AngleMinus(angle, 16383)))) / 100 + y;

						x = xhilfe;
						y = yhilfe;
						xhilfe = (3 * cos_Taylor(uint16DegreeRelativeToX(AngleMinus(angle, 16383)))) / 100 + xnew;
						yhilfe = (3 * sin_Taylor(uint16DegreeRelativeToX(AngleMinus(angle, 16383)))) / 100 + ynew;

						draw_line(x, y, xhilfe, yhilfe);
						store_Map(1);
					}
				}
			}
#ifdef SEND_MAP
			msg.dataLength = 6;
			msg.data[0] = 22;
			msg.data[1] = x;
			msg.data[2] = y;
			msg.data[3] = xnew;
			msg.data[4] = ynew;
			if (type == OBSTACLE)
				msg.data[5] = 13;
			else
				msg.data[5] = direction + 10;
			wirelessSend(&msg);
#endif
			break;
		case (REAR_SENSOR):
			break;
	}
}

/**
 * Speichert die Hindernisse punktweise ab
 */
void set_coord_obstacle_withoutZone(int16_t coord_x, int16_t coord_y) {
	load_map(coord_x, coord_y, 2);
	coord_x %= MAPSIZE;
	//Gibt negatives Ergebnis bei modulo mit negativen Zahlen zurück
	if (coord_x < 0)
		coord_x += MAPSIZE;
	coord_y %= MAPSIZE;
	if (coord_y < 0)
		coord_y += MAPSIZE;
	//Hier Bitweise schreiben
	Obstacle_SET(coord_y, coord_x);
	//Karte mit Hindernissen
}

/**
 * Speichert die Koordinaten die befahren werden dürfen
 */
void set_coord_driven(int16_t coord_x, int16_t coord_y) {
	load_map(coord_x, coord_y, 1);
	coord_x %= MAPSIZE;
	//Gibt negatives Ergebnis bei modulo mit negativen Zahlen zurück
	if (coord_x < 0)
		coord_x += MAPSIZE;
	coord_y %= MAPSIZE;
	if (coord_y < 0)
		coord_y += MAPSIZE;
	//Hier Bitweise schreiben
	DrivenArea_SET(coord_y, coord_x);
}

/**
 * Funktion zum setzen einzelner Hindernisse, Vergrößert das Hindernis automatisch um eine Sicherheitszone
 */
void set_coord_obstacle(int16_t coord_x, int16_t coord_y) {
	uint8_t i = 0, j = 0, c_x, c_y;

	load_map(coord_x, coord_y, 2);
	c_x = coord_x % MAPSIZE;
	//Gibt negatsives Ergebnis bei modulo mit negativen Zahlen zurück
	if (coord_x < 0)
		c_x = MAPSIZE - 1 + c_x;
	c_y = coord_y % MAPSIZE;
	if (coord_y < 0)
		c_y = MAPSIZE - 1 + c_y;
	Obstacle_SET(c_y, c_x);
	for (i = 0; i <= norm / 2; i++) {
		for (j = 0; j <= norm / 2; j++) {
			//if abfrage um array Grenzen zu beachten
			load_map(coord_x + i, coord_y + j, 2);
			if (coord_x + i < MAPSIZE && coord_y + j < MAPSIZE) {
				c_x = (coord_x + i) % MAPSIZE;
				if ((coord_x + i) < 0)
					c_x = MAPSIZE - 1 + c_x;
				c_y = (coord_y + j) % MAPSIZE;
				if ((coord_y + j) < 0)
					c_y = MAPSIZE - 1 + c_y;
				Obstacle_SET(c_y, c_x);
			}
			load_map(coord_x - i, coord_y - j, 2);
			;
			if (coord_x - i >= 0 && coord_y - j >= 0) {
				c_x = (coord_x - i) % MAPSIZE;
				if ((coord_x - i) < 0)
					c_x = MAPSIZE - 1 + c_x;
				c_y = (coord_y - j) % MAPSIZE;
				if ((coord_y - j) < 0)
					c_y = MAPSIZE - 1 + c_y;
				Obstacle_SET(c_y, c_x);
			}
			load_map(coord_x + i, coord_y - j, 2);
			if (coord_x + i < MAPSIZE && coord_y - j >= 0) {
				c_x = (coord_x + i) % MAPSIZE;
				if ((coord_x + i) < 0)
					c_x = MAPSIZE - 1 + c_x;
				c_y = (coord_y - j) % MAPSIZE;
				if ((coord_y - j) < 0)
					c_y = MAPSIZE - 1 + c_y;
				Obstacle_SET(c_y, c_x);
			}
			load_map(coord_x - i, coord_y + j, 2);
			if (coord_x - i >= 0 && coord_y + j < MAPSIZE) {
				c_x = (coord_x - i) % MAPSIZE;
				if ((coord_x - i) < 0)
					c_x = MAPSIZE - 1 + c_x;
				c_y = (coord_y + j) % MAPSIZE;
				if ((coord_y + j) < 0)
					c_y = MAPSIZE - 1 + c_y;
				Obstacle_SET(c_y, c_x);
			}
		}
	}
}

/**
 * liefert den Wert der an der Stelle x,y gespeichert ist zurück
 * @return 1 wenn belget, 0 wenn frei
 */
uint8_t get_coord_driven(int16_t coord_x, int16_t coord_y) {
	load_map(coord_x, coord_y, 1);

	coord_x %= MAPSIZE;
	if (coord_x < 0)
		coord_x += MAPSIZE;
	coord_y %= MAPSIZE;
	if (coord_y < 0)
		coord_y += MAPSIZE;

	if (checkDrivenArea == TRUE
	)
		return !DrivenArea_ISSET(coord_y,coord_x);
	else
		return DrivenArea_ISSET(coord_y,coord_x);
}

/**
 * liefert die den Wert der an der Stelle x,y gespeichert ist zurück (Gespeicherte Hindernisse)
 */
uint8_t get_coord_obstacle(int16_t coord_x, int16_t coord_y) {

	load_map(coord_x, coord_y, 2);

	coord_x %= MAPSIZE;
	if (coord_x < 0)
		coord_x += MAPSIZE;
	coord_y %= MAPSIZE;
	if (coord_y < 0)
		coord_y += MAPSIZE;
	//Bitweise lesen!!
	return Obstacle_ISSET(coord_y,coord_x);
}

/**
 * Umrechnung von Koordinaten übergeben durch den TUIO client auf Kartenkoordiaten
 */
int16_t CoordinatesToMap(int32_t Koordinate) {
	int32_t rest;

	//Unterteilung falls ein Überlauf möglich ist
	if (Koordinate >= 6550 || Koordinate <= -6550) {
		Koordinate = Koordinate / 4;
		if (Koordinate >= 0)
			Koordinate = (Koordinate + 33) * 5;
		else
			Koordinate = (Koordinate - 33) * 5;
		rest = Koordinate % 1638;
		Koordinate = Koordinate / 1638;
		Koordinate = Koordinate * 4;
		if (rest > 1638 / 2)
			Koordinate++;
	} else {
		if (Koordinate >= 0)
			Koordinate = (Koordinate + 33) * 5;
		else
			Koordinate = (Koordinate - 33) * 5;
		rest = Koordinate % 1638;
		Koordinate = Koordinate / 1638;
		if (rest > 1638 / 2)
			Koordinate++;
		else if (rest < 0)
			Koordinate--;
	}
	return Koordinate;
}

/**
 * Umrechnung von Kartenkoordinate auf TUIO Client Koordinaten
 */
uint16_t MapToCoordinates(int16_t Koordinate) {
	uint16_t hilfe, hilfe2;

	//Fall falls Überlauf möglich ist
	if (Koordinate > 20) {
		if (Koordinate < 80) {
			hilfe = Koordinate % 4;
			Koordinate = Koordinate / 4;
			hilfe2 = Koordinate;
			Koordinate = 4 * (Koordinate * 1638 / 4);
			switch (hilfe) {
				case 1:
					hilfe = ((hilfe2 + 1) * 1638 / 5) * 4;
					Koordinate = Koordinate - (hilfe - Koordinate) / 4 + (hilfe - Koordinate) / 2;
					break;
				case 2:
					hilfe = ((hilfe2 + 1) * 1638 / 5) * 4;
					Koordinate = Koordinate + (hilfe - Koordinate) / 2;
					break;
				case 3:
					hilfe = ((hilfe2 + 1) * 1638 / 5) * 4;
					Koordinate = Koordinate + (hilfe - Koordinate) / 4 + (hilfe - Koordinate) / 2;
					break;
			}
		}
	} else {
		Koordinate = Koordinate * 1638 / 5;
	}
	return Koordinate;
}

/**
 * Eingabe: Start-und Zielkoordinaten
 * Überprüft ob zwischen Punkt a und b eine Kollision vorliegt. Wenn Kollision return
 * @return 1 wenn Kollsion, sonst 0
 */
uint8_t calc_reachability(int16_t* xCollision, int16_t* yCollision, int16_t ownx, int16_t owny, int16_t targetx,
		int16_t targety) {
	int16_t diffx, diffy, i, startx, starty, border;
	int16_t directionx = 1, directiony = 1, faktor = 0, rest = 0, toggle = 0;
	int8_t o = 0;
	*xCollision = 0;
	*yCollision = 0;

	//Richtung bestimmen
	if (targetx < ownx)
		directionx = -1; //x Richtung
	if (targety < owny)
		directiony = -1; //y Richtung

	diffx = abs16(targetx - (ownx + directionx));
	diffy = abs16(targety - (owny + directiony));

	//Berechne genügend Punkte die auf der geraden zwischen Start und Ziel liegen und berechne diese basierend auf Normierung
	startx = ownx; //KoordinatenAufKarte(own->x);
	starty = owny; //KoordinatenAufKarte(own->y);

	//Verhältnis zwischen Diffy und Diffx berechnen
	if (diffy != 0 && diffx != 0) {
		if (diffx > diffy) {
			faktor = diffx / diffy;
			rest = diffx % diffy;
			border = (diffy - 1) * 10 / 3;
		} else {
			faktor = diffy / diffx;
			rest = diffy % diffx;
			border = (diffx - 1) * 10 / 3;
		}
		//rest genauer betrachten, notwendig um steigung besser zu approximieren
		if (rest * 10 <= border) {
			rest = 0;
			border = 1; // wichtig damit kein toggle modus
		} else if (rest * 10 > 2 * border)
			rest = 1;
		else {
			rest = 1;
			border = 0;
		}
		if (border != 0)
			faktor = faktor + rest;
	} else {
		if (diffy == 0) {
			faktor = diffx;
			directiony = 0;
		}
		if (diffx == 0) {
			faktor = diffy;
			directionx = 0;
		}
	}
	rest = 0;

	if (diffx > diffy)
		rest = 1;

	//Absicherung das Startpunkt nur betrachtet wird, wenn vom Startpunkt aus in Richtung des Hindernisses geschaut wird
	if ((startx == targetx) && (starty == targety)) {
		return 0;
	}
	//Abhängig vom Szenario und der angefragten Daten müssen unterschiedliche Bedingungen überprüft werden
	if ((createMap_isActivated() == TRUE
			&& (get_coord_driven(startx, starty) == 1 || get_coord_driven(startx + directionx, starty) == 1
					|| get_coord_driven(startx, starty + directiony) == 1))
			|| (getOnlyDrivenMap == FALSE
					&& (get_coord_obstacle(startx, starty) == 1 && get_coord_obstacle(startx + directionx, starty) == 1
							&& get_coord_obstacle(startx, starty + directiony) == 1 /* && get_coord_obstacle(startx,starty) != 100 */))) {
		*xCollision = startx;
		*yCollision = starty;
		return 1;
	}
	//Erstelle Anzahl Intervalle Punkte die überprüft werden sollen (laufe Treppenfuktion ab)
	while (!(diffx <= 0 && diffy <= 0)) {
		//hier faktor in jeder iteration verändern wenn bedingung (grenze == 0) erfüllt

		if (border == 0) {
			if (toggle == 0) {
				faktor = faktor + 1;
				toggle = 1;
			} else {
				faktor = faktor - 1;
				toggle = 0;
			}
		}
		//Entscheide ob x oder y Achse primär abgelaufen werden soll
		if (rest == 1) {
			for (i = 0; i < faktor; i++) {
				if (createMap_isActivated() == TRUE && i > 100) {
					Seg_Hex(0x0b);
					os_wait(2000);
					return 0;
				}
				startx = startx + directionx;
				if ((startx == targetx) && (starty == targety)) {
					return 0;
				}
				//Überprüfe in jedem Schritt ob eine Kollision stattfindet
				if ((createMap_isActivated() == TRUE
						&& (get_coord_driven(startx, starty) == 1 || get_coord_driven(startx + directionx, starty) == 1
								|| get_coord_driven(startx, starty + directiony) == 1))
						|| (getOnlyDrivenMap == FALSE
								&& (get_coord_obstacle(startx, starty) == 1
										&& get_coord_obstacle(startx + directionx, starty) == 1
										&& get_coord_obstacle(startx, starty + directiony) == 1 /* && get_coord_obstacle(startx,starty) != 100*/))) {
					*xCollision = startx;
					*yCollision = starty;
					return 1;
				}
				if (diffx >= 1)
					diffx = diffx - 1;
				else {
					diffx = diffx - 1;
					i = faktor;
				}
			}
			starty = starty + directiony;
			diffy = diffy - 1;
		} else {
			for (i = 0; i < faktor; i++) {
				if (createMap_isActivated() == TRUE && i > 100) {
					Seg_Hex(0x0b);
					os_wait(2000);
					return 0;
				}
				starty = starty + directiony;
				if ((startx == targetx) && (starty == targety)) {
					return 0;
				}
				if ((createMap_isActivated() == TRUE
						&& (get_coord_driven(startx, starty) == 1 || get_coord_driven(startx + directionx, starty) == 1
								|| get_coord_driven(startx, starty + directiony) == 1))
						|| (getOnlyDrivenMap == FALSE
								&& (get_coord_obstacle(startx, starty) == 1
										&& get_coord_obstacle(startx + directionx, starty) == 1
										&& get_coord_obstacle(startx, starty + directiony) == 1 /* && get_coord_obstacle(startx,starty) != 100*/))) {
					*xCollision = startx;
					*yCollision = starty;
					return 1;
				}
				if (diffy >= 1)
					diffy = diffy - 1;
				else {
					diffy = diffy - 1;
					i = faktor;
				}

			}
			startx = startx + directionx;
			diffx = diffx - 1;
		}

		if ((startx == targetx) && (starty == targety)) {
			return 0;
		}
		if ((getOnlyDrivenMap == FALSE
				&& (createMap_isActivated() == FALSE && get_coord_obstacle(startx, starty) == 1
						&& get_coord_obstacle(startx + directionx, starty) == 1
						&& get_coord_obstacle(startx, starty + directiony) == 1 /* && get_coord_obstacle(startx,starty) != 100*/))
				|| (createMap_isActivated() == TRUE && get_coord_driven(startx, starty) == 1
						&& get_coord_driven(startx + directionx, starty) == 1
						&& get_coord_driven(startx, starty + directiony) == 1) /* && get_coord(startx,starty) != 100*/) {
			*xCollision = startx;
			*yCollision = starty;
			return 1;
		}
	}
	//Falls keine Kollision entdeckt;
	return 0;
}

