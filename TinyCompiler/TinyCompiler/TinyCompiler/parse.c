//
//  parse.c
//  TinyCompiler
//
//  Created by Jiaqi Yan on 2/2/14.
//  Copyright (c) 2014 littlepretty. All rights reserved.
//

#include <stdio.h>
#include "parse.h"
#include "scan.h"
#include "util.h"

static TokenType token;

static void syntaxError(char *message)
{
    fprintf(listing, "\n>>> ");
    fprintf(listing, "Syntax Error at Line %d: %s", line_no, message);
    Error = TRUE;
}

static void match(TokenType expected)
{
    if (token == expected) {
        token = getToken();
    } else {
        syntaxError("Unexpected Token -> ");
        printToken(token, tokenString);
        fprintf(listing, "\t\t");
    }
}

static TreeNode* stmt_sequence();
static TreeNode* statement();
static TreeNode* if_stmt();
static TreeNode* repeat_stmt();
static TreeNode* assign_stmt();
static TreeNode* read_stmt();
static TreeNode* write_stmt();
static TreeNode* exp();
static TreeNode* simple_exp();
static TreeNode* term();
static TreeNode* factor();

static TreeNode* stmt_sequence()
{
    TreeNode *t = statement();
    TreeNode *temp = t;
    
    while (token != END_FILE && token != UNTIL
           && token != ELSE && token != END) {
        
        match(SEMI);
        TreeNode *q = statement();
        if (q != NULL) {
            if (t == NULL) {
                t = temp = q;
            } else {
                temp->sibling = q;
                temp = q;
            }
            
        }
    }
    return t;
}

static TreeNode* statement()
{
    TreeNode *t = NULL;
    switch (token) {
        case IF:
            t = if_stmt();
            break;
        case REPEAT:
            t = repeat_stmt();
            break;
        case ID:
            t = assign_stmt();
            break;
        case READ:
            t = read_stmt();
            break;
        case WRITE:
            t = write_stmt();
            break;
        default:
            syntaxError("Unexpected Token -> ");
            printToken(token, tokenString);
            token = getToken();
            break;
    }
    return t;
}

static TreeNode* if_stmt()
{
    TreeNode *t = newStmtNode(IfK);
    match(IF);
    if (t != NULL) {
        t->children[0] = exp();
    }
    match(THEN);
    if (t != NULL) {
        t->children[1] = stmt_sequence();
    }
    if (token == ELSE) {
        match(ELSE);
        if (t != NULL) {
            t->children[2] = stmt_sequence();
        }
    }
    match(END);
    
    return t;
    
}

static TreeNode* repeat_stmt()
{
    TreeNode *t = newStmtNode(RepeatK);
    match(REPEAT);
    if (t != NULL) {
        t->children[0] = stmt_sequence();
    }
    match(UNTIL);
    if (t != NULL) {
        t->children[1] = exp();
    }
    return t;
}

static TreeNode* assign_stmt()
{
    TreeNode *t = newStmtNode(AssignK);
    if (t != NULL && token == ID) {
        t->attr.name = copyString(tokenString);
    }
    match(ID);
    match(ASSIGN);
    if (t != NULL) {
        t->children[0] = exp();
    }
    return t;
}

static TreeNode* read_stmt()
{
    TreeNode *t = newStmtNode(ReadK);
    
    match(READ);
    
    if (t != NULL && token == ID) {
        t->attr.name = copyString(tokenString);
    }
    match(ID);

    return t;
}

static TreeNode* write_stmt()
{
    TreeNode *t = newStmtNode(WriteK);
    
    match(WRITE);
    if (t != NULL) {
        t->children[0] = exp();
    }
    return t;
}

static TreeNode* exp()
{
    TreeNode *t = simple_exp();
    if (t != NULL && (token == LT || token ==EQ)) {
        TreeNode* p = newExpNode(OpK);
        if (p != NULL) {
            p->children[0] = t;
            p->attr.op = token;
            t = p;
        }
        match(token);
        if (t != NULL) {
            t->children[1] = simple_exp();
        }
    }
    return t;

}

static TreeNode* simple_exp()
{
    TreeNode *t = term();
    while (token == PLUS || token == MINUS) {
        TreeNode* p = newExpNode(OpK);
        if (p != NULL) {
            p->children[0] = t;
            p->attr.op = token;
            t = p;
        }
        match(token);
        if (t != NULL) {
            t->children[1] = term();
        }
    }
    return t;
    
}
static TreeNode* term()
{
    TreeNode *t = factor();
    while (token == TIMES || token == OVER) {
        TreeNode *p = newExpNode(OpK);
        if (p != NULL) {
            p->children[0] = t;
            p->attr.op = token;
            t = p;
        }
        match(token);
        if (t != NULL) {
            t->children[1] = factor();
        }
    }
    return t;
}
static TreeNode* factor()
{
    TreeNode *t = NULL;
    
    if (token == LPAREN) {
        match(LPAREN);
        t = exp();
        match(RPAREN);
        
    } else if (token == NUMBER) {
        t = newExpNode(ConstK);
        if (t != NULL) {
            t->attr.val = atoi(tokenString);
            match(NUMBER);
        }
    } else if (token == ID) {
        t = newExpNode(Idk);
        if (t != NULL) {
            t->attr.name = copyString(tokenString);
            match(ID);
        }
    } else {
        syntaxError("Unexpected Token -> ");
        printToken(token, tokenString);
        token = getToken();
    }
    return t;
    
}

TreeNode *parse()
{
    TreeNode *t;
    token = getToken();
    t = stmt_sequence();
    if (token != END_FILE) {
        syntaxError("Code End Before File");
    }
    return t;
}




