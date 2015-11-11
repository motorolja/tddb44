program foo;
var
	i : integer;
        a : integer;
	r : real;
        v : real;
begin
    i := 3 * 1;
    a := i + 1;
    r := 15.0 / 2.0;
    v := r + 1;
    if(2 - 3 < 4) then
	i := a + 5;
    end; 
    if(5.0 - 4.0 > -1) then
	v := v + 1;
    end;
end.
