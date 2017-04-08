#ifndef __INTERP__H__
#define __INTERP__H__
#include "util.h"

extern void sem(BILENV, BILFON, Noeud *);
extern int semval(BILENV, BILFON, Noeud *);
void init_memoire();/* initialise le tas: TAS, ADR, TAL    */
/* decrit la memoire: ADR, TAL, TAS */
void ecrire_memoire(int maxadr, int maxtal, int maxtas);
/*---------------------VARIABLES globales ---------------------------------------*/
extern Noeud* syntree;          /* arbre syntaxique                              */
extern int TAS [TAILLEMEM];  /* le tas; (NIL=0); "vraies" adresses >=1(y.tab.c)  */
extern int ADR[TAILLEADR];   /* ADR[i]=adresse dans le tas du tab i              */
extern int TAL[TAILLEADR];   /* TAL[i]=taille du tab i                           */
extern int ptasl;            /* premiere place libre dans TAS[]                  */
extern int padrl;            /* premiere place libre dans ADR[]                  */
#endif
