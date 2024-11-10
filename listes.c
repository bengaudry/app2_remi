#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#ifdef NCURSES
#include <ncurses.h>
#endif
#include "listes.h"


bool silent_mode = false;


/* Crée une cellule de séquence de commandes */
cellule_t* nouvelleCellule (void)
{
    cellule_t *cel = malloc(sizeof(cellule_t));
    return cel;
}


/* Convertit une chaine de caractères en séquence de commandes chainée */
void conversion (char *texte, sequence_t *seq)
{
    int i;
    cellule_t *cel, *suiv;

    seq->tete = nouvelleCellule();
    cel = seq->tete;
    cel->command = texte[0];
    for (i = 1; texte[i] != '\0'; i++) {
        if (texte[i] != ' ' && texte[i] != '\n') {
            // Pour chaque caractère dans le texte on crée une cellule et on l'ajoute dans la sequence
            suiv = nouvelleCellule();
            suiv->command = texte[i];
            suiv->suivant = NULL;

            cel->suivant = suiv;
            cel = cel->suivant;
        }
    }
}


/* Libère la mémoire attribuée à une cellule */
void detruireCellule (cellule_t* cel)
{
    free(cel);
}


/* Libére une séquences de commandes et chaque cellule qu'elle contient */
void liberer_seq_cmd (sequence_t *seq) {
    cellule_t *cel, *cel_p;

    cel = seq->tete;
    while (cel != NULL) {
        cel_p = cel;
        cel = cel->suivant;
        detruireCellule(cel_p);
    }
}


/* Ajoute une cellule en fin de séquence */
void ajout_en_queue(sequence_t *seq, char c) {
    cellule_t *cel, *cel_t;
    
    cel = nouvelleCellule();
    cel->command = c;
    cel->suivant = NULL;
    cel_t = seq->tete;

    // Si la séquence est vide, on l'ajoute en tête et on termine la fonction
    if (seq->tete == NULL) {
        seq->tete = cel;
        return;
    }

    // On parcourt la séquence jusqu'à la dernière cellule non nulle
    while (cel_t->suivant != NULL) {
        cel_t = cel_t->suivant;
    }
    cel_t->suivant = cel;
}


/* Affiche une séquence de commandes */
void afficher (sequence_t* seq) {
    if (silent_mode) return;
    assert (seq); /* Le pointeur doit être valide */

    cellule_t *cel = seq->tete;
    while (cel != NULL) {
        printf("%c", cel->command);
        cel = cel->suivant;
    }
}

