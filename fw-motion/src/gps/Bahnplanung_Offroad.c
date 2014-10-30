/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file Bahnplanung_Offroad.c
 * @author Team GPS
 * @brief Dieses Modul berechnet verschiedene Wege um ein oder mehrere Hindernisse
 **/

#include "Bahnplanung_Offroad.h"

/**
 * Vermindert die Anzahl rekursiver aufrufe. Läuft am Hindernis entlang
 * Input: aktuelle Position x,y
 * Return: x_freeL = neuer Wegpunkt links, y_freeL = neuer Wegpunkt rechts
 * return: 1 wenn neuer Zwischenpunkt gefunden, sonst 0
 */
int8_t find_Pos_Depth(int16_t* x_freeL,int16_t* y_freeL,int16_t xleft, int16_t yleft){
	int8_t i = 1;

	if(get_coord_obstacle(xleft,yleft) != 0){
		if(get_coord_obstacle(xleft,yleft+1) == 1){ 	//Hindernis erstreckt sich nach oben
			while(get_coord_obstacle(xleft,yleft+i) == 1 && (get_coord_obstacle(xleft-1,yleft+i) == 0 || get_coord_obstacle(xleft+1,yleft+i) == 0) ){
				get_coord_obstacle(xleft,yleft+i-1);
				i++;
			}
			*x_freeL = xleft;
			*y_freeL = yleft+i;
			i = 1;
			return 1;
		}
		else if(get_coord_obstacle(xleft,yleft-1) == 1){ //Hindernis erstreckt sich nach unten
			while(get_coord_obstacle(xleft,yleft-i) == 1 && (get_coord_obstacle(xleft-1,yleft-i) == 0 || get_coord_obstacle(xleft+1,yleft-i) == 0)){
				get_coord_obstacle(xleft,yleft-i+1);
				i++;
			}
			*x_freeL = xleft;
			*y_freeL = yleft-i;
			i = 1;
			return 1;
		}
		//Hindernis erstreckt sich ja links
		else if(get_coord_obstacle(xleft-1,yleft) == 1){
			while(get_coord_obstacle(xleft-i,yleft) == 1 && (get_coord_obstacle(xleft-i,yleft-1) == 0 || get_coord_obstacle(xleft-i,yleft+1) == 0)){
				get_coord_obstacle(xleft-i+1,yleft);
				i++;
			}
			*x_freeL = xleft-i;
			*y_freeL = yleft;
			i = 1;
			return 1;
		}
		//Hindernis erstreckt sich nach rechts
		else if (get_coord_obstacle(xleft+1,yleft) == 1){
			while(get_coord_obstacle(xleft+i,yleft) == 1 && (get_coord_obstacle(xleft+i,yleft-1) == 0 || get_coord_obstacle(xleft+i,yleft+1) == 0)){
				get_coord_obstacle(xleft+i-1,yleft);
				i++;
			}
			*x_freeL = xleft+i;
			*y_freeL = yleft;
			i = 1;
			return 1;
		}
	}
	return 0;
}

/**
 * Geht links und rechts an einem Horizontalen Hindernis vorbei
 * Die ersten 4 Parameter sind Rückgabewerte für den neuen Weg
 * richtung gibt an ob Annäherung von oben an das Hindernis oder von unten (1 = unten)
 */
