/* arbre.c */
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bilquad.h"
#include "util.h"
#include "environ.h"
#include "../ppascalbison.h"

char *strdup(const char *s);
/* iimp.tab.h APRES arbre,h, sinon le type NOE est inconnu de gcc    */
/*-------------------------------------------------------------------*/
/* ----------------------------types---------------------------------*/
/* NOE,PILCOM,ENV : definis dans arbre.h                             */
/* QUAD,BILQUAD: definis dans codec3a.h                              */
/*-------------------------------------------------------------------*/
/*---------------------quadruplets-----------------------------------*/

/* retourne une nouvelle chaine */
char *gensym(char *prefix) {
  static int counter=0;
  char *chaine;char *chcounter;
  chcounter=Idalloc();
  chaine=Idalloc();
  strcpy(chaine,prefix);
  sprintf(chcounter,"%d",counter);   /* prefix+chaine(counter)*/
  counter=counter+1;
  strcat(chaine,chcounter);
  return(chaine);
}

/* test de gensym */
void test_constantes(int n) {
  char *ident;
  for(int i = 0; i < n; i++) {
    ident = gensym("CONS");
    printf("%s \n",ident);
  }
}


/* retourne bq +  skip */
BILQUAD ajouterskip(BILQUAD bq) {
  QUAD nq;
  BILQUAD nbq;
  nq = creer_quad(gensym("ET"), Sk, NULL, NULL, NULL); /* instruction skip */
  nbq = creer_bilquad(nq);
  return (concatq(bq, nbq));
}

/* retourne la "forme normale" de bq: dernier quad = skip */
BILQUAD normal(BILQUAD bq) {
  if (bq.fin == NULL) {
    return(ajouterskip(bq));
  }
  else {
    if (bq.fin->OP != Sk)        /* pas normal-> on normalise */
    return(ajouterskip(bq));
    else                      /* deja normal */
    return(bq);
  }
}

/* affiche le quadruplet (pour generer code) avec separateur":" ;
puis saute a la ligne */
void ecrire_sep_quad(QUAD qd) {
  if(strcmp(qd->ETIQ, "") == 0) {     /* etiquette= mot vide */
    printf("%-10s: ","");
  }
  else {
    printf("%-10s:", qd->ETIQ);
  }
  printf("%-6s:",nomop(qd->OP));
  if (qd->ARG1 != NULL) {
    printf("%-10s:", qd->ARG1);
  }
  else {
    printf("%-10s:", "");
  }
  if (qd->ARG2 != NULL) {
    printf("%-10s:", qd->ARG2);
  }
  else {
    printf("%-10s:", "");
  }
  if (qd->RES != NULL) {
    printf("%-10s\n", qd->RES);
  }
  else {
    printf("\n");
  }
}


/* affiche la biliste de quad (avec separateurs) */
void ecrire_sep_bilquad(BILQUAD bq) {
  QUAD qcour;
  qcour = bq.debut;
  while(qcour != NULL) {
    ecrire_sep_quad(qcour);
    qcour = qcour->SUIV;
  }
}

/*-------------------------------------------------------------------*/
/*------------------------pp-vers-quad------------------------------*/

/* traduit une (expression ou commande) en biliste de quadruplets */
/* met a jour l'environnement (var globale)                      */
// Faire une fonction qui traduit un Environnement (BILENV.debut) en BILQUAD
// Faire une focntion qui traduit un
int nparam = 0; // Le nombre de paramètres à passer à la fonction appelée

