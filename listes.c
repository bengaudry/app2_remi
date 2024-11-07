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
    cellule_t *cel = malloc(sizeof(cellule_t));
    return cel;
}


void detruireCellule (cellule_t* cel)
{
    free(cel);
}


void conversion (char *texte, sequence_t *seq)
{
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

void liberer_seq_cmd (sequence_t *seq) {
    cellule_t *cel, *cel_p;

    cel = seq->tete;
    while (cel != NULL) {
        cel_p = cel;
        cel = cel->suivant;
        detruireCellule(cel_p);
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
    if (silent_mode) return;
    assert (seq); /* Le pointeur doit être valide */

    cellule_t *cel = seq->tete;
    while (cel != NULL) {
        printf("%c", cel->command);
        cel = cel->suivant;
    }
}

