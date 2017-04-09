%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <stdbool.h>
  #include <unistd.h>
  #include "../ppascalbison.h"
  #include "util.h"
  #include "environ.h"
  #include "interp.h"
  #include "bilquad.h"

  int yywrap();
  int yylex();
  void yyerror();
  char *strdup(const char *s);
  extern int yylineno;

  BILENV env_cour, env_global; //par defaut env_cur correspond a env_global
  Noeud* syntree; /* commande  globale */
  ENV envrnt;
  BILFON liste_fct;

  type* type_base;
  char* fct_cour;
  BILENV param_cour;
  int type_fct; // vaut 0 pour les procedures



  int i =0;

  %}

%error-verbose

%union{
  struct Noeud * Noeud;
  struct stype* codop;
  char* terminal;
  struct bilenv* b_env;
  struct bilfon* bf;
 }

%token T_bool T_int T_ar T_err T_com T_bot T_fon T_pro//type
%token NFon NPro //identificateur
%token NewAr //new
%token<Noeud> Mp Def Dep Af Se If Th El Wh Do Pl Mo Mu And Or Not Lt Eq Sk //operateur
%token Ind Afc AfInd Jp Jz St Param Call Ret //utile pour le c3a
/* Codes C3A: Ind Affectation_constatnte, AfInd Jump Jumpzero Stop Paraètre Call Ret*/
%token True False Var
%token<terminal> I V //token terminaux
%token PO PF AO AF CO CF Virgule DPoints //parenthése accolade crochet deux points

%left Virgule
%left Af
%left If Th El Wh Do
%left Mo Mu Or Lt Eq And Not Pl
%left Se
%right PO AO CO


%type<Noeud> E Et C L_argsnn L_args
%type<b_env>  L_vartnn L_vart Argt L_argt L_argtnn
%type<bf> LD D D_entf D_entp
%type<codop> TP


%start MP
%%
MP: L_vart LD C {
                  env_global = $1;
                  syntree = create_noeud($3, NULL, "Mp", Mp, creer_type(0,T_com));
                  //ecrire_bilenv($1);
                  //print_tree_ter($3); printf("\n");
                  ecrire_prog($1,$2,$3);
                  YYACCEPT;
 }


E: E Pl E {$$ = create_noeud($1,$3,"Pl",Pl,creer_type(0,T_int));}
| E Mo E {$$ = create_noeud($1,$3,"Mo",Mo,creer_type(0,T_int));}
| E Mu E {$$ = create_noeud($1,$3,"Mu",Mu,creer_type(0,T_int));}
| E Or E {$$ = create_noeud($1,$3,"Or",Or,creer_type(0,T_bool));}
| E Lt E {$$ = create_noeud($1,$3,"Lt",Lt,creer_type(0,T_bool));}
| E Eq E {$$ = create_noeud($1,$3,"Eq",Eq,creer_type(0,T_bool));}
| E And E {$$ = create_noeud($1,$3,"And",And,creer_type(0,T_bool));}
| Not E  {$$ = create_noeud($2,NULL,"Not",Not,creer_type(0,T_bool));}
| PO E PF {$$ = $2;}
| I {$$ = create_noeud(NULL,NULL,$1,I,creer_type(0,T_int));} // Hamza: J'ai remplacé le 4ème argument (T_int) par I. Sinon, le compilateur ne marche pas
| V {ENV pos = rech2($1,env_cour->debut,env_global->debut);
   if(pos == NULL){
     yyerror("Variable non déclarée");
     exit(EXIT_FAILURE);
     }
   $$ = create_noeud(NULL,NULL,pos->ID,V,pos->typeno);} // Hamza: J'ai remplacé le 4ème argument (pos->typeno->TYPEF) par V. Sinon, le compilateur ne marche pas

| True {$$ = create_noeud(NULL,NULL,"true",True,creer_type(0,T_bool));}
| False {$$ = create_noeud(NULL,NULL,"false",False,creer_type(0,T_bool));}
| NewAr TP CO E CF /*NewAr TP [ E ] */{$$ = NULL;}

| V PO L_args PF /*V ( L_args )*/{
  LFON pos = rechfon($1,liste_fct->debut);
  if(pos == NULL){
    yyerror("Fonction/Procedure non déclarée");
    exit(EXIT_FAILURE);
  }
  $$ = create_noeud($3,NULL,$1,T_fon,pos->typeno);
 }
| Et {$$ = $1;}
;

Et: V CO E CF /* V [ E ] */{
  ENV pos = rech2($1,env_cour->debut,env_global->debut);
  if(pos == NULL){
     yyerror("Variable non déclarée");
     exit(EXIT_FAILURE);
   }
   Noeud *n = create_noeud(NULL,NULL,pos->ID,V,pos->typeno);
   $$ = create_noeud(n, $3, $1, Ind, n->typeno) ;
 }
  | Et CO E CF /*Et [ E ] */{
    $$ = create_noeud($1, $3, $1->ETIQ, Ind, $1->typeno) ;
  }
  ;

C: C Se C {$$ = create_noeud($1,$3,"Se",Se,creer_type(0,T_com));}

| Et Af E {$$ = create_noeud($1,$3,"Af",Af,creer_type(0,T_com));}

