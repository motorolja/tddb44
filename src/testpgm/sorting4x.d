program testpgm_large;

const SIZE = 10;

var 
  s : array[SIZE] of integer;
  s2 : array[SIZE] of integer;
  m : integer;

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

procedure swap(l : integer; r : integer);
var
   temp : integer;
begin
  temp := s[l];
  s[l] := s[r];
  s[r] := temp;
end;

procedure bubblesort(r : integer);
var
  i : integer;
  j : integer;
begin
  i := 0;
  while (i < r) do
    j := 0;
    while (j < r - 1) do
      if (s[j]>s[j+1]) then
	swap(j,j+1);
      end;
      j := j + 1;   
    end;
  i := i + 1;
  end;
end;

procedure insertionsort(r : integer);
var
  i : integer;
  j : integer;
  value : integer;
begin
    i := 1;
    j := 0;

    while (i  < r) do
      value := s[i];
      j := i - 1;
      while ((j > -1) and (s[j] > value)) do
        s[j + 1] := s[j];
        j := j - 1;
      end;
      s[j + 1] := value;
      i := i + 1;
    end;
end;

procedure selectionsort(r : integer);
var
  i : integer;
  j : integer;
  min : integer;
  swap : integer;
begin
  i := 0;
  j := 0;

  while (i < r - 1) do

    min := i;

    j := i + 1;
    while (j < r) do  
      
      if (s[j] < s[min]) then
        min := j;
      end;
    
      j := j + 1;
    end;

    if not (i = min) then
      swap := s[i];
      s[i] := s[min];
      s[min] := swap;
    end; 
   
    i := i + 1;  
  end;  
end; 

begin

{ 4 different ways of sorting. }

readsequence();

m := 0;
while (m < SIZE) do
  s2[m] := s[m];
  m := m + 1;
end;

quicksort(0, SIZE - 1);
writesequence();
write(10);

m := 0;
while (m < SIZE) do
  s[m] := s2[m];
  m := m + 1;
end;

bubblesort(SIZE);
writesequence();
write(10);

m := 0;
while (m < SIZE) do
  s[m] := s2[m];
  m := m + 1;
end;

insertionsort(SIZE);
writesequence();
write(10);

m := 0;
while (m < SIZE) do
  s[m] := s2[m];
  m := m + 1;
end;

selectionsort(SIZE);
writesequence(); 
write(10);

end.