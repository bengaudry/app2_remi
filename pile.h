#ifndef PILE_H
#define PILE_H

#include "listes.h"


////// DEFINITION DES TYPES POUR LA PILE //////

// Valeur d'une cellule de pile
typedef struct valeur {
    int v_bool; //vaut 0 si groupe de commandes
                //vaut 1 si int
    int v_int;
    sequence_t groupe; //gp pour groupe de commandes
} val;


// Cellule de pile pour stocker les opérations, mesures, programmes, etc...
typedef struct cel {
    val valeur;
    struct cel *suivant;
} cellule_pile;

// Type de la pile
typedef struct {
    cellule_pile *tete;
} pile;


////// FONCTIONS LIEES A LA PILE //////

/* Ajout d'un élément en haut de la pile */
void empiler(pile *p, val valeur);

/* Dépile et renvoie l'element au sommet */
val depiler(pile *p);

/* Dépile et renvoie l'élément en fin de pile */
val depiler_fin(pile *p);

/* 
 * Inversion de la pile
 * a1,a2,...,an devient an,an-1,...,a1
 */
void inverser_pile (pile *l);

/* Libère la mémoire allouée à chaque cellule de la pile, et à la pile elle même*/
void liberer_pile(pile *p);

/* Echange les 2 elements au sommet entre eux */
void echanger(pile *p);

/* Clone l'element au sommet de la pile */
void cloner(pile *p);

/* Deplace l'element d'indice n au sommet */
void deplacer(pile *p, int n);

/* Effectue x rotations des n premiers elements */
void rotation(pile *p, int n, int x);

/* Execute un groupe de commandes */
int exec_groupe_commandes(val *cmd1, val *cmd2, val *valeur, bool debug);

/* Affiche la valeur d'une cellule de la pile */
void afficher_valeur_cellule(val valeur);

/* Affiche les éléments de la pile */
void afficher_pile(pile *p);


#endif