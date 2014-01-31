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

#define MAX_RESERVERED 8

typedef enum
{
    END_FILE, ERROR,
    IF, THEN, ELSE, END, REPEAT, UNTIL, READ, WRITE,
    ID, NUMBER,
    ASSIGN, EQ, LT, PLUS, MINUS, TIMES, OVER, LPAREN, RPAREN, SEMI
} TokenType;

extern FILE *source;
extern FILE *listing;
extern FILE *code;

extern int line_no;

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
    
    ExpType type;
    
} TreeNode;











#endif
