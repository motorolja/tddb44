program test;

var r : real;
    i : integer;

#include "stdio.d"

begin
    { 2^42 }
    i := 4398046511104;

    write_int((i div 2) * 2);
    newline();

    write_int(i + i);
    newline();

    write_int((i + 1) mod 2);
    newline();

    r := i;
    write_real(i + 0.12345);
    newline();
end.
