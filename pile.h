#ifndef PILE_H
#define PILE_H

#include "listes.h"

//structure de valeur
struct valeur {
    int v_bool; //vaut 0 si groupe de commandes
                //vaut 1 si int
    int v_int;
    sequence_t groupe; //gp pour groupe de commandes
};
typedef struct valeur val;


//structure pile pour stocker les opérations, mesures, programmes, etc.
struct cel {
    val valeur;
    struct cel *suivant;
};
typedef struct cel cellule_val;

struct pile {
    cellule_val *tete;
};
typedef struct pile pile_val;



//ajout au sommet
void empiler(pile_val *p, val valeur);

//dépile et renvoie l'element au sommet
val depiler(pile_val *p);

void liberer_pile(pile_val *p);

//echange les 2 elements au sommet entre eux
void echanger(pile_val *p);

//clone l'element au sommet
void cloner(pile_val *p);

//deplace l'element d'indice n au sommet
void deplacer(pile_val *p, int n);

//fait x rotations des n premiers elements
void rotation(pile_val *p, int n, int x);

//execute un groupe de commandes
int exec_groupe_commandes(val *cmd1, val *cmd2, val valeur, bool debug);

void afficher_pile(pile_val *p);


#endif