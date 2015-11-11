program qsort;

const SIZE = 20;

var s : array[SIZE] of integer;

#include "stdio.d"

procedure readsequence;
var i : integer;
begin
    i := 0;
    while (i < SIZE) do
	s[i] := read_int();
	i := i + 1;
    end;
end;

procedure writesequence;
const SPACE = 32;
var i : integer;
begin
    i := 0;
    while (i < SIZE) do
	write_int(s[i]);
	write(SPACE);
	i := i + 1;
    end;
end;

procedure quicksort(l : integer; r : integer);
var
    i : integer;
    j : integer;
    t : integer;
    pivot : integer;
begin
    i := l; 
    j := r;
    pivot := s[(l+r) div 2];
    while not(i > j) do
        while (s[i] < pivot) do
	    i := i + 1;
	end;
        while (pivot < s[j]) do
	    j := j - 1;
	end;
        if (i - j < 1) then
            t := s[i];
	    s[i] := s[j];
	    s[j] := t;
	    i := i + 1;
	    j := j - 1;
	end;
    end;
    if (j > l) then
	quicksort(l, j);
    end;
    if (r > i) then
	quicksort(i, r);
    end;
end;

begin
    readsequence();
    quicksort(0, SIZE-1);
    writesequence();
end.
