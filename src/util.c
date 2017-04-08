#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../ppascalbison.h"
#include "environ.h"
#include "util.h"

Noeud* Nalloc()
{
  return((Noeud*)malloc(sizeof(struct Noeud)));
}

LFON Lfonalloc()
{
  LFON new_l = (LFON)malloc(sizeof(struct cellfon));
  new_l->ID = (char*)malloc(sizeof(char)*MAXIDENT);
  return new_l;
}

/* ecrit l'expression n en notation prefixe*/
void prefix(Noeud *n){
  print_tree_ter(n);
}

int est_feuille(Noeud* n){
  return (n->droit == NULL && n->gauche == NULL);
}

type* talloc(){
  return (type*)malloc(sizeof(type));
}

/*****************Noeud ***********************************/
Noeud* create_noeud(Noeud *fgauche, Noeud* fdroit,char* id,int ntype,type* tk){
  Noeud* new_noeud = (Noeud*)malloc(sizeof(struct Noeud));
  new_noeud->gauche = fgauche;
  new_noeud->droit = fdroit;
  new_noeud->ETIQ = id;
  new_noeud->codop = ntype;
  new_noeud->typeno = tk;

  return new_noeud;

}

void print_tree(Noeud* n, FILE* f) {}

void print_tree_ter(Noeud* n) {
  if(n!= NULL){

    printf("%s ",n->ETIQ);
    print_tree_ter(n->gauche);
    print_tree_ter(n->droit);

  }

}

/*****************Biliste d'environnement *****************/

int type_eq(type* t1, type* t2){
  if(t1->DIM == t2->DIM)
    if(t1->TYPEF == t2->TYPEF)
      return 1;
  return 0;
}

void type_copy(type* tcop, type* torig){
  tcop = creer_type(torig->DIM,torig->TYPEF);
}

void type_affect(ENV rho, type* tvar){
  rho->typeno = tvar;
}

type* creer_type(int dm, int tf){
  type* n_type = (type*)malloc(sizeof(type));
  n_type->DIM = dm;
  n_type->TYPEF = tf;
  return n_type;
}

type* type_res_op(int op) {}

void ecrire_type(type* tp){
  printf("dim: %d type: %d", tp->DIM, tp->TYPEF);
}

ENV creer_env(char* etiq, int val, type* typeno){
  ENV e = Envalloc();
  strcpy(e->ID,etiq);
  e->VAL = val;
  e->typeno = typeno;
  e->SUIV = NULL;
  return e;
}

ENV copier_env(ENV env){

  return creer_env(env->ID,env->VAL,env->typeno);

}

//Regarde d'abord dans l'environnement rho_lc (environnement local)
ENV rech2(char *chaine, ENV rho_gb, ENV rho_lc){
  if( chaine != NULL && (rho_gb != NULL || rho_lc != NULL)){
    ENV e = rech(chaine,rho_lc);
    if( e != NULL)
      return e;
    else
      return rech(chaine,rho_gb);
  }
  return NULL;
}

/*****************Biliste de Var *****************/

void inbilenv(BILENV phro, char * var, type* t){
  ENV new_e =creer_env(var,0,t);
  if(phro->debut == NULL){
    phro->debut = new_e;
    phro->fin = new_e;
  }
  else
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
  new_be->debut = var;
  new_be->fin = var;

  return new_be;
}

BILENV copier_bilenv(BILENV b){
  if(b == NULL)
    return NULL;
  BILENV copie_b = bilenv_vide();
  ENV curseur, copie_curseur;
  curseur = b->debut;
  copie_curseur = copier_env(curseur);
  copie_b->debut = copie_curseur;
  curseur = curseur->SUIV;

  while(curseur != NULL){
    copie_curseur->SUIV = copier_env(curseur);
    copie_curseur = copie_curseur->SUIV;
    curseur = curseur->SUIV;
  }
  copie_b->fin = copie_curseur;

  return copie_b;
}

BILENV concat(BILENV b1, BILENV b2){
  if(b1->debut == NULL){
    b1->debut = b2->debut;
    b1->fin = b2->fin;
  }
  else{
    b1->fin->SUIV = b2->debut;
    b1->fin = b2->fin;
  }
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

void liberer_bilenv(BILENV be){
  ENV curseur = be->debut;
  while(curseur!=NULL){
    be->debut = be->debut->SUIV;
    liberer_env(curseur);
    curseur = be->debut;
  }
}

/*****************fonctions*****************/
LFON creer_fon(char *nfon,BILENV lparam, BILENV lvar, Noeud* com, type* tp) {
  LFON new_f = Lfonalloc();
  strcpy(new_f->ID,nfon);
  new_f->PARAM = lparam;
  new_f->VARLOC = lvar;
  new_f->CORPS = com;
  new_f->typeno = tp;
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

char* typetochar(int tp){
  switch (tp){
  case T_int :
    return "integer";
  case T_bool :
    return "boolean";
  default:
   break;
  }
  return "";
}

void ecrire_fon(LFON bfn){
  printf("\t\t%s : ", bfn->ID);
  ecrire_type(bfn->typeno);
  printf("\n*****Param*****\n");
  if(bfn->PARAM != NULL)
    ecrire_env(bfn->PARAM->debut);
  printf("*****Varloc*****\n");
  if(bfn->VARLOC != NULL)
    ecrire_env(bfn->VARLOC->debut);
  printf("*****Corps*****\n");
  prefix(bfn->CORPS);
  printf("\n");

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

void liberer_lfon(LFON fon){
  free(fon->ID);
  liberer_bilenv(fon->PARAM);
  liberer_bilenv(fon->VARLOC);
  //liberer_arbre(fon->CORPS);
  free(fon);
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
  if(b1->debut == NULL){
    b1->debut = b2->debut;
    b1->fin = b2->fin;
  }
  else{
    b1->fin->SUIV = b2->debut;
    b1->fin = b2->fin;
  }
  return b1;
}

/* retourne la concaténation des variables locals et globals */
/*BILENV allvars(BILFON bfon){
  return concat(bfon->PARAM, bfon->VARLOC);
  }*/

void ecrire_bilfon(BILFON bfn){
  if(bfn != NULL){
    LFON curseur = bfn->debut;
    
    while(curseur != NULL){
      ecrire_fon(curseur);
      curseur = curseur->SUIV;
    }
  }
}

void liberer_bilfon(BILFON bfon){
  LFON curseur = bfon->debut;
  while(curseur != NULL){
    bfon->debut = bfon->debut->SUIV;
    liberer_lfon(curseur);
    curseur = bfon->debut;
  }
}

void ecrire_prog(BILENV argb, BILFON argbf, Noeud* argno){
  printf("/**Programme avant execution !**/\n");
  printf("/***** Variables globales *****/\n");
  ecrire_bilenv(argb);
  printf("/***** Fonctions *****/\n");
  ecrire_bilfon(argbf);
  printf("/***** prog principal *****/\n");
  prefix(argno);
  printf("\n");
}
