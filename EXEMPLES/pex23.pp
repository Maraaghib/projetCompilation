/* pex23.pp: test de detection d'erreurs semantiques */

var X1 : integer,
var X2 : integer,
var X3 : boolean,
var X4 : array of array of integer

X1 := X1;
X2 := (X2+X3)+X1;
/* Se Se Se Af X1 Ind X4 X1 Af X1 + X1 X2 Af X2 + X1 X3 Af X1 Ind Ind X4 + X1 1 X2 */
X1 := X4 [X1];
X2 := X4[X4] + X3;
X1 := X4 [X1+1][X2]; 
X2:= X2 + true
