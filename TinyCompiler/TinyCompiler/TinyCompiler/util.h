//
//  util.h
//  TinyCompiler
//
//  Created by Jiaqi Yan on 1/31/14.
//  Copyright (c) 2014 littlepretty. All rights reserved.
//

#ifndef TinyCompiler_util_h
#define TinyCompiler_util_h

#include "globals.h"


void printToken(TokenType token, const char* tokenString);

TreeNode* newExpNode(ExpKind kind);
TreeNode* newStmtNode(StmtKind kind);

char* copyString(const char *s);

void printTree(TreeNode *tree);



#endif
