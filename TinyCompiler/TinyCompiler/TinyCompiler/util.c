//
//  util.c
//  TinyCompiler
//
//  Created by Jiaqi Yan on 1/31/14.
//  Copyright (c) 2014 littlepretty. All rights reserved.
//

#include <stdio.h>
#include "util.h"


void printToken(TokenType token, const char* tokenString)
{
    switch (token) {
        case IF:
        case THEN:
        case ELSE:
        case END:
        case REPEAT:
        case UNTIL:
        case READ:
        case WRITE:
            fprintf(listing, "reserved word: %s\n", tokenString);
            break;
            
        case ASSIGN:
            fprintf(listing, ":=\n");
            break;
        case LT:
            fprintf(listing, "<\n");
            break;
        case EQ:
            fprintf(listing, "=\n");
            break;
        case PLUS:
            fprintf(listing, "+\n");
            break;
        case MINUS:
            fprintf(listing, "-\n");
            break;
        case TIMES:
            fprintf(listing, "*\n");
            break;
        case OVER:
            fprintf(listing, "/\n");
            break;
        case LPAREN:
            fprintf(listing, "(\n");
            break;
        case RPAREN:
            fprintf(listing, ")\n");
            break;
        case SEMI:
            fprintf(listing, ";\n");
            break;
        case END_FILE:
            fprintf(listing, "EOF\n");
            break;
        
        case NUMBER:
            fprintf(listing, "NUMBER, val = %s\n", tokenString);
            break;
        case ID:
            fprintf(listing, "ID, name = %s\n", tokenString);
            break;
        case ERROR:
            fprintf(listing, "ERROR: %s\n", tokenString);
            break;
           
        default:
            fprintf(listing, "Unknown token: %s\n", tokenString);
            break;
    }
    
}

TreeNode* newStmtNode(StmtKind kind)
{
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
    if (t == NULL) {
        fprintf(listing, "Out Of Memory Error At Line %d", line_no);
    } else {
        for (int i = 0 ; i < MAX_CHILDREN; i++) {
            t->children[i] = NULL;
        }
        t->sibling = NULL;
        t->node_kind = StmtK;
        t->kind.stmt = kind;
        t->line_no = line_no;
    }
    
    return t;
}

TreeNode* newExpNode(ExpKind kind)
{
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
    if (t == NULL) {
        fprintf(listing, "Out Of Memory Error At Line %d", line_no);
    } else {
        for (int i = 0; i < MAX_CHILDREN; i++) {
            t->children[i] = NULL;
        }
        t->sibling = NULL;
        t->node_kind = ExpK;
        t->kind.exp = kind;
        t->line_no = line_no;
        t->type = Void;
    }
    
    return t;
}

char* copyString(const char *s)
{
    if (s == NULL) {
        return NULL;
    }
    char *t = malloc(strlen(s) + 1);
    if (t == NULL) {
        fprintf(listing, "Out Of Memory Error At Line %d", line_no);
    } else {
        strcpy(t, s);
    }
    return t;
}

static int indent_no = 0;

#define INDENT (indent_no += 2)
#define UNINDENT (indent_no -= 2)

static void printIndentSpaces()
{
    for (int i = 0; i < indent_no; i++) {
        fprintf(listing, " ");
    }
}

void printTree(TreeNode *tree)
{
    INDENT;
    
    while (tree != NULL) {
        printIndentSpaces();
        
        if (tree->node_kind == StmtK) {
            switch (tree->kind.stmt) {
                case IfK:
                    fprintf(listing, "If\n");
                    break;
                case RepeatK:
                    fprintf(listing, "Repeat\n");
                    break;
                case AssignK:
                    fprintf(listing, "Assign to: %s\n", tree->attr.name);
                    break;
                case ReadK:
                    fprintf(listing, "Read: %s\n", tree->attr.name);
                    break;
                case WriteK:
                    fprintf(listing, "Write\n");
                    break;
                default:
                    fprintf(listing, "Unknown StmtNode Kind\n");
                    break;
            }
        } else if (tree->node_kind == ExpK) {
            switch (tree->kind.exp) {
                case OpK:
                    fprintf(listing, "Op: ");
                    printToken(tree->attr.op, "\0");
                    break;
                case ConstK:
                    fprintf(listing, "Const: %d\n", tree->attr.val);
                    break;
                case Idk:
                    fprintf(listing, "Id: %s\n", tree->attr.name);
                    break;
                default:
                    fprintf(listing, "Unknown ExpNode Kind\n");

                    break;
            }
        } else {
            fprintf(listing, "Unknown Node Kind\n");
        }
        
        for (int i = 0; i < MAX_CHILDREN; i++) {
            printTree(tree->children[i]);
        }
        tree = tree->sibling;
        
    }
    UNINDENT;
}





















