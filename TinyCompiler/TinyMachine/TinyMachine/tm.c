//
//  tm.c
//  TinyMachine
//
//  Created by Littlepretty Yan on 2/11/14.
//  Copyright (c) 2014 littlepretty. All rights reserved.
//

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "tm.h"

int iLoc = 0;
int dLoc = 0;
int TraceFlag = TRUE;
int InstructionCountFlag = TRUE;


INSTRUCTION iMem[IADDR_SIZE];
int dMem[DADDR_SIZE];
int reg[NO_REGS];

char *opCodeTab[] = {
    "HALT", "IN", "OUT", "ADD", "SUB", "MUL", "DIV", "????",
    "LD", "ST", "????",
    "LDA", "LDC", "JLT", "JLE", "JGT", "JGE", "JEQ", "JNE", "????" };

char *stepResultTab[] = {
    "OK", "Halted", "Instruction Memory Fault", "Data Memory Fault", "Divided By 0" };

extern char pgmName[20];
extern FILE *pgm;

extern int done;

size_t lineLen;
int inCol;
int num;
char word[WORD_SIZE];
char in_line[LINE_SIZE];
char ch;


int opClass(int c)
{
    if (c <= opRRLIM) {
        return opclRR;
    } else if (c <= opRMLIM) {
        return opclRM;
    } else {
        return opclRA;
    }
}

void writeInstruction(int loc)
{
    printf("%5d", loc);
    
    if (loc >=0 && loc < IADDR_SIZE) {
        
        printf("%6s%3d,", opCodeTab[iMem[loc].iOp], iMem[loc].iArg1);
        
        switch (opClass(iMem[loc].iOp)) {
            case opclRR:
                printf("%1d, %1d", iMem[loc].iArg2, iMem[loc].iArg3);
                break;
            case opclRM:
            case opclRA:
                printf("%3d(%1d)", iMem[loc].iArg2, iMem[loc].iArg3);
            default:
                break;
        }
        printf("\n");
    }
}

void getCh()
{
    if (++inCol < lineLen) {
        ch = in_line[inCol];
    } else {
        ch = ' ';
    }
}

int nonBlank()
{
    while (inCol < lineLen && in_line[inCol] == ' ') {
        inCol++;
    }
    if (inCol < lineLen) {
        ch = in_line[inCol];
        return TRUE;
    } else {
        ch = ' ';
        return FALSE;
    }
}

int getNum()
{
    int sign;
    int term;
    int temp = FALSE;
    num = 0;
    
    do {
        sign = 1;
        while (nonBlank() && (ch == '+' || ch == '-')) {
            temp = FALSE;
            if (ch == '-') {
                sign = -sign;
            }
            getCh();
        }
        
        term = 0;
        nonBlank();
        while (isdigit(ch)) {
            temp = TRUE;
            term = term*10 + (ch - '0');
            getCh();
        }
        num = num + term * sign;
        
    } while (nonBlank() && (ch == '+'|| ch == '-'));
    return temp;
    
}

int getWord()
{
    int temp = FALSE;
    int length = 0;
    if (nonBlank()) {
        while (isalnum(ch)) {
            if (length < WORD_SIZE - 1) {
                word[length++] = ch;
            }
            getCh();
            
        }
        word[length] = '\0';
        temp = length != 0;
    }
    return temp;
}

int skipCh(char c)
{
    int temp = FALSE;
    if (nonBlank() && ch == c) {
        getCh();
        temp = TRUE;
    }
    return temp;
}

int atEOL()
{
    return !nonBlank();
}

int error(char *msg, int lineNO, int instNO)
{
    printf("Line %d", lineNO);
    if (instNO >= 0) {
        printf(" (Instruction %d)", instNO);
    }
    printf(" %s\n", msg);
    return FALSE;
}

