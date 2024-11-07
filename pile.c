#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "pile.h"
#include "listes.h"
#include "interprete.h"


void empiler (pile_val *p, val valeur) {
    cellule_val *cel = malloc(sizeof(cellule_val));

    cel->valeur = valeur;
    cel->suivant = p->tete;
    p->tete = cel;
}   

val depiler(pile_val *p) {
    val valeur = p->tete->valeur;
    p->tete = p->tete->suivant;
    return valeur;
}

//echange entre eux les 2 elements en sommet de pile
void echanger(pile_val *p) {
    cellule_val *tete = p->tete;
    cellule_val *suiv = tete->suivant;
    
    tete->suivant = suiv->suivant;
    suiv->suivant = tete;
    p->tete = suiv;
}

void cloner(pile_val *p) {
    cellule_val *cel = malloc(sizeof(cellule_val));

    if (p == NULL) return;

    cel->valeur = p->tete->valeur;   
    cel->suivant = p->tete;          
    p->tete = cel;      
}

void deplacer(pile_val *p, int n) {
    cellule_val *cel = p->tete;
    cellule_val *cel_pre = p->tete;
    
    for (int i=2; i<n; i++) {
        cel_pre = cel_pre->suivant;
    }
    cel = cel_pre->suivant;

    empiler(p, cel->valeur);
    cel_pre->suivant = cel->suivant;
}

void rotation(pile_val *p, int n, int x) {
    for (int i=1; i<=x; i++) {
        deplacer(p, n);
    }
}

int exec_groupe_commandes(val cmd1, val cmd2, val valeur, bool debug) {
    int ret;
    if (valeur.v_int == 0) {
        printf("\n\nExecution du sous programme:\n");
        ret = interprete(&cmd1.gp, debug);
    }
    else {
        printf("\n\nExecution du sous programme:\n");
        ret = interprete(&cmd2.gp, debug);
    }
    return ret;
}


void afficher_pile(pile_val *p) {
    cellule_val *cel = p->tete;
    val valeur;
    while (cel != NULL) {
        valeur = cel->valeur;
        if (valeur.v_bool == 1) {
            printf("%d\n", valeur.v_int);
        }
        else {
            printf("{");
            afficher(&valeur.gp);
            printf("}\n");
        }
        cel = cel->suivant;
    }
}





