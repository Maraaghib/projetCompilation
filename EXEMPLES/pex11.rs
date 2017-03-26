Les variables globales:
------------------------:
variable X1 DIM:0,TAILLE:0,TYPEF:313valeur 0 
variable X2 DIM:0,TAILLE:0,TYPEF:313valeur 0 
fin d'environnement 

Les fonctions:
------------------------:
nfon:foo1
 parametres:
variable X DIM:0,TAILLE:0,TYPEF:313valeur 0 
variable Y DIM:0,TAILLE:0,TYPEF:314valeur 0 
fin d'environnement 
var locales:
variable foo1 DIM:0,TAILLE:0,TYPEF:313valeur 0 
variable Z DIM:0,TAILLE:0,TYPEF:313valeur 0 
fin d'environnement 
corps (notation prefixe) :
Af foo1 + X 1 
 type resultat:
DIM:0,TAILLE:0,TYPEF:313

nfon:foo2
 parametres:
variable X DIM:0,TAILLE:0,TYPEF:313valeur 0 
fin d'environnement 
var locales:
variable foo2 DIM:0,TAILLE:0,TYPEF:313valeur 0 
variable T DIM:0,TAILLE:0,TYPEF:313valeur 0 
fin d'environnement 
corps (notation prefixe) :
Af foo2 + X T 
 type resultat:
DIM:0,TAILLE:0,TYPEF:313

fin de liste de fonctions 

Le programme principal:
------------------------:
IfThEl true  Af X1 Ap foo1 ArgAp X1 ArgAp true Af X1 Ap foo2 ArgAp X2 ArgAp false 
erreur de typage

TYPERROR: appel de fonction, trop d'arguments : ligne 14