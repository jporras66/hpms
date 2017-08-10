      ******************************************************************
      *  Open Cobol ESQL (Ocesql) Sample Program
      *
      *  FETCHTBL --- demonstrates CONNECT, SELECT COUNT(*), 
      *               DECLARE cursor, FETCH cursor, COMMIT, 
      *               ROLLBACK, DISCONNECT
      *
      *  Copyright 2013 Tokyo System House Co., Ltd.
      ******************************************************************
       IDENTIFICATION              DIVISION.
      ******************************************************************
       PROGRAM-ID.                 FETCHTBL.
       AUTHOR.                     TSH.
       DATE-WRITTEN.               2013-06-28.

      ******************************************************************
       DATA                        DIVISION.
      ******************************************************************
       WORKING-STORAGE             SECTION.
       01  D-EMP-REC.
           05  D-EMP-NO            PIC  9(04).
           05  FILLER              PIC  X.
           05  D-EMP-NAME          PIC  X(20).
           05  FILLER              PIC  X.
           05  D-EMP-SALARY        PIC  --,--9.

OCESQL*EXEC SQL BEGIN DECLARE SECTION END-EXEC.
       01  DBNAME                  PIC  X(30) VALUE SPACE.
       01  USERNAME                PIC  X(30) VALUE SPACE.
       01  PASSWD                  PIC  X(10) VALUE SPACE.
       01  EMP-REC-VARS.
           05  EMP-NO              PIC S9(04).
           05  EMP-NAME            PIC  X(20) .
           05  EMP-SALARY          PIC S9(04).
       01  EMP-CNT                 PIC  9(04).
OCESQL*EXEC SQL END DECLARE SECTION END-EXEC.

OCESQL*EXEC SQL INCLUDE SQLCA END-EXEC.
OCESQL     copy "sqlca.cbl".
      ******************************************************************
