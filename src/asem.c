#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "asem.h"
#include "util.h"
#include "environ.h"
#include "../ppascalbison.h"
/*------------------FONCTIONS ---------------------------------------------------*/
/*------------------types--------------------------------------------------------*/

void typ_error(char *mess, int ligne){
  fprintf(stderr, "\x1b[31m""TYPERROR: %s: ligne %d \n""\x1b[0m", mess, ligne);
  return;
}

/* renvoie le type de noeud : TYPEF \in T_int,  T_bool, T_com, T_err, T_bot          */
/* affecte ce type a la racine                                                 */
/* suppose corrects les types des fils                                         */
/* envoie message d'erreur si mal type a la racine et fils bien types          */
/* renvoie T_err si l'arbre est mal-type                                       */
type *calcul_type(BILENV rho_gb, Noeud *noeud, int ligne){
  type *tp;
  tp = creer_type(0,T_bot); /* type par defaut */
  if(noeud != NULL){
    type *tgauche, *tdroit;                /* type  du fils gauche, fils droit */
    type *terr = creer_type(0,T_err);   /* type  erreur                 */
    /* un des fils est mal-type */
    if ((noeud->gauche && type_eq(noeud->gauche->typeno,terr))||(noeud->droit && type_eq(noeud->droit->typeno,terr))){
	     type_copy(tp,terr); /* valeur du  type                              */
	     type_copy(noeud->typeno,terr); /* affecte type  erreur           x     */
	     return tp;
    } /* noeud != NULL et tous les fils sont bien-types */
    type *tint, *tboo, *tcom, *tthen, *telse;
    ENV pos;
    switch(noeud->codop){
      case True:
        tp = noeud->typeno;
        return tp; /* typeno deja affecte     */
      case False:
        tp = noeud->typeno;
        return tp; /* typeno deja affecte     */
      case Ind: /* (tab_dim k of tau) x   int -> (tab_dim k-1 of tau) */
        if (noeud->gauche->typeno->TYPEF != T_err && noeud->droit->typeno->TYPEF != T_err){
          if (noeud->gauche->typeno->DIM > 0){
            if (noeud->droit->typeno->TYPEF == T_int){
              type_copy(tp, noeud->gauche->typeno);
              noeud->gauche->typeno->DIM--;
              return tp;
            } else
              typ_error("indice non entier ", ligne);
          } else
            typ_error("ce n'est pas un tableau ", ligne);
        }
        type_copy(tp, terr);
        type_copy(noeud->typeno, terr);
        return tp;
      case Pl:case Mo:case Mu: /* int x  int -> int */
        /* type  des fils  */
        tint = creer_type(0,T_int);
        tgauche = noeud->gauche->typeno;
        tdroit = noeud->droit->typeno;
        if (type_eq(tgauche,tint) && type_eq(tdroit,tint)){
          type_copy(noeud->typeno,tgauche); /* affecte le type */
          type_copy(tp,tgauche);         /* calcule le type */
        } else {
          type_copy(tp,terr); /* calcule le type                               */
          type_copy(noeud->typeno,terr); /* affecte type                         */
          typ_error("op entier sur args non-entiers",ligne); /* message erreur  */
        }
        return tp;
      case Lt:case Eq:         /* int x int -> boo                   */
        /* types  entier, booleen             */
        tint = creer_type(0,T_int);
        tboo = creer_type(0,T_bool);
        tgauche = noeud->gauche->typeno;
        tdroit = noeud->droit->typeno;
        if (type_eq(tgauche,tint) && type_eq(tdroit,tint)){
          type_copy(tp,tboo);         /* valeur du  type */
          type_copy(noeud->typeno,tboo); /* affecte le type */
        } else {
          type_copy(tp,terr);/* valeur du  type                               */
          type_copy(noeud->typeno,terr); /* affecte type                         */
          typ_error("op de comparaison sur args non-entiers",ligne); /* message */
        }
        return tp;
      case And:case Or:        /* boo x boo -> boo                   */
        tboo = creer_type(0,T_bool);
        tgauche = noeud->gauche->typeno;
        tdroit = noeud->droit->typeno;
        if (type_eq(tgauche,tboo) && type_eq(tdroit,tboo)){
          type_copy(tp,tboo);         /* valeur du  type */
          type_copy(noeud->typeno,tboo); /* affecte le type */
        } else {
          type_copy(tp,terr);/* valeur du  type                              */
          type_copy(noeud->typeno,terr); /* affecte type                        */
          typ_error("op booleen sur args non-booleens",ligne); /*message erreur*/
        }
        return tp;
      case Not:                /* boo  -> boo                        */
        tboo = creer_type(0,T_bool);
        tgauche = noeud->gauche->typeno;
        assert(noeud->droit == NULL); /* op unaire                         */
        if (type_eq(tgauche,tboo)){
          type_copy(tp,tboo);         /* valeur du  type */
          type_copy(noeud->typeno,tboo); /* affecte le type */
        } else {
          type_copy(tp,terr);/* valeur du  type                               */
          type_copy(noeud->typeno,terr); /* affecte type                         */
          typ_error("op booleen sur arg non-booleen",ligne); /* message erreur  */
        }
        return tp;
      case I:                  /* constante T_int        */
        tint = creer_type(0,T_int);
        assert(type_eq(noeud->typeno,tint));/*verif du  type */
        type_copy(tp,tint); /* valeur du  type         */
        return tp;
      case V:                  /* variable              */
        pos = rech(noeud->ETIQ ,rho_gb->debut);               /* pos var dans rho */
        if (pos != NULL){
          type_copy(tp, pos->typeno);        /* valeur du  type := rho(var)      */
          type_copy(noeud->typeno, pos->typeno);/* affecte le type                  */
        } else {
          type_copy(tp,terr);       /* tp := erreur                             */
          type_copy(noeud->typeno,tp); /* affecte type                             */
          typ_error("variable inconnue ",ligne); /* message erreur                */
        }
        return tp;                           /* renvoie le type                  */
      case NewAr:                                             /* creation tableau */
       if (type_eq(noeud->gauche->typeno, terr) || type_eq(noeud->droit->typeno, terr)){
          type_copy(tp, terr);
          type_copy(noeud->typeno, terr);
          typ_error("initialisation de tableau avec des types differents ", ligne);
        } else
          type_copy(tp, noeud->typeno);
        return tp;
      case Af:                                                     /* affectation */
        if (type_eq(noeud->gauche->typeno,noeud->droit->typeno) == 0){/* type(lhs) <> type(rhs)    */
          type_copy(noeud->typeno,terr); /* affecte type                        */
          typ_error("affectation de types incoherents ", ligne);
          return terr;
        } else                                     /* type(lhs) == type(rhs)      */
          type_copy(noeud->typeno, creer_type(0,T_com));
        return tp;
      case Se:
        tcom = creer_type(0,T_com);         /* type  commande               */
        type_copy(noeud->typeno,tcom);
        type_copy(tp,tcom);
        return tp;
      case If:
        tcom = creer_type(0,T_com);         /* type  commande               */
        tboo = creer_type(0,T_bool);          /* type  booleen                */
        tgauche = noeud->gauche->typeno;                       /* type  des  3 fils            */
        tthen = noeud->droit->gauche->typeno;
        telse = noeud->droit->droit->typeno;
        if (type_eq(tgauche,tboo) == 0){              /* type arg0  <> booleen         */
          type_copy(noeud->typeno,terr);        /* affecte type                  */
          type_copy(tp,terr);
          typ_error("condition non booleenne dans un if", ligne);
        } else if (!type_eq(tthen,tcom) || !type_eq(telse,tcom)){ /* arg <> tcom   */
          type_copy(noeud->typeno,terr);                        /* affecte type  */
          type_copy(tp,terr);
        } else {
          type_copy(noeud->typeno,tcom);                         /* affecte type */
          type_copy(tp,tcom);
        }
        return tp;
      case Wh:
        tcom = creer_type(0, T_com);  /* type commande */
        tboo = creer_type(0, T_bool); /* type booleen */
        tgauche = noeud->gauche->typeno;
        tdroit = noeud->droit->typeno;
        if (type_eq(tgauche,tboo) == 0){
          type_copy(noeud->typeno,terr);
          type_copy(tp, terr);
          typ_error("condition non booleenne dans un while", ligne);
        } else if ( !type_eq(tdroit, tcom) ){
          type_copy(noeud->typeno,terr);
          type_copy(tp, terr);
        } else {
          type_copy(noeud->typeno,tcom);                         /* affecte type */
          type_copy(tp,tcom);
        }
        return tp;
      default : return tp;                            /* codop inconnu au bataillon */
    }/* fin switch          */
  } else /* fin if (noeud!=NULL)         */
    return tp;                                     /* noeud==NULL, arbre vide, type T_bot */
}
