

%{
#include "bilquad.h"
#include "environ.h"
#include "../ppascalbison.h"
#include <string.h>
char *strdup(const char *s);

BILQUAD list;
char *etiq=NULL,*arg1=NULL,*arg2=NULL,*res=NULL;
int op = 0;
int first = 1;
char *copy(char *src){
    int len = strlen(src);
    char *dst = malloc(sizeof(int)*(len+1));
    return strcpy(dst,src);
}
int fileno(); 
%}
%option nounput noinput
%s Op Arg1 Arg2 Dest
%%

(JMP|ET)[0-9]+                {
                            BEGIN Op; free(etiq); etiq=copy(yytext); 
                        }
<Op>:[A-Za-z]+         {
                            BEGIN Arg1;
                            if(strncmp("Pl",yytext+1,2)==0) {
                                op = Pl;
                            } else if (strncmp("Mo",yytext+1,2)==0) { 
                                op = Mo;   
                            } else if (strncmp("Mu",yytext+1,2)==0) { 
                                op = Mu;   
                            } else if (strncmp("Afc",yytext+1,3)==0) { 
                                op = Afc;   
                            } else if (strncmp("Af",yytext+1,2)==0) { 
                                op = Af;   
                            } else if (strncmp("Sk",yytext+1,2)==0) { 
                                op = Sk;   
                            } else if (strncmp("Jp",yytext+1,2)==0) { 
                                op = Jp;   
                            } else if (strncmp("Jz",yytext+1,2)==0) { 
                                op = Jz;   
                            } else if (strncmp("St",yytext+1,2)==0) { 
                                op = St;   
                            }              
                        }
<Arg1>:[A-Za-z0-9]*      {
                            BEGIN Arg2;
                            free(arg1); 
                            arg1=copy(yytext+1);
                        }
<Arg2>:[A-Za-z0-9]*        {
                            BEGIN Dest;
                            free(arg2); 
                            arg2=copy(yytext+1);
                        }
<Dest>:[A-Za-z0-9]*        {
                            BEGIN INITIAL;
                            char *dst=NULL;
                            dst = copy(yytext+1);
                            if(first) {
                                first = 0;
                                list = creer_bilquad(creer_quad(etiq,op,arg1,arg2,dst));
                            } else {
                                BILQUAD tmp = creer_bilquad(creer_quad(etiq,op,arg1,arg2,dst));
                                list = concatq(list,tmp);
                                
                            }
                            free(dst);
                        }
[ \t\n]                 ;
. {;}
%%
int yywrap(){
return -1;}

int main(int argc, char *argv[]) {
    yylex();
    BILENV benv = bilenv_vide();
    ENV env = Envalloc();
    QUAD elem = list.debut;
    char* dest;QUAD curseur;
    while (elem != NULL) {
		switch(elem->OP){
			case Afc  :
				initenv(&env,elem->RES,env->typeno);
				affect(env, elem->RES, atoi(elem->ARG1));
				elem = elem->SUIV;
				break;         
			case Mu :
			case Mo :
			case Pl :
			case And :
			case Or :
			case Lt :
			case Eq :
			case Not:
				 /*creer la var res dans e (si elle existe elle ne sera pas créée)
					ensuite recuperer les valeur des arguments pour faire les opérations dessus*/	
				initenv(&env,elem->RES,env->typeno);
				initenv(&env,elem->ARG1,env->typeno);
				initenv(&env,elem->ARG2,env->typeno);
				int res = eval(elem->OP, valch(env, elem->ARG1), valch(env, elem->ARG2));
				affect(env, elem->RES, res);
				elem = elem->SUIV;
				break;            
			case Af : 
				/*recuperer l'environnement associé et affecté la valeur*/
				initenv(&env,elem->ARG1,env->typeno);
				initenv(&env,elem->ARG2,env->typeno);
				affect(env, elem->ARG1, valch(env, elem->ARG2));
				elem = elem->SUIV;
				break;          
			case Jp : 
				elem=rechbq(elem->RES, list);
				break;            
			case Jz :
				curseur = list.debut;
				dest = strdup(elem->RES);
				if(valch(env, elem->ARG1) == 0){
					while (strcmp(curseur->ETIQ,dest) != 0){
						curseur = curseur->SUIV;
					}
					elem=curseur;
				}else{
					elem = elem->SUIV;
				}
				break;
			case St :
				break;
			case Sk :		
				elem = elem->SUIV;
				break;
				
			case NewAr :
			
			case T_ar :
			
			case T_bool :
			
			case T_int :
			
			case Def :
			
				
			default :
			        break;
			}
		}
    ecrire_bilenv(benv);
	ecrire_env(env);

}
