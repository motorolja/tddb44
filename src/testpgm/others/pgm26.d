program floating_test;

var
    x : real;
    y : real;

#include "../stdio.d"

begin
    x := read_real();
    y := read_real();
    write_real(x);
    newline();
    write_real(y);
    newline();
end.
