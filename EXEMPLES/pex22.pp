var X1 : integer,
var X2 : integer

defpro proo(X : integer, Y : integer)
    var Z : boolean
    X1:= X+Y
    
defun foo(X : integer, Y : boolean):integer
    var T : boolean
    T := true;
    if Y
       then foo := X + T
       else foo := X - T

X2 := 10;
if true
   then proo(X2,2)
   else X1 := foo(X2,false)

/* test d' erreur de type */
/* detecteea  aux lignes 13,15 = lookahead de leur reduction */