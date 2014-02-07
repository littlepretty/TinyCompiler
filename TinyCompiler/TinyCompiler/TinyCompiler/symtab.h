//
//  symtab.h
//  TinyCompiler
//
//  Created by Littlepretty Yan on 2/7/14.
//  Copyright (c) 2014 littlepretty. All rights reserved.
//

#ifndef TinyCompiler_symtab_h
#define TinyCompiler_symtab_h

void st_insert(char *name, int lineno, int loc);

int st_lookup(char *name);

void printSymTab(FILE *listing);

#endif
