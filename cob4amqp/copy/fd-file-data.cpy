      *
      * 
       FD FILE-DATA 
            VALUE OF FILE-ID IS WS-FILE-DATA
            RECORDING MODE F
            LABEL RECORD STANDARD
            BLOCK 0.
      *
       01 WW02-FILE-DATA.             
          05 FILLER        PIC X(15)   VALUE 'DATE :    '.
          05 WW02-DATE     PIC X(25)   .
          05 WW02-ALFA1    PIC X(37)
             VALUE      'abcdefghijklmnñopqrstuvwxyz0123456789'.
          05 WW02-ALFA2    PIC X(37)
             VALUE      'ABCDEFGHIJKLMNÑOPQRSTUVWXYZ0123456789'.
          05 FILLER        PIC X(01) VALUE '-'.
          05 WW02-ID1      PIC 9(04) USAGE  COMP    VALUE 0.
          05 FILLER        PIC X(01) VALUE '-'.
          05 WW02-ID2      PIC 9(04) USAGE  COMP-3  VALUE 0.
          05 FILLER        PIC X(01) VALUE '-'.
          05 WW02-ID3      PIC 9(04) USAGE  COMP-5  VALUE 0.
          05 FILLER        PIC X(01) VALUE '-'.
          05 WW02-ID4      PIC 9(04) USAGE  DISPLAY VALUE 0.
          05 FILLER        PIC X(01) VALUE '-'.
          05 WW02-BUFFER   PIC X(1868) VALUE SPACES.
          05 FILLER        PIC X(01) VALUE '-'.
          05 FILLER        PIC X(01) VALUE X'00'.  
      *


