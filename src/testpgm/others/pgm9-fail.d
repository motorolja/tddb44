program test;
var a : integer;
    b : real;

begin
    a := 3.0;   { coercion into integer }
    b := 1;
    a := a * b;
    b := b / a;
end.
