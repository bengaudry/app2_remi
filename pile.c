#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "pile.h"
#include "listes.h"
#include "interprete.h"


/* Ajout d'un élément en haut de la pile */
void empiler (pile *p, val valeur) {
    cellule_pile *cel = malloc(sizeof(cellule_pile));

    cel->valeur = valeur;
    cel->suivant = p->tete;
    p->tete = cel;
}


/* Dépile et renvoie l'element au sommet */
val depiler(pile *p) {
    val valeur;
    cellule_pile *ancienne_tete;

    ancienne_tete = p->tete;
    valeur = p->tete->valeur;
    p->tete = p->tete->suivant;
    
    free(ancienne_tete);
    return valeur;
}


/* Dépile et renvoie l'élément en fin de pile */
val depiler_fin(pile *p) {
    cellule_pile *cel;
    cellule_pile *temp;
    val valeur;

    // Si la pile contient un seul élément
    if (p->tete->suivant == NULL) {
        valeur = p->tete->valeur;
        free(p->tete);
        p->tete = NULL;
        return valeur;
    }

    // Parcourt la pile jusqu'à l'avant-dernier élément
    cel = p->tete;
    while (cel->suivant->suivant != NULL) {
        cel = cel->suivant;
    }

    // Sauvegarde la valeur, libère la dernière cellule, et ajuste le pointeur
    valeur = cel->suivant->valeur;
    temp = cel->suivant;
    cel->suivant = NULL;
    free(temp);

    return valeur;
}


/* 
 * Inversion de la pile
 * a1,a2,...,an devient an,an-1,...,a1
 */
void inverser_pile (pile *l) {
    cellule_pile *precedent = NULL;
    cellule_pile *courant = l->tete;
    cellule_pile *suivant = NULL;

    while (courant != NULL) {
        suivant = courant->suivant;
        courant->suivant = precedent;

        precedent = courant;
        courant = suivant;
    }

    l->tete = precedent;
}


/* Libère la mémoire allouée à chaque cellule de la pile, et à la pile elle même*/
void liberer_pile(pile *p) {
    cellule_pile *cel = p->tete;
    while (cel != NULL) {
        cellule_pile *cel_p = cel;
        if (!cel->valeur.est_int) { 
            liberer_seq_cmd(&cel->valeur.groupe); // Libère les groupes de commandes
        }
        cel = cel->suivant;
        free(cel_p);
    }
    p->tete = NULL; // Réinitialise la tête pour éviter un pointeur invalide
}


/* Echange les 2 elements au sommet entre eux */
void echanger(pile *p) {
    cellule_pile *tete = p->tete;
    cellule_pile *suiv = tete->suivant;
    
    tete->suivant = suiv->suivant;
    suiv->suivant = tete;
    p->tete = suiv;
}


/* Clone l'element au sommet de la pile */
void cloner(pile *p) {
    cellule_pile *cel = malloc(sizeof(cellule_pile));

    if (p == NULL) return;

    cel->valeur = p->tete->valeur;   
    cel->suivant = p->tete;          
    p->tete = cel;      
}


/* Deplace l'element d'indice n au sommet */
void deplacer(pile *p, int n) {
    cellule_pile *cel = p->tete;
    cellule_pile *cel_pre = NULL;

    if (n == 1 || p->tete == NULL) return; // Aucun déplacement nécessaire

    // Parcourt jusqu’à l'élément n
    for (int i = 1; i < n - 1 && cel->suivant != NULL; i++) {
        cel = cel->suivant;
    }

    // Pointe sur l’élément à déplacer
    cel_pre = cel;
    cellule_pile *a_deplacer = cel_pre->suivant;

    if (a_deplacer == NULL) return; // Rien à déplacer

    // Retire a_deplacer de sa position et l'empile
    cel_pre->suivant = a_deplacer->suivant;
    empiler(p, a_deplacer->valeur);

    // Libère l'ancienne cellule maintenant déplacée au sommet
    free(a_deplacer);
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
int exec_groupe_commandes(val *V, val *F, val *valeur, bool debug) {
    int n, ret;

    n = valeur->v_int;
    if (valeur->v_int == 0) {
        if (!silent_mode)
            printf("\n\nExecution du sous programme:\n");
        ret = interprete(&(V->groupe), debug);
    }
    else {
        if (!silent_mode)
            printf("\n\nExecution du sous programme:\n");
        ret = interprete(&(F->groupe), debug);
    }
    liberer_seq_cmd(&(F->groupe));
    liberer_seq_cmd(&(V->groupe));
    return ret;
}


/* Affiche la valeur d'une cellule de la pile */
void afficher_valeur_cellule(val valeur) {
    if (silent_mode) return;

    if (valeur.est_int == 1) {
        printf("%d\n", valeur.v_int);
    }
    else {
        printf("{");
        afficher(&valeur.groupe);
        printf("}\n");
    }
}


/* Affiche les éléments de la pile */
void afficher_pile(pile *p) {
    if (silent_mode) return;

    val valeur;
    cellule_pile *cel;

    cel = p->tete;
    while (cel != NULL) {
        valeur = cel->valeur;
        afficher_valeur_cellule(valeur);
        cel = cel->suivant;
    }
}
