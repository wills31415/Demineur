#ifndef _DEMINEUR_H
#define _DEMINEUR_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "libTableauNoir.h"


/* Déclaration des types et structures du Démineur. */

typedef struct grille_s grille_t;
struct grille_s
{
    int dimX;
    int dimY;
    int NbMines;
    int** grid;
    int NbMinesDecouvertes;
    int** grid2;
};

/* Déclaration des fonctions du Démineur. */

grille_t* creer_grille(int dimX, int dimY, int NbMines);

int proximite_mine(grille_t* grille, int i, int j);
void proximite_grille(grille_t* grille);

void afficher_grille(grille_t* grille);
void afficher_grille2(grille_t* grille);

int jouer_coup(grille_t* grille, int i, int j, int mine);

int grille_resolue(grille_t* grille);

grille_t* nouvelle_partie(int dimX, int dimY, int NbMines);


#endif
