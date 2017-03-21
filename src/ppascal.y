%{
  #include <stdio.h>
  #include <stdlib.h>

  %}

%token T_bool T_int T_ar  //type
%token NFon NPro //identificateur
%token NewAr //new 
%token Def Dep Af Se If Th El Wh Do Pl Mo Mu And Or Not Lt Eq //operateur
%token True False Var I V //token terminaux
%token PO PF AO AF CO CF Virgule DPoints //parenthése accolade crochet deux points 

MP: L_vart LD C
E: E Pl E
| E Mo E {}
| E Mu E {}
| E Or E {}
| E Lt E {}
| E Eq E {}
| E And E {}
| Not E {}
| PO E PF {}
| I {}
| V {}
| True {}
| False {}
| V PO L_args PF {}
| NewAr TP CO E CF {}
| Et {}
;

Et: V CO E CF {}
| Et CO E CF {}
;

C: C Se C {}
| Et Af E {}
| V Af E {}
| Sk {}
| AO C AF {}
| If E Th C El C {}
| Wh E Do C {}
| V PO L_args PF {}
  ;

L_args: %empty {}
| L_argsnn {}
;

L_argsnn: E {}
| E Virgule L_argsnn {}
  ;

L_argt: %empty {}
| L_argtnn {}
L_argtnn: Argt {}
| L_argtnn Virgule Argt {}
  ;

Argt: V DPoints TP {}
;

TP: T_bool {}
| T_int {}
| T_ar TP {}
;

L_vart: %empty {}
| L_vartnn {}
;

L_vartnn: Var Argt {}
| L_vartnn Virgule Var Argt {}
  ;

D_entp: Dep NPro PO L_argt PF {}
  ;

D_entf: Def NFon PO L_argt PF DPoints TP {}
;

D: D_entp L_vart C {}
| D_entf L_vart C {}
;

LD: %empty {}
| LD D
;


int main(int argc, char **argv){
  yyparse();
  return EXIT_SUCCESS;
}
