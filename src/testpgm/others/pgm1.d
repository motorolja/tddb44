program foo;
var
    x : integer;
    y : real;

procedure p(i : integer);
begin
    i := i + 1;
end;

begin
    p(x);
    y := y + x;
end.

