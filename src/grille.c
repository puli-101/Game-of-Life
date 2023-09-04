#include "grille.h"
/** 
 * \file grille.c 
 * code pour les grilles
 * \author David Pulido Cornejo
 */

/**
 * cree une grille a partir du fichier
 * \relatesalso grille
 * \param filename char* (adresse + nom du fichier de la grille)
 * \param g grille*
 * \returns \c void
 */
void init_grille_from_file (char * filename, grille* g){
	FILE * pfile = NULL;
	pfile = fopen(filename, "r");
	assert (pfile != NULL);
	
	int i,j,n,l,c,vivantes=0,non_viables = 0;
	
	fscanf(pfile, "%d", & l);
	fscanf(pfile, "%d", & c);
	
	alloue_grille(l,c,g);
	
	fscanf(pfile, "%d", & vivantes);
	for (n=0; n< vivantes; ++n){
		fscanf(pfile, "%d", & i);
		fscanf(pfile, "%d", & j);
		set_vivante(i,j,*g);
	}
	
	fscanf(pfile, "%d", & non_viables);
	for (n=0; n< non_viables; ++n){
		fscanf(pfile, "%d", & i);
		fscanf(pfile, "%d", & j);
		set_non_viable(i,j,*g);
	}
	
	fclose (pfile);
	return;
}


/**
 * copie des valeurs des cellules d'une grille S vers une grille D
 * \relatesalso grille
 * \param gs grille (grille S)
 * \param gd grille (grille D)
 * \returns \c void
 */
void copie_grille (grille gs, grille gd){
	int i, j;
	for (i=0; i<gs.nbl; ++i) for (j=0; j<gs.nbc; ++j) gd.cellules[i][j] = gs.cellules[i][j];
	return;	
}

/**
 * alloue une grille de taille l*c, et initialise toutes les cellules à mortes
 * \relatesalso grille
 * \param l int (nombre de lignes)
 * \param c int (nombre de colonnes)
 * \param g grille*
 * \returns \c void
 */
void alloue_grille (int l, int c, grille* g) {
	g->cellules = (int**)malloc(l*sizeof(int*));
	for (int i = 0; i < l; i++)
		g->cellules[i] = (int*)malloc(c*sizeof(int));
	//initialise toutes les cellules à mortes
	for (int i = 0; i < l; i++) {
		for (int j = 0; j < c; j++)
			g->cellules[i][j] = 0;
	}
	//initialise les champs nbl et nbc
	g->nbl = l;
	g->nbc = c;
}

/**
 * libère une grille
 * \relatesalso grille
 * \param g grille*
 * \returns \c void
 */
void libere_grille (grille* g) {
	for (int i = 0; i < g->nbl; i++)
		free(g->cellules[i]);
	free(g->cellules);
}


/**
 * après avoir désactivé le calcul de vieillissement on réinitialise les ages des cellules vivantes à 1
 * \relatesalso grille
 * \param l int (nombre de lignes)
 * \param c int (nombre de colonnes du grille)
 * \param g grille* 
 * \returns \c void
*/
void reset_ages(int l, int c, grille *g) {
	for (int i = 0; i < l; i++) {
		for (int j = 0; j < c; j++) {
			if (est_vivante(i,j,*g))
				set_vivante(i,j,*g);
		}
	}
}

/**
 * teste si une grille a des cellules vivantes
 * \relatesalso grille
 * \param g grille* 
 * \returns 1 si la grille est "vide" (pas des cellules vivantes) , 0 sinon
*/
int grille_vide(grille* g) {
	for (int i = 0; i < g->nbl; i++) {
		for (int j = 0; j < g->nbc; j++) {
			if (g->cellules[i][j] > 0)
				return 0;
		}
	}
	return 1;
}

/**
 * teste si deux grilles g1 et g2 sont strictement egaux
 * \relatesalso grille
 * \param g1 grille* 
 * \param g2 grille* 
 * \returns 1 si les deux grilles sont egaux 0 sinon
*/
int grilles_egaux(grille* g1, grille* g2) {
	for (int i = 0; i < g1->nbl; i++) {
		for (int j = 0; j < g1->nbc; j++) {
			if (g1->cellules[i][j] != g2->cellules[i][j])
				return 0;
		}
	}
	return 1;
}