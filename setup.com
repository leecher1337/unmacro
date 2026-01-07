$! SETUP.COM for UNMACRO Disassembler
$!
$ What  = F$ENV("PROCEDURE")
$ Where = F$PARSE(What,,, "DEVICE") + F$PARSE(What,,, "DIRECTORY")
$!
$ DEF UNMACRO_DIR 'Where'
$ UNMAC*RO :== $UNMACRO_DIR:UNMACRO
$!
$ EXIT