void resetMachine()
{
    /**
     *	initialize registers
     */
    for (int regNO = 0; regNO < NO_REGS; regNO++) {
        reg[regNO] = 0;
    }
    
    /**
     *	put data memory's size at data memeory's first entry
     */
    dMem[0] = DADDR_SIZE - 1;
    
    /**
     *	initialize data memory
     */
    for (int loc = 1; loc < DADDR_SIZE; loc++) {
        dMem[loc] = 0;
    }

}

int readInstructions()
{
    
    resetMachine();
    
    /**
     *	initialize instruction memory
     */
    for (int loc = 0; loc < IADDR_SIZE; loc++) {
        iMem[loc].iOp = opHALT;
        iMem[loc].iArg1 = 0;
        iMem[loc].iArg2 = 0;
        iMem[loc].iArg3 = 0;
    }
    
    int lineNO = 0;
    OP_CODE op;
    int arg1, arg2, arg3;
    
    /**
     *	go over program file, write instructions into iMem[]
     */
    while (!feof(pgm)) {
        
        fgets(in_line, LINE_SIZE - 2, pgm);
        inCol = 0;
        lineNO++;
        lineLen = strlen(in_line) - 1;
        
        if (in_line[lineLen] == '\n') {
            in_line[lineLen] = '\0';
        } else {
            in_line[++lineLen] = '\0';
        }
        
        if (nonBlank() && in_line[inCol] != '*') {
            
            /**
             *	get instruction location
             */
            if (!getNum()) {
                return error("Bad Location", lineNO, -1);
            }
            int loc = num;
            if (loc > IADDR_SIZE) {
                return error("Instructon Location Too Large", lineNO, loc);
            }
            /**
             *	skip : after location
             */
            if (!skipCh(':')) {
                return error("Missing Colon", lineNO, loc);
            }
            /**
             *	get opcode word
             */
            if (!getWord()) {
                return error("Missing Opcode", lineNO, loc);
            }
            /**
             *	by opcode word, find opcode type from code table
             */
            op = opHALT;
            while (op < opRALIM && strncmp(opCodeTab[op], word,4) != 0) {
                op++;
            }
            if (strncmp(opCodeTab[op], word, 4) != 0) {
                return error("Illegal Opcode", lineNO, loc);
            }
            
            switch (opClass(op)) {
                case opclRR:
                    /**
                     *	RR instructions looks like: OpWord r,s,t
                     */
                    if (!getNum() || num < 0 || num >= NO_REGS) {
                        return error("Bad First Register", lineNO, loc);
                    }
                    arg1 = num;
                    
                    if (!skipCh(',')) {
                        return error("Missing Comma", lineNO, loc);
                    }
                    if (!getNum() || num < 0 || num >= NO_REGS) {
                        return error("Bad Second Register", lineNO, loc);
                    }
                    arg2 = num;
                    if (!skipCh(',')) {
                        return error("Missing Comma", lineNO, loc);
                    }
                    if (!getNum() || num < 0 || num >= NO_REGS) {
                        return error("Bad Third Register", lineNO, loc);
                    }
                    arg3 = num;
    
                    break;
                
                case opclRM:
                case opclRA:
                    /**
                     *	RM instrunctions looks like: OpWord r,d(s)
                     */
                    if (!getNum() || num < 0 || num >= NO_REGS) {
                        return error("Bad First Register", lineNO, loc);
                    }
                    arg1 = num;
                    
                    if (!skipCh(',')) {
                        return error("Missing Comma", lineNO, loc);
                    }
                    
                    if (!getNum()) {
                        return error("Bad Displacement", lineNO, loc);
                    }
                    arg2 = num;
                    
                    if (!skipCh('(') && !skipCh(',')) {
                        return error("Missing Left Paren", lineNO, loc);
                    }
                    if (!getNum() || num < 0 || num >= NO_REGS) {
                        return error("Bad Second Register", lineNO, loc);
                    }
                    
                    arg3 = num;
                    
                    break;
                default:
                    break;
            }
            iMem[loc].iOp = op;
            iMem[loc].iArg1 = arg1;
            iMem[loc].iArg2 = arg2;
            iMem[loc].iArg3 = arg3;
        }
        
    }
    return TRUE;
}

