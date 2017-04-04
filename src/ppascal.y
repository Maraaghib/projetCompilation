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

  int i =0;

  %}

%error-verbose

%union{
  struct Noeud * Noeud;
  int codop;
  char* terminal;
  //enum type etype;
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

%type<Noeud> E Et C L_argsnn L_argtnn Argt
%type<codop> TP

%start MP
%%
MP: L_vart LD C {ecrire_bilenv(env_cour);
   print_tree_ter($3); printf("\n");}

E: E Pl E {$$ = create_noeud($1,$3,"Pl",0,op);}
| E Mo E {$$ = create_noeud($1,$3,"Mo",0,op);}
| E Mu E {$$ = create_noeud($1,$3,"Mu",0,op);}
| E Or E {$$ = create_noeud($1,$3,"Or",0,op);}
| E Lt E {$$ = create_noeud($1,$3,"Lt",0,op);}
| E Eq E {$$ = create_noeud($1,$3,"Eq",0,op);}
| E And E {$$ = create_noeud($1,$3,"And",0,op);}
| Not E {}
| PO E PF {}
| I {$$ = create_noeud(NULL,NULL,$1,T_int,constante); printf("%s\n",$1);}
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

C: C Se C {$$ = create_noeud($1,$3,"Se",0,op);}
| Et Af E {$$ = create_noeud($1,$3,"Af",0,op);}
| V Af E {ENV pos = rech2($1,env_cour->debut,env_global->debut);
   if(pos == NULL){
     yyerror("Variable non déclaré");
       exit(EXIT_FAILURE);
   }
   $$ = create_noeud(create_noeud(NULL,NULL,pos->ID,pos->type,variable),$3,"AF",0,op);
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

L_argt: %empty {printf("Largt vide\n");}
| L_argtnn {printf("argtnn\n");}

L_argtnn: Argt {printf("argt\n");}
| L_argtnn Virgule Argt {printf("argtnn virgule\n");}
  ;

Argt: V DPoints TP {printf("argt DPoints\n");
   BILENV now = bilenv_vide();
   inbilenv(now,$1,$3);
   env_cour = concat(env_cour,now);
   }
;

TP: T_bool {$$  = T_bool;}
| T_int {$$ = T_int;}
| T_ar TP {}
;

L_vart: %empty {printf("VIDE\n");}
| L_vartnn {printf("NON VIDE\n");}
;

L_vartnn: Var Argt {}
| L_vartnn Virgule Var Argt {}
;

D_entp: Dep V PO L_argt PF /*Dep NPro ( L_argt )*/{}
;

D_entf: Def V PO L_argt PF DPoints TP /* Def NFon ( L_argt ) : TP*/{printf("FUNCTION DEF\n ");}
;

D: D_entp L_vart C {}
| D_entf L_vart C {}
;

LD: %empty {}
| LD D
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
