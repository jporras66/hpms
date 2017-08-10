OCOBOL >>SOURCE FORMAT IS FIXED
      *-----------------------------------------------------------------
      * Authors:   Brian Tiffin, Asger Kjelstrup, human
      * Date:      27-Jan-2010
      * Purpose:   Hex Dump display
      * Tectonics: cobc -c CBL_OC_DUMP.cob
      *     Usage: cobc -x program.cob -o CBL_OC_DUMP
      *            export OC_DUMP_EXT=1 for explanatory text on dumps
      *            (memory address and dump length)
      *            export OC_DUMP_EXT=Y for extended explanatory text
      *            (architecture   and endian-order)
      *-----------------------------------------------------------------
       identification division.
       program-id. cbl_oc_dump.
      *
       environment      division.
       configuration    section.
      *
       data division.
       working-storage section.
       77  addr                             usage pointer.
       77  addr2addr                        usage pointer.
       77  counter               pic 999999 usage comp-5.
       77  byline                pic 999    usage comp-5.
       77  offset                pic 999999.
       01  some                  pic 999    usage comp-5.
           88 some-is-printable-iso88591
              values 32 thru 126, 160 thru 255.
           88 some-is-printable-ebcdic
              values 64, 65, 74 thru 80, 90 thru 97,
                     106 thru 111, 121 thru 127, 129 thru 137, 143,
                     145 thru 153, 159, 161 thru 169, 176,
                     186 thru 188, 192 thru 201, 208 thru 217, 224,
                     226 thru 233, 240 thru 249.
       77  high-var              pic 99     usage comp-5.
       77  low-var               pic 99     usage comp-5.
      *
       01  char-set              pic x(06).
           88 is-ascii           value 'ascii'.
           88 is-ebdic           value 'ebcdic'.
           88 is-unknown         value '?'.
       01  architecture          pic x(06).
           88 is-32-bit          value '32-bit'.
           88 is-64-bit          value '64-bit'.
       01  endian-order          pic x(10).
           88 is-big-endian-no   value 'little-big'.
           88 is-big-endian-yes  value 'big-little'.
      *
       77  hex-line              pic x(48).
       77  hex-line-pointer      pic 9(02) value 1.
      *
       77  show                  pic x(16).
       77  dots                  pic x value '.'.
       77  dump-dots             pic x.
      *
       77  hex-digit             pic x(16)  value '0123456789abcdef'.
       01  extended-infos        pic x.
           88 show-extended-infos      values '1', '2', 'y', 'y'.
           88 show-very-extended-infos values '2', 'y', 'y'.
      *
       77  len                   pic 999999 usage comp-5.
       77  len-display           pic 999999.
      *
       linkage section.
       01  buffer                pic x       any length.
       77  byte                  pic x.
      *-----------------------------------------------------------------
       procedure division using buffer.
      *
       main section.
       00.
           perform starting-address
      *
           perform varying counter from 0 by 16
                   until   counter  >=   len
              move counter to offset
              move spaces  to hex-line, show
              move '-'     to hex-line (24:01)
              move 1       to hex-line-pointer
              perform varying byline from 1 by 1
                      until   byline  >  16
                 if (counter + byline) > len
                    if byline < 9
                       move space to hex-line (24:01)
                    end-if
                    inspect show (byline:) replacing all spaces by dots
                    exit perform
                 else
                    move buffer (counter + byline : 1) to byte
                    perform calc-hex-value
                    if ((some-is-printable-iso88591 and is-ascii) or
                        (some-is-printable-ebcdic   and is-ebdic)   )
                       move byte to show (byline:1)
                    else
                       move dots to show (byline:1)
                    end-if
                 end-if
              end-perform
              display offset '  ' hex-line '  ' show
              end-display
           end-perform
           display ' '
           end-display
      *
           continue.
       ex. exit program.
      *-----------------------------------------------------------------
       calc-hex-value section.
       00.
           subtract 1 from function ord(byte) giving some
           end-subtract
           divide   some by 16 giving high-var remainder low-var
           end-divide
           string hex-digit (high-var + 1:1)
                  hex-digit (low-var  + 1:1)
                  space
                  delimited by size
                  into hex-line
                  with pointer hex-line-pointer
           end-string
      *
           continue.
       ex. exit.
      *-----------------------------------------------------------------
       starting-address section.
       00.
      * get the length of the transmitted buffer
           call 'C$PARAMSIZE' using 1
              giving len
           end-call
      * if wanted, change the dots to something different than points
           accept dump-dots from environment 'oc_dump_dots'
             not on exception
                 move dump-dots to dots
           end-accept
      *
           perform test-ascii
           perform test-endian
           set addr      to address of buffer
           set addr2addr to address of addr
      *
           if len > 0
      * to show hex-address, reverse if big-little endian
              if is-big-endian-yes
                 set addr2addr up   by length of addr
                 set addr2addr down by 1
              end-if
              move 1 to hex-line-pointer
              perform varying byline from 1 by 1
                      until byline > length of addr
                 set address of byte to addr2addr
                 perform calc-hex-value
                 if is-big-endian-yes
                    set addr2addr down by 1
                 else
                    set addr2addr up   by 1
                 end-if
              end-perform
           end-if
      *
      * get and display characteristics and headline
           accept extended-infos from environment 'oc_dump_ext'
           end-accept
           if show-extended-infos
              display ' '
              end-display
              if len > 0
                 display 'dump of memory beginning at hex-address: '
                          hex-line (1 : 3 * (byline - 1) )
                 end-display
              end-if
              move len to len-display
              display 'length of memory dump is: ' len-display
              end-display
              if show-very-extended-infos
                 perform test-64bit
                 display 'program runs in '
                         architecture ' architecture. '
                         'char-set is '
                         function trim (char-set) '.'
                 end-display
                 display 'byte order is ' endian-order
                         ' endian.'
                 end-display
              end-if
           end-if
      *
      * do we have anything to dump?
           if len > 0
      * ensure that the passed size is not too big
              if len > 999998
                 move 999998 to len, len-display
                 display 'warning, only the first '
                         len-display  ' bytes are shown!'
                 end-display
              end-if
              display ' '
              end-display
              display 'offset  '
                      'hex-- -- -- -5 -- -- -- -- 10 '
                      '-- -- -- -- 15 -- '
                      '  '
                      'chars----1----5-'
              end-display
           else
              display ' '
              end-display
              display 'nothing to dump.'
              end-display
           end-if
      *
           continue.
       ex. exit.
      *-----------------------------------------------------------------
       test-ascii section.
      *function: discover if running ascii or ebcdic
       00.
           evaluate space
              when x'20'
                 set  is-ascii   to true
              when x'40'
                 set  is-ebdic   to true
              when other
                 set  is-unknown to true
           end-evaluate
      *
           continue.
       ex. exit.
      *-----------------------------------------------------------------
       test-64bit section.
      *function: discover if running 32/64 bit
       00.
      *    longer pointers in 64-bit architecture
           if function length (addr) <= 4
              set  is-32-bit to true
           else
              set  is-64-bit to true
           end-if
      *
           continue.
       ex. exit.
      *-----------------------------------------------------------------
       test-endian section.
       00.
      *    number-bytes are shuffled in big-little endian
           move 128 to byline
           set address of byte to address of byline
           if function ord(byte) > 0
              set  is-big-endian-yes to true
           else
              set  is-big-endian-no  to true
           end-if
      *
           continue.
       ex. exit.
      *-----------------------------------------------------------------
       end program cbl_oc_dump.