STEP_RESULT stepTM()
{
    INSTRUCTION currentInstruction;
    int pc = reg[PC_REG];
    
    if (pc < 0 || pc > IADDR_SIZE) {
        return srIMEM_ERR;
    }
    reg[PC_REG] = pc + 1;
    currentInstruction = iMem[pc];
    
    int r, s, t, m;
    switch (opClass(currentInstruction.iOp)) {
        case opclRR:
            r = currentInstruction.iArg1;
            s = currentInstruction.iArg2;
            t = currentInstruction.iArg3;
            break;
            
        case opclRM:
            r = currentInstruction.iArg1;
            s = currentInstruction.iArg3;
            m = currentInstruction.iArg2 + reg[s];
            if (m < 0 || m > DADDR_SIZE) {
                return srDMEM_ERR;
            }
            break;
            
        case opclRA:
            r = currentInstruction.iArg1;
            s = currentInstruction.iArg3;
            m = currentInstruction.iArg2 + reg[s];
            break;
        default:
            break;
    }
    
    int ok = FALSE;
    /**
     *	simulate instruction
     */
    switch (currentInstruction.iOp) {
            
        /**
         *	RR instructions
         */
        case opHALT:
            printf("HALT: %1d,%1d,%1d\n", r, s, t);
            return srHALT;
            break;
            
        case opIN:
            
            do {
                printf("Enter Integer Value for In Instruction: ");
                fflush(stdin);
                fflush(stdout);
                gets(in_line);
                lineLen = strlen(in_line);
                inCol = 0;
                ok = getNum();
                if (!ok) {
                    printf("Illegal Value\n");
                } else {
                    reg[r] = num;
                }
                
            } while (!ok);
            break;
            
        case opOUT:
            printf("OUT Instruction Prints: %d\n", reg[r]);
            break;
        case opADD:
            reg[r] = reg[s] + reg[t];
            break;
        case opSUB:
            reg[r] = reg[s] - reg[t];
            break;
        case opMUL:
            reg[r] = reg[s] * reg[t];
            break;
        case opDIV:
            if (reg[t] != 0) {
                reg[r] = reg[s] / reg[t];
            } else {
                return srZERO_DIVIDE;
            }
            break;
            
        /**
         *	RM instructions
         */
        case opLD:
            reg[r] = dMem[m];
            break;
        case opST:
            dMem[m] = reg[r];
            break;
        /**
         *	RA instructions
         */
        case opLDA:
            reg[r] = m;
            break;
        case opLDC:
            reg[r] = currentInstruction.iArg2;
            break;
        case opJLT:
            if (reg[r] < 0) {
                reg[PC_REG] = m;
            }
            break;
        case opJLE:
            if (reg[r] <= 0) {
                reg[PC_REG] = m;
            }
            break;
        case opJGT:
            if (reg[r] > 0) {
                reg[PC_REG] = m;
            }
            break;
        case opJGE:
            if (reg[r] >= 0) {
                reg[PC_REG] = m;
            }
            break;
        case opJEQ:
            if (reg[r] == 0) {
                reg[PC_REG] = m;
            }
            break;
        case opJNE:
            if (reg[r] != 0) {
                reg[PC_REG] = m;
            }
            break;

        default:
            break;
    }
    
    return srOKAY;
}


