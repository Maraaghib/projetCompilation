var X1 : integer,
var X2 : integer

defpro proo(X : integer, Y : boolean)
    var Z : boolean
    X1:= X+Y
    
defun foo(X : integer, Y : boolean):integer
    var T : integer
    T := 20;
    if Y
       then foo := X + T
       else foo := X - T

X2 := 10;
if true
   then proo(X2,true)
   else X1 := foo(X2,false)

/* test d' erreur de type */
/* detete a la ligne 7 = lookahead de la reduction de X + T */