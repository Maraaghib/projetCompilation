/* pex25.pp */

var X1 : boolean,
var X2 : integer,
var AA : array of array of integer

defun foo(X3 : boolean, X4 : array of array of integer): boolean
      var T : integer, var B : boolean
      T := T + 1;
      X4 := new array of array of integer [10];
      T := T * T;
      foo := (true or false);
      B := foo(true,AA);
      T := 2
      
defun foo2():integer
      foo2 := 1
      
defpro proc(Y: integer, Z : boolean)
      var T : integer
      Y := T + 1;
      proc(2,true);
      X2 := Y

X2 := foo2();
AA := new array of integer [X2];
X1 := not X1;
X2 := 10
