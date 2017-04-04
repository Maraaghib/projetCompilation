/* bilquad.h */
#ifndef BILQUAD_H
#define BILQUAD_H
/* ----------------------------types--------------------------------------------*/
/* biliste de quadruplets etiquetes (stocke C3A ou Y86 )*/
typedef struct cellquad{
  char *ETIQ;
  int  OP;
  char *ARG1, *ARG2, *RES;
  struct cellquad *SUIV;
} *QUAD;

typedef struct{
  QUAD debut;
  QUAD fin;
} BILQUAD;

/*---------------------fonctions ------------------------------------------------*/
extern QUAD creer_quad(char *etiq, int op, char *arg1, char *arg2, char *res);
extern BILQUAD bilquad_vide() ;                    /* retourne une biliste vide  */
extern BILQUAD creer_bilquad(QUAD qd); /* retourne une biliste  a un element     */
extern QUAD rechbq(char *chaine, BILQUAD bq);/*ret le quad etiquete par chaine   */
extern BILQUAD concatq(BILQUAD bq1, BILQUAD bq2);/* retourne la concatenation    */
extern char *nomop(int codop); /* traduit entier vers chaine (= nom operation)    */

/* --------------- Fonctions Utilisées dans tradpp2c3a.c */
extern char *gensym(char *prefix); /* fabrique de nouvelles chaines              */
extern BILQUAD normal(BILQUAD bq);/* retourne la f.n. de bq: skip a la fin       */
extern void ecrire_sep_bilquad(BILQUAD bq);/* affiche bq , avec separateurs ":"  */
extern void ecrire_quad(QUAD qd); /* affiche le quadruplet                       */
extern void ecrire_bilquad(BILQUAD bq); /* affiche la biliste de quadruplets     */
extern BILQUAD pp2quad(NOE ec); /* traduit une commande en quadriplet */


#endif
