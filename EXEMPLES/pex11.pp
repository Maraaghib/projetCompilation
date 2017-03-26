var X1 : integer,
var X2 : integer

defun foo1(X : integer, Y : boolean):integer
    var Z : integer
    foo1:= X+1
    
defun foo2(X : integer):integer
    var T : integer
    foo2 := X+T

if true
   then X1 := foo1(X1,true)
   else X1 := foo2(X2,false)
