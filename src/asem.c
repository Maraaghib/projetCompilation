#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arbre.h"
#include "ppascalbison.h"
/*------------------FONCTIONS ---------------------------------------------------*/
/*------------------types--------------------------------------------------------*/

void typ_error(char *mess, int ligne){
  fprintf(stderr, "TYPERROR: %s: ligne %d \n", mess, ligne);
  return;
}

/* renvoie le type de noeud : TYPEF \in T_int,  T_bool, T_com, T_err, T_bot          */
/* affecte ce type a la racine                                                 */
/* suppose corrects les types des fils                                         */
/* envoie message d'erreur si mal type a la racine et fils bien types          */
/* renvoie T_err si l'arbre est mal-type                                       */
type calcul_type(BILENVTY rho_gb, Noeud *noeud, int ligne){
  type tp;
  tp = creer_type(0,T_bot); /* type par defaut */
  if(noeud != NULL){
    type tgauche,tdroit;                /* type  du fils gauche, fils droit */
    type terr = creer_type(0,T_err);   /* type  erreur                 */
    /* un des fils est mal-type */
    if ((noeud->gauche && type_eq(noeud->gauche->typno,terr))||(noeud->droit && type_eq(noeud->droit->typno,terr))){
	     type_copy(&tp,terr); /* valeur du  type                              */
	     type_copy(&(noeud->typno),terr); /* affecte type  erreur           x     */
	     return tp;
    } /* noeud != NULL et tous les fils sont bien-types */
    switch(noeud->codop){
      case true:
        tp = noeud->typno;
        return tp; /* typno deja affecte     */
      case false:
        tp = noeud->typno;
        return tp; /* typno deja affecte     */
      case Ind: /* (tab_dim k of tau) x   int -> (tab_dim k-1 of tau) */
        if (noeud->gauche->typeno->TYPEF != T_err && noeud->droit->typeno->TYPEF != T_err){
          if (noeud->gauche->typeno->DIM > 0){
            if (noeud->droit->typeno->TYPEF == T_int){
              type_copy(&tp, noeud->gauche->typeno);
              noeud->gauche->DIM--;
              return tp;
            } else
              typ_error("indice non entier ", ligne);
          } else
            typ_error("ce n'est pas un tableau ", ligne);
        }
        type_copy(&tp, terr);
        type_copy(&(noeud->typeno), terr);
        return tp;
      case Pl:case Mo:case Mu: /* int x  int -> int */
        type tint;    /* type  des fils  */
        tint = creer_type(0,T_int);
        tgauche = noeud->gauche->typno;
        tdroit = noeud->droit->typno;
        if (type_eq(tgauche,tint) && type_eq(tdroit,tint)){
          type_copy(&(noeud->typno),tgauche); /* affecte le type */
          type_copy(&tp,tgauche);         /* calcule le type */
        } else {
          type_copy(&tp,terr); /* calcule le type                               */
          type_copy(&(noeud->typno),terr); /* affecte type                         */
          typ_error("op entier sur args non-entiers",ligne); /* message erreur  */
        }
        return tp;
      case Lt:case Eq:         /* int x int -> boo                   */
        type tint,tboo;       /* types  entier, booleen             */
        tint = creer_type(0,T_int);
        tboo = creer_type(0,T_bool);
        tgauche = noeud->gauche->typno;
        tdroit = noeud->droit->typno;
        if (type_eq(tgauche,tint) && type_eq(tdroit,tint)){
          type_copy(&tp,tboo);         /* valeur du  type */
          type_copy(&(noeud->typno),tboo); /* affecte le type */
        } else {
          type_copy(&tp,terr);/* valeur du  type                               */
          type_copy(&(noeud->typno),terr); /* affecte type                         */
          typ_error("op de comparaison sur args non-entiers",ligne); /* message */
        }
        return tp;
      case And:case Or:        /* boo x boo -> boo                   */
        type tboo;            /* type  booleen                      */
        tboo = creer_type(0,T_bool);
        tgauche = noeud->gauche->typno;
        tdroit = noeud->droit->typno;
        if (type_eq(tgauche,tboo) && type_eq(tdroit,tboo)){
          type_copy(&tp,tboo);         /* valeur du  type */
          type_copy(&(noeud->typno),tboo); /* affecte le type */
        } else {
          type_copy(&tp,terr);/* valeur du  type                              */
          type_copy(&(noeud->typno),terr); /* affecte type                        */
          typ_error("op booleen sur args non-booleens",ligne); /*message erreur*/
        }
        return tp;
      case Not:                /* boo  -> boo                        */
        type tboo;            /* type  booleen                      */
        tboo = creer_type(0,T_bool);
        tgauche = noeud->gauche->typno;
        assert(noeud->droit == NULL); /* op unaire                         */
        if (type_eq(tgauche,tboo)){
          type_copy(&tp,tboo);         /* valeur du  type */
          type_copy(&(noeud->typno),tboo); /* affecte le type */
        } else {
          type_copy(&tp,terr);/* valeur du  type                               */
          type_copy(&(noeud->typno),terr); /* affecte type                         */
          typ_error("op booleen sur arg non-booleen",ligne); /* message erreur  */
        }
        return tp;
      case I:                  /* constante T_int        */
        type tint;            /* type  du noeud         */
        tint = creer_type(0,T_int);
        assert(type_eq(noeud->typno,tint));/*verif du  type */
        type_copy(&tp,tint); /* valeur du  type         */
        return tp;
      case V:                  /* variable              */
        ENVTY pos=rechty(noeud->ETIQ,rho_gb.debut);               /* pos var dans rho */
        if (pos != NULL){
          type_copy(&tp, pos->TYPE);        /* valeur du  type := rho(var)      */
          type_copy(&(noeud->typno), pos->TYPE);/* affecte le type                  */
        } else {
          type_copy(&tp,terr);       /* tp := erreur                             */
          type_copy(&(noeud->typno),tp); /* affecte type                             */
          typ_error("variable inconnue ",ligne); /* message erreur                */
        }
        return tp;                           /* renvoie le type                  */
      case NewAr:                                             /* creation tableau */
        if (type_eq(noeud->gauche->typeno, terr) || type_eq(noeud->droit->typeno)){
          type_copy(&tp, terr);
          type_copy(&(noeud->typeno, terr), terr);
          typ_error("initialisation de tableau avec des types differents ", ligne);
        } else
          type_copy(&tp, noeud->typeno);
        return tp;
      case Af:                                                     /* affectation */
        if (type_eq(noeud->gauche->typno,noeud->droit->typno) == 0){/* type(lhs) <> type(rhs)    */
          type_copy(&(noeud->typno),terr); /* affecte type                        */
          typ_error("affectation de types incoherents ", ligne);
          return(terr);
        } else                                     /* type(lhs) == type(rhs)      */
          type_copy(&(noeud->typno), creer_type(0,T_com));
        return tp;
      case Se:
        type tcom = creer_type(0,T_com);         /* type  commande               */
        type_copy(&(noeud->typno),tcom);
        type_copy(&tp,tcom);
        return tp;
      case If:
        type tcom = creer_type(0,T_com);         /* type  commande               */
        type tboo = creer_type(0,T_bool);          /* type  booleen                */
        tgauche = noeud->gauche->typno;                       /* type  des  3 fils            */
        type tthen = noeud->droit->gauche->typno;
        type telse = noeud->droit->droit->typno;
        if (type_eq(tgauche,tboo) == 0){              /* type arg0  <> booleen         */
          type_copy(&(noeud->typno),terr);        /* affecte type                  */
          type_copy(&tp,terr);
          typ_error("condition non booleenne dans un if", ligne);
        } else if (!type_eq(tthen,tcom) || !type_eq(telse,tcom)){ /* arg <> tcom   */
          type_copy(&(noeud->typno),terr);                        /* affecte type  */
          type_copy(&tp,terr);
        } else {
          type_copy(&(noeud->typno),tcom);                         /* affecte type */
          type_copy(&tp,tcom);
        }
        return tp;
      case Wh:
        type tcom = creer_type(0, T_com);  /* type commande */
        type tboo = creer_type(0, T_bool); /* type booleen */
        tgauche = noeud->gauche->typno;
        tdroit = noeud->droit->typeno;
        if (type_eq(tgauche,tboo) == 0){
          type_copy(&(noeud->typeno),terr);
          type_copy(&tp, terr);
          typ_error("condition non booleenne dans un while", ligne);
        } else if ( !type_eq(tdroit, tcom) ){
          type_copy(&(noeud->typeno),terr);
          type_copy(&tp, terr);
        } else {
          type_copy(&(noeud->typno),tcom);                         /* affecte type */
          type_copy(&tp,tcom);
        }
        return tp;
      default : return tp;                            /* codop inconnu au bataillon */
    }/* fin switch          */
  } else /* fin if (noeud!=NULL)         */
    return tp;                                     /* noeud==NULL, arbre vide, type T_bot */
}
