//
//  util.c
//  TinyCompiler
//
//  Created by Jiaqi Yan on 1/31/14.
//  Copyright (c) 2014 littlepretty. All rights reserved.
//

#include <stdio.h>
#include "util.h"


void printToken(TokenType token, const char* tokenString)
{
    switch (token) {
        case IF:
        case THEN:
        case ELSE:
        case END:
        case REPEAT:
        case UNTIL:
        case READ:
        case WRITE:
            fprintf(listing, "reserved word: %s\n", tokenString);
            break;
            
        case ASSIGN:
            fprintf(listing, ":=\n");
            break;
        case LT:
            fprintf(listing, "<\n");
            break;
        case EQ:
            fprintf(listing, "=\n");
            break;
        case PLUS:
            fprintf(listing, "+\n");
            break;
        case MINUS:
            fprintf(listing, "-\n");
            break;
        case TIMES:
            fprintf(listing, "*\n");
            break;
        case OVER:
            fprintf(listing, "/\n");
            break;
        case LPAREN:
            fprintf(listing, "(\n");
            break;
        case RPAREN:
            fprintf(listing, ")\n");
            break;
        case SEMI:
            fprintf(listing, ";\n");
            break;
        case END_FILE:
            fprintf(listing, "EOF\n");
            break;
        
        case NUMBER:
            fprintf(listing, "NUMBER, val = %s\n", tokenString);
            break;
        case ID:
            fprintf(listing, "ID, name = %s\n", tokenString);
            break;
        case ERROR:
            fprintf(listing, "ERROR: %s\n", tokenString);
            break;
           
        default:
            fprintf(listing, "Unknown token: %s\n", tokenString);
            break;
    }
    
}