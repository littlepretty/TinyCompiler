//
//  globals.h
//  TinyCompiler
//
//  Created by Jiaqi Yan on 1/31/14.
//  Copyright (c) 2014 littlepretty. All rights reserved.
//

#ifndef TinyCompiler_globals_h
#define TinyCompiler_globals_h

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define MAX_RESERVERED_WORDS 8


typedef enum
{

    END_FILE, ERROR,
    /*
     *  reserved words
     */
    IF, THEN, ELSE, END, REPEAT, UNTIL, READ, WRITE,
    /*
     *  multicharacter tokens
     */
    ID, NUMBER,
    /*
     *  operator and special symbols
     */
    ASSIGN, EQ, LT, PLUS, MINUS, TIMES, OVER, LPAREN, RPAREN, SEMI
    
} TokenType;

extern int line_no;

extern FILE *source;    //!<  file handler for program file

extern FILE *listing;   //!<  file handler for listing compile output

extern FILE *code;  //!<  file handler for code on TM simulator



/************************************************************/
/*                  syntax tree for parsing                 */
/************************************************************/

typedef enum {StmtK, ExpK} NodeKind;
typedef enum {IfK, RepeatK, AssignK, ReadK, WriteK} StmtKind;
typedef enum {OpK, ConstK, Idk} ExpKind;

typedef enum {Void, Integer, Boolean} ExpType;

#define MAX_CHILDREN 3

typedef struct treeNode
{
    struct treeNode *children[MAX_CHILDREN];
    struct treeNode *sibling;
    
    int line_no;
    NodeKind node_kind;
    
    union {
        StmtKind stmt; ExpKind exp;
    } kind;
    
    union {
        TokenType op;
        int val;
        char *name;
    } attr;
    
    ExpType type;   //!< for type checking
    
} TreeNode;


extern int EchoSource;

extern int TraceScan;

extern int TraceParse;

extern int TraceAnalyze;

extern int TraceCode;


#endif
