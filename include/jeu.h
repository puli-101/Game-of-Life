#ifndef __JEU_H
#define __JEU_H

#include "grille.h"

/**
 * Numero de tests pour le calcul du periode d'oscillation
 */
#define N_TESTS 100

/**
 * \file jeu.h
 * header avec des fonctions auxilieres pour le deroulement du jeu
 * \author David Pulido Cornejo
 */

/**
 * modulo modifié pour traiter correctement les bords i=0 et j=0 dans le calcul des voisins avec bords cycliques
 * \param i int
 * \param m int
 * \returns (i+m)%m
 */
static inline int modulo(int i, int m) {return (i+m)%m;}

/**
 * determine si les coordonnees (i,j) se trouvent dedans la grille de taille x * y
 * \param i int
 * \param j int
 * \param x int
 * \param y int
 * \returns 1 si (i,j) dedans la grille , 0 sinon
 */
static inline int dedans_grille(int i, int j, int x, int y) {return ( i>=0 && i < x && j >= 0 && j < y );}

//compte le nombre de voisins vivants de la cellule (i,j)
//les bords sont cycliques.
int compte_voisins_cyclique (int i, int j, grille g);

int compte_voisins_noncyclique (int i, int j, grille g);

// fait évoluer la grille g d'un pas de temps
void evolue (grille *g, grille *gc);

//calcul de la periodicité de la grille actuelle
int calcul_periode(grille* g);

//calcul de la periode la plus proche au temps d'evolution courant de la grille actuel
int calcul_oscillation(grille* g, int* delta_temps_ev);

/**
 * boolean qui determine si le calcul de vieillissement est active
*/
extern int vieillissement;

/**
 * pointeur de fonction qui determine le type de comptage de voisins vivantes
*/
extern int (*compte_voisins_vivants) (int, int, grille);

#endif
