#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
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
//p.tete = NULL;

void stop (void)
{
    char enter = '\0';
    printf ("Appuyer sur entrée pour continuer...\n");
    while (enter != '\r' && enter != '\n') { enter = getchar(); }
}



int interprete (sequence_t* seq, bool debug)
{
    // Version temporaire a remplacer par une lecture des commandes dans la
    // liste chainee et leur interpretation.

    char commande;
 
    

    //debug = true; /* À enlever par la suite et utiliser "-d" sur la ligne de commandes */

    printf ("Programme:");
    afficher(seq);
    printf ("\n");
    if (debug) stop();

    // À partir d'ici, beaucoup de choses à modifier dans la suite.
    //printf("\n>>>>>>>>>>> A Faire : interprete.c/interprete() <<<<<<<<<<<<<<<<\n");
    cellule_t *cel = seq->tete;
    int ret;         //utilisée pour les valeurs de retour

    while (cel != NULL) { //à modifier: condition de boucle
        val valeur;
        val x, y;
        sequence_t gp;
        gp.tete = NULL;

        commande = cel->command;
        printf("commande:%c", commande);
        switch (commande) {
            /* Ici on avance tout le temps, à compléter pour gérer d'autres commandes */
            case 'A':
                ret = avance();
                if (ret == VICTOIRE) return VICTOIRE; /* on a atteint la cible */
                if (ret == RATE)     return RATE;     /* tombé dans l'eau ou sur un rocher */
                break; /* à ne jamais oublier !!! */
            case 'G':
                gauche();
                break;
            case 'D':
                droite();
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                valeur.v_bool = 1;
                valeur.v_int = commande-'0';
                empiler(&p, valeur);
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
                valeur.gp = gp;
                empiler(&p, valeur);
                break;
            case '?':
                x = depiler(&p);
                y = depiler(&p);
                valeur = depiler(&p);
                ret = exec_groupe_commandes(x, y, valeur, debug);
                if (ret == VICTOIRE) return VICTOIRE;
                break;
            case '!':
                x = depiler(&p);
                printf("\n\nExecution du sous programme:\n");
                if (interprete(&x.gp, debug) == VICTOIRE) return VICTOIRE;
                break;
            case 'X':
                echanger(&p);
                break;
            case 'B':
                while (p.tete->valeur.v_int > 0) {
                    printf("\n\nExecution du sous programme:\n");
                    if (interprete(&p.tete->suivant->valeur.gp, debug) == VICTOIRE) return VICTOIRE;
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
                eprintf("Caractère inconnu: '%c'\n", commande);
        }
        cel = cel->suivant;

        /* Affichage pour faciliter le debug */
        afficherCarte();
        printf ("Programme:");
        afficher(seq);
        printf ("\n");
        printf("Pile:\n");
        afficher_pile(&p);
        if (debug) stop();
    }

    /* Si on sort de la boucle sans arriver sur la cible,
     * c'est raté :-( */

    return CIBLERATEE;
}
