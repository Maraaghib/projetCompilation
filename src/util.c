#include <stdio.h>
#include <stdlib.h>

#include "environ.h"
#include "util.h"

NOE Nalloc()
{
  return((NOE)malloc(sizeof(struct noeud)));
}

LFON Lfonalloc()
{
  return (LFON)malloc(sizeof(struct cellfon));
}

void prefix(NOE n)
/* ecrit l'expression n en notation prefixe*/
{ if(n != NULL)
    {printf("%s ",n->ETIQ);
      prefix(n->FG);
      prefix(n->FD);
    };
}

ENV creer_env(char* etiq, int val, TYPE type)
{
  ENV e = Envalloc();

  initenv(&e, etiq, type);
  affect(e,etiq,val);

  return e;
}

ENV copier_env(ENV env){
  ENV copie_env = Envalloc();

  copie_env->ID = env->ID;
  copie_env->type = env->type;
  copie_env->VAL = env->VAL;

  return copie_env;
}

/***** A faire ******/
char* nomop(int codop) {}


//Regarde d'abord dans l'environnement rho_lc (environnement local)
ENV rech2(char *chaine, ENV rho_gb, ENV rho_lc){
  ENV e = rech(chaine,rho_lc);
  if( e != NULL)
    return e;
  else
    return rech(chaine,rho_gb);
}



