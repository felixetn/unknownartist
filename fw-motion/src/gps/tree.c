/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file tree.c
 * @author Team GPS
 * @brief Dieses Modul stellt einen Baum zur Verfügung wo in jedem Knoten ein Struct gespeichert ist.
 * Basierend auf dem Baum können mehrere Wege zum Ziel rekursiv ausgelesen werden
 **/

#include "tree.h"
#include "help_methods.h"

struct waypoints{
		uint8_t x;
		uint8_t y;
		struct waypoints* left;
		struct waypoints* right;
		struct waypoints* root;
};

static struct waypoints *tree;
struct waypoints *tmp;
uint8_t found;
static uint8_t found_dest;

/**
 * initialisiert den Baum mit den Startkoordinaten
 */
void tree_init(uint8_t x, uint8_t y){

	tree = (struct waypoints *)malloc(sizeof(struct waypoints));
	tree->left = NULL;
	tree->right = NULL;
	tree->root = NULL;
	tree->x = x;
	tree->y = y;
	found = 0;
	found_dest = 0;
}



/**
 * Baum durchsuchen bis Stelle von x_last, y_last gefunden wurde. Als neues Kind neue Koordinaten einfügen
 * Input: aktuelle Position, neue x/y Koordinate, alte x/y Koordinate, Richtung ob neues Kind links oder rechts eingefügt wird
 */
void tree_find(struct waypoints *weg, uint8_t x_next, uint8_t y_next, uint8_t x_last, uint8_t y_last, uint8_t direction){
	struct waypoints *new;

	if(found == 1) return;		//Rekursionsanker

	if(weg->x == x_last && weg->y == y_last){
			new = (struct waypoints *)malloc(sizeof(struct waypoints));
			new->left = NULL;
			new->right = NULL;
			new->root = weg;
			new->x = x_next;
			new->y = y_next;
			found = 1;
			if(direction == 1) weg->left = new;
			else weg->right = new;
			return;
	}
	else{
		if(weg->left != NULL) tree_find(weg->left,  x_next, y_next, x_last, y_last, 1);
		if(weg->right != NULL)tree_find(weg->right, x_next, y_next, x_last, y_last, 0);
	}
}

/**
 * Fügt die 2 neuen Koordinaten von dem Hindernis basierend auf den vorherigen Koordinaten ein
 * direction 1 = left, 0 = right
 */
void tree_insert(uint8_t x_next, uint8_t y_next, uint8_t x_last, uint8_t y_last, uint8_t direction){

	found = 0;
	tree_find(tree, x_next, y_next, x_last, y_last, direction);

}

/**
 * Hilfsmethode für delete_tree()
 */
void delete(struct waypoints *weg){

	if(weg->left != NULL){
		tmp = weg->left;
		delete(tmp);
	}
	if(weg->right != NULL){
		tmp = weg->right;
		delete(tmp);
	}
	free(weg);
	weg = NULL;
}

/**
 * löscht den Baum
 */
void tree_delete(){
	delete(tree);
	free(tmp);
	tmp = NULL;
}

/**
 * Hilfsmethode für tree_getpath() und tree_get_shortest_path()
 */
void find_destination(struct waypoints *weg, uint8_t xZiel, uint8_t yZiel){

	if (found_dest == 1) return;

	if(weg->x == xZiel && weg->y == yZiel){
		found_dest = 1;
		tmp = weg;
		return;
	}

	if(weg->left != NULL) find_destination(weg->left, xZiel, yZiel);
	if(weg->right != NULL)find_destination(weg->right, xZiel, yZiel);
}

/**
 * Gibt Schrittweise den ersten gefundenen Pfad zum Ziel aus
 * Input: Zielkoordinaten
 * Return: xWay = Array mit x Koordinaten, yWay = Array mit y Koordinante, j = Weglänge
 * return: 1 wenn kein neuer Weg, 0 sonst
 */
uint8_t tree_getpath(uint8_t *xWay, uint8_t *yWay, uint8_t* j, uint8_t xZiel, uint8_t yZiel){
		uint8_t ret = 0,i = 0;

		//finde Element mit den Zielkoordinaten
		find_destination(tree, xZiel, yZiel);
		if (found_dest == 1){
			ret = 1;
			found_dest = 0;
		}
		else return ret;
		if(tree_isEmpty() == 0 && tmp != NULL){
			xWay[i] = tmp->x;
			yWay[i] = tmp->y;
			tmp->x = 0;		//Damit dieser Pfad nicht nochmal ausgelesen wird
			tmp->y = 0;
			i++;
			//liest den kompletten Pfad aus
			while(tmp->root != NULL && tmp->root->root != NULL){
				tmp = tmp->root;
				xWay[i] = tmp->x;
				yWay[i] = tmp->y;
				i++;
			}
			*j = i; //Tiefe des Baumes
		}
		return ret;
	}

/**
 * Berechnet den kürzesten Weg basierend auf der Anzahl der Zwischenpunkte
 * Input: Zielkoordinaten
 * Return: xWay = Array mit x Koordinaten, yWay = Array mit y Koordinante, j = Weglänge
 * return: 0 wenn mindestens ein Pfad gefunden
 */
uint8_t tree_get_shortest_path(uint8_t *xWay, uint8_t *yWay, uint8_t* j, uint8_t xZiel, uint8_t yZiel){
	uint8_t xnew[50], ynew[50];
	uint8_t k =0,i =0;

	tree_getpath(xWay, yWay, &k,  xZiel,  yZiel);

	while(tree_getpath(xnew, ynew, &i,  xZiel,  yZiel) != 0){
		//nehme neuen Baum wenn weniger Wegpunkte
		if (i < k){
			k = i;
			for(i = 0; i < k; i++){

				xWay[i] = xnew[i];
				yWay[i] = ynew[i];
			}
		}

	}
	*j = k;
	return 0;
}

/**
 * return: 1 wenn voll, 0 wenn leer
 */
uint8_t tree_isEmpty(){
	if (tree == NULL) return 1;
	if(tree->left == NULL && tree->right == NULL && tree->root == NULL)
		return 1;
	else return 0;
}
