trouve X en position 0x206fa90 
analyse lex, token I,l1 
analyse lex,token I,l2 
analyse lex, ETIQ :1 
trouve X1 en position 0x206f1e0 
analyse lex, token I,l1 
analyse lex,token I,l2 
analyse lex, ETIQ :20 
trouve T en position 0x2078ad0 
trouve Y en position 0x2078c10 
trouve X en position 0x2078df0 
trouve T en position 0x20790c0 
trouve foo en position 0x2079430 
trouve X en position 0x20795c0 
trouve T en position 0x20798e0 
trouve foo en position 0x2079c50 
analyse lex, token I,l1 
analyse lex,token I,l2 
analyse lex, ETIQ :10 
trouve X2 en position 0x206f230 
trouve X2 en position 0x206f230 
trouve proo en position 0x206f550 
trouve X2 en position 0x206f230 
trouve X1 en position 0x206f1e0 
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
variable Y DIM:0,TAILLE:0,TYPEF:314valeur 0 
fin d'environnement 
var locales:
variable Z DIM:0,TAILLE:0,TYPEF:313valeur 0 
variable U DIM:0,TAILLE:0,TYPEF:313valeur 0 
fin d'environnement 
corps (notation prefixe) :
Af X1 + X 1 
 type resultat:
DIM:0,TAILLE:0,TYPEF:269

nfon:foo
 parametres:
variable X DIM:0,TAILLE:0,TYPEF:313valeur 0 
variable Y DIM:0,TAILLE:0,TYPEF:314valeur 0 
fin d'environnement 
var locales:
variable foo DIM:0,TAILLE:0,TYPEF:313valeur 0 
variable T DIM:0,TAILLE:0,TYPEF:313valeur 0 
variable U DIM:0,TAILLE:0,TYPEF:313valeur 0 
fin d'environnement 
corps (notation prefixe) :
Se Af T 20 IfThEl Y  Af foo + X T Af foo - X T 
 type resultat:
DIM:0,TAILLE:0,TYPEF:313

fin de liste de fonctions 

Le programme principal:
------------------------:
Se Af X2 10 IfThEl true  Ap proo ArgAp X2 ArgAp true Af X1 Ap fo1 ArgAp X2 ArgAp false 
erreur de typage