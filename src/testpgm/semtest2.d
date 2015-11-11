program semtest2;

{ food for semantic analyzer }

var
	a : integer;
	b : integer;
	x : real;
	y : real;

procedure index(i : integer);
begin
end;

function max(a : integer; x : real) : integer;
begin
	a := 0; 
	{ don't RETURN anything }
end;

procedure nasty(i : integer; j : integer; x : real; y : real);
begin
	x := j;
	x := +i div j;
	i := i/j;
	if ((x = i) and not (i < j)) then
		x := max(y, i);
		i := 1.2;
	end;
	RETURN i;
end;

begin
	index(a, 2);
	index();
	nasty(5, 1/2, 1 div 2, 8.0+7);
end.

