//
//  cgen.c
//  TinyCompiler
//
//  Created by Littlepretty Yan on 2/11/14.
//  Copyright (c) 2014 littlepretty. All rights reserved.
//

#include <stdio.h>
#include "cgen.h"
#include "symtab.h"
#include "code.h"

static int tmpOffset = 0;

static void cGen(TreeNode *tree);

static void genStmt(TreeNode *tree)
{
    TreeNode *p0, *p1, *p2;
    int savedLocation1, savedLocation2, currentLocation;
    int loc;
    switch (tree->kind.stmt) {
        case IfK:   // if-stmt need back up patch twice!
            if (TraceCode) {
                emitComment("-> if");
            }
            p0 = tree->children[0];
            p1 = tree->children[1];
            p2 = tree->children[2];
            
            /**
             *	generate for test expression
             */
            cGen(p0);
            savedLocation1 = emitSkip(1);   // skip JEQ
            emitComment("if: jump to else belongs here");
            
            /**
             *	generate for then part
             */
            cGen(p1);
            savedLocation2 =emitSkip(1);    // skip LDA
            emitComment("if: jump to end belongs here");
            
            /**
             *	back up patch jump instruction and address before then part
             */
            currentLocation = emitSkip(0);
            emitBackup(savedLocation1);
            emitRM_Abs("JEQ", ac, currentLocation, "if: jump to else");
            emitRestore();
            
            /**
             *	generate for else part
             */
            cGen(p2);
            
            /**
             *	back up patch jump instruction and address after then part
             */
            currentLocation = emitSkip(0);
            emitBackup(savedLocation2);
            emitRM_Abs("LDA", pc, currentLocation, "if: jump to end");
            emitRestore();
            
            if (TraceCode) {
                emitComment("<- if");
            }
            break;
            
        case RepeatK:
            /* repeat-stmt do not need to back up patch, just save the address for repeat body and emit JEQ to that address at last */
            if (TraceCode) {
                emitComment("-> repeat");
            }
            p0 = tree->children[0];
            p1 = tree->children[1];

            savedLocation1 = emitSkip(0);   // get the address for repeat body
            emitComment("repeat: jump after body comes back here");
            /**
             *	generate repeat body
             */
            cGen(p0);
            /**
             *	generate repeat test
             */
            cGen(p1);
            emitRM_Abs("JEQ", ac, savedLocation1, "repeat: jump back to body");
            
            if (TraceCode) {
                emitComment("<- repeat");
            }
            break;
            
        case AssignK:
            if (TraceCode) {
                emitComment("-> assign");
            }
            /**
             *	generate code for rhs
             */
            cGen(tree->children[0]);
            
            /**
             *	emit store instruction: save value in ac to loc
             */
            loc = st_lookup(tree->attr.name);
            emitRM("ST", ac, loc, gp, "assign: store value");
            if (TraceCode) {
                emitComment("<- assign");
            }
            break;
            
        case ReadK:
            if (TraceCode) {
                emitComment("-> read");
            }
            
            emitRO("IN", ac, 0, 0, "read integer value");
            loc = st_lookup(tree->attr.name);
            emitRM("ST", ac, loc, gp, "read: store value");
            
            if (TraceCode) {
                emitComment("<- read");
            }
            break;
        case WriteK:
            if (TraceCode) {
                emitComment("-> write");
            }
            
            cGen(tree->children[0]);
            emitRO("OUT", ac, 0, 0, "write ac");
            
            if (TraceCode) {
                emitComment("<- write");
            }
            break;
            
        default:
            break;
    }
}

static void genExp(TreeNode *tree)
{
    int loc;
    TreeNode *p0, *p1;
    
    switch (tree->kind.exp) {
        case ConstK:
            if (TraceCode) {
                emitComment("-> Const");
            }
            
            emitRM("LDC", ac, tree->attr.val, 0, "load const");
            
            if (TraceCode) {
                emitComment("<- Const");
            }
            break;
            
        case Idk:
            if (TraceCode) {
                emitComment("-> Id");
            }
            
            loc = st_lookup(tree->attr.name);
            emitRM("LD", ac, loc, gp, "load id value");
            
            if (TraceCode) {
                emitComment("<- Id");
            }
            break;
            
        case OpK:
            
            if (TraceCode) {
                emitComment("-> Op");
            }
            
            p0 = tree->children[0];
            p1 = tree->children[1];
            
            cGen(p0);
            emitRM("ST", ac, tmpOffset--, mp, "op: store left as tmp variable");
            
            cGen(p1);   // this will flush ac with right operator
            emitRM("LD", ac1, ++tmpOffset, mp, "op: load left to register ac1");
            
            switch (tree->attr.op) {
                case PLUS:
                    emitRO("ADD", ac, ac1, ac, "op +");
                    break;
                    
                case MINUS:
                    emitRO("SUB", ac, ac1, ac, "op -");
                    break;
                    
                case TIMES:
                    emitRO("MUL", ac, ac1, ac, "op *");
                    break;
                    
                case OVER:
                    emitRO("DIV", ac, ac1, ac, "op /");
                    break;
                    
                case LT:
                    emitRO("SUB", ac, ac1, ac, "op <");     // do sub
                    emitRM("JLT", ac, 2, pc, "br if true"); // jump to true case if <
                    emitRM("LDC", ac, 0, ac, "false case"); // load 0 to ac
                    emitRM("LDA", pc, 1, pc, "unconditional jump"); // skip true case
                    emitRM("LDC", ac, 1, ac, "true case");  // load 1 to ac
                    break;
                    
                case EQ:
                    emitRO("SUB", ac, ac1, ac, "op ==");
                    emitRM("JEQ", ac, 2, pc, "br if true"); // jump to true case if ==
                    emitRM("LDC", ac, 0, ac, "false case"); // load 0 to ac
                    emitRM("LDA", pc, 1, pc, "unconditional jump"); // skip true case
                    emitRM("LDC", ac, 1, ac, "true case");  // load 1 to ac
                    break;
                    
                default:
                    emitComment("Bug: Unknown Operator");
                    break;
            }
            
            if (TraceCode) {
                emitComment("<- Op");
            }
            break;
            
        default:
            break;
    }
    
}

static void cGen(TreeNode *tree)
{
    if (tree != NULL) {
        switch (tree->node_kind) {
            case StmtK:
                genStmt(tree);
                break;
            case ExpK:
                genExp(tree);
                break;
                
            default:
                break;
        }
        cGen(tree->sibling);
    }
}

void codeGen(TreeNode *syntaxTree, char* codeFile)
{
    char *s = malloc(strlen(codeFile) + 7);
    strcpy(s, "File: ");
    strcat(s, codeFile);
    
    emitComment("TINY Compilation to TM Code");
    emitComment(s);
    
    emitComment("Standard Prelude:");
    emitRM("LD", mp, 0, ac, "load max address from memory location 0");
    emitRM("ST", ac, 0, ac, "clear loaction 0");
    emitComment("End of Standard Prelude.");
    
    cGen(syntaxTree);
    
    emitComment("End of Execution.");
    emitRO("HALT", 0, 0, 0, "");
    
    free(s);
    
}


