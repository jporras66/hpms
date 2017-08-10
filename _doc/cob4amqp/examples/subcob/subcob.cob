	   identification division.
	   program-id. subcob.
       data division.
       linkage section.
       01 arg1 pic x(7).
       01 arg2 pic x(7).
       01 arg3 usage binary-long.
       procedure division using
          by value arg1,
          by reference arg2,
          by reference arg3.
       000-main.
          display 'starting cobsub.cbl'.
          display 'arg1=' arg1.
          display 'arg2=' arg2.
          display 'arg3=' arg3.
          move 'x' to arg1 (1:1).
          move 'y' to arg2 (1:1).
          move 987654321 to arg3.
          move 2 to return-code.
          goback.