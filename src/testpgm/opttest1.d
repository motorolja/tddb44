program opttest1;
const
	FOO = 2;
var
	i : integer;
	a : integer;
	p : real;
	q : real;
begin
	i := FOO + 2;
	i := 3 * 1;
	a := i + 1;
	if(2 - 3 < 4) then
		i := a + 5;
	end;
	if(5 AND 2) then
		p := 15.0 / 2;
		q := 5.6 + 1.0;
	end;
end.
