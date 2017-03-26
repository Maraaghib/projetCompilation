var X1 : integer,
var X2 : integer,
var X3 : boolean,
var X4 : boolean,
var X5 : array of boolean,
var X6 : array of integer

X2 := 5;
X5 := new array of boolean [6];
X5[0] := true;
X5[1] := false;
X5[2] := true;
X5[3] := false;
X5[4] := true;
X5[5] := false;
X3 := X5[0] or X5[1];
X4 := not ( X5[0] and  X5[1]);
X6 := new array of integer [4];
X6[0] := 10;
X6[1] := 11;
X6[2] := 12;
X6[3] := 13
