#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#ifdef NCURSES
#include <ncurses.h>
#endif
#include "listes.h"


/*
 *  Auteur(s) :
 *  Date :
 *  Suivi des Modifications :
 *
 */

bool silent_mode = false;


cellule_t* nouvelleCellule (void)
{
    /* À compléter (utiliser malloc) */
    //printf("\n>>>>>>>>>>> A Faire : liste.c/nouvelleCellule() <<<<<<<<<<<<<<<<\n");
    cellule_t *cel = malloc(sizeof(cellule_t));
    return cel;
}


void detruireCellule (cellule_t* cel)
{
    /* À compléter (utiliser free) */
    //printf("\n>>>>>>>>>>> A Faire : liste.c/detruireCellule() <<<<<<<<<<<<<<<<\n");
    free(cel);
}


void conversion (char *texte, sequence_t *seq)
{
    /* À compléter */
    //printf("\n>>>>>>>>>>> A Faire : liste.c/conversion() <<<<<<<<<<<<<<<<\n");
    seq->tete = nouvelleCellule();
    cellule_t *cel = seq->tete;
    cel->command = texte[0];
    int i=1;
    while (texte[i] != '\0') {
        cellule_t *suiv = nouvelleCellule();
        suiv->command = texte[i];
        suiv->suivant = NULL;
        cel->suivant = suiv;
        cel = cel->suivant;
        i++;
    }
}

void ajout_en_queue(sequence_t *seq, char c) {
    cellule_t *cel = nouvelleCellule();
    cellule_t *cel_t = seq->tete;
    cel->command = c;
    cel->suivant = NULL;

    if (seq->tete == NULL) {
        seq->tete = cel;
        return;
    }
    while (cel_t->suivant != NULL) {
        cel_t = cel_t->suivant;
    }
    cel_t->suivant = cel;
}

void afficher (sequence_t* seq) {
    assert (seq); /* Le pointeur doit être valide */
    /* À compléter */
    //printf("\n>>>>>>>>>>> A Faire : liste.c/afficher() <<<<<<<<<<<<<<<<\n");
    cellule_t *cel = seq->tete;
    while (cel != NULL) {
        printf("%c", cel->command);
        cel = cel->suivant;
    }
}

