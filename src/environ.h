/* environ.h */
#ifndef ENVIRON_H
#define ENVIRON_H
#include <stdbool.h>
/* ----------------------------types--------------------------------------------*/

  // data
typedef union u_val{
  unsigned long int integer; // long to reach 32 bits , but i'm not sure about using unsigned
  bool boolean;
  // add types missing
}u_val;

  // type of data
enum{ TYPE_INT, TYPE_BOOL};

/* environnement := liste de couples (identificateur, entier) */
typedef struct cellenv{
  char *ID;
  val  VAL;
  int TYPE;
  struct cellenv *SUIV;
} *ENV;

/*------------------FONCTIONS ---------------------------------------------------*/
/*---------------------allocation memoire----------------------------------------*/
extern char *Idalloc();      /* retourne un tableau de MAXIDENT char             */
extern ENV Envalloc();       /* retourne un ENV                                  */
/*---------------------environnements--------------------------------------------*/
extern int initenv(ENV *prho,char *var);/* initialise l'ident var dans *prho     */
extern int ecrire_env(ENV rho);/* affiche l'environnement                        */
extern int eval(int op, int arg1, int arg2); /* retourne (arg1 op arg2)          */
extern ENV rech(char *chaine, ENV listident);/* retourne la position de chaine   */
extern int affect(ENV rho, char *var, int val);/* affecte val a la variable var  */
extern int valch(ENV rho, char *var); /* valeur de var dans envirnt rho          */
/* ------------------CONSTANTES -------------------------------------------------*/
#define MAXIDENT 16          /* long max d'un identificateur de variable         */
#define MAXQUAD  5*MAXIDENT  /* long max d'un quadruplet c3a                     */
#define Pl 257
#define Mo 258
#define Mu 259
#define Af 260
#define Afc 261
#define Sk 262
#define Jp 263
#define Jz 264
#define St 265
#endif
