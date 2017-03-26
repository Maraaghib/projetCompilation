#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "environ.h"
#include "util.h"

NOE Nalloc()
{
  return((NOE)malloc(sizeof(struct noeud)));
}

LFON Lfonalloc()
{
  LFON new_l = (LFON)malloc(sizeof(struct cellfon));
  new_l->ID = (char*)malloc(sizeof(char)*MAXIDENT);
  return new_l;
}

void prefix(NOE n)
/* ecrit l'expression n en notation prefixe*/
{ if(n != NULL)
    {printf("%s ",n->ETIQ);
      prefix(n->FG);
      prefix(n->FD);
    };
}

ENV creer_env(char* etiq, int val, TYPE type)
{
  ENV e = Envalloc();

  initenv(&e, etiq, type);
  affect(e,etiq,val);

  return e;
}

ENV copier_env(ENV env){
  ENV copie_env = Envalloc();

  strcpy(copie_env->ID,env->ID);
  copie_env->type = env->type;
  copie_env->VAL = env->VAL;

  return copie_env;
}

//Regarde d'abord dans l'environnement rho_lc (environnement local)
ENV rech2(char *chaine, ENV rho_gb, ENV rho_lc){
  ENV e = rech(chaine,rho_lc);
  if( e != NULL)
    return e;
  else
    return rech(chaine,rho_gb);
}


/*****************Biliste de Var *****************/

void inbilenv(BILENV phro, char * var, TYPE t){
  ENV new_e = Envalloc();
  initenv(&new_e,var, t);
  phro->fin->SUIV = new_e;
  phro->fin = new_e;
}

BILENV bilenv_vide(){
  BILENV new_be = (BILENV) malloc(sizeof(struct bilenv));
  new_be->debut =NULL;
  new_be->fin = NULL;

  return new_be ;
}


BILENV creer_bilenv(ENV var){
  BILENV new_be = bilenv_vide();
  new_be->debut = var ;
  new_be->fin = var ;

  return new_be;
}

BILENV copier_bilenv(BILENV b){
  BILENV copie_b = bilenv_vide();
  ENV curseur, copie_curseur;
  curseur = b->debut;
  copie_curseur = copier_env(curseur);

  while(curseur != NULL){
    copie_curseur->SUIV = copier_env(curseur);
    copie_curseur = copie_curseur->SUIV;
    curseur = curseur->SUIV;
  }
  copie_b->fin = copie_curseur;

  return copie_b;
}

BILENV concat(BILENV b1, BILENV b2){
  BILENV b_cat1 = copier_bilenv(b1);
  BILENV b_cat2 = copier_bilenv(b2);
  
  b_cat1->fin->SUIV = b_cat2->debut;
  b_cat1->fin = b_cat2->fin;

  return b1;
}

void ecrire_bilenv(BILENV b){
  ecrire_env(b->debut);
}

void affectb(BILENV rho_gb, BILENV rho_lc, char *lhs, int rhs){
  //tentative d'affectation dans la l'environ local
  if(affect(rho_lc->debut, lhs, rhs) != EXIT_SUCCESS)
    //tentative d'affectation dans l'environ global
    if(affect(rho_gb->debut,lhs,rhs) != EXIT_SUCCESS)
      fprintf(stderr,"La var %s est inexistante\n", lhs);
  
  return;
}

/*****************fonctions*****************/
LFON creer_fon(char *nfon,BILENV lparam, BILENV lvar, NOE com, TYPE tp) {
  LFON new_f = Lfonalloc();
  strcpy(new_f->ID,nfon);
  new_f->PARAM = lparam;
  new_f->VARLOC = lvar;
  new_f->CORPS = com;
  new_f->type = tp;
  new_f->SUIV = NULL;
}

/*Créer une simple copie de fonction lfn */
LFON copier_fon(LFON lfn){
  LFON new_f = Lfonalloc();
  strcpy(new_f->ID,lfn->ID);
  new_f->PARAM = copier_bilenv(lfn->PARAM);
  new_f->VARLOC = copier_bilenv(lfn->VARLOC);
  new_f->SUIV = NULL;
}

char* typetochar(TYPE tp){
  switch (tp){
  case integer :
    return "integer";
  case boolean :
    return "boolean";
  case array :
    return "array";
  default:
   break;
  }
  return "";
}

void ecrire_fon(LFON bfn){
  printf("%s : %s \n", bfn->ID, typetochar(bfn->type));
  printf("*****Param*****\n");
  ecrire_env(bfn->PARAM->debut);
  printf("*****Varloc*****\n");
  ecrire_env(bfn->VARLOC->debut);
  printf("*****Corps*****\n");
  prefix(bfn->CORPS);

}

LFON rechfon(char* chaine, LFON listident){
  LFON curseur = listident;

  while(curseur != NULL){
    if(strcmp(chaine, curseur->ID) == 0)
      break;
    curseur = curseur->SUIV;
  }

  return curseur;
}

/*****************Bilfon*****************/

BILFON bilfon_vide(){
  BILFON new_bf = (BILFON)malloc(sizeof(struct bilfon));
  new_bf->debut = NULL;
  new_bf->fin = NULL;

  return new_bf;
}

BILFON creer_bilfon(LFON pfon){
  BILFON new_bf = (BILFON)malloc(sizeof(struct bilfon));
  new_bf->debut = pfon;
  new_bf->fin = pfon;
  
  return new_bf;
}

BILFON copier_bilfon(BILFON bfn){
  BILFON copie_bf = (BILFON)malloc(sizeof(struct bilfon));
  LFON curseur = bfn->debut;
  LFON copie_curseur = copier_fon(curseur);
  copie_bf->debut = copie_curseur;

  while(curseur != NULL){
    copie_curseur->SUIV = copier_fon(curseur);
    copie_curseur = copie_curseur->SUIV;
    curseur = curseur->SUIV;
  }

  copie_bf->fin=copie_curseur;

  return copie_bf;
}

BILFON concatfn(BILFON b1, BILFON b2){
  BILFON b_cat, b_cat2;
  b_cat = copier_bilfon(b1);
  b_cat2 = copier_bilfon(b2);
  b_cat->fin->SUIV = b_cat2->debut;
  b_cat->fin = b_cat2->fin;

  return b1;
}

/* retourne la concaténation des variables locals et globals */
/*BILENV allvars(BILFON bfon){
  return concat(bfon->PARAM, bfon->VARLOC);
  }*/

void ecrire_bilfon(BILFON bfn){
  LFON curseur = bfn->debut;

  while(curseur != NULL){
    ecrire_fon(curseur);
    curseur = curseur->SUIV;
  }
}


void ecrire_prog(BILENV argb, BILFON argbf, NOE argno){
  printf("/***** Arguments global *****/\n");
  ecrire_bilenv(argb);
  printf("/***** Fonctions *****/\n");
  ecrire_bilfon(argbf);
  printf("/***** prog principal *****/\n");
  prefix(argno);
}
