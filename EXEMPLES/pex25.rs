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
variable B DIM:0,TAILLE:0,TYPEF:314valeur 0 
fin d'environnement 
corps (notation prefixe) :
Se Se Se Se Se Af T + T 1 Af X4 New_array_of  10 Af T * T T Af foo Or true false Af B Ap foo ArgAp true ArgAp AA Af T 2 
 type resultat:
DIM:0,TAILLE:0,TYPEF:314

nfon:foo2
 parametres:
fin d'environnement 
var locales:
variable foo2 DIM:0,TAILLE:0,TYPEF:313valeur 0 
fin d'environnement 
corps (notation prefixe) :
Af foo2 1 
 type resultat:
DIM:0,TAILLE:0,TYPEF:313

nfon:proc
 parametres:
variable Y DIM:0,TAILLE:0,TYPEF:313valeur 0 
variable Z DIM:0,TAILLE:0,TYPEF:314valeur 0 
fin d'environnement 
var locales:
variable T DIM:0,TAILLE:0,TYPEF:313valeur 0 
fin d'environnement 
corps (notation prefixe) :
Se Se Af Y + T 1 Ap proc ArgAp 2 ArgAp true Af X2 Y 
 type resultat:
DIM:0,TAILLE:0,TYPEF:269

fin de liste de fonctions 

Le programme principal:
------------------------:
Se Se Se Af X2 Ap foo2 Af AA New_array_of  X2 Af X1 Not X1 Af X2 10 
erreur de typage

TYPERROR: affectation de types incoherents : ligne 26 