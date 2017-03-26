var TAB1 : array of integer,
var TAL1 : integer,
var min : integer

defun MIN(X : array of integer, indmax : integer): integer
var n : integer,
var valmin : integer

n := 0; valmin := X[0];
n := n+1;
while n < indmax do
    { if X[n] < valmin
         then valmin := X[n]
         else skip;
       n := n+1
    };
MIN := valmin
    

TAL1 := 3;
TAB1 := new array of integer [TAL1];
min := 0;
while min < TAL1 do
    {TAB1[min] := (min - 5) * (min - 5);
     min := min +1
    };
min := MIN(TAB1,TAL1)

