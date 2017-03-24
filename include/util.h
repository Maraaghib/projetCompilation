/* util.h */
#ifndef UTIL_H
#define UTIL_H

#include "environ.h"

/* ----------------------------types--------------------------------------------*/


/* commande := arbre abstrait */
typedef struct noeud{
  int codop;  /* I,V, operateur */
  char *ETIQ;
  struct noeud *FG, *FD;} *NOE;

/* biliste de var ou param */
typedef struct{
  ENV debut;
  ENV fin;}BILENV;

/* listefonctions := liste de triples (identificateur, BILparametres, BILvarloc) */
typedef struct cellfon{
  char *ID;
  BILENV PARAM;    /* pametres formels types   */
  BILENV VARLOC;   /* variables locales typees */
  NOE CORPS;
  struct cellfon *SUIV;} *LFON;

/* biliste de fonctions */
typedef struct{
  LFON debut;
  LFON fin;}BILFON;


/*------------------FONCTIONS -----------------------------------------------*/
/*---------------------------------------------------------------------------*/
extern int yylex();          /* fonction generee par flex                    */
extern int yyerror();        /* fonction generee par flex/bison              */
/*---------------------allocation memoire------------------------------------*/
extern NOE Nalloc();         /* retourne un NOE                              */
extern LFON  Lfonalloc();    /* retourne un LFON                             */
/*---------------------parcours d'arbres-------------------------------------*/
extern void prefix(NOE n);   /* ecrit l'expression n en notation prefixe     */
/*---------------------environnements----------------------------------------*/
extern ENV creer_env(char *etiq, int val, TYPE type);/*pointe vers cette var            */
extern ENV copier_env(ENV env);/*pointe vers une copie                     */
extern char *nomop(int codop);/* traduit entier vers chaine (= nom operation)*/
/* retourne la position de chaine (rho_lc est prioritaire) */
extern ENV rech2(char *chaine, ENV rho_gb, ENV rho_lc);
/*---------------------bilistes-de-var---------------------------------------*/
extern void inbilenv(BILENV *prho,char *var);             /* initialise var  */
extern BILENV bilenv_vide() ;                  /* retourne une biliste vide  */
extern BILENV creer_bilenv(ENV var);   /* retourne une biliste a un element  */
extern BILENV copier_bilenv(BILENV b);   /*pointe vers copie                 */
extern BILENV concat(BILENV b1, BILENV b2);  /* retourne la concatenation    */
extern void ecrire_bilenv(BILENV bty);   /* affiche la biliste de quadruplets*/
/* affecte  la valeur rhs a la variable lhs (rho_lc prioritaire)             */
extern void affectb(BILENV rho_gb, BILENV rho_lc, char *lhs, int rhs);
/*---------------------fonctions --------------------------------------------*/
extern LFON creer_fon(char *nfon, BILENV lparam,BILENV lvars,NOE com,TYPE tp);
/* pointe vers cette fonction */
extern LFON copier_fon(LFON lfn);    /* pointe vers une copie                */
extern void ecrire_fon(LFON bfn);
extern LFON rechfon(char *chaine, LFON listident);/* retourne la position de chaine*/ 
/*---------------------bilistes-de-fonctions --------------------------------*/
extern BILFON bilfon_vide() ;                  /* retourne une biliste vide  */
extern BILFON creer_bilfon(LFON pfon);  /* retourne une biliste a un element */
extern BILFON copier_bilfon(BILFON bfn);/* pointe vers une copie             */
extern BILFON concatfn(BILFON bfn1, BILFON bfn2);/* retourne la concatenation*/
extern BILENV allvars(BILFON bfon);/*les variables de bfon (params puis varloc)*/
extern void ecrire_bilfon(BILFON bfn);   /* affiche la biliste de fonctions  */
/*---------------------programmes -------------------------------------------*/
void ecrire_prog(BILENV argb,BILFON argbf,NOE argno);/* affiche le programme */
/* --------------------CONSTANTES -------------------------------------------*/
#define MAXIDENT 16          /* long max d'un identificateur de variable     */
/*---------------------VARIABLES globales -----------------------------------*/
/* definies au  debut de y.tab.c , non-declarees dans y.tab.h                */
//extern NOE syntree;          /* arbre syntaxique                  (y.tab.c)*/
//extern BILENV benv;          /* environnement global              (y.tab.c)*/
//extern BILFON lfonctions;    /* liste des fonctions globale       (y.tab.c)*/
#endif

