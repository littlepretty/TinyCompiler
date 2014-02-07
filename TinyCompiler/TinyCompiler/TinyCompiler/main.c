//
//  main.c
//  TinyCompiler
//
//  Created by Jiaqi Yan on 1/31/14.
//  Copyright (c) 2014 littlepretty. All rights reserved.
//

#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"
#include "analyze.h"

#include "symtab.h"


#define NO_PARSE FALSE
#define NO_ANALYZE FALSE
#define NO_CODE FALSE

int line_no = 0;

FILE *source;
FILE *listing;
FILE *code;

int TraceScan = TRUE;
int TraceParse = TRUE;
int TraceAnalyze = TRUE;
int TraceCode = TRUE;
int EchoSource = TRUE;
int Error = FALSE;

int main(int argc, const char * argv[])
{
    
    char pgm[128];
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <Filename>\n", argv[0]);
    }
    
    strcpy(pgm, argv[1]);
    if (strchr(pgm, '.') == NULL) {
        strcat(pgm, ".tny");
    }
    
    source = fopen(pgm, "r");
    
    if (source == NULL) {
        fprintf(stderr, "File %s Not Found\n", pgm);
        exit(1);
    }
    
    listing = stdout;
    fprintf(listing, "\nTINY COMPILATION: %s\n", pgm);
    
#if NO_PARSE
    while (getToken() != END_FILE); //  first pass for scan
#else
    
    TreeNode *syntaxTree = parse();
    
    if (syntaxTree) {
        fprintf(listing, "\nSyntax Tree:\n");
        printTree(syntaxTree);
    }
    
#if NO_ANALYZE
#else
    if (!Error) {
        fprintf(listing, "\nBuilding Symbol Table...\n");
        buildSymTab(syntaxTree);
        fprintf(listing, "\nChecking Types...\n");
        typeCheck(syntaxTree);
        fprintf(listing, "\nType Checking Finished\n");
    }
    
#endif

#endif
    
    fclose(source);
    
    return 0;
}

