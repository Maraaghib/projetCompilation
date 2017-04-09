/* util.h */
#ifndef UTIL_H
#define UTIL_H
#include "environ.h"


/*plus besoin tout type est tableau de dim 0 type basique()typedef enum token {constante,variable,op,funct,proc} TOKENTYPE;*/


/* ----------------------------types--------------------------------------------*/

/* Structure representant un arbre binaire.
remplie en priorité a gauche (c.a.d. si un noeud doit
avoir un seul fils alors il sera à gauche) */
typedef struct Noeud Noeud;
struct Noeud{
  char *ETIQ;
  int codop; // correspond au valeur de l'enum contenu dans ppasclabison.h
  type* typeno; /*[0,T_com](si commande) ou [k,(T_int,T_boo)] (si expression) */
  Noeud *droit;
  Noeud *gauche;
};
//typedef Noeud* NOE; // Pointeur vers la structure Noeud

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
  type* typeno;
  struct cellfon *SUIV;} *LFON;

/* biliste de fonctions */
typedef struct bilfon{
  LFON debut;
  LFON fin;}*BILFON;

/*******************ARBRES****************************************************/
/* Initialise un noeud , avec pour ETIQ le char *. */
Noeud *create_noeud(Noeud *,Noeud *,char *,int,type*); // A RECUP
/* Affiche l'arbre à partir du noeud dans un fichier. */
void print_tree(Noeud *, FILE *);  // A RECUP
/*Affiche l'arbre à partir du noeud dans le terminal */
void print_tree_ter(Noeud *);  // A RECUP


/*------------------FONCTIONS -----------------------------------------------*/
/*---------------------------------------------------------------------------*/
extern int yylex();          /* fonction generee par flex                    */ //OK (fait par flex)
extern void yyerror();        /* fonction generee par flex/bison              */ //OK
/*---------------------allocation memoire------------------------------------*/
extern Noeud *Nalloc();         /* retourne un Noeud                              */ //OK
extern LFON  Lfonalloc();    /* retourne un LFON                             */ //OK
extern type *talloc();       /* retourne un pointeur sur type *                  */ //à coder
/*---------------------parcours d'arbres-------------------------------------*/
extern void prefix(Noeud* n);   /* ecrit l'expression n en notation prefixe     */ //OK ?
extern int est_feuille(Noeud* n);/* 1 si est une feuille, 0 sinon                   */ //à coder
/*---------------------environnements----------------------------------------*/
extern int  type_eq(type* t1, type* t2);/* 1 si t1 ==t2 , 0 sinon                  */
extern void type_copy(type *tcop, type* torig);/* copie torig vers tcop           */
extern void type_affect(ENV rho, type* tvar);/* affecte le type  de   *rho      */
extern type* creer_type(int dm, int tf);/* retourne le type                       */
extern type* type_res_op(int op);/* retourne le type resultat de op               */
extern void ecrire_type(type* tp);      /* ecrit le type  */
extern ENV creer_env(char *etiq, int val, type* typvar);/*pointe vers cette var            */ //DONE \/ \/
extern ENV copier_env(ENV env);/*pointe vers une copie                     */
extern char *nomop(int codop);/* traduit entier vers chaine (= nom operation)*/ // PAS OK
/* retourne la position de chaine (rho_lc est prioritaire) */
extern ENV rech2(char *chaine, ENV rho_gb, ENV rho_lc);
/*---------------------bilistes-de-var---------------------------------------*/
extern void inbilenv(BILENV prho,char *var, type* typeno);             /* initialise var  */
extern BILENV bilenv_vide() ;                  /* retourne une biliste vide  */
extern BILENV creer_bilenv(ENV var);   /* retourne une biliste a un element  */
extern BILENV copier_bilenv(BILENV b);   /*pointe vers copie                 */
extern BILENV concat(BILENV b1, BILENV b2);  /* retourne la concatenation    */
extern void ecrire_bilenv(BILENV b);   /* affiche la biliste de quadruplets*/
/* affecte  la valeur rhs a la variable lhs (rho_lc prioritaire)             */
extern void affectb(BILENV rho_gb, BILENV rho_lc, char *lhs, int rhs);
extern void liberer_bilenv(BILENV be); /*libére tout l'environnement */
/*---------------------fonctions --------------------------------------------*/
extern LFON creer_fon(char *nfon, BILENV lparam,BILENV lvars,Noeud* com,type* tp);
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
