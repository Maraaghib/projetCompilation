
%{
#include "include/bilquad.h"
#include "include/environ.h"
#include "ppascalbison.h"
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
                            } else if (strncmp("Ind",yytext+1,2)==0) { 
                                op = NewAr;   
                            } else if (strncmp("Call",yytext+1,2)==0) { 
                                op = Call;   
                            } else if (strncmp("Ret",yytext+1,2)==0) { 
                                op = Ret;   
                            } else if (strncmp("AfInd",yytext+1,2)==0) { 
                                op = AfInd;   
                            } else if (strncmp("Param",yytext+1,2)==0) { 
                                op = Param;   
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
    ENV env= Envalloc(); /* env local*/
    ENV G ; /* env global*/
    ENV L, P;
    QUAD elem = list.debut;
    int fin =0;
    int local =0;
    QUAD retaddr;//"adresse" de retour : quadruplet qui suit un call
    char* retvar; //variable qui stocke la valeur de retour
    while (elem != NULL) {
        if (elem->OP == Afc) {
	    //if(local)
            	initenv(&env, elem->RES,creer_type(0,0));
            affect(env, elem->RES, atoi(elem->ARG1));
            elem = elem->SUIV;
        }            
        /*Pl Mu Mo Not And Or Lt Eq creer la var res dans e (si elle existe elle ne sera pas créée)
					ensuite recuperer les valeur des arguments pour faire les opérations dessus*/
        else if (elem->OP == Pl || elem->OP == Mu || elem->OP == Mo || elem->OP == Not || elem->OP == And || elem->OP == Or || elem->OP == Lt || elem->OP == Eq) {
            initenv(&env, elem->RES,creer_type(0,0));
            initenv(&env, elem->ARG1,creer_type(0,0));
            initenv(&env, elem->ARG2,creer_type(0,0));
            int res = eval(elem->OP, valch(env, elem->ARG1), valch(env, elem->ARG2));
            affect(env, elem->RES, res);
            elem = elem->SUIV;
        }            
        /*recuperer l'environnement associé et affecté la valeur*/
        else if (elem->OP == Af) {
	  //if(local){ 	
            initenv(&env, elem->ARG1,creer_type(0,0));
            initenv(&env, elem->ARG2,creer_type(0,0));
          /*}*/  affect(env, elem->ARG1, valch(env, elem->ARG2));
            elem = elem->SUIV;
        }            
        else if (elem->OP == Jp) {
            QUAD listStart = list.debut;
            char *dest = elem->RES;
            while (strcmp(listStart->ETIQ,dest) != 0)
                listStart = listStart->SUIV;
            elem=listStart;
        }            
        else if (elem->OP == Jz) {
            QUAD listStart = list.debut;
            char *dest = elem->RES;
            if(valch(env, elem->ARG1) == 0){
                while (strcmp(listStart->ETIQ,dest) != 0){
                    listStart = listStart->SUIV;
                }
                elem=listStart;
            }else{
                elem = elem->SUIV;
            }
        } else if (elem->OP == St){
            break;
        } else if (elem->OP == Sk){
            elem = elem->SUIV;
        } else if (elem->OP == Ind){
			affect(env,elem->RES,valchTab(env,elem->ARG1,atoi(elem->ARG2)));
			elem = elem->SUIV;
            break;
        }
        else if (elem->OP == AfInd){
            affectTab(env,elem->ARG1,valch(env,elem->RES),atoi(elem->ARG2));
            elem = elem->SUIV;
            break; 
        }/* else if (elem->OP == Param){
            initenv(&P, elem->ARG1, creer_type(0,0));
            affect(P, elem->ARG1, valch(G, elem->ARG2));
            elem = elem->SUIV;
            break;
         } else if (elem->OP == Call){
		envcpy(*P, L);
                destructEnv(&P);
                local = 1;
                retvar = cur->RES;
                retaddr = cur->SUIV;
                next = rechbq(cur->ARG1, bilist);
            break;
        }  */else if (elem->OP == Ret){
		     printf("break\n");
		     break;
        }
    }

    ecrire_env(env);


}

