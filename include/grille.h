#ifndef __GRILLE_H
#define __GRILLE_H

/**
 * \file grille.h
 * header pour les grilles
 * \author David Pulido Cornejo
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/**
 * \typedef \struct grille
 * Déclaration de la structure grille
 * structure grille : nombre de lignes, nombre de colonnes, tableau de tableau de cellules
 */
typedef struct {
	int nbl; /**< nombre de lignes*/
	int nbc; /**< nombre de colonnes*/
	int** cellules; /**< tableau de tableau de cellules */
} grille;

// alloue une grille de taille l*c, et initialise toutes les cellules à mortes
void alloue_grille (int l, int c, grille* g);

// libère une grille
void libere_grille (grille* g);

// alloue et initalise la grille g à partir d'un fichier
void init_grille_from_file (char * filename, grille* g);

/**
 * rend vivante la cellule (i,j) de la grille g
 * \param i int
 * \param j int
 * \param g grille
 * \returns \c void
 */
static inline void set_vivante(int i, int j, grille g){g.cellules[i][j] = 1;}

/**
 * rend non viable la cellule (i,j) de la grille g
 * \param i int
 * \param j int
 * \param g grille
 * \returns \c void initialise la cellule à la valeur -1
 */
static inline void set_non_viable(int i, int j, grille g){g.cellules[i][j] = -1;}

/**
 * determine si une cellule (i,j) est non viable
 * \param i int
 * \param j int
 * \param g grille*
 * \returns \c int 1 si la cellule est non viable, 0 sinon
 */
static inline int est_non_viable(int i, int j, grille *g){return g->cellules[i][j] == -1;}

/**
 * rend morte la cellule (i,j) de la grille g
 * \param i int
 * \param j int
 * \param g grille
 * \returns \c void
 */
static inline void set_morte(int i, int j, grille g){g.cellules[i][j] = 0;}

/*
 * teste si la cellule (i,j) de la grille g est vivante
 * \param i int
 * \param j int
 * \param g grille
 * \returns \c int 0 si vivante 1 si morte
 */
static inline int est_vivante(int i, int j, grille g){return g.cellules[i][j] >= 1;}

//après avoir désactivé le calcul de vieillissement on réinitialise les ages des cellules vivantes à 1
void reset_ages(int l, int c, grille* g);

// recopie gs dans gd (sans allocation)
void copie_grille (grille gs, grille gd);

// teste si deux grilles sont egaux
int grilles_egaux(grille* g1, grille* g2);

// teste si un grille a au moins une cellule vivante
int grille_vide(grille* g);

#endif
