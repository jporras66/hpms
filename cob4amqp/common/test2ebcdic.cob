      *---------------------------------------------------------------*
      *          i d e n t i f i c a t i o n   d i v i s i o n        *
      *          =============================================        *
      *---------------------------------------------------------------*
       identification division. 
      *-----------------------.	
       program-id. 	test2ebcdic.
       author. 		fjavier.porras@gmail.com.           
       date-written.22/03/2015.
      *---------------------------------------------------------------*
      *             e n v i r o n m e n t   d i v i s i o n           *
      *             =======================================           *
      *---------------------------------------------------------------*
       environment division.
      *--------------------.
       configuration section. 
      *---------------------.
       special-names.
           decimal-point is comma
           alphabet alpha is native
           alphabet beta  is ebcdic.
       input-output section.
      *--------------------.
       file-control.        
      * 
      *---------------------------------------------------------------*
      *                    d a t a   d i v i s i o n                  *
      *---------------------------------------------------------------*
       data division.
      *-------------.
       file section.
      *------------.      
      * 
       working-storage section.
      *-----------------------.
       01 i      pic 9(04) value 0.
       01 c      pic x(01) value spaces.
       01 vlength usage binary-short value 0.
       01 var    pic x(62) value is
       '0123456789abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz'. 
       01 var2   pic x(256) value spaces.
       procedure division.
      *
       move function length(var2) to vlength  
       call 'inithexa' using var2, vlength end-call
       call 'cbl_oc_dump' using var2 end-call
      *
       call 'asc2ebc' using var2, vlength end-call 
       call 'cbl_oc_dump' using var2 end-call 
      *   
       call 'ebc2asc' using var2, vlength end-call
      *
       call 'cbl_oc_dump' using var2 end-call 
       goback.
       
