program test;

const
    ASCII0 = 48;
var
    a : array[10] of integer;

begin
    a[1] := 0;
    a[a[1]] := ASCII0;
    write(a[a[1]]);
    write(10);
end.

