#include "jeu.h"

/**
 * \file jeu.c
 * code source avec des fonctions auxilieres pour le deroulement du jeu
 * \author David Pulido Cornejo
 */

int vieillissement;
int (*compte_voisins_vivants) (int, int, grille);

/**
 * compte le nombre de voisins vivants de la cellule (i,j) les bords sont cycliques.
 * \param i int
 * \param j int
 * \param g grille
 * \returns \c int : n
 */
int compte_voisins_cyclique (int i, int j, grille g){
	int v = 0, l=g.nbl, c = g.nbc;
	v+= est_vivante(modulo(i-1,l),modulo(j-1,c),g);
	v+= est_vivante(modulo(i-1,l),modulo(j,c),g);
	v+= est_vivante(modulo(i-1,l),modulo(j+1,c),g);
	v+= est_vivante(modulo(i,l),modulo(j-1,c),g);
	v+= est_vivante(modulo(i,l),modulo(j+1,c),g);
	v+= est_vivante(modulo(i+1,l),modulo(j-1,c),g);
	v+= est_vivante(modulo(i+1,l),modulo(j,c),g);
	v+= est_vivante(modulo(i+1,l),modulo(j+1,c),g);

	return v;
}

/**
 * compte le nombre de voisins vivants de la cellule (i,j) les bords ne sont pas cycliques.
 * \param i int
 * \param j int
 * \param g grille
 * \returns \c int : n
 */
int compte_voisins_noncyclique (int i, int j, grille g){
	int v = 0, l=g.nbl, c = g.nbc;
	int voisins[8][2] = {{1,0},{0,1},{1,1},{1,-1},{-1,1},{-1,0},{0,-1},{-1,-1}};

	for (int q = 0; q < 8; q++) {
        if (dedans_grille(i+voisins[q][0],j+voisins[q][1],l,c))
            v+=est_vivante(i+voisins[q][0],j+voisins[q][1],g);
	}

	return v;
}

/**
 * fait évoluer la grille g d'un pas de temps
 * \param g grille*
 * \param gc grille*
 * \returns \c void
*/
void evolue (grille *g, grille *gc){
	copie_grille (*g,*gc); // copie temporaire de la grille

	int i,j,l=g->nbl, c = g->nbc,v;

	for (i=0; i<l; i++)
	{
		for (j=0; j<c; ++j)
		{
			v = compte_voisins_vivants (i, j, *gc);
			if (est_vivante(i,j,*g))
			{ // evolution d'une cellule vivante
                		if (vieillissement)
                    			g->cellules[i][j]++;
				if ( (v!=2 && v!= 3) || (vieillissement && g->cellules[i][j] >= 8) ) set_morte(i,j,*g);
			}
			else
			{ // evolution d'une cellule morte
				if ( v==3 && !est_non_viable(i,j,g)) set_vivante(i,j,*g);
			}
		}
	}
	return;
}

/** calcul de la periodicité de la grille actuelle
 * \param g grille*
 * \returns periode int (periode == N_TESTS s'il n'y a pas de periodicité dans les prochaines N_TESTS pas)  
 */
int calcul_periode(grille* g) {
	int periode = 0;

	grille test_osc;
	grille test_osc_copie;

	alloue_grille(g->nbl, g->nbc, &test_osc);
	alloue_grille(g->nbl, g->nbc, &test_osc_copie);

	copie_grille(*g,test_osc);

	do {
		evolue(&test_osc, &test_osc_copie);
		periode++;
	} while (periode < N_TESTS && !grilles_egaux(&test_osc, g));
	//100 tests pour voir si la grille est oscillante (lorsequ'elle n'est pas vide)

	libere_grille(&test_osc);
	libere_grille(&test_osc_copie);

	return periode;
}

/**
 * calcul de la periode la plus proche au temps d'evolution courant de la grille actuel
 * \param g grille*
 * \param delta_temps_ev int* offset entre temps d'evolution actuel et temps d'evolution où il y a une colonie oscillante
 * \returns periode int, N_TESTS si on n'a pas trouvé d'oscillation
 */
int calcul_oscillation(grille* g, int *delta_temps_ev) {
	int periode;
	int i;

	grille g_actuel;
	grille g_actuel_copie;

	alloue_grille(g->nbl, g->nbc, &g_actuel);
	alloue_grille(g->nbl, g->nbc, &g_actuel_copie);
	
	copie_grille(*g, g_actuel);

	for (i = 0; i < N_TESTS; i++) {
		periode = calcul_periode(&g_actuel);
		//si on a trouvé une periodicité alors renvoyer ce periode
		if (periode != N_TESTS) break;
		//test de la prochaine grille
		evolue(&g_actuel, &g_actuel_copie);
	}

	libere_grille(&g_actuel);
	libere_grille(&g_actuel_copie);

	*delta_temps_ev = i; 
	return periode;
}

