program test;

var c : real;
    i : integer;

function calc(a : real; b : real) : real;
begin
    return a + b;
end;


begin
    { test that the float stack is cleaned up correctly }
    i := 100;
    while (i > 0) do
        c := calc(1.0, 2.0);
        i := i - 1;
    end;
end.