| V Af E {ENV pos = rech2($1,env_cour->debut,env_global->debut);
   if(pos == NULL){
     yyerror("Variable non déclarée");
     exit(EXIT_FAILURE);
   }
   $$ = create_noeud(create_noeud(NULL,NULL,pos->ID,V,pos->typeno),$3,"AF",Af,creer_type(0,T_com));
 }
| Sk {$$ = create_noeud(NULL,NULL,"Sk",Sk,creer_type(0,T_com));}

| AO C AF {$$ = $2;} //{ C }

| If E Th C El C {
  $$ = create_noeud($2, create_noeud($4, $6, "", 0, creer_type(0, T_com)), "IfThEl", If, creer_type(0, T_com));
 }

| Wh E Do C {
  $$ = create_noeud($2,create_noeud($4,NULL,"Do",Do,creer_type(0,T_com)),"Wh",Wh,creer_type(0,T_com));
 }

| V PO L_args PF /*V ( L_args )*/{
  LFON pos = rechfon($1,liste_fct->debut);
  if(pos == NULL){
    yyerror("Fonction/Procedure non déclarée");
    exit(EXIT_FAILURE);
  }
  $$ = create_noeud($3,NULL,$1,T_fon,pos->typeno);
 }
;


L_args: %empty {$$ = NULL;}
| L_argsnn {$$ = $1;}
;

L_argsnn: E {$$ = $1;}
| E Virgule L_argsnn {$$ = create_noeud($1,$3,"Virgule",Virgule,creer_type(0,T_com)); }
;

L_argt: %empty {$$ = NULL;}
| L_argtnn {$$ = $1;}

L_argtnn: Argt {$$ = $1;}
| L_argtnn Virgule Argt {$$ = concat($1,$3);}
;

Argt: V DPoints TP {
  BILENV now = bilenv_vide();
  inbilenv(now,$1,$3);
  $$ = now;
 }
;

TP: T_bool  {$$  = creer_type(0,T_bool);}
| T_int   {$$ = creer_type(0,T_int);}
| T_ar TP {$2->DIM ++;
   $$ = $2;}
;

L_vart: %empty {$$ = NULL;}
| L_vartnn {$$ = $1;env_cour = concat(env_cour,$1);}
;

L_vartnn: Var Argt {$$ = $2;}
| L_vartnn Virgule Var Argt {$$ = concat($1,$4);}
;

D_entp: Dep V PO L_argt PF /*Dep NPro ( L_argt )*/{
  env_global = env_cour;
  env_cour =$4;//$4: env_param, changement env_cour à un env_local
  $$ = creer_bilfon(creer_fon($2,copier_bilenv($4),NULL,NULL,creer_type(0,T_pro)));
  liste_fct = concatfn(liste_fct,$$);
  if(env_cour == NULL)
    env_cour = bilenv_vide();
  inbilenv(env_cour,$2,creer_type(0,T_com));/*Ajout du nom de la procedure dans les vars locals*/
 }
;

D_entf: Def V PO L_argt PF DPoints TP /* Def NFon ( L_argt ) : TP*/{
  env_global = env_cour;
  env_cour = $4;//$4: env_param, changement env_cour à un env_local
  $$ = creer_bilfon(creer_fon($2,copier_bilenv($4),NULL,NULL,$7));
  liste_fct = concatfn(liste_fct,$$);
  if(env_cour== NULL)
    env_cour = bilenv_vide();
  inbilenv(env_cour,$2,$7);/*Ajout du nom de la fonction dans les vars locals*/}
;

D: D_entp L_vart C {
  if($2 == NULL)
    $2 = bilenv_vide();
  inbilenv($2,$1->debut->ID,$1->debut->typeno);
  $1->debut->VARLOC = $2;
  $1->debut->CORPS = $3;
  $$ = $1;
  env_cour = env_global;}

| D_entf L_vart C {
  if($2 == NULL)
    $2 = bilenv_vide();
  inbilenv($2,$1->debut->ID,$1->debut->typeno);
  $1->debut->VARLOC = $2 ;
  $1->debut->CORPS = $3;
  $$ = $1;
  env_cour = env_global;}
;

LD: %empty {$$ = NULL;}
| LD D {$$ = liste_fct;}
;

%%

void yyerror(char *s){
  fprintf(stderr, "***ERROR l : %d  %s\n", yylineno, s);
}

int yywrap(){}

int main(int argc, char **argv){
  env_global = bilenv_vide();
  env_cour = env_global;
  type_base  = creer_type(0,T_bot);
  liste_fct = bilfon_vide();

  /* Compiler Pseudo-Pascal to C3A */
  /*printf("\n L'arbre de syntaxe astraite: \n");
  prefix(syntree);*/
  yyparse();
  /*printf("*****************| JE SUIS LA ! ;) : %p|*****************\n", syntree);*/
  printf("Avant pp2quad\n");
  sem(&env_global, &liste_fct, syntree);
  ecrire_bilenv(env_global);
  BILQUAD bilq = pp2quad(syntree);
  /*printf("Après pp2quad\n");*/
   printf("\n\n");
  printf("\n******* Le code C3A du programme Pseudo-Pascal: *******\n\n");
  ecrire_sep_bilquad(bilq);
  printf("\n");
  /*********************************/

  return EXIT_SUCCESS;
}
