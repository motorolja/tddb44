program multi;

var a : integer;
    b : integer;

procedure p1(i : integer);
var v1 : integer;
begin
    v1 := v1 + i;
end;

procedure p2(i : integer);
var v2 : integer;
begin
    v2 := v2 + i;
end;

procedure p3(i : integer);
var v3 : integer;

    procedure p4(i : integer);
    var v4 : integer;
    begin
    	v4 := v4 + i;
    end;

begin
    v3 := v3 + i;
end;

begin { main program }
    a := a + b;
end.

