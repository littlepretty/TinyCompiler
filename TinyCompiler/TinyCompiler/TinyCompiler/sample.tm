* TINY Compilation to TM Code
* File: TinyCompiler/TinyCompiler/sample.tm
* Standard Prelude:
  0:    LD 6,0(0) 	load max address from memory location 0
  1:    ST 0,0(0) 	clear loaction 0
* End of Standard Prelude.
* -> read
  2:    IN 0,0,0 	read integer value
  3:    ST 0,0(5) 	read: store value
* <- read
* -> if
* -> Op
* -> Const
  4:   LDC 0,0(0) 	load const
* <- Const
  5:    ST 0,0(6) 	op: store left as tmp variable
* -> Id
  6:    LD 0,0(5) 	load id value
* <- Id
  7:    LD 1,0(6) 	op: load left to register ac1
  8:   SUB 0,1,0 	op <
  9:   JLT 0,2(7) 	br if true
 10:   LDC 0,0(0) 	false case
 11:   LDA 7,1(7) 	unconditional jump
 12:   LDC 0,1(0) 	true case
* <- Op
* if: jump to else belongs here
* -> assign
* -> Const
 14:   LDC 0,1(0) 	load const
* <- Const
 15:    ST 0,1(5) 	assign: store value
* <- assign
* -> repeat
* repeat: jump after body comes back here
* -> assign
* -> Op
* -> Id
 16:    LD 0,1(5) 	load id value
* <- Id
 17:    ST 0,0(6) 	op: store left as tmp variable
* -> Id
 18:    LD 0,0(5) 	load id value
* <- Id
 19:    LD 1,0(6) 	op: load left to register ac1
 20:   MUL 0,1,0 	op *
* <- Op
 21:    ST 0,1(5) 	assign: store value
* <- assign
* -> assign
* -> Op
* -> Id
 22:    LD 0,0(5) 	load id value
* <- Id
 23:    ST 0,0(6) 	op: store left as tmp variable
* -> Const
 24:   LDC 0,1(0) 	load const
* <- Const
 25:    LD 1,0(6) 	op: load left to register ac1
 26:   SUB 0,1,0 	op -
* <- Op
 27:    ST 0,0(5) 	assign: store value
* <- assign
* -> Op
* -> Id
 28:    LD 0,0(5) 	load id value
* <- Id
 29:    ST 0,0(6) 	op: store left as tmp variable
* -> Const
 30:   LDC 0,0(0) 	load const
* <- Const
 31:    LD 1,0(6) 	op: load left to register ac1
 32:   SUB 0,1,0 	op ==
 33:   JEQ 0,2(7) 	br if true
 34:   LDC 0,0(0) 	false case
 35:   LDA 7,1(7) 	unconditional jump
 36:   LDC 0,1(0) 	true case
* <- Op
 37:   JEQ 0,-22(7)	repeat: jump back to body
* <- repeat
* -> write
* -> Id
 38:    LD 0,1(5) 	load id value
* <- Id
 39:   OUT 0,0,0 	write ac
* <- write
* if: jump to end belongs here
 13:   JEQ 0,27(7)	if: jump to else
 40:   LDA 7,0(7)	if: jump to end
* <- if
* End of Execution.
 41:  HALT 0,0,0 	
