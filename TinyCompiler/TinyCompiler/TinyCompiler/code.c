//
//  code.c
//  TinyCompiler
//
//  Created by Littlepretty Yan on 2/11/14.
//  Copyright (c) 2014 littlepretty. All rights reserved.
//

#include <stdio.h>
#include "globals.h"
#include "code.h"

/**
 *	Location number for current instruction emission
 */
static int emitLocation = 0;

/**
 *	Highest location emitted so far
 */
static int highEmitLocation = 0;

void emitComment(char *c)
{
    if (TraceCode) {
        fprintf(code, "* %s\n", c);
    }
}

void emitRO(char *op, int r, int s, int t, char *c)
{
    fprintf(code, "%3d: %5s %d,%d,%d ", emitLocation++, op, r, s, t);
    if (TraceCode) {
        fprintf(code, "\t%s",c);
    }
    fprintf(code, "\n");
    if (highEmitLocation < emitLocation) {
        highEmitLocation = emitLocation;
    }
}

void emitRM(char *op, int r, int d, int s, char *c)
{
    fprintf(code, "%3d: %5s %d,%d(%d) ", emitLocation++, op, r, d, s);
    if (TraceCode) {
        fprintf(code, "\t%s",c);
    }
    fprintf(code, "\n");
    if (highEmitLocation < emitLocation) {
        highEmitLocation = emitLocation;
    }
}

int emitSkip(int steps)
{
    int i = emitLocation;       //!<    save current location
    emitLocation += steps;
    if (highEmitLocation < emitLocation) {
        highEmitLocation = emitLocation;
    }
    return i;
}


void emitBackup(int loc)
{
    if (loc > highEmitLocation) {
        emitComment("Bug in emitBackup");
    }
    emitLocation = loc;
}

void emitRestore()
{
    emitLocation = highEmitLocation;
}

void emitRM_Abs(char *op, int r, int a, char *c)
{
    fprintf(code, "%3d: %5s %d,%d(%d)", emitLocation, op, r, a-(emitLocation+1), pc);
    ++emitLocation;
    if (TraceCode) {
        fprintf(code, "\t%s", c);
    }
    fprintf(code, "\n");
    if (highEmitLocation < emitLocation) {
        highEmitLocation = emitLocation;
    }
}



































