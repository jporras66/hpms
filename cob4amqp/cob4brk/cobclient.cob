      *---------------------------------------------------------------*
      *          i d e n t i f i c a t i o n   d i v i s i o n        *
      *          =============================================        *
      *---------------------------------------------------------------*
       identification division. 
      *-----------------------.    
       program-id.      cobclient.
       author.          fjavier.porras@gmail.com.           
       date-written.    19/09/2016.
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
       01 filler pic x(050)
                 value  'pgm-cobclient-ws-start'.
       01 pgm-name pic x(15) 
                 value 'cobclient.cob'.
      *
       01 w-data.
          05 file-eof           pic 9.                
             88 eof             value 1.              
             88 neof            value 0.              
          05 reposition         pic 9.                
             88 rep-found       value 1.              
             88 rep-nfound      value 0.              
          05 full-current-date.
             10 full-datetime.
                15 f-date.
                   25 f-year       pic 9(4) value zeros.
                   25 f-month      pic 99   value zeros.
                   25 f-day        pic 99   value zeros.
                15  f-time.
                   25 c-hour       pic 99   value zeros.
                   25 c-minutes    pic 99   value zeros.
                   25 c-seconds    pic 99   value zeros.
                   25 c-sec-hund   pic 99   value zeros.
             10 c-time-diff.
                15 c-gmt-dir    pic x    value spaces.
                15 c-hour       pic 99   value zeros.
                15 c-minutes    pic 99   value zeros.
      *
       01 i  pic 9(04) value zeros.
       01 j  pic 9(04) value zeros.
       01 k  pic 9(04) value zeros.
       01 err pic  x(100). 
      *
      * =============================================== 
      *
           copy "message.cpy" replacing leading
           ==prefix-== BY ==q_==.
      *       
      * ===============================================  
      * 
       01 rc     binary-short value zero.   
       01  filler pic x(050)
                  value  'pgm-cobclient-ws-end'.
      *
       linkage section.
       01 lnk_input      pic  x(101).
       01 lnk_output     pic  x(101).
       01 lnk_error      pic  x(101).
       01 lnk_timeout pic  x(101).      
       01 errstatus pic  x(101).        
      *
      *---------------------------------------------------------------*
      *               p r o c e d u r e   d i v i s i o n             *
      *---------------------------------------------------------------*
      *
       procedure division using
               by reference lnk_input ,     
               by reference lnk_output ,
               by reference lnk_error ,
               by reference lnk_timeout ,               
               by reference errstatus.     
      *       
       1000-main.
      *
           move function current-date to full-current-date.
           display 'start : ' pgm-name ' - '  full-datetime.
           move lnk_input    to q_input
           move lnk_output   to q_output
           move lnk_error    to q_error
           move lnk_timeout  to q_timeout.   
      *       
           perform 9001-show-linkage
      *       
           perform 2100-queue-read thru 2100-queue-read-exit
           until eof ;
      *           
           perform 3000-end.     
      *        
       1000-main-exit.
           exit.
      *       
       2100-queue-read.
      * 
           display pgm-name " - amqpRecvQueueMpCC before   "
           move low-values to q_message q_mimetype q_properties 
                              errstatus
      *
      * rc contains message length (>= 0) or -1 (error) 
      * 
      * int16_t amqpRecvMessageCC ( const char *queue, char *msgid, char *msg, char *mimetype, PropertyList *propertylist, char *errstatus  )
      *
           call 'amqpRecvMessageCC' using   
                by reference q_input,
                by reference q_msgid,
                by reference q_message,
                by reference q_mimetype,            
                by reference q_properties,
                by reference errstatus
           returning rc
           on exception 
              string 'can not find program name :  amqpRecvMessageCC' 
              into err
              perform 3000-end
           end-call.                    
           if rc < 0 then 
              display pgm-name " - amqpRecvQueueMpCC error rc is : " rc
              perform 3000-end
           end-if.
      *
           perform 9002-show-msg.           
      *
      *    CALL to a paragraph to perform : Business Logic      
      *
           perform 2100-queue-write thru 2100-queue-write-exit.
      *
       2100-queue-read-exit.
           exit.
      *       
       2100-queue-write.
      * 
           display pgm-name " - amqpSendMessageCC before     "
           move rc to q_msglen            
           perform 9003-show-msg.           
      *
      * Writes to output_queue
      *
      * rc contains message length (>= 0) or -1 (error)
      *
      * int16_t amqpSendMessageCC    ( const char *queue, const char *msg, const int16_t msglen, const char *mimetype, const PropertyArr *p, char *errstatus ) 
      *
           call 'amqpSendMessageCC' using    
                by reference q_output,
                by reference q_message,
      * !! numbers passed by value !!
                by value q_msglen,
                by reference q_mimetype,
                by reference q_properties,
                by reference errstatus
           returning rc
           on exception 
              string 'can not find program name :  amqpSendMessageCC' 
              into err
              perform 3000-end
           end-call.                    
           if rc < 0 then 
              display "cobclient - amqpSendMessageCC error rc is : " 
              rc
              perform 3000-end
           end-if.
      *
      *    perform 9003-show-msg.           
      * 
       2100-queue-write-exit.
           exit.           
      *
       2000-msg.  
      *           
           if err = spaces then 
              string 'bye !!! :-)'  
              x'00' delimited by size into errstatus
              move 0 to return-code    
           else
              string err
              x'00' delimited by size into errstatus
              move -1 to return-code
           end-if.         
      *        
       2000-msg-exit.
           exit.
      *            
       3000-end.  
      *    
           display 'end   : ' pgm-name ' - '  full-datetime.              
           perform 2000-msg        
           move function current-date to full-current-date                  
           goback.    
      *        
       3000-end-exit.
           exit. 
      *  
      * ========================================================================================
      *          
       9001-show-linkage.  
      *           
           display pgm-name ' q_input   : ' q_input
           display pgm-name ' q_output  : ' q_output
           display pgm-name ' q_error   : ' q_error
           display pgm-name ' q_timeout : ' q_timeout
           display pgm-name ' errstatus : ' errstatus.
      *        
       9001-show-linkage-exit.
           exit.
      *          
       9002-show-msg.  
      *           
           display pgm-name ' q_msglen    : ' rc      
           display pgm-name ' q_input     : ' q_input
           display pgm-name ' q_msgid     : ' q_msgid
           display pgm-name ' q_message   : ' q_message
           display pgm-name ' q_mimetype  : ' q_mimetype 
           display pgm-name ' q_num       : ' q_num
           display pgm-name ' q_properties    : ' q_properties
           display pgm-name ' errstatus   : ' errstatus.
      *        
       9002-show-msg-exit.
           exit.
      *
       9003-show-msg.  
      *           
           display pgm-name ' q_msglen    : ' q_msglen      
           display pgm-name ' q_output    : ' q_output
           display pgm-name ' q_message   : ' q_message            
           display pgm-name ' errstatus   : ' errstatus.
      *        
       9003-show-msg-exit.
           exit.
      *            
