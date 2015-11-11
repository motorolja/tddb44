program foo;

{ prints 'A' if everything is OK }
{ prints '!' : there is an array or parameter handling error }
{ prints 'X' or nothing : there is an elseif handling error }

const ARSZ = 1000;
      blaz = 'idi';

var woodchuck: array[ArSz] of real;

function half(bar : integer; gazonk : integer):integer;
begin
  return bar div gazonk;
end;

procedure chuckwood(stop:integer; start:integer; val:integer);
var i:integer;
begin
  if((not (start<stop)) or (start<0) or (stop>ARSZ)) then
    return;
  end;
  i:=start;
  while(i<stop) do
    woodchuck[i] := val;
    i := i + 1;
  end;
  return;
end;

function checkwood:integer;
var z:integer;
begin
  z := 0;
  while(z<ARSZ) do
    if(woodchuck[z]<>17.0) then
      return 0;
    end;
    z := z + 1;
  end;
  return 1;
end;

function zero:integer;
begin
  return 0;
end;

begin
  chuckwood(half(ARSZ*2,2), zero(), 17);
  if(not checkwood()) then
    write(33); { ! }
    write(10);
  elsif 1>0 then
    write(65); { A }
    write(10);
  elsif 0=0 then
    write(88); { X }
    write(10);
  end;
end.
