program bar;
const
	FOOB = 2;
	BAR = FOOB;
	BOSSE = 'fooooo';
	LASSE = 'f''o';
	NISSE = 'a''b''c''d''e';
	LENTE = 'a  b';
var
	x : integer;
	y : integer;

function foo(x : integer) : integer;
var
	y : integer;
begin 
	if(x < 0) then
		return -2*x;
	else
		return 2*x;
	end;
	y := 1;
end; 

begin
	y := 2;
	x := foo(2);
end.
