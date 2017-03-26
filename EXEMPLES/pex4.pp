var X1 : integer,
var X2 : integer,
var Z: integer,
var X3 : array of boolean,
var X4 : array of array of boolean

X2:= 2;
X4 := new array of array of boolean [3];
X4[0] := new array of boolean [2];
X4[1] := new array of boolean [3];
X4[2] := new array of boolean [4];
X4[2][3]:= 0 - 1;
X3 := X4[X2];
Z := X4[2][3]



