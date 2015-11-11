program prog;
var
    a : integer;
    b : integer;
    c : integer;

    procedure p1(d : integer);
    var
    	b : integer;
	c : integer;

        procedure p2;
        var
            c : integer;
        begin
            c := b + a;
        end;

    begin
        c := b + a + d;
    end;

begin
    a := b;
    p1(c);
end.
