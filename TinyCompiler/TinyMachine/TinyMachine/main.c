//
//  main.c
//  TinyMachine
//
//  Created by Littlepretty Yan on 2/11/14.
//  Copyright (c) 2014 littlepretty. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tm.h"

char pgmName[1024];
FILE *pgm;

int done;

int main(int argc, const char * argv[])
{
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        exit(1);
    }
    
    strcpy(pgmName, argv[1]);
    if (strchr(pgmName, '.') == NULL) {
        strcat(pgmName, ".tm");
    }
    pgm = fopen(pgmName, "r");
    if (pgm == NULL) {
        printf("File %s Not Found\n", pgmName);
        exit(1);
    }
    
    if (!readInstructions()) {
        exit(1);
    }
    
    printf("TM Simulation (Enter h for Help)...\n");
    
    do
    {
        done = ! doCommand();
        
    } while (!done);
    printf("Simulation Done.\n");
    
    return 0;

}

