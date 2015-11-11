program indexed;

var a : array[10] of integer;

begin
    a[1] := 0;
    a[a[1]] := 1;
    write(a[0] + 64);
end.
