#ifndef __INTERP__H__
#define __INTERP__H__
#include "util.h"
char * strdup(char *);
extern void sem(BILENV, BILENV, Noeud *);
extern int semval(BILENV, BILENV, Noeud *);
int op_binaire(char *);
int op_unaire(char *);

#endif
