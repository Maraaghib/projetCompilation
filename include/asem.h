#ifndef ANASEM_H
#define ANASEM_H
#include "environ.h"
#include "util.h"

/*------------------FONCTIONS ---------------------------------------------------*/
/*------------------types--------------------------------------------------------*/

/* envoie message s d'erreur de type en ligne ligne                              */
extern void typ_error(char *s, int ligne);
/*renvoie le type de e*/
extern type *calcul_type(BILENV rho_gb, Noeud *noeud, int ligne);
#endif
