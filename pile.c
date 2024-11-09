#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "pile.h"
#include "listes.h"
#include "interprete.h"


/* Ajout d'un élément en haut de la pile */
void empiler (pile *p, val valeur) {
    cellule_val *cel = malloc(sizeof(cellule_val));

    cel->valeur = valeur;
    cel->suivant = p->tete;
    p->tete = cel;
}


/* Dépile et renvoie l'element au sommet */
val depiler(pile *p) {
    val valeur;
    cellule_val *ancienne_tete;

    ancienne_tete = p->tete;
    valeur = p->tete->valeur;
    p->tete = p->tete->suivant;
    
    free(ancienne_tete);
    return valeur;
}


/* Libère la mémoire allouée à chaque cellule de la pile, et à la pile elle même*/
void liberer_pile(pile *p) {
    cellule_val *cel = p->tete;
    while (cel != NULL) {
        cellule_val *cel_p = cel;
        if (!cel->valeur.v_bool) { 
            liberer_seq_cmd(&cel->valeur.groupe); // Libère les groupes de commandes
        }
        cel = cel->suivant;
        free(cel_p);
    }
    p->tete = NULL; // Réinitialise la tête pour éviter un pointeur invalide
}


/* Echange les 2 elements au sommet entre eux */
void echanger(pile *p) {
    cellule_val *tete = p->tete;
    cellule_val *suiv = tete->suivant;
    
    tete->suivant = suiv->suivant;
    suiv->suivant = tete;
    p->tete = suiv;
}


/* Clone l'element au sommet de la pile */
void cloner(pile *p) {
    cellule_val *cel = malloc(sizeof(cellule_val));

    if (p == NULL) return;

    cel->valeur = p->tete->valeur;   
    cel->suivant = p->tete;          
    p->tete = cel;      
}


/* Deplace l'element d'indice n au sommet */
void deplacer(pile *p, int n) {
    cellule_val *cel = p->tete;
    cellule_val *cel_pre = p->tete;
    
    for (int i = 2; i < n; i++) {
        cel_pre = cel_pre->suivant;
    }
    cel = cel_pre->suivant;

    empiler(p, cel->valeur);
    cel_pre->suivant = cel->suivant;
}


/* Effectue x rotations des n premiers elements */
void rotation(pile *p, int n, int x) {
    for (int i = 1; i <= x; i++) {
        deplacer(p, n);
    }
}


/* 
 * Execute un groupe de commandes selon une certaine valeur n : 
 * n V F ? exécute V si n != 0 sinon exécute F
 */
int exec_groupe_commandes(val *V, val *F, val valeur, bool debug) {
    int n, ret;

    n = valeur.v_int;
    if (valeur.v_int == 0) {
        liberer_seq_cmd(&(F->groupe));
        if (!silent_mode)
            printf("\n\nExecution du sous programme:\n");
        ret = interprete(&(V->groupe), debug);
        liberer_seq_cmd(&(V->groupe));
    }
    else {
        liberer_seq_cmd(&(V->groupe));
        if (!silent_mode)
            printf("\n\nExecution du sous programme:\n");
        ret = interprete(&(F->groupe), debug);
        liberer_seq_cmd(&(F->groupe));
    }
    return ret;
}


/* Affiche les éléments de la pile */
void afficher_pile(pile *p) {
    if (silent_mode) return;

    cellule_val *cel = p->tete;
    val valeur;
    while (cel != NULL) {
        valeur = cel->valeur;
        if (valeur.v_bool == 1) {
            printf("%d\n", valeur.v_int);
        }
        else {
            printf("{");
            afficher(&valeur.groupe);
            printf("}\n");
        }
        cel = cel->suivant;
    }
}
