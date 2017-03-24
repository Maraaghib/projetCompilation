/* environ.c */
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "environ.h"
#include "../ppascalbison.h"
/*---------------------allocation memoire----------------------------*/
char *Idalloc()
{
  return((char *)malloc(MAXIDENT*sizeof(char)));
}
ENV Envalloc()
{
  ENV e = (ENV)malloc(sizeof(struct cellenv));
  e->ID = Idalloc();
  e->VAL = 0;
  e->SUIV = NULL;
  return e;
}

/*-------------------------------------------------------------------*/
/*-----------------------------environnements------------------------*/

/* initialise l'environnement *prho par  var=0    */
/* la chaine var est copiee dans l' environnement */
int initenv(ENV *prho,char *var, TYPE type)
{ENV pos, newcell;
  pos=rech(var,*prho);/* adresse de la cellule contenant var */
  if (pos == NULL)
    /*on insere var en fin de envrnt*/
    { newcell=Envalloc();
      strcpy(newcell->ID,var);
      ENV curseur = *prho;
      newcell->type = type;
      while (curseur->SUIV != NULL)
	curseur = curseur->SUIV;
      curseur->SUIV = newcell;
      return (EXIT_SUCCESS);
    }
  else
    {
      return(EXIT_SUCCESS);
    }
}
/* retourne (arg1 op arg2) */
int eval(int op, int arg1, int arg2)
{switch(op)
    {case Pl:
	return(arg1 + arg2);
    case Mo:
      return(arg1 - arg2);
    case Mu:
      return(arg1 * arg2);
    default:
      return(0);
    }
  return(0);
}
 
/* retourne l'adresse de la cellule contenant chaine. NULL si la chaine est absente */
ENV rech(char *chaine, ENV listident)
{if (listident!=NULL)
    {if (strcmp(listident->ID,chaine)==0)
        {return listident;}
      else
	return rech(chaine,listident->SUIV);
    }
  else
    return NULL;
}

/* affecte val a la variable var , dans rho */
int affect(ENV rho, char *var, int val)
{ENV pos;
  pos=rech(var,rho);/* adresse de la cellule contenant var */
  if (pos != NULL)
    {(pos->VAL)=val;
      return(EXIT_SUCCESS);
    }
  else
    return(EXIT_FAILURE);
}

/* affiche l'environnement */  
int ecrire_env(ENV rho)
{ if (rho==NULL)
    {printf("fin d' environnement \n");
      return(EXIT_SUCCESS);}
  else
    {printf("variable %s valeur %d \n",rho->ID,rho->VAL);
      ecrire_env(rho->SUIV); 
      return(EXIT_SUCCESS);
    };
}

/* valeur de var dans rho */
int valch(ENV rho, char *var)
{ENV pos;
  pos=rech(var,rho);/* adresse de la cellule contenant var */
  if (pos != NULL)
    return(pos->VAL);
  else
    return(0);
}


