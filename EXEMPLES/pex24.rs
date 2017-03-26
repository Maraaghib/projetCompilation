Les variables globales:
------------------------:
variable X1 DIM:0,TAILLE:0,TYPEF:314valeur 0 
variable X2 DIM:0,TAILLE:0,TYPEF:313valeur 0 
variable AA DIM:2,TAILLE:0,TYPEF:313valeur 0 
fin d'environnement 

Les fonctions:
------------------------:
nfon:foo
 parametres:
variable X3 DIM:0,TAILLE:0,TYPEF:314valeur 0 
variable X4 DIM:2,TAILLE:0,TYPEF:313valeur 0 
fin d'environnement 
var locales:
variable foo DIM:0,TAILLE:0,TYPEF:314valeur 0 
variable T DIM:0,TAILLE:0,TYPEF:313valeur 0 
fin d'environnement 
corps (notation prefixe) :
Se Se Af T + T 1 Af X4 New_array_of  10 Af foo true 
 type resultat:
DIM:0,TAILLE:0,TYPEF:314

fin de liste de fonctions 

Le programme principal:
------------------------:
Se Af X1 Ap foo ArgAp true ArgAp AA Af X2 Not X1 
erreur de typage

TYPERROR: affectation de types incoherents : ligne 17