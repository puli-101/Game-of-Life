#include "io.h"

/**
 * \file io.c
 * code source pour la gestion des inputs et outputs
 * \author David Pulido Cornejo
 */

/**
 * affichage d'un trait horizontal
 * \param c int (taille du trait)
 * \returns \c void
 */
void affiche_trait (int c){
	int i;
	for (i=0; i<c; ++i) printf ("|---");
	printf("|\n");
	return;
}


/**
 * affichage d'une ligne de la grille , si la cellule est vivante alors on affiche l'age si le vieillissement est active, sinon on affiche le caractere 'O'
 * \relatesalso grille
 * \param c int , taille de la ligne
 * \param ligne int* , pointeur vers la ligne
 * \returns \c void
 */
void affiche_ligne (int c, int* ligne){
        int i;
        for (i=0; i<c; ++i) {
        	if (ligne[i] == 0 )
                	printf ("|   ");
                else if (ligne[i] == -1)
                	printf("| X ");
                else {
                	if (vieillissement)
                		printf ("| %d ",(ligne[i]-1));
                	else
                		printf ("| O ");
                }
        }
        printf("|\n");
        return;
}

/**
 *  affichage d'une grille
 *  \relatesalso grille
 *  \param g grille
 *  \param temps int (temps d'évolution courant)
 *  \param cyclique int (1 si le calcul de voisinage des cellules est fait cycliquement, 0 sinon)
 *  \returns \c void
 */
void affiche_grille (grille g, int temps, int cyclique){
	int i, l=g.nbl, c=g.nbc;

	printf("\n");
        printf("Temps d’évolution : %d\nCalcul des ages : %s  ,  Calcul cyclique : %s\n", temps,
                        (vieillissement?"ACTIVÉ":"DESACTIVÉ"),(cyclique?"ACTIVÉ":"DESACTIVÉ"));
	affiche_trait(c);
	for (i=0; i<l; ++i) {
		affiche_ligne(c, g.cellules[i]);
		affiche_trait(c);
	}
	printf("\n");
	return;
}

/**
 * effacement d'une grille
 * \relatesalso grille
 * \param g grille
 * \returns \c void
 */
void efface_grille (grille g){
    //printf("\n\e[%dA",num_lignes);
    printf("\e[1;1H\e[2J");
}

/**
 * debute le jeu , le voisinage cyclique et le calcul du vieillissement sont initialisés comme vrais
 * \relatesalso grille
 * \param g grille* , grille principale
 * \param gc grille* , grille utilisée pour des operations intermediaires
 * \returns \c \void
 */
void debut_jeu(grille *g, grille *gc){
	char c;

	//initialisation des variables de control
	int t_evolution = 0;
    	int v_cyclique = 1;
	vieillissement = 1;

	compte_voisins_vivants = compte_voisins_cyclique;

	//premier affichage
	affiche_grille(*g, t_evolution, v_cyclique);
	c = getchar();

	while (c != 'q') // touche 'q' pour quitter
	{
		switch (c) {
			case '\n' :
			{
				// touche "entree" pour évoluer
				evolue(g,gc);
				t_evolution++;
				efface_grille(*g);
				affiche_grille(*g, t_evolution, v_cyclique);
				break;
			}
			case 'n' :
			{
				//touche 'n' pour charger dynamiquement une nouvelle grille
				char str[STR_SIZE];

				printf("Saisissez le nom d’une nouvelle grille (exemple : ./grilles/grille3.txt)\n:");
				scanf(" %s",str);

				//on efface la terminale
				printf("\e[1;1H\e[2J");

                		//on libere la derniere grille
				libere_grille(g);
				libere_grille(gc);
				//on allue la nouvelle grille
				init_grille_from_file(str,g);
				alloue_grille(g->nbl, g->nbc, gc);
				//on reinitialise t_evolution, v_cyclique, vieillissement
				compte_voisins_vivants = compte_voisins_cyclique;
				vieillissement = 1;
				t_evolution = 0;
				v_cyclique = 1;
                		//et on l'affiche
				affiche_grille(*g, t_evolution, v_cyclique);

                c = getchar();
				break;
			}
			case 'c':
			{
				//touche 'c' pour activer / désactiver le voisinage cyclique
		        v_cyclique = !v_cyclique;

				//on point vers le calcul cyclique si v_cyclique
                if (v_cyclique)
                	compte_voisins_vivants = compte_voisins_cyclique;
                else
                	compte_voisins_vivants = compte_voisins_noncyclique;

				//on affiche la grille à nouveau dans le meme temps d'evolution
				efface_grille(*g);
				affiche_grille(*g, t_evolution, v_cyclique);

                c = getchar();
                break;
			}
			case 'v':
			{
				//touche 'v' pour activer / désactiver le vieillisement
                vieillissement = !vieillissement;
				//si on desactive le calcul du vieillissement on reinitialise les ages a 1
                if (!vieillissement)
					reset_ages(g->nbl, g->nbc, g);

				//on affiche la grille à nouveau dans le meme temps d'evolution
				efface_grille(*g);
				affiche_grille(*g, t_evolution, v_cyclique);

                c = getchar();
                break;
			}
			case 'o':
			{
				int periode = 0;
				int delta_temps_ev;

				//voir jeu.c
				periode = calcul_oscillation(g, &delta_temps_ev);

				if (periode != N_TESTS)
					printf("Oscillation de periode %d à partir du temps d'évolution %d\n",
						periode,(t_evolution + delta_temps_ev));
				else
					printf("La colonie n'est pas oscillante dans les prochaines %d temps d'evolution\n",N_TESTS);

				c = getchar();
				break;
			}
			default :
			{
				// touche non traitée
				printf("\n\e[1A");
				break;
			}
		}
		c = getchar();
	}
	return;
}

