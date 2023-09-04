#include "io_cairo.h"

/**
 * \file io_cairo.c
 * code source pour la gestion des inputs et outputs avec un affichage graphique cairo
 * \author David Pulido Cornejo
 */

/**
 * affichage du perimetre de la grille
 * \relatesalso grille
 * \param cr cairo_t* , contexte de dessin cairo
 * \param g grille* , pointeur vers la grille
 * \returns \c void
 */
void paint_grid(cairo_t* cr, grille* g) {

	double start_x = (SIZEX - SQUARE_SIZE * g->nbc) / 2;
	double start_y = (SIZEY - SQUARE_SIZE * g->nbl) / 2;

	double max_x = start_x + SQUARE_SIZE * g->nbc; //valeur maximal en x
	double max_y = start_y + SQUARE_SIZE * g->nbl; //valeur maximal en y pour faire le perimetre
	cairo_set_source_rgb (cr, 1.0, 1.0, 1.0);

	//draw lines
	for (int i = 0; i < g->nbl + 1; i++) {
		cairo_move_to (cr, start_x, start_y + (i*SQUARE_SIZE));
		cairo_line_to(cr, max_x, start_y + (i*SQUARE_SIZE));
		cairo_set_line_width (cr, 1);
		cairo_stroke (cr);
	}
	//draw columns
	for (int i = 0; i < g->nbc + 1; i++) {
		cairo_move_to (cr, start_x + (i*SQUARE_SIZE), start_y);
		cairo_line_to(cr, start_x + (i*SQUARE_SIZE), max_y);
		cairo_set_line_width (cr, 1);
		cairo_stroke (cr);
	}
}

/**
 * affichage d'un carré RBG dans les coordonnées (i,j)
 * \param cr cairo_t* , contexte de dessin cairo
 * \param g grille*
 * \param i int
 * \param j int
 * \param R double , valeur rouge
 * \param G double , valeur vert
 * \param B double , valeur bleu
 * \returns \c void
 */
void paint_square_at(cairo_t* cr, grille* g, double i, double j, double R, double G, double B) {
	double start_x = (SIZEX - SQUARE_SIZE * g->nbc) / 2;
	double start_y = (SIZEY - SQUARE_SIZE * g->nbl) / 2;

	double x = start_x + i * SQUARE_SIZE;
	double y = start_y + j * SQUARE_SIZE;

	cairo_rectangle(cr, x, y, SQUARE_SIZE, SQUARE_SIZE);
	cairo_set_source_rgb (cr, R, G, B);
	cairo_fill(cr);
}

/**
 * affichage de tous les cellules vivantes (mais aussi non viables) de la grille,
 * de plus si le vieillisement est desactivé on affiche chaque cellule comme un carré vert, sinon, la couler tend vers le bleu
 * \relatesalso grille
 * \param cr cairo_t* , contexte de dessin cairo
 * \param g grille* , pointeur vers la grille
 * \returns \c void
 */
void paint_vivantes(cairo_t* cr, grille* g) {
	double age;

	for (int i = 0; i < g->nbl; i++) {
		for (int j = 0; j < g->nbc; j++) {
			if (g->cellules[i][j] >= 1) {
				age = g->cellules[i][j] - 1;
				if (!vieillissement)
					paint_square_at(cr, g, j, i, 0, 1, 0); //paint green square
				else
					paint_square_at(cr, g, j, i, 0, 1-(age/8),(age/10) ); //paint square that shifts to blue
			}
			else if (g->cellules[i][j] == -1)
				paint_square_at(cr, g, j, i, 1, 0, 0);  //paint red square
		}
	}
}

/**
 * affichage du perimetre de la grille et des cellules
 * \relatesalso grille
 * \param cr cairo_t* , contexte de dessin cairo
 * \param g grille* , pointeur vers la grille
 * \returns \c void
 */
void paint_grille (cairo_surface_t *surface, grille* g) {
	cairo_t *cr;
	cr = cairo_create(surface);

	paint_vivantes(cr, g);
	paint_grid(cr, g);

	cairo_destroy(cr);
}

/**
 * nettoyage du display
 * \param surface cairo_surface_t*
 * \returns \c \void
*/
void clean_screen(cairo_surface_t *surface) {
	cairo_t *cr = cairo_create(surface);

	cairo_set_operator (cr, CAIRO_OPERATOR_CLEAR);
	cairo_paint (cr);

	cairo_destroy(cr);
}

