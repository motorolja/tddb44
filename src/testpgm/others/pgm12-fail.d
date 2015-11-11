program demo;

var
    a : integer;
    b : integer;

begin
    a := 10;
    b := 2;
    if (a = 3) then
    	a := a + b
    elsif (a = 2) then
    	a := a - b
    elsif (a = 1) then
    	a := a * b
    else
    	a := a / b
    end
end.

