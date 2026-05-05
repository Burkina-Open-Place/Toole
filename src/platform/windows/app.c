#include "discovery.h"
#include "file_transfert.h"
#include "network.h"
#include "curses.h"
#include <stdio.h>
#include <stdlib.h>

WINDOW *menu_win;
WINDOW *content_win;
WINDOW *cfg_box;
WINDOW *log_box;

void setup_layout();

int main(void)
{
    // Initialisation
    initscr();// en gros c'est comme si on allume la machine
    clear();
    noecho();
    cbreak();
    curs_set(0); // Cache le curseur
    keypad(stdscr, TRUE); // Active les touches flechees
    nodelay(stdscr, TRUE);// sa permet a l'applicaton de ne pas bloquer a ecouter.


    // boucle du d'affichage .
}


void setup_layout(){

    int w_menu = 25;

    menu_win = newwin(LINES,w_menu,0,0);
    content_win = newwin(LINES,COLS - w_menu,0,20);

    int hauteur = 8;
    int largeur = (COLS - w_menu) -6;
    cfg_box = derwin(content_win, hauteur, largeur, 3, 3);

    int log_h = 6;
    int log_w = largeur;
    log_box = derwin(content_win, log_h, log_w, LINES - 9, 3);

}