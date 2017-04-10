#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "interp.h"
#include "util.h"
#include "ppascalbison.h"

/* De jolies couleurs                                                 */
#define RED "\x1b[31m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define RESET "\x1b[0m"

/* ------------------VARIABLES GLOBALES ------------------------------*/
/* le tas; (NIL=0); "vraies" adresses >=1                             */
int TAS[TAILLEMEM];
/* ADR[i]=adresse dans le tas du tab i                                */
int ADR[TAILLEADR];
/* TAL[i]=taille du tab i                                             */
int TAL[TAILLEADR];
int ptasl = 1; /* premiere place libre dans TAS[]                     */
int padrl = 1; /* premiere place libre dans ADR[]                     */
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
/* decrit la memoire:                                               */
/* ADR[i]: adresse du tableau i dans le TAS                         */
/* TAL[i]: taille du tableau i; de ADR[i] a ADR[i] + TAL[i]         */
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

int semval(BILENV *env, BILFON *fon, Noeud *noeud){
  if(noeud != NULL){
    ENV pos; LFON fpos, ftmp; BILFON bfon;
    ENV penv; BILENV benv;
    Noeud *ntmp;
    int res, taille;
    switch(noeud->codop){
      case True:
        return true;
      case False:
        return false;
      case Ind:
        return TAS[ADR[semval(env, fon, noeud->gauche)] + semval(env, fon, noeud->droit)];
      case Pl:case Mo:case Mu:case And:case Or:case Lt:case Eq:/* op binaire     */
        return eval(noeud->codop, semval(env, fon, noeud->gauche), semval(env, fon, noeud->droit));
      case Not:                                            /* operation unaire      */
        return eval(noeud->codop, semval(env, fon, noeud->gauche), 0);
      case I:                        /* numeral          */
        return atoi(noeud->ETIQ);
      case T_fon:
        fpos = rechfon(noeud->ETIQ, (*fon)->debut);
        bfon = creer_bilfon(fpos);
        affectParam(fpos->PARAM->debut, env, noeud->gauche);
        if (fpos != NULL)
          sem(&fpos->VARLOC, &bfon, fpos->CORPS);
        pos = rech(fpos->ID, fpos->VARLOC->debut);
        return pos->VAL;
      case V:                         /* variable        */
				if ((*fon)->debut != NULL){
					pos = rech2(noeud->ETIQ, (*env)->debut, (*fon)->debut->VARLOC->debut);
					if (pos == NULL)
						pos = rech2(noeud->ETIQ, (*env)->debut, (*fon)->debut->PARAM->debut);
        } else {
					pos = rech(noeud->ETIQ, (*env)->debut);
        }
        if (pos != NULL)
          return pos->VAL;            /* env(var)        */
        return EXIT_FAILURE;
      case NewAr:                     /*creation tableau */
        taille = semval(env, fon, noeud->droit);
        ADR[padrl] = ptasl;
        ptasl += taille;
        TAL[padrl] = taille;
        return  padrl++;
      default: return EXIT_FAILURE;  /* codop inconnu au bataillon */
    }
	} else
    return EXIT_FAILURE;
}

/* semantique op a grands pas des commandes                       */
/* fait agir noeud sur env, le  modifie                           */
void sem(BILENV *env, BILFON *fon, Noeud *noeud){
  char *lhs;
  int rhs, cond;
  LFON fpos;
  BILFON bfon;
  if (noeud != NULL){
    switch(noeud->codop){
      case Mp:
        sem(env, fon, noeud->gauche);
        break;
      case T_fon: // cas des procedures
        fpos = rechfon(noeud->ETIQ, (*fon)->debut);
        bfon = creer_bilfon(fpos);
        assert(fpos != NULL);
        affectParam(fpos->PARAM->debut,env, noeud->gauche);
        if (fpos != NULL)
          sem(env, fon, fpos->CORPS);
        break;
      case Af:
        if (noeud->gauche->codop == V){        /* affectation a une variable */
          lhs = noeud->gauche->ETIQ;
          rhs = semval(env, fon, noeud->droit);
          if (noeud->droit->codop == T_fon){ // cas des fonctions
            fpos = rechfon(noeud->droit->ETIQ, (*fon)->debut);
            if (fpos != NULL)
              affectb(*env, fpos->VARLOC, lhs, rhs);
          } else
            affect((*env)->debut, lhs, rhs);
        } else {
          assert(noeud->gauche->codop == Ind);/* affectation a un tableau */
          int tab = semval(env, fon, noeud->gauche->gauche);
          int ind = semval(env, fon, noeud->gauche->droit);
          TAS[ADR[tab] + ind] = semval(env, fon, noeud->droit);
        }
        break;
      case Sk: break;
      case Se:
        sem(env, fon, noeud->gauche);
        sem(env, fon, noeud->droit);
        break;
      case If:
        if (semval(env , fon, noeud->gauche))
          sem(env, fon, noeud->droit->gauche);
        else
          sem(env, fon, noeud->droit->droit);
        break;
      case Wh:
        while(semval(env , fon, noeud->gauche))
          sem(env, fon, noeud->droit);
        break;
      default: break;
    }
  }
  return;
}

void affectParam(ENV param, BILENV *env, Noeud *noeud){
  if (noeud == NULL)
    return ;
  BILENV penv;
  ENV suiv = param;
  ENV pos;
  if (param != NULL){
    switch (noeud->codop) {
      case V:
        pos = rech(noeud->ETIQ, (*env)->debut);
        if (pos != NULL){
          int rhs = pos->VAL;
          param->VAL = rhs;
          suiv = param->SUIV;
        }
        break;
      case True:
        param->VAL = true;
        suiv = param->SUIV;
        break;
      case False:
        param->VAL = false;
        suiv = param->SUIV;
        break;
      case I:
        param->VAL = atoi(noeud->ETIQ);
        break;
    }
  }
  affectParam(suiv, env, noeud->gauche);
  suiv = param->SUIV;
  affectParam(suiv, env, noeud->droit);
  ecrire_env(param);
  return ;
}
