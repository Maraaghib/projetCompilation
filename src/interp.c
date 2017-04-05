 #include <stdio.h> 
 #include <stdlib.h>
 #include <string.h>
 #include "include/util.h"
 #include "ppascalbison.h"
#include "interp.h"

/*-------------------------------------------------------------------*/
/* ----------------------------types---------------------------------*/
/*  NOE,ENVTY,BILENVTY  : definis dans arbre.h                       */
/*-------------------------------------------------------------------*/


/* ------------------VARIABLES GLOBALES ------------------------------*/
/* le tas; (NIL=0); "vraies" adresses >=1                             */
int TAS[TAILLEMEM];
/* ADR[i]=adresse dans le tas du tab i                                */
int ADR[TAILLEADR];
/* TAL[i]=taille du tab i                                             */
int TAL[TAILLEADR];  
int ptasl=1; /* premiere place libre dans TAS[]                       */
int padrl=1; /* premiere place libre dans ADR[]                       */
/*--------------------------------------------------------------------*/
/* ----------------------traitement--memoire--------------------------*/
void init_memoire()
{int i=0;
while (i < TAILLEMEM)
  TAS[i++]=0;
i=0;
while (i < TAILLEADR)
  {ADR[i++]=0;
   TAL[i]=0;
  }
}
/* decrit la memoire: */
/* ADR[i]: adresse du tableau i dans le TAS */ 
/* TAL[i]: taille du tableau i; de ADR[i] a ADR[i] + TAL[i]-1 */
/* TAS: tableau (statique) contenant tous les tableaux (dynamiques) */
void ecrire_memoire(int maxadr, int maxtal, int maxtas)
{int i;
 printf("Le tableau ADR:\n");
 printf("------------------------:\n");
 for(i=0; i < maxadr;i++)
   printf("%d:",ADR[i]);
 printf("\n");
 printf("Le tableau TAL:\n");
 printf("------------------------:\n");
 for(i=0; i < maxtal;i++)
   printf("%d:",TAL[i]);
 printf("\n");
 printf("Le tableau TAS:\n");
 printf("------------------------:\n");
 for(i=0; i < maxtas;i++)
   printf("%d:",TAS[i]);
 printf("\n");
 return;
}
/*--------------------------------------------------------------------*/	    
/*---------------semantique-------------------------------------------*/
int traitement(ENV *e,Noeud *n){
	ENV pos; 
 	if (!n) return 0;
 	switch(n->codop){
 		case I:
			return atoi(n->ETIQ);
		case V:
			{ pos=rech(n->ETIQ,e->debut);
			return (pos->VAL);
			}
		case If:
			traitement(e,n->gauche);
                        traitement(e,n->droit);
 			return 0;
		case El:
			if(traitement(e,n->gauche)){
 				traitement(e,n->droit);
				return 0;
			}
		case Th:
			if(!traitement(e,n->gauche))
 				traitement(e,n->droit);
			return 0;
		case Wh:
			if(traitement(e,n->gauche)){
				traitement(e,n->droit);
				traitement(e,n);
				}
		case Af:
			initenv(e,n->gauche->ETIQ);
 			return affect(*e, n->gauche->ETIQ, traitement(e,n->droit));
 		case Se:
 			traitement(e,n->gauche);
 			return traitement(e,n->droit);
 		case Pl:
 		case Mo:
 		case Mu:
			return eval(n->type, traitement(e, n->gauche), traitement(e, n->droit));
		case And:
			return(traitement(e, n->gauche) & traitement(e, n->droit));
		case Or:
			return(traitement(e, n->gauche) | traitement(e, n->droit));
		case not:
			return(!traitement(e,n->gauche));
		case lt:
			if(traitement(e, n->gauche) < traitement(e, n->droit))
				return 1;
			return 0;
		case eq:
			if(traitement(e, n->gauche) == traitement(e, n->droit))
				return 1;
			return 0;
		case NewAr:	
			return 0;
		case T_int:
			return atoi(n->ETIQ);
		case T_bool:
				return atoi(n->ETIQ);
		case T_array:
					return 0;
			
		case Def:
			creer_bilfon(creer_fon(n->ETIQ,
			;
		
     }
     return 0;
 }
 
/*------------------FONCTIONS ---------------------------------------------------*/
/*------------------types--------------------------------------------------------*/

void typ_error(char *mess,int ligne)
{fprintf(stderr, "TYPERROR: %s: ligne %d \n", mess, ligne);
 return;}

/* renvoie le type de e : TYPEF \in T_int, T_boo, T_com, T_err, T_bot          */
/* affecte ce type a la racine                                                 */
/* suppose corrects les types des fils                                         */
/* envoie message d'erreur si mal type a la racine et fils bien types          */
/* renvoie T_err si l'arbre est mal-type                                       */
type calcul_type(BILENVTY rho_gb, NOE e, int ligne)
{ type tp;
  tp= creer_type(0,T_bot);/* type par defaut */
  if(e != NULL)
    { type tfg,tfd;                /* type  du fils gauche, fils droit */
      type terr=creer_type(0,T_err);   /* type  erreur                 */
      /* un des fils est mal-type */
      if ((e->FG && type_eq(e->FG->typno,terr))||(e->FD && type_eq(e->FD->typno,terr)))
	{type_copy(&tp,terr);/* valeur du  type                              */
	 type_copy(&(e->typno),terr); /* affecte type  erreur                */
	 return(tp);}
      /* e != NULL et tous les fils sont bien-types */
      switch(e->codop)
	{case true:
	   tp=e->typno;
	   return(tp);/* typno deja affecte     */
	 case false:
	   tp=e->typno;
	   return(tp);/* typno deja affecte     */
	 case Ind: /* (tab_dim k of tau) x   int -> (tab_dim k-1 of tau) */
	   {/* a ecrire */
	   };
	   return(tp);}
	case Pl:case Mo:case Mu: /* int x  int -> int */
	  {type tint;    /* type  des fils  */
	     tint=creer_type(0,T_int);
	     tfg=e->FG->typno;
	     tfd=e->FD->typno;
	     if (type_eq(tfg,tint) && type_eq(tfd,tint))
	       { type_copy(&tp,tfg);        /* calcule le type */
		 type_copy(&(e->typno),tfg);/* affecte le type */
	       }
	     else
	       {type_copy(&tp,terr);/* calcule le type                               */
		type_copy(&(e->typno),terr); /* affecte type                         */
		typ_error("op entier sur args non-entiers",ligne);/* message erreur  */
	       }
	     return(tp);
	  }
	case Lt:case Eq:         /* int x int -> boo                   */
	  {type tint,tboo;       /* types  entier, booleen             */
	     tint=creer_type(0,T_int);
	     tboo=creer_type(0,T_boo);
	     tfg=e->FG->typno;
	     tfd=e->FD->typno;
	     if (type_eq(tfg,tint) && type_eq(tfd,tint))
	       { type_copy(&tp,tboo);         /* valeur du  type */
		 type_copy(&(e->typno),tboo); /* affecte le type */
	       }
	     else
	       {type_copy(&tp,terr);/* valeur du  type                               */
		type_copy(&(e->typno),terr); /* affecte type                         */
		typ_error("op de comparaison sur args non-entiers",ligne);/* message */
	       }
	     return(tp);
	  }
	case And:case Or:        /* boo x boo -> boo                   */
	  {type tboo;            /* type  booleen                      */
	     tboo=creer_type(0,T_boo);
	     tfg=e->FG->typno;
	     tfd=e->FD->typno;
	     if (type_eq(tfg,tboo) && type_eq(tfd,tboo))
	       { type_copy(&tp,tboo);         /* valeur du  type */
		 type_copy(&(e->typno),tboo); /* affecte le type */
	       }
	     else
	       {type_copy(&tp,terr);/* valeur du  type                              */
		type_copy(&(e->typno),terr); /* affecte type                        */
		typ_error("op booleen sur args non-booleens",ligne);/*message erreur*/
	       }
	     return(tp);
	  }
	case Not:                /* boo  -> boo                        */
	  {type tboo;            /* type  booleen                      */
	     tboo=creer_type(0,T_boo);
	     tfg=e->FG->typno;
	     assert(e->FD==NULL); /* op unaire                         */
	     if (type_eq(tfg,tboo))
	       { type_copy(&tp,tboo);         /* valeur du  type */
		 type_copy(&(e->typno),tboo); /* affecte le type */
	       }
	     else
	       {type_copy(&tp,terr);/* valeur du  type                               */
		type_copy(&(e->typno),terr); /* affecte type                         */
		typ_error("op booleen sur arg non-booleen",ligne);/* message erreur  */
	       }
	     return(tp);
	  }
	case I:                  /* constante T_int        */
	  {type tint;            /* type  du noeud         */
	   tint=creer_type(0,T_int);
	   assert(type_eq(e->typno,tint));/*verif du  type */
	   type_copy(&tp,tint); /* valeur du  type         */
	   return(tp);
	  }
	case V:                  /* variable              */
	  { ENVTY pos=rechty(e->ETIQ,rho_gb.debut);               /* pos var dans rho */ 
	    if (pos!=NULL)
	      { type_copy(&tp, pos->TYPE);        /* valeur du  type := rho(var)      */
		type_copy(&(e->typno), pos->TYPE);/* affecte le type                  */
	      }
	    else
	      {type_copy(&tp,terr);       /* tp := erreur                             */
	       type_copy(&(e->typno),tp); /* affecte type                             */
	       typ_error("variable inconnue ",ligne);/* message erreur                */
	      };
	    return(tp);                           /* renvoie le type                  */
	  }
	case NewAr:                                             /* creation tableau */
	  {/* a ecrire */
	    return(tp);
	  }
	case Af:                                                     /* affectation */
	    {if (type_eq(e->FG->typno,e->FD->typno)==0)/* type(lhs) <> type(rhs)    */
		{ type_copy(&(e->typno),terr); /* affecte type                        */
		  typ_error("affectation de types incoherents ", ligne);
		  return(terr);
		}
	      else                                     /* type(lhs) == type(rhs)      */
		type_copy(&(e->typno), creer_type(0,T_com));
	      return(tp); 
	     }
	case Se:
	    { type tcom= creer_type(0,T_com);         /* type  commande               */
	      type_copy(&(e->typno),tcom);
	      type_copy(&tp,tcom);
	      return(tp);                             
	      }
	case If:
	    { type tcom= creer_type(0,T_com);         /* type  commande               */
	      type tboo=creer_type(0,T_boo);          /* type  booleen                */
	      tfg=e->FG->typno;                       /* type  des  3 fils            */
	      type tthen=e->FD->FG->typno; 
	      type telse=e->FD->FD->typno;
	      if (type_eq(tfg,tboo)==0)              /* type arg0  <> booleen         */
		{type_copy(&(e->typno),terr);        /* affecte type                  */
		 type_copy(&tp,terr);
		 typ_error("condition non booleenne dans un IfThEl", ligne);
		 }
	      else if (!type_eq(tthen,tcom) ||!type_eq(telse,tcom))  /* arg <> tcom   */
		{type_copy(&(e->typno),terr);                        /* affecte type  */
		 type_copy(&tp,terr);
		 }
	      else 
		{type_copy(&(e->typno),tcom);                         /* affecte type */
		 type_copy(&tp,tcom);
		}
	      return(tp);
	    }
	case Wh:
	  {/* a ecrire */
		}
	      return(tp);
	    }
	default: return(tp);                            /* codop inconnu au bataillon */
	}/* fin switch          */
   }/* fin if (e!=NULL)         */
  else
    return(tp);                                     /* e==NULL, arbre vide, type T_bot */
}




 int start(ENV *e,noeud* n){
     int res = traitement(e,n);
     ecrire_env(*e);
     libererArbre(n);
     return res;
 }

 
