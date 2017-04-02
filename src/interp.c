 #include <stdio.h> 
 #include <stdlib.h>
 #include <string.h>
 #include "include/util.h"
 #include "ppascalbison.h"

 char *strdup(const char *s);

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



 int traitement(ENV *e,Noeud *n){
	ENV pos; 
 	if (!n) return 0;
 	switch(n->tokentype){
 		case I:
			return atoi(n->data);
		case V:
			{ pos=rech(n->data,e->debut);
			return (pos->VAL);
			}
		case If:
			traitement(e,n->gauche);
                        traitement(e,n->droit);
 			return 0;
		case El:
			if(traitement(e,n->gauche)){
 				traitement(e,n->droit);
				return 0;
			}
		case Th:
			if(!traitement(e,n->gauche))
 				traitement(e,n->droit);
			return 0;
		case Wh:
			if(traitement(e,n->gauche)){
				traitement(e,n->droit);
				traitement(e,n);
				}
		case Af:
			initenv(e,n->gauche->data);
 			return affect(*e, n->gauche->data, traitement(e,n->droit));
 		case Se:
 			traitement(e,n->gauche);
 			return traitement(e,n->droit);
 		case Pl:
 		case Mo:
 		case Mu:
			return eval(n->type, traitement(e, n->gauche), traitement(e, n->droit));
		case And:
			return(traitement(e, n->gauche) & traitement(e, n->droit));
		case Or:
			return(traitement(e, n->gauche) | traitement(e, n->droit));
		case not:
			return(!traitement(e,n->gauche));
		case lt:
			if(traitement(e, n->gauche) < traitement(e, n->droit))
				return 1;
			return 0;
		case eq:
			if(traitement(e, n->gauche) == traitement(e, n->droit))
				return 1;
			return 0;
		case NewAr:	
			return 0;
		case T_int:
			return atoi(n->data);
		case T_bool:
				return atoi(n->data);
		case T_array:
					return 0;
			
		case Def:
			creer_bilfon(creer_fon(n->data,
			;
		
     }
     return 0;
 }
 
 int start(ENV *e,noeud* n){
     int res = traitement(e,n);
     ecrire_env(*e);
     libererArbre(n);
     return res;
 }

 
