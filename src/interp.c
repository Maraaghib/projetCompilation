#include <stdio.h>
#include <stdlib.h>
#include "interp.h"

TAS TAS_alloc(){
  return (TAS) malloc(sizeof(struct TAS));
}

ADR ADR_alloc(){
  return (ADR) malloc(sizeof(struct ADR));
}

TAL TAL_alloc(){
  return (TAL) malloc(sizeof(struct TAL));
}

int init_tas(TAS *tas){
  TAS newcell = TAS_alloc();
  if (newcell == NULL)
    return EXIT_FAILURE;
  newcell->taille = 0; // 0 vaut null ici
  newcell->suiv = *tas;
  return EXIT_SUCCESS;
}

int init_adr(ADR *adr){
  ADR newcell = ADR_alloc();
  if (newcell == NULL)
    return EXIT_FAILURE;
  newcell->adresse = 0; // 0 vaut null ici
  newcell->suiv = *tas;
  return EXIT_SUCCESS;
}

int init_tal(TAL *tal){
  TAL newcell = TAL_alloc();
  if (newcell == NULL)
    return EXIT_FAILURE;
  newcell->taille = 0; // 0 vaut null ici
  newcell->suiv = *adr;
  return EXIT_SUCCESS;
}

int init_pile(TAS *tas, ADR *adr, TAL *tal){
  int res = 1;
  res &= init_tas(TAS) ;
  res &= init_adr(adr);
  res &= init_tal(TAL);
  return res;
}
