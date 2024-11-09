#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#ifdef NCURSES
#include <ncurses.h>
#endif
#include "listes.h"
#include "curiosity.h"
#include "pile.h"


/*
 *  Auteur(s) :
 *  Date :
 *  Suivi des Modifications :
 *
 */

//init de la pile en dehors de la fonction
//pour ne pas la perdre au cours de l'execution
pile_val p;
int profondeur_interprete = 0;
//p.tete = NULL;

void stop (void)
{
    if (silent_mode) return;
    char enter = '\0';
    printf ("Appuyer sur entrée pour continuer...\n");
    while (enter != '\r' && enter != '\n') { enter = getchar(); }
}



int interprete (sequence_t* seq, bool debug)
{
    profondeur_interprete++;
    cellule_t *cel;
    char commande;
    int ret;         //utilisée pour les valeurs de retour

    debug = true;

    if (!silent_mode) {
        printf ("Programme:");
        afficher(seq);
        printf ("\n");
        if (debug) stop();
    }

    cel = seq->tete;
    while (cel != NULL) { //à modifier: condition de boucle
        val valeur;
        val x, y;
        sequence_t gp;
        gp.tete = NULL;

        commande = cel->command;
        switch (commande) {
            case 'A':
                ret = avance();
                if (ret == VICTOIRE) {
                    //liberer_pile(&p);
                    profondeur_interprete--;
                    if (profondeur_interprete <= 0) liberer_seq_cmd(seq);
                    return VICTOIRE; /* on a atteint la cible */
                }
                if (ret == RATE) {
                    //liberer_pile(&p);
                    profondeur_interprete--;
                    if (profondeur_interprete <= 0) liberer_seq_cmd(seq);
                    return RATE; /* tombé dans l'eau ou sur un rocher */
                }
                break;
            case 'G':
                gauche();
                break;
            case 'D':
                droite();
                break;
            case '+':
                x = depiler(&p);
                y = depiler(&p);
                valeur.v_bool = 1;
                valeur.v_int = x.v_int + y.v_int;
                empiler(&p, valeur);
                break;
            case '-':
                x = depiler(&p);
                y = depiler(&p);
                valeur.v_bool = 1;
                valeur.v_int = y.v_int - x.v_int;
                empiler(&p, valeur);
                break;
            case '*':
                x = depiler(&p);
                y = depiler(&p);
                valeur.v_bool = 1;
                valeur.v_int = x.v_int * y.v_int;
                empiler(&p, valeur);
                break;
            case 'P':
                x = depiler(&p);
                pose(x.v_int);
                break;
            case 'M':
                x = depiler(&p);
                valeur.v_bool = 1;
                valeur.v_int = mesure(x.v_int);
                empiler(&p, valeur);
                break;
            case '{':
                cel = cel->suivant;
                int nb_o = 1; //nombre d'accolades ouvertes
                int nb_c = 0; //nombre d'accolades fermées
                while (cel->command != '}' || nb_o-nb_c != 1) {
                    if (cel->command == '{') nb_o++;
                    if (cel->command == '}') nb_c++;
                    ajout_en_queue(&gp, cel->command);
                    cel = cel->suivant;
                } 
                valeur.v_bool = 0;
                valeur.groupe = gp;
                empiler(&p, valeur);
                break;
            case '?':
                x = depiler(&p);
                y = depiler(&p);
                valeur = depiler(&p);
                ret = exec_groupe_commandes(&x, &y, valeur, debug);
                if (ret == VICTOIRE) {
                    profondeur_interprete--;
                    if (profondeur_interprete <= 0) liberer_seq_cmd(seq);
                    return VICTOIRE;
                };
                break;
            case '!':
                x = depiler(&p);
                if (!silent_mode)
                    printf("\n\nExecution du sous programme:\n");
                if (interprete(&x.groupe, debug) == VICTOIRE) {
                    profondeur_interprete--;
                    if (profondeur_interprete <= 0) liberer_seq_cmd(seq);
                    return VICTOIRE;
                }    
                break;
            case 'X':
                echanger(&p);
                break;
            case 'B':
                while (p.tete->valeur.v_int > 0) {
                    if (!silent_mode)
                        printf("\n\nExecution du sous programme:\n");
                    if (interprete(&p.tete->suivant->valeur.groupe, debug) == VICTOIRE) {
                        profondeur_interprete--;
                        if (profondeur_interprete <= 0) liberer_seq_cmd(seq);
                        return VICTOIRE;
                    }
                    p.tete->valeur.v_int--;
                }
                p.tete->suivant = p.tete->suivant->suivant;
                break;
            case 'C':
                cloner(&p);
                break;
            case 'I':
                depiler(&p);
                break;
            case 'R':
                x = depiler(&p);
                y = depiler(&p);
                rotation(&p, y.v_int, x.v_int);
                break;
            default:
                if (isdigit(commande)) {
                    valeur.v_bool = 1;
                    valeur.v_int = commande-'0';
                    empiler(&p, valeur);
                }
                if (!silent_mode)
                    eprintf("Caractère inconnu: '%c'\n", commande);
                break;
        }
        cel = cel->suivant;

        /* Affichage pour faciliter le debug */
        if (!silent_mode) {
            afficherCarte();
            printf ("Programme:");
            afficher(seq);
            printf ("\n");
            printf("Pile:\n");
            afficher_pile(&p);
            if (debug) stop();
        }
    }

    /* Si on sort de la boucle sans arriver sur la cible,
     * c'est raté :-( */
    profondeur_interprete--;
    if (profondeur_interprete <= 0) liberer_seq_cmd(seq);
    return CIBLERATEE;
}