void move_AroundHorizontal(int8_t* xleft, int8_t* xright, int8_t* yleft, int8_t* yright, uint8_t xCollision, uint8_t yCollision, int8_t richtung){
	uint8_t l,r,h,j;
	uint8_t wayx = 0;
	uint8_t wayy = 0;
	l = 1;
	r = 1;
	h = 0;
	j = 0;

	while(wayx == 0 || wayy == 0){

		//nach links
		if (wayx == 0){
			//Bedingung l > 1 damit nicht die vorhergenden Position erneut überprüft wird. Diese muss in der Regel = 2 sein
			if (get_coord_obstacle(xCollision-l, yCollision) >= 2 || (l > 1 && get_coord_obstacle(xCollision, yCollision + richtung*j) >= 2)){	//Fall das diese Koordinaten bereits überprüft wurden oder außerhalb der Karte sind
				wayx = 1;
				*xleft = -1;
				*yleft = -1;
			}
			else
				if (get_coord_obstacle(xCollision-l, yCollision + richtung*(-1)*j) == 1){
					while (get_coord_obstacle(xCollision-l, yCollision + richtung*(-1)*(j+1)) == 1){ //gehe Hindernis entlang das zwischen aktuellem und Startpunkt liegt
						j = j+1;
					}
					if (j > 0){
						*xleft = xCollision - l;
						*yleft = yCollision + richtung*(-1)*j;
						wayx = 1;
					}
					l++;
				}
				else
					{
						//Finde Abstandswert für y
						l--;
						*xleft = xCollision - l;
						*yleft = yCollision + richtung*j; // + richtung*(-1)*k1;
						wayx = 1;
					}
		}
		// nach rechts
		if (wayy == 0){
			if (get_coord_obstacle(xCollision+r, yCollision) >= 2 || (r > 1 &&  get_coord_obstacle(xCollision, yCollision + richtung*h)) >= 2){
				wayy = 1;
				*xright = -1;
				*yright = -1;
			}
			else
				if (get_coord_obstacle(xCollision+r, yCollision) == 1){
					while (get_coord_obstacle(xCollision+r, yCollision + richtung*(-1)*(h+1)) == 1){ //gehe Hindernis entlang das zwischen aktuellem und Startpunkt liegt
						h = h+1;
					}
					if (h > 0){
						*xright = xCollision + r;
						*yright = yCollision + richtung*(-1)*h;
						wayy = 1;
					}
					r++;
				}
				else
				{
						r--;
						*xright = xCollision + r;
						*yright = yCollision + richtung*h;
						wayy = 1;
					}
		}
	}
	if (*xleft == xCollision && *yleft == yCollision){
		*xleft = -1;
		*yleft = -1;
	}
	if (*xright == xCollision && *yright == yCollision){
		*xright = -1;
		*yright = -1;
	}
}

/**
 * Geht oben und unten an einem Senkrechten Hindernis vorbei
 * Die ersten 4 Parameter sind Rückgabewerte für den neuen Weg
 * richtung gibt an ob Annäherung von links an das Hindernis oder von rechts (1 = links)
 */
void move_AroundVertical(int8_t* xleft, int8_t* xright, int8_t* yleft, int8_t* yright, uint8_t xCollision, uint8_t yCollision, int8_t richtung){
	uint8_t l,r,h,j;
	uint8_t wayx = 0;
	uint8_t wayy = 0;
	l = 1;
	r = 1;
	h = 0;
	j = 0;

	while(wayx == 0 || wayy == 0){

		// an Abbruchbedingung denken falls Rückgabewert = 100
		//nach unten
		if (wayx == 0){
			if (get_coord_obstacle(xCollision, yCollision-l) >= 2 || (l > 1 && get_coord_obstacle(xCollision + richtung*j, yCollision)) >= 2){
				wayx = 1;
				*xleft = -1;
				*yleft = -1;
			}
			else
				if (get_coord_obstacle(xCollision, yCollision-l) == 1){
					while (get_coord_obstacle(xCollision + richtung*(-1)*(j+1), yCollision - l) == 1){ //gehe Hindernis entlang das zwischen aktuellem und Startpunkt liegt
						j = j+1;
					}
					if (j > 0){
						*xleft = xCollision + richtung*(-1)*j;
						*yleft = yCollision - l;
						wayx = 1;
					}
					l++;
				}
				else
				{
						//Finde Abstandswert für y
						l--;
						*xleft = xCollision + richtung*j;
						*yleft = yCollision - l;
						wayx = 1;
					}
		}
			// nach oben
		if (wayy == 0){
			if (get_coord_obstacle(xCollision, yCollision+r) >= 2 || (r > 1 && get_coord_obstacle(xCollision + richtung*h, yCollision)) >= 2){
				wayy = 1;
				*xright = -1;
				*yright = -1;
			}
			else
				if (get_coord_obstacle(xCollision, yCollision+r) == 1){
					while (get_coord_obstacle(xCollision + richtung*(-1)*(h+1), yCollision + r) == 1){ //gehe Hindernis entlang das zwischen aktuellem und Startpunkt liegt
						h = h+1;
					}
					if (h > 0){
						*xright = xCollision + richtung*(-1)*h;
						*yright = yCollision + r;
						wayy = 1;
					}
					r++;
				}
				else
				{
						r--;
						*xright = xCollision + richtung*h;
						*yright = yCollision + r;
						wayy = 1;
					}
		}
	}
	if (*xleft == xCollision && *yleft == yCollision){
		*xleft = -1;
		*yleft = -1;
	}
	if (*xright == xCollision && *yright == yCollision){
		*xright = -1;
		*yright = -1;
	}

}

/**
 * Erkennt ob die Kollision an der Ecke eines Hindernisses stattfindet
 */
