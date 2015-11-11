program test;
var
    k : integer;

    procedure p(i : integer);
    var
    	j : integer;
	l : integer;

    	procedure r(o : integer);
	begin
	    l := o;
	end;

    	procedure q(m : integer);
	var
	    n : integer;
	begin
	    r(51);
	    n := 53;
	    write(i);
	    write(j);
	    write(k);
	    write(l);
	    write(m);
	    write(n);
	end;

    begin
    	j := 49;
	q(52);
    end;
    
begin
    k := 50;
    p(48);
end.

{ i = 48, j = 49, k = 50, l = 51, m = 52, n = 53 }
