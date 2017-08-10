#|/bin/bash
ocesql FETCHTBL.cbl inserttbl.cob
cobc -x inserttbl.cob -L/opt/ocesql/lib -locesql -lpq 