uint8_t isEcke(uint8_t xCollision, uint8_t yCollision){
	if (get_coord_obstacle(xCollision, yCollision-1) != 0 && get_coord_obstacle(xCollision +1 ,yCollision) != 0) return 1;
	else if (get_coord_obstacle(xCollision, yCollision+1) != 0 && get_coord_obstacle(xCollision +1 ,yCollision) != 0) return 1;
	else if (get_coord_obstacle(xCollision, yCollision-1) != 0 && get_coord_obstacle(xCollision -1 ,yCollision) != 0) return 1;
	else if (get_coord_obstacle(xCollision, yCollision+1) != 0 && get_coord_obstacle(xCollision -1 ,yCollision) != 0) return 1;
	else return 0;
}

/**
 * Findet die 2 (Rand)Koordinaten die das Hindernis aus Sicht des Autos abgrenzen. Findet von dort einen Weg zum Ziel
 */
void scan_Obstacles(int8_t xCollision, int8_t yCollision, gps_reducedData_t* own, gps_reducedData_t* target, int8_t x_old, int8_t y_old){
	int16_t x_freeL, y_freeL, x_freeR, y_freeR;
	int8_t xleft = -1, yleft = -1, xright = -1, yright = -1;

	//positiver Pfad
	//Annäherung an Hindernis von unten
	if(get_coord_obstacle(xCollision, yCollision-1) == 0 && (get_coord_obstacle(xCollision-1, yCollision) == 1 || get_coord_obstacle(xCollision+1, yCollision) == 1)){
		move_AroundHorizontal( &xleft, &xright, &yleft, &yright, xCollision, yCollision, 1);
		if (xleft >= 0 && yleft >= 0){
			if (calc_reachability(&x_freeL, &y_freeL,CoordinatesToMap(own->x), CoordinatesToMap(own->y), xleft, yleft) == 1){
				x_freeL = xCollision;
				y_freeL = yCollision;  //von Hinderniskante zu Auto ist kein direkter Weg möglich. Nehme zwischeschritt
			}
		}
		if (xright >= 0 && yright >= 0){
			if (calc_reachability(&x_freeR, &y_freeR, CoordinatesToMap(own->x), CoordinatesToMap(own->y), xright, yright) == 1){
				x_freeR = xCollision;
				y_freeR = yCollision;
			}
		}


	}
	else
	//Annäherung an Hindernis von oben
	if(get_coord_obstacle(xCollision, yCollision-1) != 0 && get_coord_obstacle(xCollision,yCollision+1) == 0 && (get_coord_obstacle(xCollision-1, yCollision) == 1 || get_coord_obstacle(xCollision+1, yCollision) == 1)){
		move_AroundHorizontal( &xleft, &xright, &yleft, &yright, xCollision, yCollision, -1); // -1 = Hindernis von oben
		if (xleft >= 0 && yleft >= 0){
			if (calc_reachability(&x_freeL, &y_freeL,CoordinatesToMap(own->x), CoordinatesToMap(own->y), xleft, yleft) == 1){
				x_freeL = xCollision;
				y_freeL = yCollision;	//von Hinderniskante zu Auto ist kein direkter Weg möglich. Nehme zwischeschritt
			}
		}
		if (xright >= 0 && yright >= 0){
			if (calc_reachability(&x_freeR, &y_freeR, CoordinatesToMap(own->x), CoordinatesToMap(own->y), xright, yright) == 1){
				x_freeR = xCollision;
				y_freeR = yCollision;
			}
		}
	}
	else
	//Sonderfall senkrechtes Hindernis
	if((get_coord_obstacle(xCollision, yCollision-1) != 0 && get_coord_obstacle(xCollision,yCollision+1) == 1) || isEcke(xCollision,yCollision) == 1 ){
		//Überpürung ob Weg Rechts oder Links vom Hinderniss
		if (get_coord_obstacle(xCollision-1, yCollision) == 0 || get_coord_obstacle(xCollision-1, yCollision-1) == 0 || get_coord_obstacle(xCollision-1, yCollision +1) == 0){
			move_AroundVertical( &xleft, &xright, &yleft, &yright, xCollision, yCollision, 1);	//Links
			x_freeL = 1;
		}
		else if (get_coord_obstacle(xCollision+1, yCollision) == 0  || get_coord_obstacle(xCollision+1, yCollision-1) == 0 || get_coord_obstacle(xCollision+1, yCollision +1) == 0)
			move_AroundVertical( &xleft, &xright, &yleft, &yright, xCollision, yCollision, -1);

		if (xleft >= 0 && yleft >= 0){
			if (calc_reachability(&x_freeL, &y_freeL,CoordinatesToMap(own->x), CoordinatesToMap(own->y), xleft, yleft) == 1){
					x_freeL = xCollision;
					y_freeL = yCollision;
			}
		}
		if (xright >= 0 && yright >= 0){
			if (calc_reachability(&x_freeR, &y_freeR, CoordinatesToMap(own->x), CoordinatesToMap(own->y), xright, yright) == 1){
					x_freeR = xCollision;
					y_freeR = yCollision;
			}
		}
		}

	//hier neue Zwischenpunkte abspeichern
	if(xleft  >= 0 && yleft  >= 0 ){
		if(x_freeL == xCollision){
			tree_insert(x_freeL, y_freeL, x_old, y_old, 1); //zwischenschritt vom zwischenschritt abspeichern
			tree_insert(xleft,yleft,x_freeL,y_freeL, 1);
		}
		else tree_insert(xleft,yleft,x_old,y_old,1);	//ansonsten nur die neuen Koordinaten abspeichern

		//Prüfen ob Ziel bereits direkt erreichbar
		if (calc_reachability(&x_freeL, &y_freeL, xleft, yleft, CoordinatesToMap(target->x), CoordinatesToMap(target->y)) == 0){	// In diesem Fall ist das Ziel erreicht
			tree_insert(CoordinatesToMap(target->x), CoordinatesToMap(target->y), xleft,yleft,1);
			return;
		}

		//Hier berechnen wie Dick das Hinderniss ist und gleich neuen Wegpunkt abspeichern -> reduziert Anzahl rekursiver Aufrufe
		if(find_Pos_Depth(&x_freeL, &y_freeL, xleft, yleft) == 1){
			tree_insert(x_freeL,y_freeL,xleft,yleft,1);
			xleft = x_freeL;
			yleft = y_freeL;
		}

		if (calc_reachability(&x_freeL, &y_freeL, xleft, yleft, CoordinatesToMap(target->x), CoordinatesToMap(target->y)) == 0){	// In diesem Fall ist das Ziel erreicht
			tree_insert(CoordinatesToMap(target->x), CoordinatesToMap(target->y), xleft,yleft,1);
		}
		else{
			setCoordinates(own, own->cam_id,own->tag_id,MapToCoordinates(xleft),MapToCoordinates(yleft),own->angle,own->isWorld);
			scan_Obstacles(x_freeL, y_freeL, own, target,  xleft,  yleft);
		}
	}
	if(xright >= 0 && yright  >= 0){
		if(x_freeR == xCollision){
			tree_insert(x_freeR, y_freeR, x_old, y_old, 0); //zwischenschritt vom zwischenschritt abspeichern
			tree_insert(xright,yright,x_freeR,y_freeR, 0);
		}
		else tree_insert(xright,yright,x_old,y_old,0);

		if (calc_reachability(&x_freeR, &y_freeR, xright, yright, CoordinatesToMap(target->x), CoordinatesToMap(target->y)) == 0){
			tree_insert(CoordinatesToMap(target->x), CoordinatesToMap(target->y), xright,yright,0);
			return;
		}

		if(find_Pos_Depth(&x_freeR, &y_freeR, xright, yright) == 1){
			tree_insert(x_freeR,y_freeR,xright,yright,0);
			xright = x_freeR;
			yright = y_freeR;
		}
		if (calc_reachability(&x_freeR, &y_freeR, xright, yright, CoordinatesToMap(target->x), CoordinatesToMap(target->y)) == 0){
			tree_insert(CoordinatesToMap(target->x), CoordinatesToMap(target->y), xright,yright,0);
		}
		else{
			setCoordinates(own, own->cam_id,own->tag_id,MapToCoordinates(xright),MapToCoordinates(yright),own->angle,own->isWorld);
			scan_Obstacles(x_freeR, y_freeR, own, target,  xright,  yright);
		}
	}
	return; //toter Pfad, wird nicht mehr zur Zielfindung benötigt

}

/**
 * Überprüft auf eine Kollision
 * Return: diff = neues Struct direkt zum Ziel
 * return: 0 wenn keine Kollision, 1 sonst
 */
uint8_t calc_Offroad(gps_reducedData_t* diff, gps_reducedData_t* own, gps_reducedData_t* target){
	int16_t xCollision, yCollision;

	tree_init(CoordinatesToMap(own->x), CoordinatesToMap(own->y));
	if (calc_reachability(&xCollision, &yCollision,CoordinatesToMap(own->x), CoordinatesToMap(own->y),CoordinatesToMap(target->x), CoordinatesToMap(target->y)) == 0){
		setCoordinates(diff, target->cam_id, target->tag_id, target->x, target->y, target->angle, target->isWorld);
		return 0;
	}
	else{
		scan_Obstacles(xCollision, yCollision, own, target, CoordinatesToMap(own->x), CoordinatesToMap(own->y)); //übergibt hier die Kollisionskoordinaten
	}
	return 1;
}



