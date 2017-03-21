%{
  #include <stdio.h>
  #include <stdlib.h>

  %}

%token T_bool T_int T_ar  //type
%token NFon NPro //identificateur
%token NewAr //new 
%token Def Dep Af Se If Th El Wh Do Pl Mo Mu And Or Not Lt Eq //operateur
%token True False Var I V //token terminaux 

MP: L_vart LD C
E: E Pl E
| E Mo E {}
| E Mu E {}
| E Or E {}
| E Lt E {}
| E Eq E {}
| E And E {}
| Not E {}
| ’(’ E ’)’ {}
| I {}
| V {}
| True {}
| False {}
| V ’(’ L_args ’)’ {}
| NewAr TP ’[’ E ’]’ {}
| Et {}
;

Et: V ’[’ E ’]’ {}
| Et ’[’ E ’]’ {}
;

C: C Se C {}
| Et Af E {}
| V Af E {}
| Sk {}
| ’{ ’ C ’ }’ {}
| If E Th C El C {}
| Wh E Do C {}
| V ’(’ L_args ’)’ {}
  ;

L_args: %empty {}
| L_argsnn {}
;

L_argsnn: E {}
| E ’,’ L_argsnn {}
  ;

L_argt: %empty {}
| L_argtnn {}
L_argtnn: Argt {}
| L_argtnn ’,’ Argt {}
  ;

Argt: V ’:’ TP {}
;

TP: T_bool {}
| T_int {}
| T_ar TP {}
;

L_vart: %empty {}
| L_vartnn {}
;

L_vartnn: Var Argt {}
| L_vartnn ’,’ Var Argt {}
  ;

D_entp: Dep NPro ’(’ L_argt ’)’ {}
  ;

D_entf: Def NFon ’(’ L_argt ’)’ ’:’ TP {}
;

D: D_entp L_vart C {}
| D_entf L_vart C {}
;

LD: %empty {}
| LD D
;
