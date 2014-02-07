//
//  analyze.c
//  TinyCompiler
//
//  Created by Littlepretty Yan on 2/7/14.
//  Copyright (c) 2014 littlepretty. All rights reserved.
//

#include <stdio.h>
#include "analyze.h"
#include "symtab.h"
#include "globals.h"

static int location = 0;

static void traverse(TreeNode *t, void(* preFunc)(TreeNode *), void(*postFunc)(TreeNode *))
{
    if (t != NULL) {
        
        preFunc(t);
        
        for (int i =0 ; i < MAX_CHILDREN; i++) {
            traverse(t->children[i], preFunc, postFunc);
        }
        
        postFunc(t);
        traverse(t->sibling, preFunc, postFunc);
    }
}

static void nullFunc(TreeNode *t)
{
    if (t == NULL) {
        return;
    } else {
        return;
    }
    
}

static void insertNode(TreeNode *t)
{
    switch (t->node_kind) {
        case StmtK:
            switch (t->kind.stmt) {
                case AssignK:
                case ReadK:
                    if (st_lookup(t->attr.name) == -1) {
                        st_insert(t->attr.name, t->line_no, location++);
                    } else {
                        st_insert(t->attr.name, t->line_no, 0);
                    }
                    break;
                    
                default:
                    break;
            }
            break;
        case ExpK:
            switch (t->kind.exp) {
                case Idk:
                    if (st_lookup(t->attr.name) == -1) {
                        st_insert(t->attr.name, t->line_no, location++);
                    } else {
                        st_insert(t->attr.name, t->line_no, 0);
                    }
                    
                    break;
                    
                default:
                    break;
            }
            break;
        default:
            break;
    }
}


void buildSymTab(TreeNode *tree)
{
    traverse(tree, insertNode, nullFunc);
    if (TraceAnalyze) {
        fprintf(listing, "\nSymbol Table:\n");
        printSymTab(listing);
    }
}

void typeError(TreeNode *t, char *message)
{
    fprintf(listing, "Type Error at Line %d: %s\n", t->line_no, message);
    Error = TRUE;
}

void checkNode(TreeNode *t)
{
    switch (t->node_kind) {
        case StmtK:
            switch (t->kind.stmt) {
                case IfK:
                    if (t->children[0]->type != Boolean) {
                        typeError(t->children[0], "If Test Is Not Boolean Type");
                    }
                    break;
                case AssignK:
                    if (t->children[0]->type != Integer) {
                        typeError(t->children[0], "Assignment of Non Integer Value");
                    }
                    break;
                case WriteK:
                    if (t->children[0]->type != Integer) {
                        typeError(t->children[0], "Write of Non Integer Value");
                    }
                    break;
                case RepeatK:
                    if (t->children[1]->type != Boolean) {
                        typeError(t->children[0], "Repeat Test Is Not Boolean Type");
                    }
                    break;
                default:
                    break;
            }
            
            break;
        case ExpK:
            switch (t->kind.exp) {
                case OpK:
                    if (t->children[0]->type != Integer || t->children[1]->type != Integer) {
                        typeError(t, "Operator Applied to Non Integer");
                    }
                    if (t->attr.op == EQ || t->attr.op == LT) {
                        t->type = Boolean;
                    } else {
                        t->type = Integer;
                    }
                    break;
                case ConstK:
                case Idk:
                    t->type = Integer;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

void typeCheck(TreeNode *tree)
{
    traverse(tree, nullFunc, checkNode);
}





















