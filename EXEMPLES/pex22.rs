Les variables globales:
------------------------:
variable X1 DIM:0,TAILLE:0,TYPEF:313valeur 0 
variable X2 DIM:0,TAILLE:0,TYPEF:313valeur 0 
fin d'environnement 

Les fonctions:
------------------------:
nfon:proo
 parametres:
variable X DIM:0,TAILLE:0,TYPEF:313valeur 0 
variable Y DIM:0,TAILLE:0,TYPEF:313valeur 0 
fin d'environnement 
var locales:
variable Z DIM:0,TAILLE:0,TYPEF:314valeur 0 
fin d'environnement 
corps (notation prefixe) :
Af X1 + X Y 
 type resultat:
DIM:0,TAILLE:0,TYPEF:269

nfon:foo
 parametres:
variable X DIM:0,TAILLE:0,TYPEF:313valeur 0 
variable Y DIM:0,TAILLE:0,TYPEF:314valeur 0 
fin d'environnement 
var locales:
variable foo DIM:0,TAILLE:0,TYPEF:313valeur 0 
variable T DIM:0,TAILLE:0,TYPEF:314valeur 0 
fin d'environnement 
corps (notation prefixe) :
Se Af T true IfThEl Y  Af foo + X T Af foo - X T 
 type resultat:
DIM:0,TAILLE:0,TYPEF:313

fin de liste de fonctions 

Le programme principal:
------------------------:
Se Af X2 10 IfThEl true  Ap proo ArgAp X2 ArgAp 2 Af X1 Ap foo ArgAp X2 ArgAp false 
erreur de typage

TYPERROR: op entier sur args non-entiers: ligne 13 
TYPERROR: op entier sur args non-entiers: ligne 15 