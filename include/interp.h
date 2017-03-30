#ifndef __INTERP__H__
#define __INTERP__H__

typedef struct TAS{
  int taille;
  struct TAS* suiv;
} *TAS;

typedef struct ADR{
  int adresse;
  struct ADR* suiv;
} *ADR;

typedef struct TAL{
  int taille;
  struct TAL* suiv;
} *TAL;

TAS TAS_alloc();
ADR ADR_alloc();
TAL TAL_alloc();

int init_tas(TAS * tas);
int init_adr(ADR *adr);
int init_tal(TAL *tal);
int init_pile(TAS *tas, ADR * adr, TAl *tal);

#endif
