#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <time.h>
#include "libTableauNoir.h"

#include "demineur.h"


// Déclaration des types et structures utilisateur.


// Déclaration des fonctions utilisateur.



int main(int argc, char* argv[])
{
    int dimX = 10;
    int dimY = 10;
    int NbMines = 10;

    if(argc == 4)
    {
        dimX = atoi(argv[1]);
        dimY = atoi(argv[2]);
        NbMines = atoi(argv[3]);
    }
    else if(argc > 1)
    {
        fprintf(stderr, "Usage : ./%s [NbLignes NbColonnes NbMines]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    grille_t* grille = nouvelle_partie(dimX, dimY, NbMines);

    int i;
    int j;
    int drapeau = 0;
    int gagne;
    do
    {
        afficher_grille2(grille);
        printf("Ligne : ");
        scanf("%d", &i);
        printf("Colonne : ");
        scanf("%d", &j);
        //printf("mine ? ");
        //scanf("%d", &drapeau);
        printf("\n");
        gagne = jouer_coup(grille, i-1, j-1, drapeau);
        if(!drapeau) ++drapeau;
    }
    while(!gagne);

    printf("\n");
    afficher_grille(grille);

    if(gagne == -1) printf("Partie PERDUE... \n");
    if(gagne == 1) printf("Partie GAGNÉE !!! \n");

    return EXIT_SUCCESS;
}


// Définition des fonctions utilisateur.
