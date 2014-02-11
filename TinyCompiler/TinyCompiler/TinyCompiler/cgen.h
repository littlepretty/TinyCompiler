//
//  cgen.h
//  TinyCompiler
//
//  Created by Littlepretty Yan on 2/11/14.
//  Copyright (c) 2014 littlepretty. All rights reserved.
//

#ifndef TinyCompiler_cgen_h
#define TinyCompiler_cgen_h

#include "globals.h"

/**
 *	Generate code to file by traversal of the syntax tree
 *
 *	@param	syntaxTree	syntax tree
 *	@param	codeFile	file name 
 */
void codeGen(TreeNode *syntaxTree, char* codeFile);

#endif
