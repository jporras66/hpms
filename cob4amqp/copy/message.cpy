      ******************************************************************
      *       MESSAGE.CPY : see message.h                              *
      ******************************************************************
      *
      * =============================================== 
      *
       01 prefix-input     pic  x(101).
       01 prefix-output    pic  x(101).
       01 prefix-error     pic  x(101).
       01 prefix-queuename pic  x(101).
       01 prefix-timeout   pic  x(101).
      *
      * UUID property name 
      *
       01 MESSAGEUUID pic x(11) value "messageUUID".
       01 prefix-dummy       pic x(12) value low-values.
      *
      * MAX_MESSAGE_LENGTH definition 
       01 prefix-message   pic x(4096) value low-values.
      *
      * MAX_MESSAGE_ID_LENGTH definition 
       01 prefix-msgid       pic x(61)  value spaces.
       01 prefix-messageUUID pic x(61)  value spaces.
      *
      * MAX_MIMETYPE_LENGTH definition 
       01 prefix-mimetype  pic x(51)  value spaces.
      *
       01 prefix-rc     binary-short value zero.   
       01 prefix-msglen binary-short value zero.
      *
      * =============================================== 
      *
      * MAX_MESSAGE_PROPERTIES_LENGTH definition (1026)
      * Note : see message.h
      *
       01 prefix-properties  pic x(1026).             
       01 prefix-betta   redefines prefix-properties.
          05 prefix-num  binary-short.   
          05 prefix-list pic x(1024).
      *
      *
      * ===============================================  
      *
    
