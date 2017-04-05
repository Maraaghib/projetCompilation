/* interp.h */
#ifndef INTERP_H
#define INTERP_H

/*----------------------semantique-----------------------------------------------*/
/* initialisation  memoire                                                       */
void init_memoire();/* initialise le tas: TAS, ADR, TAL                          */
/* decrit la memoire: ADR, TAL, TAS */
void ecrire_memoire(int maxadr, int maxtal, int maxtas);
/* semantique op a grands pas des expressions */  
int traitement(ENV *e,Noeud *n);
/*---------------------VARIABLES globales ---------------------------------------*/
extern NOE syntree;          /* arbre syntaxique                      (y.tab.c)  */
extern int TAS [TAILLEMEM];  /* le tas; (NIL=0); "vraies" adresses >=1(y.tab.c)  */
extern int ADR[TAILLEADR];   /* ADR[i]=adresse dans le tas du tab i              */
extern int TAL[TAILLEADR];   /* TAL[i]=taille du tab i                           */
extern int ptasl;            /* premiere place libre dans TAS[]                  */
extern int padrl;            /* premiere place libre dans ADR[]                  */


/*------------------FONCTIONS ---------------------------------------------------*/
/*------------------types--------------------------------------------------------*/

/* envoie message s d'erreur de type en ligne ligne                              */
extern void typ_error(char *s,int ligne);
/*renvoie le type de e*/
extern type calcul_type(BILENVTY rho_gb, NOE e,int ligne);


#endif