OCESQL*
OCESQL 01  SQ0001.
OCESQL     02  FILLER PIC X(016) VALUE "SET NAMES 'UTF8'".
OCESQL     02  FILLER PIC X(1) VALUE X"00".
OCESQL*
OCESQL 01  SQ0002.
OCESQL     02  FILLER PIC X(026) VALUE "SELECT COUNT( * ) FROM EMP".
OCESQL     02  FILLER PIC X(1) VALUE X"00".
OCESQL*
OCESQL 01  SQ0003.
OCESQL     02  FILLER PIC X(060) VALUE "SELECT EMP_NO, EMP_NAME, EMP_S"
OCESQL  &  "ALARY FROM EMP ORDER BY EMP_NO".
OCESQL     02  FILLER PIC X(1) VALUE X"00".
OCESQL*
OCESQL 01  SQ0004.
OCESQL     02  FILLER PIC X(014) VALUE "DISCONNECT ALL".
OCESQL     02  FILLER PIC X(1) VALUE X"00".
OCESQL*
       PROCEDURE                   DIVISION.
      ******************************************************************
       MAIN-RTN.
           DISPLAY "*** FETCHTBL STARTED ***".
           
      *    WHENEVER IS NOT YET SUPPORTED :(
      *      EXEC SQL WHENEVER SQLERROR PERFORM ERROR-RTN END-EXEC.
      *           
      *    CONNECT
      *    MOVE  "testdb"          TO   DBNAME.
      * 
      * Note : Syntax is :  DBNAME@HOST:PORT
      *
      *        DBNAME@HOST:PORT can point to connection pooling tools for example pgbouncer
      *
      *        PostgreSQL configured to listen in : 5432
      *        PGBOUNCER  configured to listen in : 6432
      *
      *    MOVE  "HPS@localhost:5432"   TO   DBNAME. 
           MOVE  "HPS@192.168.254.130:5432"   TO   DBNAME.
           MOVE  "hps"             TO   USERNAME.
      *    MOVE  SPACE             TO   PASSWD.
           MOVE  "password"        TO   PASSWD.
OCESQL*    EXEC SQL
OCESQL*        CONNECT :USERNAME IDENTIFIED BY :PASSWD USING :DBNAME 
OCESQL*    END-EXEC.
OCESQL     CALL "OCESQLConnect" USING
OCESQL          BY REFERENCE SQLCA
OCESQL          BY REFERENCE USERNAME
OCESQL          BY VALUE 30
OCESQL          BY REFERENCE PASSWD
OCESQL          BY VALUE 10
OCESQL          BY REFERENCE DBNAME
OCESQL          BY VALUE 30
OCESQL     END-CALL.
           IF  SQLCODE NOT = ZERO PERFORM ERROR-RTN STOP RUN.
      *  
           DISPLAY "*** BEFORE SET NAMES ***".  
OCESQL*    EXEC SQL
OCESQL*        SET NAMES 'UTF8'
OCESQL*    END-EXEC.
OCESQL     CALL "OCESQLExec" USING
OCESQL          BY REFERENCE SQLCA
OCESQL          BY REFERENCE SQ0001
OCESQL     END-CALL.
           IF  SQLCODE NOT = ZERO 
               PERFORM ERROR-RTN 
               STOP RUN
           END-IF. 
           DISPLAY "*** AFTER SET NAMES ***".
      *           
      *    SELECT COUNT(*) INTO HOST-VARIABLE
OCESQL*    EXEC SQL 
OCESQL*        SELECT COUNT(*) INTO :EMP-CNT FROM EMP
OCESQL*    END-EXEC.
OCESQL     CALL "OCESQLStartSQL"
OCESQL     END-CALL
OCESQL     CALL "OCESQLSetResultParams" USING
OCESQL          BY VALUE 1
OCESQL          BY VALUE 4
OCESQL          BY VALUE 0
OCESQL          BY REFERENCE EMP-CNT
OCESQL     END-CALL
OCESQL     CALL "OCESQLExecSelectIntoOne" USING
OCESQL          BY REFERENCE SQLCA
OCESQL          BY REFERENCE SQ0002
OCESQL          BY VALUE 0
OCESQL          BY VALUE 1
OCESQL     END-CALL
OCESQL     CALL "OCESQLEndSQL"
OCESQL     END-CALL.
           DISPLAY "TOTAL RECORD: " EMP-CNT.
           
      *    DECLARE CURSOR
OCESQL*    EXEC SQL 
OCESQL*        DECLARE C1 CURSOR FOR
OCESQL*        SELECT EMP_NO, EMP_NAME, EMP_SALARY 
OCESQL*               FROM EMP
OCESQL*               ORDER BY EMP_NO
OCESQL*    END-EXEC.
OCESQL     CALL "OCESQLCursorDeclare" USING
OCESQL          BY REFERENCE SQLCA
OCESQL          BY REFERENCE "fetchtbl_C1" & x"00"
OCESQL          BY REFERENCE SQ0003
OCESQL     END-CALL.
OCESQL*    EXEC SQL
OCESQL*        OPEN C1
OCESQL*    END-EXEC.
OCESQL     CALL "OCESQLCursorOpen" USING
OCESQL          BY REFERENCE SQLCA
OCESQL          BY REFERENCE "fetchtbl_C1" & x"00"
OCESQL     END-CALL.
           
      *    FETCH
           DISPLAY "---- -------------------- ------".
           DISPLAY "NO   NAME                 SALARY".
           DISPLAY "---- -------------------- ------".
OCESQL*    EXEC SQL 
OCESQL*        FETCH C1 INTO :EMP-NO, :EMP-NAME, :EMP-SALARY
OCESQL*    END-EXEC.
OCESQL     CALL "OCESQLStartSQL"
OCESQL     END-CALL
OCESQL     CALL "OCESQLSetResultParams" USING
OCESQL          BY VALUE 3
OCESQL          BY VALUE 4
OCESQL          BY VALUE 0
OCESQL          BY REFERENCE EMP-NO
OCESQL     END-CALL
OCESQL     CALL "OCESQLSetResultParams" USING
OCESQL          BY VALUE 16
OCESQL          BY VALUE 20
OCESQL          BY VALUE 0
OCESQL          BY REFERENCE EMP-NAME
OCESQL     END-CALL
OCESQL     CALL "OCESQLSetResultParams" USING
OCESQL          BY VALUE 3
OCESQL          BY VALUE 4
OCESQL          BY VALUE 0
OCESQL          BY REFERENCE EMP-SALARY
OCESQL     END-CALL
OCESQL     CALL "OCESQLCursorFetchOne" USING
OCESQL          BY REFERENCE SQLCA
OCESQL          BY REFERENCE "fetchtbl_C1" & x"00"
OCESQL     END-CALL
OCESQL     CALL "OCESQLEndSQL"
OCESQL     END-CALL.
           PERFORM UNTIL SQLCODE NOT = ZERO
              MOVE  EMP-NO        TO    D-EMP-NO
              MOVE  EMP-NAME      TO    D-EMP-NAME
              MOVE  EMP-SALARY    TO    D-EMP-SALARY
              DISPLAY D-EMP-REC
OCESQL*       EXEC SQL 
OCESQL*           FETCH C1 INTO :EMP-NO, :EMP-NAME, :EMP-SALARY
OCESQL*       END-EXEC
OCESQL     CALL "OCESQLStartSQL"
OCESQL     END-CALL
OCESQL     CALL "OCESQLSetResultParams" USING
OCESQL          BY VALUE 3
OCESQL          BY VALUE 4
OCESQL          BY VALUE 0
OCESQL          BY REFERENCE EMP-NO
OCESQL     END-CALL
OCESQL     CALL "OCESQLSetResultParams" USING
OCESQL          BY VALUE 16
OCESQL          BY VALUE 20
OCESQL          BY VALUE 0
OCESQL          BY REFERENCE EMP-NAME
OCESQL     END-CALL
OCESQL     CALL "OCESQLSetResultParams" USING
OCESQL          BY VALUE 3
OCESQL          BY VALUE 4
OCESQL          BY VALUE 0
OCESQL          BY REFERENCE EMP-SALARY
OCESQL     END-CALL
OCESQL     CALL "OCESQLCursorFetchOne" USING
OCESQL          BY REFERENCE SQLCA
OCESQL          BY REFERENCE "fetchtbl_C1" & x"00"
OCESQL     END-CALL
OCESQL     CALL "OCESQLEndSQL"
OCESQL     END-CALL
           END-PERFORM.
           
      *    CLOSE CURSOR
OCESQL*    EXEC SQL 
OCESQL*        CLOSE C1 
OCESQL*    END-EXEC. 
OCESQL     CALL "OCESQLCursorClose"  USING
OCESQL          BY REFERENCE SQLCA
OCESQL          BY REFERENCE "fetchtbl_C1" & x"00"
OCESQL     END-CALL
OCESQL    .
           
      *    COMMIT
OCESQL*    EXEC SQL 
OCESQL*        COMMIT WORK
OCESQL*    END-EXEC.
OCESQL     CALL "OCESQLStartSQL"
OCESQL     END-CALL
OCESQL     CALL "OCESQLExec" USING
OCESQL          BY REFERENCE SQLCA
OCESQL          BY REFERENCE "COMMIT" & x"00"
OCESQL     END-CALL
OCESQL     CALL "OCESQLEndSQL"
OCESQL     END-CALL.
           
      *    DISCONNECT
OCESQL*    EXEC SQL
OCESQL*        DISCONNECT ALL
OCESQL*    END-EXEC.
OCESQL     CALL "OCESQLDisconnect" USING
OCESQL          BY REFERENCE SQLCA
OCESQL     END-CALL.
           
      *    END
           DISPLAY "*** FETCHTBL FINISHED ***".
           STOP RUN.

      ******************************************************************
       ERROR-RTN.
      ******************************************************************
           DISPLAY "*** SQL ERROR ***".
           DISPLAY "SQLCODE: " SQLCODE " " NO ADVANCING.
           EVALUATE SQLCODE
              WHEN  +10
                 DISPLAY "Record not found"
              WHEN  -01
                 DISPLAY "Connection falied"
              WHEN  -20
                 DISPLAY "Internal error"
              WHEN  -30
                 DISPLAY "PostgreSQL error"
                 DISPLAY "ERRCODE: "  SQLSTATE
                 DISPLAY SQLERRMC
              *> TO RESTART TRANSACTION, DO ROLLBACK.
OCESQL*          EXEC SQL
OCESQL*              ROLLBACK
OCESQL*          END-EXEC
OCESQL     CALL "OCESQLStartSQL"
OCESQL     END-CALL
OCESQL     CALL "OCESQLExec" USING
OCESQL          BY REFERENCE SQLCA
OCESQL          BY REFERENCE "ROLLBACK" & x"00"
OCESQL     END-CALL
OCESQL     CALL "OCESQLEndSQL"
OCESQL     END-CALL
              WHEN  OTHER
                 DISPLAY "Undefined error"
                 DISPLAY "ERRCODE: "  SQLSTATE
                 DISPLAY SQLERRMC
           END-EVALUATE.
      ******************************************************************



