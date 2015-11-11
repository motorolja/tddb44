program bar;
const
	BOSSE = 2;
var
	y : integer;

function foo(x : integer) : integer; 
begin 
	return (-BOSSE)*x;
end; 

begin
	y := foo(2);
end.
