#ifndef LISTES_H
#define LISTES_H

#include <stdbool.h>

/*
 * Pour réaliser des tests de performance, désactiver tous les 
 * affichages.
 * Pour cela, le plus simple est de redefinir les fonctions principales 
 * en decommentant les 3 lignes suivantes et en commentant l'ancienne 
 * definition de 'eprintf' juste dessous.
 */

#ifdef SILENT

#define printf(fmt, ...) (0)
#define eprintf(fmt, ...) (0)
#define putchar(c) (0)

#else

#define eprintf(...) fprintf (stderr, __VA_ARGS__)

#endif

extern bool silent_mode;



////// DEFINITION DES TYPES DE SEQUENCES CHAINEES ///////
struct cellule {
    char   command;
    /* vous pouvez rajouter d'autres champs ici */
    struct cellule *suivant;
};
typedef struct cellule cellule_t;

struct sequence {
    cellule_t *tete;
};
typedef struct sequence sequence_t;




////// FONCTIONS SUR LES SEQUENCES DE COMMANDES CHAINEES ///////

/* Crée une cellule de séquence de commandes */
cellule_t* nouvelleCellule (void);

/* Convertit une chaine de caractères en séquence de commandes chainée */
void conversion (char *texte, sequence_t *seq);

/* Libère la mémoire attribuée à une cellule */
void detruireCellule (cellule_t*);


/* Libére une séquences de commandes et chaque cellule qu'elle contient */
void liberer_seq_cmd (sequence_t *seq);

/* Ajoute une cellule en tete de séquence */
void ajout_en_tete(sequence_t *seq, char c);

/* Ajoute une cellule en fin de séquence */
void ajout_en_queue(sequence_t *seq, char c);

void inverser_seq(sequence_t *seq);

/* Affiche une séquence de commandes */
void afficher (sequence_t* seq);


#endif