/**
 * affichage du temps d'evolution et des variables de control au dessus de la grille dans le display
 * \param surface cairo_surface_t*
 * \param temps int , temps d'evolution
 * \param v_cyclique int , boolean qui represent l'etat du voisinage cyclique
 * \returns \c \void
*/
void paint_texte(cairo_surface_t *cs, int temps, int v_cyclique) {
	char buffer[STR_SIZE];
	cairo_t *cr = cairo_create(cs);

	sprintf(buffer, "%d", temps);	//int to string (du temps d'evolution)

	cairo_select_font_face (cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size (cr, 16);
	cairo_set_source_rgb (cr, 1, 1, 1);
	cairo_move_to (cr, OFFSETX - SQUARE_SIZE, OFFSETY/2);

    cairo_show_text (cr, "Temps d’évolution : ");
    cairo_show_text (cr, buffer);
    cairo_show_text (cr, "    ,   Calcul des ages : ");
    if (vieillissement) {
		cairo_set_source_rgb (cr, 0, 1, 0); 	//set color to green
    	cairo_show_text(cr,"ACTIVÉ");
	}
	else {
		cairo_set_source_rgb (cr, 1, 0, 0);
    	cairo_show_text(cr,"DESACTIVÉ");		//set color to red
	}
	cairo_set_source_rgb (cr, 1, 1, 1);
    cairo_show_text (cr, "    ,   Calcul cyclique : ");
    if (v_cyclique) {
		cairo_set_source_rgb (cr, 0, 1, 0);		//set color to green
    	cairo_show_text(cr,"ACTIVÉ");
	}
    else {
		cairo_set_source_rgb (cr, 1, 0, 0);		//set color to red
    	cairo_show_text(cr,"DESACTIVÉ");
	}
	cairo_destroy(cr);
}

/**
 * Affichage du texte et du grille après avoir nettoyé l'ecran
 * \param surface cairo_surface_t*
 * \param g grille*
 * \param temps int , temps d'evolution
 * \param v_cyclique int , boolean qui represent l'etat du voisinage cyclique
 * \returns \c \void
*/
void paint(cairo_surface_t *cs, grille* g, int temps, int v_cyclique) {
	clean_screen(cs);
	paint_texte(cs,temps, v_cyclique);
	paint_grille(cs,g);
}

/**
 * debute le jeu , le voisinage cyclique et le calcul du vieillissement sont initialisés comme vrais
 * \relatesalso grille
 * \param g grille* , grille principale
 * \param gc grille* , grille utilisée pour des operations intermediaires
 * \returns \c \void
 */
void debut_jeu_cairo(grille *g, grille *gc) {
	//initialisation des variables de control
	int quit = 0;
	int t_evolution = 0;
  int v_cyclique = 1;
	vieillissement = 1;

	compte_voisins_vivants = compte_voisins_cyclique;

	// X11 display
	Display *dpy;
	Window rootwin;
	Window win;
	XEvent e;
	int scr;

	KeySym key;
	char keybuffer[8];

	// init the display
	if(!(dpy=XOpenDisplay(NULL))) {
		fprintf(stderr, "ERROR: Could not open display\n");
		exit(1);
	}

	scr=DefaultScreen(dpy);
	rootwin=RootWindow(dpy, scr);

	win=XCreateSimpleWindow(dpy, rootwin, 1, 1, SIZEX, SIZEY, 0,
			BlackPixel(dpy, scr), BlackPixel(dpy, scr));

	XStoreName(dpy, win, "Game of Life - David Pulido");
	XSelectInput(dpy, win, ExposureMask|ButtonPressMask|KeyPressMask);
	XMapWindow(dpy, win);

	// create cairo surface
	cairo_surface_t *cs;
	cs = cairo_xlib_surface_create(dpy, win, DefaultVisual(dpy, 0), SIZEX, SIZEY);

	XNextEvent(dpy, &e);
	while (!(e.type == ButtonPress && e.xbutton.button == Button3) && !quit) //while not right clicking
	{
		XNextEvent(dpy, &e);
		paint(cs, g, t_evolution,v_cyclique);

		//if left mouse click
		if (e.type == ButtonPress && e.xbutton.button == Button1) {
			evolue(g,gc);
			t_evolution++;
			paint(cs, g, t_evolution,v_cyclique);
		}
		//if keyboard stroke
		else if (e.type == KeyPress) {
			//on obtien la touche specifique a partir d'un xkey
			XLookupString(&e.xkey, keybuffer, sizeof(keybuffer), &key ,NULL);
			switch (key) {
				case 'n' :
				{
					//touche 'n' pour charger dynamiquement une nouvelle grille
					char str[STR_SIZE];

					printf("Saisissez le nom d’une nouvelle grille (exemple : ./grilles/grille3.txt)\n: ");
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
					paint(cs, g, t_evolution,v_cyclique);

					break;
				}
				case 'c':
				{
					//touche 'c' pour activer / désactiver le voisinage cyclique
					v_cyclique = !v_cyclique;
		      		//on point vers le calcul cyclique si v_cyclique
          			if (v_cyclique) {
          				compte_voisins_vivants = compte_voisins_cyclique;
								}
          			else {
          				compte_voisins_vivants = compte_voisins_noncyclique;
								}
					//on affiche la grille à nouveau dans le meme temps d'evolution
					paint(cs, g, t_evolution,v_cyclique);

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
					paint(cs, g, t_evolution,v_cyclique);

					break;
				}
				case 'o':
				{
					int periode = 0;
					int delta_temps_ev;

					char buffer[STR_SIZE];
					cairo_t *cr = cairo_create(cs);

					cairo_select_font_face (cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
					cairo_set_font_size (cr, 16);
					cairo_set_source_rgb (cr, 1, 1, 1);

					cairo_move_to (cr, OFFSETX + 50, SIZEY-50);

					//voir jeu.c
					periode = calcul_oscillation(g, &delta_temps_ev);

					if (periode != N_TESTS) {
						sprintf(buffer, "%d", periode); //int to string

						cairo_show_text (cr, "Oscillation de periode ");
						cairo_show_text (cr, buffer);
						sprintf(buffer, "%d", (t_evolution + delta_temps_ev));
						cairo_show_text(cr, " à partir du temps d'evolution ");
						cairo_show_text (cr, buffer);
					} else {
						sprintf(buffer, "%d", N_TESTS); //int to string
						cairo_show_text (cr, "La colonie n'est pas oscillante dans les prochaines ");
						cairo_show_text (cr, buffer);
						cairo_show_text (cr, " temps d'evolution");
					}

					cairo_destroy(cr);
					break;
				}
				case 'q':
				{
					quit = 1;
					break;
				}
			}
		}
	}
	cairo_surface_destroy(cs); // destroy cairo surface
	XCloseDisplay(dpy); // close the display
	return;
}

