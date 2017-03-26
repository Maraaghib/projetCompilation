Les variables globales:
------------------------:
variable TAB1 DIM:1,TAILLE:0,TYPEF:313valeur 0 
variable TAL1 DIM:0,TAILLE:0,TYPEF:313valeur 0 
variable min DIM:0,TAILLE:0,TYPEF:313valeur 0 
fin d'environnement 

Les fonctions:
------------------------:
nfon:MIN
 parametres:
variable X DIM:1,TAILLE:0,TYPEF:313valeur 0 
variable indmax DIM:0,TAILLE:0,TYPEF:313valeur 0 
fin d'environnement 
var locales:
variable MIN DIM:0,TAILLE:0,TYPEF:313valeur 0 
variable n DIM:0,TAILLE:0,TYPEF:313valeur 0 
variable valmin DIM:0,TAILLE:0,TYPEF:313valeur 0 
fin d'environnement 
corps (notation prefixe) :
Se Se Se Se Af n 0 Af valmin Ind X 0 Af n + n 1 Wh Lt n indmax Se IfThEl Lt Ind X n valmin  Af valmin Ind X n skip Af n + n 1 Af MIN valmin 
 type resultat:
DIM:0,TAILLE:0,TYPEF:313

fin de liste de fonctions 

Le programme principal:
------------------------:
Se Se Se Se Af TAL1 3 Af TAB1 New_array_of  TAL1 Af min 0 Wh Lt min TAL1 Se Af Ind TAB1 min * - min 5 - min 5 Af min + min 1 Af min Ap MIN ArgAp TAB1 ArgAp TAL1 
programme bien type