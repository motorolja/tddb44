PROGRAM Cirkel;

  CONST Pi = 3.14159;

  VAR o : REAL; 
      r : REAL;

  PROCEDURE Init;
    BEGIN
      r := 17;
    END;

  FUNCTION Omkrets(Radie : REAL) : REAL;

    FUNCTION Diameter: REAL;
      BEGIN
	RETURN 2 * Radie;
      END;

  BEGIN
    RETURN Diameter() * Pi;
  END;

BEGIN
  Init();
  o := Omkrets(r);
END.
