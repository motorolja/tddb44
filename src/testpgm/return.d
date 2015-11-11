program maxtest;

#include "stdio.d"

function max(a : integer; b : integer) : integer;
begin
    if a > b then
	return a
    else
	return b
    end
end;

begin
    write_int(max(3, 7));
end.
