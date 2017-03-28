/* util.h */
#ifndef UTIL_H
#define UTIL_H

#include "environ.h"

/* ----------------------------types--------------------------------------------*/


/* Structure representant un arbre binaire. */
typedef struct Noeud Noeud;
struct Noeud{
  char *data;
  Noeud *droit;
  Noeud *gauche;

};

/* biliste de var ou param */
typedef struct bilenv{
  ENV debut;
  ENV fin;}*BILENV;

/* listefonctions := liste de triples (identificateur, BILparametres, BILvarloc) */
typedef struct cellfon{
  char *ID;
  BILENV PARAM;    /* pametres formels types   */
  BILENV VARLOC;   /* variables locales typees */
  Noeud* CORPS;
  TYPE type;
  struct cellfon *SUIV;} *LFON;

/* biliste de fonctions */
typedef struct bilfon{
  LFON debut;
  LFON fin;}*BILFON;

/*******************ARBRES****************************************************/
/* Initialise un noeud , avec pour data le char *. */
Noeud *create_noeud(Noeud *, Noeud *, char *); // A RECUP
/* Affiche l'arbre à partir du noeud dans un fichier. */
void print_tree(Noeud *, FILE *);  // A RECUP
/*Affiche l'arbre à partir du noeud dans le terminal */
void print_tree_ter(Noeud *);  // A RECUP
/* Produit l'environnement imp à partir de l'arbre (noeud). */
//int env_imp(Noeud *, ENV *);
/* Produit l'environnement c3a à partir d'un bilquad. */
//ENV env_c3a(BILQUAD);
/* Produit le code c3a à partir d'un code imp. */
//BILQUAD imp_c3a(Noeud *);
/* Permet de créer la dernière étiquette du code c3a. */
//char *etiq_fin();
/* Produit le code y86 à partir du code c3a. */
//void c3a_y86(BILQUAD);
/* Copie Init dans un fichier. */
//void debut_y86(FILE *);
/* Copie dans un fichier , MUL , MULPLUS ...
 * Initialise Data avec les variables de l'environnement. */
//void fin_y86(FILE *, ENV *);


/*------------------FONCTIONS -----------------------------------------------*/
/*---------------------------------------------------------------------------*/
extern int yylex();          /* fonction generee par flex                    */ //OK (fait par flex)
extern int yyerror();        /* fonction generee par flex/bison              */ //A faire
/*---------------------allocation memoire------------------------------------*/
extern Noeud *Nalloc();         /* retourne un Noeud                              */ //OK
extern LFON  Lfonalloc();    /* retourne un LFON                             */ //OK
/*---------------------parcours d'arbres-------------------------------------*/
extern void prefix(Noeud* n);   /* ecrit l'expression n en notation prefixe     */ //OK ?
/*---------------------environnements----------------------------------------*/
extern ENV creer_env(char *etiq, int val, TYPE type);/*pointe vers cette var            */ //DONE \/ \/
extern ENV copier_env(ENV env);/*pointe vers une copie                     */
extern char *nomop(int codop);/* traduit entier vers chaine (= nom operation)*/ // PAS OK
/* retourne la position de chaine (rho_lc est prioritaire) */
extern ENV rech2(char *chaine, ENV rho_gb, ENV rho_lc);
/*---------------------bilistes-de-var---------------------------------------*/
extern void inbilenv(BILENV prho,char *var, TYPE t);             /* initialise var  */
extern BILENV bilenv_vide() ;                  /* retourne une biliste vide  */
extern BILENV creer_bilenv(ENV var);   /* retourne une biliste a un element  */
extern BILENV copier_bilenv(BILENV b);   /*pointe vers copie                 */
extern BILENV concat(BILENV b1, BILENV b2);  /* retourne la concatenation    */
extern void ecrire_bilenv(BILENV b);   /* affiche la biliste de quadruplets*/
/* affecte  la valeur rhs a la variable lhs (rho_lc prioritaire)             */
extern void affectb(BILENV rho_gb, BILENV rho_lc, char *lhs, int rhs);
extern void liberer_bilenv(BILENV be); /*libére tout l'environnement */
/*---------------------fonctions --------------------------------------------*/
extern LFON creer_fon(char *nfon, BILENV lparam,BILENV lvars,Noeud* com,TYPE tp);
/* pointe vers cette fonction */
extern LFON copier_fon(LFON lfn);    /* pointe vers une copie                */
extern void ecrire_fon(LFON bfn);
extern LFON rechfon(char *chaine, LFON listident);/* retourne la position de chaine*/
extern void liberer_lfon(LFON fon); /* libére la fonction */
/*---------------------bilistes-de-fonctions --------------------------------*/
extern BILFON bilfon_vide() ;                  /* retourne une biliste vide  */
extern BILFON creer_bilfon(LFON pfon);  /* retourne une biliste a un element */
extern BILFON copier_bilfon(BILFON bfn);/* pointe vers une copie             */
extern BILFON concatfn(BILFON bfn1, BILFON bfn2);/* retourne la concatenation*/
extern BILENV allvars(BILFON bfon);/*les variables de bfon (params puis varloc)*/
extern void ecrire_bilfon(BILFON bfn);   /* affiche la biliste de fonctions  */
extern void liberer_bilfon(BILFON bfon); /*libére toute la bilfon */
/*---------------------programmes -------------------------------------------*/
void ecrire_prog(BILENV argb,BILFON argbf,Noeud* argno);/* affiche le programme */ //DONE /\ /\
/* --------------------CONSTANTES -------------------------------------------*/
#define MAXIDENT 16          /* long max d'un identificateur de variable     */
/*---------------------VARIABLES globales -----------------------------------*/
/* definies au  debut de y.tab.c , non-declarees dans y.tab.h                */
//extern NOE syntree;          /* arbre syntaxique                  (y.tab.c)*/
//extern BILENV benv;          /* environnement global              (y.tab.c)*/
//extern BILFON lfonctions;    /* liste des fonctions globale       (y.tab.c)*/
#endif
