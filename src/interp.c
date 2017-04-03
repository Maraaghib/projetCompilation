#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interp.h"
#include "util.h"

void sem(BILENV benv, BILFON bfon, Noeud *noeud){
  if ( noeud == NULL )
    return ;
  char *etiq;
  int value;
  // affectation
  if (!strcmp(noeud->data , "Af")){
    // variable ou fonction
    if (noeud->gauche->tokentype == variable | noeud->gauche->tokentype == funct){
      etiq = strdup(noeud->gauche->data);
      printf("etiq vaut %s\n",etiq );
      value = semval(benv, benv_loc, noeud->droit);
      printf("valeur : %d\n", value);
      affectb(benv, bfon->VARLOC, etiq , value);
    } // cas d'un tableau
    /*
    else if (noeud->gauche->ttype == array){
      int tab = semval(benv, benv_loc, noeud->gauche->gauche);
      int indice = semval(benv, benv_loc, noeud->gauche->droit);
      TAS[ADR[tab] + indice] = semval(benv, benv_loc, noeud->droit);
    }*/
    return ;
  }
  // separateur
  if (!strcmp(noeud->data, "Se")){
    sem(benv, benv_loc, noeud->gauche);
    sem(benv, benv_loc, noeud->droit);
    return ;
  }
  // if then else
  if (!strcmp(noeud->data, "If")){
    if (noeud->gauche->data)
      sem(benv, benv_loc, noeud->droit->gauche);
    else
      sem(benv, benv_loc, noeud->droit->droit);
    return ;
  }
  // while
  if (!strcmp(noeud->data, "Wh")){
    while (noeud->gauche->data)
      sem(benv, benv_loc, noeud->droit);
    return ;
  }
  return ;
}

int semval(BILENV benv, BILENV benv_loc, Noeud *noeud){
  if ( noeud == 0 )
    return EXIT_FAILURE;
  ENV env_pos;
  int res, taille;
  // cas des operateurs binaires
  if (op_binaire(noeud->data))
    return eval(noeud->data, semval(benv, benv_loc, noeud->gauche), semval(benv, benv_loc, noeud->droit));
  // cas de l'operateur unaire "Not"
  if (op_unaire(noeud->data))
    return eval(noeud->data, semval(benv, benv_loc, noeud->gauche), 0);
  // cas d'une constante
  if (noeud->tokentype == constante)
    return atoi(noeud->data);
  // cas d'une variable ou d'une fonction
  if (noeud->tokentype == variable | noeud->tokentype == funct){
    env_pos = rech2(noeud->data, benv->debut, benv_loc->debut);
    return env_pos->VAL;
  }
}


/* revoie 1 si la chaine src correspond à
 * un operateur binaire ou 0 sinon.
 */
int op_binaire(char *src){
  char *tab[7] = {"Pl", "Mo", "Mu", "And", "Or", "Lt", "Eq"};
  for (int i = 0; i < 7 ; i++)
    if (!strcmp(src, tab[i]))
      return 1;
  return 0;
}

int op_unaire(char *src){
  return !(strcmp(src, "Not"));
}
