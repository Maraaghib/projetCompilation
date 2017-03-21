%{
#include <stdio.h>
#include <stdlib.h>
#include "ppascal.h"
%}

%s Def_Function Def_Procedure

CST [1-9][0-9]*|0
VAR [a-zA-Z]+[a-zA-Z0-9]

%%

<INITIAL>{
{CST} {return I;}


"(" {return PO;}
")" {return PF;}

"var" {return Var;}


":" {return DPoints;}
":=" {return Af;}
"true" {return True;}
"false" {return False;}
";" {return Se;}
"if" {return If;}
"then" {return Th;}
"else" {return El;}
"while" {return Wh;}
"do" {return Do;}
"skip" {return Sk;}

"+" {return Pl;}
"-" {return Mo;}
"*" {return Mu;}
"and" {return And;}
"or" {return Or;}
"not" { return Not;}
"<=" {return Lt;}
"=" {return Eq;}


"new array of" {return NewAr;}
"array of" {return T_ar;}
"boolean" {return T_bool;}
"integer" {return T_int;}

"defun" {return Def;}
"defpro" {return Dep;}

{VAR} {return NFon;}
}

[.\t\n] {;} //ignore
%%


//Faire le NFon et le NPro (a gérer avec les états)
