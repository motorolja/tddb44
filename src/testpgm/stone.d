program stone;

#include "stdio.d"

procedure down(n : integer);
begin
    if n = 0 then
	return
    else
	down(n-1);
	write_int(n);
	newline();
    end;
end;

begin
    down(10);
end.