BILQUAD pp2quad(Noeud* ec) {
  extern ENV envrnt;
  BILQUAD bilq1, bilq2, bilexp, bilres; /* trad de: fg, fd, expression, resultat */
  int newop;
  char *netiq, *netiqf, *nres;        /* nouveaux ingredients */
  char *narg1 = NULL;
  char *narg2 = NULL;
  QUAD nquad;      /* nouveau quadruplet   */
  assert(ec != NULL);
  switch(ec->codop) {
    /* CAS: ec est une EXPRESSION */
    case Pl: case Mo: case Mu: case And: case Or: case Lt: case Eq:                /* operation binaire */
    /* les ingredients */
    netiq = gensym("ET");
    newop = ec->codop;
    /* les traductions des deux arguments */
    bilq1 = pp2quad(ec->gauche);
    bilq2 = pp2quad(ec->droit);
    /* se simplifie ? */
    if (ec->gauche->codop != V) {
      narg1 = Idalloc();
      strcpy(narg1, bilq1.fin->RES);
    }
    else {
      narg1 = Idalloc();
      strcpy(narg1, ec->gauche->ETIQ);
    }
    if (ec->droit->codop != V) {
      narg2 = Idalloc();
      strcpy(narg2, bilq2.fin->RES);
    }
    else {
      narg2 = Idalloc();
      strcpy(narg2, ec->droit->ETIQ);
    }
    nres = gensym("VA");
    /* le quadruplet: ETnum, Afc, chaineconst,-, VAnum */
    nquad = creer_quad(netiq, newop, narg1, narg2, nres);
    bilres = creer_bilquad(nquad);
    /* la suite de quadruplets */
    bilq2 = concatq(bilq1, bilq2);
    bilres = concatq(bilq2, bilres);
    break;
    case Not:
    /* les ingredients */
    netiq = gensym("ET");
    newop = Not;
    /* la traduction de l'unique argument */
    bilq1 = pp2quad(ec->gauche);
    /* se simplifie ? */
    if (ec->gauche->codop != V) {
      narg1 = Idalloc();
      strcpy(narg1, bilq1.fin->RES);
    }
    else {
      narg1 = Idalloc();
      strcpy(narg1, ec->gauche->ETIQ);
    }

    narg2 = NULL;
    nres = gensym("VA");
    /* le quadruplet: ETnum, Afc, chaineconst,-, VAnum */
    nquad = creer_quad(netiq, newop, narg1, narg2, nres);
    bilq2 = creer_bilquad(nquad);
    bilres = concatq(bilq1, bilq2);
    break;
    case I: case True: case False:
    /* les ingredients */
    netiq = gensym("ET");
    newop = Afc;
    narg1 = Idalloc();
    sprintf(narg1,"%s", ec->ETIQ);
    narg2 = NULL;
    nres = gensym("CT");
    /* le quadruplet: ETnum, Afc, chaineconst,-, CTnum */
    nquad = creer_quad(netiq, newop, narg1, narg2, nres);
    bilres = creer_bilquad(nquad);
    // printf("J'ai fini avec case I\n");
    break;
    case V:
    netiq = gensym("ET");
    newop = Sk;
    narg1 = NULL;
    narg2 = NULL;
    nres = ec->ETIQ;
    nquad = creer_quad(netiq, newop, narg1, narg2, nres);
    bilres = creer_bilquad(nquad);
    break;
    /* CAS: ec est une COMMANDE */
    case Mp:
    // Dans tous les cas
    bilq2 = pp2quad(ec->gauche); // Pour le programme principal
    /* les ingredients */
    netiq = gensym("ET");
    newop = St;
    narg1 = NULL;
    narg2 = NULL;
    nres = NULL;
    /* le quadruplet final: stop  (pas d'adresse de resultat) */
    nquad = creer_quad(netiq, newop, narg1, narg2, nres);
    bilres = creer_bilquad(nquad);

    bilres = concatq(bilq2, bilres);
    break;
    case Af:
    /* les ingredients */
    netiq = gensym("ET");
    newop = Af;
    /* narg1 =  chaine en lhs */
    narg1 = ec->gauche->ETIQ;
    /* narg2 =  adresse res du code du rhs */
    bilq2 = pp2quad(ec->droit);
    narg2 = Idalloc();
    strcpy(narg2, bilq2.fin->RES);
    nres = NULL;
    /* le quadruplet: ETnum, Af, chainevar1,chaineres2, NULL */
    nquad = creer_quad(netiq, newop, narg1, narg2, nres);
    bilres = concatq(bilq2, creer_bilquad(nquad));
    break;
    case Sk:
    /* les ingredients */
    netiq = gensym("ET");
    newop = Sk;
    narg1 = NULL;
    narg2 = NULL;
    nres = NULL;
    /* le quadruplet: skip  (pas d'adresse de resultat) */
    nquad = creer_quad(netiq, newop, narg1, narg2, nres);
    bilres = creer_bilquad(nquad);
    break;
    case Se:
    bilq1 = pp2quad(ec->gauche);
    // printf("I'm here \n");
    bilq2 = pp2quad(ec->droit);
    bilres = concatq(bilq1, bilq2);
    break;
    case If:
    bilexp = pp2quad(ec->gauche);    /* traduction de l'expression */
    bilq1 = pp2quad(ec->droit->gauche); /* commande (cas vrai) */
    bilq2 = pp2quad(ec->droit->droit); /* commande (cas faux) */
    bilq2 = normal(bilq2);
    /* les ingredients de Q1 */
    netiq = gensym("ET");
    newop = Jz;
    narg1 = bilexp.fin->RES;
    narg2 = NULL;
    nres = bilq2.debut->ETIQ;
    /* le quadruplet Q1 */
    nquad = creer_quad(netiq, newop, narg1, narg2, nres);
    /* nouvelle biliste */
    bilres = concatq(bilexp, creer_bilquad(nquad));
    bilres = concatq(bilres, bilq1);
    /* les ingredients de Q2 */
    netiq = gensym("ET");
    newop = Jp;
    narg1 = NULL;
    narg2 = NULL;
    nres = bilq2.fin->ETIQ;
    /* le quadruplet Q2 */
    nquad = creer_quad(netiq, newop, narg1, narg2, nres);
    /* nouvelle biliste */
    bilres = concatq(bilres, creer_bilquad(nquad));
    bilres = concatq(bilres, bilq2);
    break;
    case Wh:
    bilexp = pp2quad(ec->gauche);    /* traduction de l'expression */
    bilq1 = pp2quad(ec->droit);     /* traduction du corps        */
    bilq1 = normal(bilq1);
    /* les ingredients de Q1 */
    netiq = gensym("ET");
    newop = Jz;  /* etiquette de Q1            */
    narg1 = bilexp.fin->RES;
    narg2 = NULL;
    netiqf = gensym("ET");        /* etiquette fin de traduction */
    nres = netiqf;
    /* le quadruplet Q1 */
    nquad = creer_quad(netiq, newop, narg1, narg2, nres);
    /* nouvelle biliste */
    bilres = concatq(bilexp, creer_bilquad(nquad));
    bilres = concatq(bilres, bilq1);
    /* les ingredients de Q2 */
    newop = Jp;
    /* narg1 = narg2 = NULL; */
    nres = bilexp.debut->ETIQ;
    /* on substitue Q2 a la fin de bilres */
    bilres.fin->OP = newop;
    assert(bilres.fin->ARG1 == NULL); /* vrai si bilq1 est normalise */
    assert(bilres.fin->ARG2 == NULL);
    bilres.fin->RES = nres;
    /* les ingredients de Q3 */
    netiq = netiqf;                   /* etiquette de Q3            */
    newop = Sk;
    narg1 = NULL;
    narg2 = NULL;
    nres = NULL;
    /* le quadruplet Q3 */
    nquad = creer_quad(netiq, newop, narg1, narg2, nres);
    /* nouvelle biliste */
    bilres = concatq(bilres, creer_bilquad(nquad));
    break;
    case NewAr:

    bilq1 = pp2quad(ec->droit); // L'indice du tableau
    /* les ingredients */
    netiq = gensym("ET");
    newop = Afc;
    narg1 = Idalloc();
    // printf("case I: ec->ETIQ = %s\n", ec->ETIQ);
    sprintf(narg1,"%s", ec->ETIQ);
    narg2 = NULL;
    nres = gensym("TAB");
    /* le quadruplet: ETnum, Afc, chaineconst,-, CTnum */
    nquad = creer_quad(netiq, newop, narg1, narg2, nres);
    bilres = creer_bilquad(nquad);
    // printf("J'ai fini avec case I\n");
    break;
    case Ind:
    bilq1 = pp2quad(ec->droit); // L'indice du tableau
    bilq2 = pp2quad(ec->gauche); // Le tableau

    netiq = gensym("ET");
    newop = Ind;
    narg1 = ec->gauche->ETIQ;
    narg2 = strdup(bilq2.fin->RES);
    nres = gensym("TAB");

    nquad = creer_quad(netiq, newop, narg1, narg2, nres);
    bilres = creer_bilquad(nquad);
    /* la suite de quadruplets */
    bilq2 = concatq(bilq1, bilq2);
    bilres = concatq(bilq2, bilres);
    break;
    case AfInd:
    bilq1 = pp2quad(ec->droit); // L'indice du tableau
    bilq2 = pp2quad(ec->gauche); // Le tableau

    netiq = gensym("ET");
    newop = AfInd;
    narg1 = ec->gauche->ETIQ;
    narg2 = strdup(bilq2.fin->RES);
    nres = gensym("TAB");

    nquad = creer_quad(netiq, newop, narg1, narg2, nres);
    bilres = creer_bilquad(nquad);
    /* la suite de quadruplets */
    bilq2 = concatq(bilq1, bilq2);
    bilres = concatq(bilq2, bilres);
    break;
    case Dep: case Def:
    /* les ingredients */
    netiq = ec->ETIQ;
    newop = ec->codop;
    narg1 = NULL;
    narg2 = NULL;
    nres = NULL;
    /* le quadruplet final: stop  (pas d'adresse de resultat) */
    nquad = creer_quad(netiq, newop, narg1, narg2, nres);
    bilres = creer_bilquad(nquad);

    bilq1 = pp2quad(ec->droit); // Pour le programme principal
    bilres = concatq(bilq1, bilres);
    break;
    case T_fon:
    bilq1 = pp2quad(ec->gauche);
    netiq = gensym("ET");
    newop = Call;
    narg1 = strdup(ec->ETIQ);
    narg2 = Idalloc();
    sprintf(narg2, "%d", nparam);
    nparam = 0; // On le réinitialise pour un autre appel
    nres = NULL;

    nquad = creer_quad(netiq, newop, narg1, narg2, nres);
    /* nouvelle biliste */
    bilres = concatq(bilq1, creer_bilquad(nquad));
    break;
    case Virgule:
    if (ec->gauche != NULL) {
      bilq1 = pp2quad(ec->gauche);
      nparam ++;

      netiq = gensym("ET");
      newop = Param;
      narg1 = strdup(ec->gauche->ETIQ);
      narg2 = strdup("0"); // CAlculer la  valeur de l'arg2 à partir d'un environnemt
      nres = NULL;

      nquad = creer_quad(netiq, newop, narg1, narg2, nres);
    }

    if (ec->droit != NULL && ec->gauche != NULL) {
      bilq2 = pp2quad(ec->droit);

      netiq = gensym("ET");
      newop = Param;
      narg1 = strdup(ec->droit->ETIQ);
      narg2 = strdup("0");
      nres = NULL;

      nquad = creer_quad(netiq, newop, narg1, narg2, nres);
      /* nouvelle biliste */
      bilq2 = concatq(bilq2, creer_bilquad(nquad));
      bilres = concatq(bilq1, bilq2);
    }
    else {
      bilres = concatq(bilq1, creer_bilquad(nquad));
    }


    break;
    default: break;
  };
  return(bilres);
}
