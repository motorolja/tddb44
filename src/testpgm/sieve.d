program prime;

const
    SIZE = 8191;
    FALSE = 0;
    TRUE = 1;
    
var
    flags : array[SIZE] of integer;
    i : integer;
    prime : integer;
    k : integer;
    count : integer;
    iter : integer;

#include "stdio.d"
    
begin
    iter := 0;
    while (iter < 10) do 
    	count := 0;
	i := 0;
	while (i < SIZE) do
	    flags[i] := TRUE;
	    i := i + 1;
	end;
	i := 0;
	while (i < SIZE) do
	    if (flags[i]) then
	    	prime := i + i + 3;
		write_int(prime);
		newline();
		k := i + prime;
		while (k < size) do
		    flags[k] := FALSE;
		    k := k + prime;
		end;
		count := count + 1;
	    end;
	    i := i + 1;
	end;
    	iter := iter + 1;
    end;
end.
