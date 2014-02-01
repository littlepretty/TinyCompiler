//
//  scan.h
//  TinyCompiler
//
//  Created by Jiaqi Yan on 2/1/14.
//  Copyright (c) 2014 littlepretty. All rights reserved.
//

#ifndef TinyCompiler_scan_h
#define TinyCompiler_scan_h

#include "globals.h"

#define MAX_TOKEN_LENGTH 40

extern char tokenString[MAX_TOKEN_LENGTH + 1];

TokenType getToken();


#endif
