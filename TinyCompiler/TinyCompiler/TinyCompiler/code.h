//
//  code.h
//  TinyCompiler
//
//  Created by Littlepretty Yan on 2/11/14.
//  Copyright (c) 2014 littlepretty. All rights reserved.
//

#ifndef TinyCompiler_code_h
#define TinyCompiler_code_h

/* specific registers */
#define pc 7        //!< program conter
#define mp 6        //!< memory pointer, points to the top of memory, for temp variable storage
#define gp 5        //!< global pointer, points to the bottom of memory, for glabal variable storage
#define ac 0        //!< accumulator, default location for operation result
#define ac1 1       //!< 2nd accumulator

/**
 *	Emit register-register instruction
 *
 *	@param	op	opcode
 *	@param	r	target register
 *	@param	s	1st source register
 *	@param	t	2nd source register
 *	@param	comment	comment to be printed if TraceCode is TRUE
 */
void emitRO(char *op, int r, int s, int t, char *c);

/**
 *	Emit register-memory instructions
 *
 *	@param	op	opcode
 *	@param	r	target register
 *	@param	s	1st source register
 *	@param	t	2nd source register
 *	@param	comment	comment to be printed if TraceCode is TRUE
 */
void emitRM(char *op, int r, int d, int s, char *c);

/**
 *	Skip code locations for later back-patch
 *
 *	@param	steps	how many steps to skip
 *
 *	@return	current code position, for later backup
 */
int emitSkip(int steps);

/**
 *	Backs up to location which previously skipped
 *
 *	@param	loc	previously skipped location
 */
void emitBackup(int loc);


/**
 *	Restore the current code position to the highest previously unemitted position
 */
void emitRestore();

void emitComment(char *c);

/**
 *	Converts an absolute reference to a pc-relative reference 
 *  when emitting a register-memory instruction
 *
 *	@param	op	opcode
 *	@param	r	target register
 *	@param	a	absolute location in memory
 *	@param	comment	comment to be printed if TraceCode is TRUE
 */
void emitRM_Abs(char *op, int r, int a, char *c);





#endif




















