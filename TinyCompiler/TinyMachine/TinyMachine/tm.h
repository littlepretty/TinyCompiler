//
//  tm.h
//  TinyMachine
//
//  Created by Littlepretty Yan on 2/11/14.
//  Copyright (c) 2014 littlepretty. All rights reserved.
//

#ifndef TinyMachine_tm_h
#define TinyMachine_tm_h

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


#define IADDR_SIZE  1024        //!< instruction memory size
#define DADDR_SIZE  1024        //!< data memory size
#define NO_REGS     8           //!< number of registers
#define PC_REG      7           //!< last register is program counter

#define LINE_SIZE   121         
#define WORD_SIZE   20


typedef enum {
    opclRR,     //!< register-register
    opclRM,     //!< register-memory
    opclRA
} OP_CLASS;

typedef enum {
    /**
     *	RR instructions
     */
    opHALT,
    opIN,
    opOUT,
    opADD,
    opSUB,
    opMUL,
    opDIV,
    opRRLIM,
    
    /**
     *	RM instructions
     */
    opLD,
    opST,
    opRMLIM,
    
    /**
     *	RA instructions
     */
    opLDA,
    opLDC,
    opJLT,
    opJLE,
    opJGT,
    opJGE,
    opJEQ,
    opJNE,
    opRALIM

} OP_CODE;

typedef enum {
    srOKAY,
    srHALT,
    srIMEM_ERR,
    srDMEM_ERR,
    srZERO_DIVIDE
} STEP_RESULT;

typedef struct {
    
    int iOp;
    int iArg1;
    int iArg2;
    int iArg3;
    
} INSTRUCTION;




int readInstructions();

int doCommand();

#endif











































