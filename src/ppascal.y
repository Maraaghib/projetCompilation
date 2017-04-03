%{
  #include <stdio.h>
  #include <stdlib.h>
  #include "util.h"
  #include "environ.h"
  int yywrap();
  int yylex();
  void yyerror();
  extern int yylineno;

  BILENV env_cour, env_global;
  //par defaut env_cur correspond a env_global

  char* fct_cour;
  BILENV param_cour;
  int type_fct; // vaut 0 pour les procedures 
  
  %}

%error-verbose

%union{
  struct Noeud * Noeud;
  int type_var;
  char* terminal;
  struct bilenv* b_env;
  struct bilfon* bf;
 }

%token T_bool T_int T_ar  //type
%token NFon NPro //identificateur
%token NewAr //new
%token Def Dep Af Se If Th El Wh Do Pl Mo Mu And Or Not Lt Eq Sk //operateur
%token True False Var
%token<terminal> I V //token terminaux
%token PO PF AO AF CO CF Virgule DPoints //parenthése accolade crochet deux points

%left Virgule
%left Af
%left If Th El Wh Do
%left Mo Mu Or Lt Eq And Not Pl
%left Se
%right PO AO CO

%type<Noeud> E Et C 
%type<b_env> L_argsnn L_args L_vartnn L_vart Argt L_argt L_argtnn
%type<bf> LD D D_entf D_entp
%type<type_var> TP

%start MP
%%
MP: L_vart LD C {
  env_global = $1;
  //ecrire_bilenv($1);
  //print_tree_ter($3); printf("\n");
   ecrire_prog($1,$2,$3);
 }

E: E Pl E {$$ = create_noeud($1,$3,"Pl",0,op);}
| E Mo E {$$ = create_noeud($1,$3,"Mo",0,op);}
| E Mu E {$$ = create_noeud($1,$3,"Mu",0,op);}
| E Or E {$$ = create_noeud($1,$3,"Or",0,op);}
| E Lt E {$$ = create_noeud($1,$3,"Lt",0,op);}
| E Eq E {$$ = create_noeud($1,$3,"Eq",0,op);}
| E And E {$$ = create_noeud($1,$3,"And",0,op);}
| Not E {}
| PO E PF {}
| I {$$ = create_noeud(NULL,NULL,$1,T_int,constante);}
| V {ENV pos = rech2($1,env_cour->debut,env_global->debut);
   if(pos == NULL){
     yyerror("Variable non déclarée");
     exit(EXIT_FAILURE);
   }
   $$ = create_noeud(NULL,NULL,pos->ID,pos->type,variable);}
| True {$$ = create_noeud(NULL,NULL,"true",True,constante);}
| False {$$ = create_noeud(NULL,NULL,"false",False,constante);}
| V PO L_args PF /* V ( L_args ) */ {}
| NewAr TP CO E CF /*NewAr TP [ E ] */{}
| Et {}
;

Et: V CO E CF /* V [ E ] */{}
| Et CO E CF /*Et [ E ] */{}
;

C: C Se C {$$ = create_noeud($1,$3,"Se",0,op);printf("lol\n");printf("%s", $$->data);}
| Et Af E {$$ = create_noeud($1,$3,"Af",0,op);printf("lol\n");printf("%s", $$->data);}
| V Af E {ENV pos = rech2($1,env_cour->debut,env_global->debut);
   if(pos == NULL){
     yyerror("Variable non déclaré");
     exit(EXIT_FAILURE);
   }
   $$ = create_noeud(create_noeud(NULL,NULL,pos->ID,pos->type,variable),$3,"AF",0,op);
   printf("lol\n");printf("%s", $$->data);
 }
| Sk {printf("Sk\n");}
| AO C AF {} //{ C }
| If E Th C El C {create_noeud($2,create_noeud($4,create_noeud($6,NULL,"El",0,op),"Th",0,op),"If",0,op);}
| Wh E Do C {}
| V PO L_args PF /*V ( L_args )*/{}
  ;


L_args: %empty {}
| L_argsnn {}
;

L_argsnn: E {}
| E Virgule L_argsnn {}
  ;

L_argt: %empty {$$ = NULL;}
| L_argtnn {$$ = $1;}

L_argtnn: Argt {$$ = $1;}
| L_argtnn Virgule Argt {$$ = concat($1,$3);}
  ;

Argt: V DPoints TP {
   BILENV now = bilenv_vide();
   inbilenv(now,$1,$3);
   //env_cour = concat(env_cour,now);
   $$ = now;
   }
;

TP: T_bool {$$  = T_bool;}
| T_int {$$ = T_int;}
| T_ar TP {}
;

L_vart: %empty {$$ = NULL;}
| L_vartnn {$$ = $1;concat(env_cour,$1);}
;

L_vartnn: Var Argt {$$ = $2;}
| L_vartnn Virgule Var Argt {$$ = concat($1,$4);}
;

D_entp: Dep V PO L_argt PF /*Dep NPro ( L_argt )*/{env_global = env_cour;
   env_cour =$4;//$4: env_param // changement env_cour à un env_local
   $$ = creer_bilfon(creer_fon($2,$4,NULL,NULL,0));
   inbilenv(env_cour,$2,0);/*Ajout du nom de la procedure dans les vars locals*/}
;

D_entf: Def V PO L_argt PF DPoints TP /* Def NFon ( L_argt ) : TP*/{env_global = env_cour;
   env_cour = $4;//$4: env_param // changement env_cour à un env_local
   $$ = creer_bilfon(creer_fon($2,copier_bilenv($4),NULL,NULL,$7));
   inbilenv(env_cour,$2,$7);/*Ajout du nom de la fonction dans les vars locals*/}
;

D: D_entp L_vart C { inbilenv($2,$1->debut->ID,$1->debut->type);
  $1->debut->VARLOC = copier_bilenv($2); $1->debut->CORPS = $3;
  $$ = $1;
  env_cour = env_global;}
| D_entf L_vart C { inbilenv($2,$1->debut->ID,$1->debut->type);
   $1->debut->VARLOC = copier_bilenv($2) ;
   $1->debut->CORPS = $3;
   //printf("%s\n",$3->data);
   $$ = $1;
   env_cour = env_global;}
;

 LD: %empty {$$ = NULL;}
| LD D {$$ = concatfn($1,$2);}
;

%%

void yyerror(char *s){
  fprintf(stderr, "***ERROR l : %d  %s\n", yylineno, s);
}

int yywrap(){}

int main(int argc, char **argv){
  env_global = bilenv_vide();
  env_cour = env_global;
  yyparse();
  return EXIT_SUCCESS;
}
