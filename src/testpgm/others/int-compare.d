program test;

const
    THREE = 3;
    FOUR = 4;
    FOUR_1 = 4;

begin
    { prints 01234567 if ok }
    if (FOUR > THREE) then
        write(48); { 0 }
    end;

    if (THREE > FOUR) then
        write(33);
    else
        write(49); { 1 }
    end;

    if (THREE < FOUR) then
        write(50); { 2 }
    end;

    if (FOUR < THREE) then
        write(33);
    else
        write(51); { 3 }
    end;

    if (THREE <> FOUR) then
        write(52); { 4 }
    end;

    if (FOUR <> FOUR) then
        write(33);
    else
        write(53); { 5 }
    end;

    if (THREE = FOUR) then
        write(33);
    else
        write(54); { 6 }
    end;

    if (FOUR_1 = FOUR) then
        write(55); { 7 }
    else
        write(33);
    end;
end.