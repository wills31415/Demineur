#include "demineur.h"


/* Définition des fonctions du Démineur. */

grille_t* creer_grille(int dimX, int dimY, int NbMines)
{
    srand(time(NULL));
    if(dimX <= 0 || dimY <= 0) return NULL;
    grille_t* grille = (grille_t*)malloc(sizeof(struct grille_s));
    grille->dimX = dimX;
    grille->dimY = dimY;
    grille->NbMines = NbMines;
    grille->grid = (int**)malloc(dimY * sizeof(int*));
    for(int i = 0; i < dimY; ++i) grille->grid[i] = (int*)calloc(dimX, sizeof(int));
    grille->NbMinesDecouvertes = 0;
    grille->grid2 = (int**)malloc(dimY * sizeof(int*));
    for(int i = 0; i < dimY; ++i) grille->grid2[i] = (int*)calloc(dimX, sizeof(int));
    for(int i = 0; i < dimY; ++i) for(int j = 0; j < dimX; ++j) grille->grid2[i][j] = -2;

    /* Mélange des Mines dans la grille. */
    int ki, kj;
    while(NbMines > 0)
    {
        srand(rand() * clock());
        ki = (int)((double)rand() / RAND_MAX * dimY);
        kj = (int)((double)rand() / RAND_MAX * dimX);
        if(grille->grid[ki][kj] != -1)
        {
            grille->grid[ki][kj] = -1;
            --NbMines;
        }
    }

    return grille;
}


int proximite_mine(grille_t* grille, int i, int j)
{
    if(i < 0 || j < 0 || i >= grille->dimY || j >= grille->dimX) return 0;
    if(grille->grid[i][j] == -1) return -1;
    int c = 0;
    for(int ki = -1; ki <= 1; ++ki)
    {
        for(int kj = -1; kj <= 1; ++kj)
        {
            if(!(i+ki < 0 || j+kj < 0 || i+ki >= grille->dimY || j+kj >= grille->dimX) && grille->grid[i+ki][j+kj] == -1)
            ++c;
        }
    }
    return c;
}

void proximite_grille(grille_t* grille)
{
    for(int i = 0; i < grille->dimY; ++i)
    {
        for(int j = 0; j < grille->dimX; ++j)
        {
            grille->grid[i][j] = proximite_mine(grille, i, j);
        }
    }
}


void afficher_grille(grille_t* grille)
{
    for(int i = 0; i < grille->dimY; ++i)
    {
        for(int j = 0; j < grille->dimX; ++j)
        {
            if(grille->grid[i][j] == 0) printf("%2c ", 46);
            else if(grille->grid[i][j] == -1) printf("%2c ", 64);
            else printf("%2d ", grille->grid[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    return;
}

void afficher_grille2(grille_t* grille)
{
    for(int i = 0; i < grille->dimY; ++i)
    {
        for(int j = 0; j < grille->dimX; ++j)
        {
            if(grille->grid2[i][j] == 0) printf("%2c ", 46);
            else if(grille->grid2[i][j] == -1) printf("%2c ", 64);
            else if(grille->grid2[i][j] == -2) printf("%2c ", 35);
            else printf("%2d ", grille->grid2[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    return;
}


static void update_grille(grille_t* grille, int i, int j)
{
    if(i < 0 || i >= grille->dimY || j < 0 || j >= grille->dimX || grille->grid2[i][j] != -2) return;
    grille->grid2[i][j] = grille->grid[i][j];
    if(grille->grid2[i][j] != 0) return;
    for(int ki = -1; ki <= 1; ++ki)
    for(int kj = -1; kj <= 1; ++kj)
    if(!(i+ki < 0 || i+ki >= grille->dimY || j+kj < 0 || j+kj >= grille->dimX)) update_grille(grille, i+ki, j+kj);
    return;
}

int jouer_coup(grille_t* grille, int i, int j, int mine)
{
    if(i < 0 || i >= grille->dimY || j < 0 || j >= grille->dimX || mine < 0 || mine > 1) return 0;
    if(mine == 1)
    {
        if(grille->grid2[i][j] == -1)
        {
            grille->grid2[i][j] = -2;
            grille->NbMinesDecouvertes -= 1;
        }
        else if(grille->grid2[i][j] == -2 && grille->NbMines > grille->NbMinesDecouvertes)
        {
            grille->grid2[i][j] = -1;
            grille->NbMinesDecouvertes += 1;
        }
        else if(grille->grid2[i][j] > 0)
        {
            int mines = 0;
            for(int ki = -1; ki <= 1; ++ki)
            for(int kj = -1; kj <= 1; ++kj)
            if(!(i+ki < 0 || i+ki >= grille->dimY || j+kj < 0 || j+kj >= grille->dimX) && grille->grid2[i+ki][j+kj] == -1) ++mines;

            if(mines >= grille->grid2[i][j])
            for(int ki = -1; ki <= 1; ++ki)
            for(int kj = -1; kj <= 1; ++kj)
            if(!(i+ki < 0 || i+ki >= grille->dimY || j+kj < 0 || j+kj >= grille->dimX) && grille->grid2[i+ki][j+kj] == -2) jouer_coup(grille, i+ki, j+kj, 0);
        }
    }
    else if(proximite_mine(grille, i, j) == -1) return -1;
    else update_grille(grille, i, j);

    return grille_resolue(grille);
}
//tous les drapeaux déjà placés recouvrent des mines
// ET
// (
//toutes les mines ont été recouvertes par des drapeaux
// OU
//toutes les cases non-recouvertes sont des mines
// )
int grille_resolue(grille_t* grille)
{
    int mines_trouvees = 0, cases_inconnues = 0;
    for(int i = 0; i < grille->dimY; ++i)
    for(int j = 0; j < grille->dimX; ++j)
    {
        if(grille->grid[i][j] == -1 && grille->grid2[i][j] == -1) ++mines_trouvees;
        if(grille->grid2[i][j] == -2) ++cases_inconnues;
    }

    if(grille->NbMinesDecouvertes > mines_trouvees) return 0;

    if(grille->NbMines == grille->NbMinesDecouvertes) return 1;
    if(cases_inconnues == grille->NbMines - mines_trouvees) return 1;

    return 0;
}


grille_t* nouvelle_partie(int dimX, int dimY, int NbMines)
{
    grille_t* grille = creer_grille(dimX, dimY, NbMines);
    proximite_grille(grille);
    return grille;
}
