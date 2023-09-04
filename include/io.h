#ifndef __IO_H
#define __IO_H



/**
 * \file io.h
 * header pour la gestion des input et outputs
 * \author David Pulido Cornejo
 */

#include <stdio.h>
#include "grille.h"
#include "jeu.h"

#define STR_SIZE 256
#define PATH "./grilles/"

void affiche_trait (int c);

//affichage des lignes avec l'age des cellules
void affiche_ligne (int c, int* ligne);

void affiche_grille (grille g, int temps, int cyclique);

void efface_grille (grille g);

void debut_jeu(grille *g, grille *gc);

#endif