int doCommand()
{
    /**
     *	get command by char
     */
    do {
        printf("Enter Command: ");
        fflush(stdin);
        fflush(stdout);
        gets(in_line);
        lineLen = strlen(in_line);
        inCol = 0;
        
    } while (!getWord());
    
    char cmd = word[0];
    int step_cnt = 0;
    int print_cnt;
    
    /**
     *	setup before execute command
     */
    switch (cmd) {
        case 't':
            TraceFlag = !TraceFlag;
            printf("Trace Now");
            if (TraceFlag) {
                printf(" ON.\n");
            }else {
                printf(" OFF\n");
            }
            break;
        case 'h':
            
            printf("Commands are: \n");
            printf("s(tep  <n>         Execute n (default 1) TM Instructions\n");
            printf("g(o                Execute TM Instructions Until HALT\n");
            printf("r(egs              Print Content of Registers\n");
            printf("i(Mem <b <n>>      Print n iMem Locations Starting at b\n");
            printf("d(Mem <b <n>>      Print n dMem Locations Starting at b\n");
            printf("t(race             Toggle Instruction Trace\n");
            printf("p(rint             Toggle Print of Total Instructions Executed (go only)\n");
            printf("c(lear             Reset Simulator for New Execution of Program\n");
            printf("h(elp              Cause This List of Command to be Printed\n");
            printf("q(uit              Terminate the Simulation\n");
            break;
        
        case 'p':
            InstructionCountFlag = !InstructionCountFlag;
            printf("Printing Instruction Count Now ");
            if (InstructionCountFlag) {
                printf("On.\n");
            } else {
                printf("OFF.\n");
            }
            break;
        case 's':
            if (atEOL()) {
                step_cnt = 1;
            } else if (getNum()) {
                step_cnt = abs(num);
            } else {
                printf("Step Count?\n");
            }
            break;
        case 'g':
            step_cnt = 1;
            break;
        case 'r':
            for (int i = 0; i < NO_REGS; i++) {
                printf("%1d: %4d\t\t", i, reg[i]);
                if (i%4 == 3) {
                    printf("\n");
                }
            }
            break;
        case 'i':
            print_cnt = 1;
            
            if (getNum()) {
                iLoc = num;
                if (getNum()) {
                    print_cnt = num;
                }
            }
            if (!atEOL()) {
                printf("Instruction Locations?\n");
            } else {
                /**
                 *	write print_cnt instructions from iLoc
                 */
                while (iLoc >= 0 && iLoc < IADDR_SIZE && print_cnt > 0) {
                    writeInstruction(iLoc);
                    iLoc++;
                    print_cnt--;
                }
            }
            break;
        case 'd':
            print_cnt = 1;

            if (getNum()) {
                dLoc = num;
                if (getNum()) {
                    print_cnt = num;
                }
            }
            if (!atEOL()) {
                printf("Data Locations?\n");
            } else {
                /**
                 *	print print_cnt dMem[] from dLoc
                 */
                while (dLoc >= 0 && dLoc < DADDR_SIZE && print_cnt > 0) {
                    printf("%5d: %5d\n", dLoc, dMem[dLoc]);
                    dLoc++;
                    print_cnt--;
                }
            }
            break;
            
        case 'c':
            resetMachine();
            break;
        case 'q':
            return FALSE;
            
        default:
            printf("Unknown Command %c.\n", cmd);
            break;
    }
    
    int stepResult = srOKAY;
    if (step_cnt > 0) {
        
        if (cmd == 'g') {
            step_cnt = 0;
            while (stepResult == srOKAY) {
                iLoc = reg[PC_REG];
                if (TraceFlag) {
                    writeInstruction(iLoc);
                }
                stepResult = stepTM();
                step_cnt++;
            }
            if (InstructionCountFlag) {
                printf("Number of Instruction Executed = %d\n", step_cnt);
            }
        } else {
            while (step_cnt > 0 && stepResult == srOKAY) {
                iLoc = reg[PC_REG];
                if (TraceFlag) {
                    writeInstruction(iLoc);
                }
                stepResult = stepTM();
                step_cnt--;
            }
        }
        printf("%s\n", stepResultTab[stepResult]);
    }
    return TRUE;
}

