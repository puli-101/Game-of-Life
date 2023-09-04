#ifndef __IO_CAIRO_H
#define __IO_CAIRO_H

/**
 * \file io_cairo.h
 * header pour la gestion des input et outputs avec un affichage graphique cairo
 * \author David Pulido Cornejo
 */

#include "grille.h"
#include "jeu.h"
#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>
#include <cairo-xlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define SIZEX 800
#define SIZEY 800

#define OFFSETX 100
#define OFFSETY 200

#define SQUARE_SIZE 45

#define STR_SIZE 256
#define PATH "./grilles/"

//affichage de tous les cellules vivantes (mais aussi non viables) de la grille,
//de plus si le vieillisement est desactivé on affiche chaque cellule comme un carré vert, sinon, la couler tend vers le violet
void paint_vivantes(cairo_t* cr, grille* g);

//affichage du perimetre de la grille
void paint_grid(cairo_t* cr, grille* g);

//affichage du perimetre de la grille et des cellules
void paint_grille (cairo_surface_t *surface, grille* g);

//affichage du temps d'evolution et des variables de control au dessus de la grille dans le display
void paint_texte(cairo_surface_t *cs, int temps, int v_cyclique);

//nettoyage du display
void clean_screen(cairo_surface_t *surface);

//affichage d'un carré RBG dans les coordonnées (i,j)
void paint_square_at(cairo_t* cr, grille* g, double i, double j, double R, double G, double B);

//Affichage du texte et du grille après avoir nettoyé l'ecran
void paint(cairo_surface_t *cs, grille* g, int temps, int v_cyclique);

//debute le jeu , le voisinage cyclique et le calcul du vieillissement sont initialisés comme vrais
void debut_jeu_cairo(grille* g, grille *gc);

#endif

