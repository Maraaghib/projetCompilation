/* pex24.pp */

var X1 : boolean,
var X2 : integer,
var AA : array of array of integer

defun foo(X3 : boolean, X4 : array of array of integer): boolean
      var T : integer
      T := T+1;
      X4 := new array of array of integer [10];
      foo := true 

X1 := foo(true,AA);
X2 := not X1

/* type de foo incompatible avec +  */
/* teste: appels de foo sur types incorrects, sur nombre d'args <> 2 */