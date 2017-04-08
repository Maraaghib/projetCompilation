#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interp.h"
#include "util.h"

/* ------------------VARIABLES GLOBALES ------------------------------*/
/* le tas; (NIL=0); "vraies" adresses >=1                             */
int TAS[TAILLEMEM];
/* ADR[i]=adresse dans le tas du tab i                                */
int ADR[TAILLEADR];
/* TAL[i]=taille du tab i                                             */
int TAL[TAILLEADR];
int ptasl = 1; /* premiere place libre dans TAS[]                       */
int padrl = 1; /* premiere place libre dans ADR[]                       */
/*--------------------------------------------------------------------*/
/* ----------------------traitement--memoire--------------------------*/
void init_memoire(){
  int i = 0;
  while (i < TAILLEMEM)
    TAS[i++] = 0;
  i = 0;
  while (i < TAILLEADR){
    ADR[i++] = 0;
   TAL[i] = 0;
 }
}
/* decrit la memoire: */
/* ADR[i]: adresse du tableau i dans le TAS */
/* TAL[i]: taille du tableau i; de ADR[i] a ADR[i] + TAL[i] */
/* TAS: tableau (statique) contenant tous les tableaux (dynamiques) */
void ecrire_memoire(int maxadr, int maxtal, int maxtas){
  int i;
  printf("Le tableau ADR:\n");
  printf("------------------------:\n");
  for(i = 0; i < maxadr; i++)
    printf("%d : ", ADR[i]);
  printf("\n");
  printf("Le tableau TAL:\n");
  printf("------------------------:\n");
  for(i = 0; i < maxadr; i++)
    printf("%d : ",TAL[i]);
  printf("\n");
  printf("Le tableau TAS:\n");
  printf("------------------------:\n");
  for(i = 0; i < maxtas; i++)
    printf("%d : ",TAS[i]);
  printf("\n");
  return;
}

int semval(BILENV env,Noeud *noeud){
  if(noeud != NULL){
    ENVTY pos;
    int res, taille;
    switch(noeud->codop){
      case Ind:
        return TAS[ADR[semval(env, noeud->gauche)] + semval(env, noeud->droit)];
      case Pl:case Mo:case Mu:case And:case Or:case Lt:case Eq:/* op binaire     */
        return eval(noeud->codop, semval(env,noeud->gauche), semval(env,noeud->droit));
      case Not:                                            /* operation unaire      */
        return eval(noeud->codop, semval(env,noeud->gauche), 0);
      case I:                        /* numeral          */
        return atoi(noeud->ETIQ);
      case V:                         /* variable        */
        pos = rechty(noeud->ETIQ, env.debut);
        return pos->VAL;          /* env(var)     */
      case NewAr:                     /*creation tableau */
        int taille = semval(env, noeud->droit);
        ADR[padrl] = ptasl;
        ptasl += taille;
        TAL[padrl] = taille;
        return  padrl++;
      default: return EXIT_FAILURE;  /* codop inconnu au bataillon */
    }
	} else
    return EXIT_FAILURE;
}

/* semantique op a grands pas des commandes                      */
/* fait agir noeud sur env, le  modifie                           */
void sem(BILENV env, Noeud *noeud){
  char *lhs; int rhs; int cond;
  if (noeud != NULL){
    switch(noeud->codop){
      case Mp:
        semop_gp(env, noeud->gauche);
        break;
      case Af:
        if (noeud->gauche->codop == V){        /* affectation a une variable */
          lhs = noeud->gauche->ETIQ;
          printf("lhs vaut %s \n", lhs);
          rhs = semval(env, noeud->droit);
          printf("rhs vaut %d \n", rhs);
          affectb(env, lhs, rhs);
        } else {
          assert(noeud->gauche->codop == Ind);/* affectation a un tableau */
          /*a ecrire */
        }
        break;
      case Sk: break;
      case Se:
        semop_gp(env, noeud->gauche);
        semop_gp(env, noeud->droit);
        break;
      case If:
        if (noeud->gauche->codop)
          semop_gp(env, noeud->droit->gauche);
        else
          semop_gp(env, noeud->droit->droit);
        break;
      case Wh:
        while(noeud->gauche->codop)
          semop_gp(env, noeud->droit);
        break;
      default: break;
    }
  }
  return;
}
