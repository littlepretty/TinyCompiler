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


int line_no = 0;


FILE *source;
FILE *listing;
FILE *code;

int TraceScan = TRUE;
int TraceParse = TRUE;
int TraceAnalyze = TRUE;
int TraceCode = TRUE;
int EchoSource = TRUE;

int main(int argc, const char * argv[])
{
    
    char pgm[128];
    if (argc != 2) {
        fprintf(stderr, "usage: %s <filename>\n", argv[0]);
    }
    
    strcpy(pgm, argv[1]);
    if (strchr(pgm, '.') == NULL) {
        strcat(pgm, ".tny");
    }
    
    source = fopen(pgm, "r");
    
    if (source == NULL) {
        fprintf(stderr, "File %s not found\n", pgm);
        exit(1);
    }
    
    listing = stdout;
    fprintf(listing, "\nTINY COMPILATION: %s\n", pgm);
    
    while (getToken() != END_FILE);
    
    
    
    return 0;
}

