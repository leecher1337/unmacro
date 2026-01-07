Name:
	UNMACRO

Author:
	Robert Thomas Chojnacki
	Center for Manufacturing Engineering
	Northwestern University
	chojnacki@vaxmfg.tech.nwu.edu

Operating System:
	VAX/VMS

Source Language:
	VAX C

Keywords:
	Disassemblers

Abstract:
	Please also read INSTALL.DOC and UNMACRO.DOC.

	UNMACRO is an interactive disassembler that keeps track of what each
	byte of the program is (i.e. an instruction, .ENTRY, .QUAD, etcetera).
	You may leave a session and continue where you left off later by the
	SAVE and UNSAVE instructions.  Features include automatic labeling
	(for JMP, BR, etc. instructions), automatic .ENTRY (for CALLG, CALLS,
	transfer address); ability to write the disassembled code to a file; a
	"DISASSEMBLE UNTIL" that allows disassembly until a data byte, end of
	the image, or until a specified PC; ability to display any part of the
	image in DUMP format, ability to display the image header in DUMP
	format; easily jump from one part of the image to another; SEARCH the
	image for a byte string; displays important information such as
	transfer addresses, image base, etcetera.  Also knows the P1 system
	vectors.  User defineable symbols for CALLx instructions is included.

	Commands may be abbreviated and the user may specify that there is no
	image header, as in the case of boot blocks, etcetera.

	UNMACRO is not finished but functional.  Unfinished parts include most
	of the .ASCIx directives.  A help-format file is included.
