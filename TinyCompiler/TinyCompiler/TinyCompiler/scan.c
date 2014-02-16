//
//  scan.c
//  TinyCompiler
//
//  Created by Jiaqi Yan on 2/1/14.
//  Copyright (c) 2014 littlepretty. All rights reserved.
//

#include <stdio.h>
#include "scan.h"

#include "globals.h"
#include "util.h"

typedef enum {
    
    START, IN_COMMENT, IN_NUM, IN_ID, IN_ASSIGN, DONE

} StateType;

char tokenString[MAX_TOKEN_LENGTH + 1];

#define BUFFER_LENGTH 256

static char lineBuffer[BUFFER_LENGTH];
static int linePosition = 0;
static int bufferSize = 0;
static int EOF_flag = FALSE;

char getNextChar()
{
    if (!(linePosition < bufferSize)) {
        line_no++;
        
        if (fgets(lineBuffer, BUFFER_LENGTH-1, source)) {
            if (EchoSource) {
                fprintf(listing, "%4d: %s", line_no, lineBuffer);
            }
            bufferSize = (int)strlen(lineBuffer);
            linePosition = 0;
            return lineBuffer[linePosition++];
        } else {
            EOF_flag = TRUE;
            return EOF;
        }
    } else {
        return lineBuffer[linePosition++];
    }
}

void unGetNextChar()
{
    if (!EOF_flag) {
        linePosition--;
    }

}

static struct {
    char *str;
    TokenType token;
} reservedWords[MAX_RESERVERED_WORDS] = {
    
    {"if", IF}, {"else", ELSE}, {"then", THEN}, {"end", END},
    {"repeat", REPEAT}, {"until", UNTIL},
    {"read", READ}, {"write", WRITE}
};

TokenType reservedLookup(const char *tokenString)
{
    for (int i =0; i < MAX_RESERVERED_WORDS; i++) {
        if (!strcmp(tokenString, reservedWords[i].str)) {
            return reservedWords[i].token;
        }
    }
    return ID;
}

TokenType getToken()
{
    int tokenStringIndex = 0;
    TokenType currentToken;
    StateType state = START;
    
    int save;
    
    while (state != DONE) {
        
        char currentChar = getNextChar();
        save = TRUE;
        
        switch (state) {
            case START:
                if (isdigit(currentChar)) {
                    state = IN_NUM;
                } else if (isalpha(currentChar)) {
                    state = IN_ID;
                } else if (currentChar == ':') {
                    state = IN_ASSIGN;
                } else if (currentChar == '{') {
                    save = FALSE;
                    state = IN_COMMENT;
                } else if (currentChar == ' ' || currentChar == '\t'
                    || currentChar == '\n'){
                    save = FALSE;
                } else {
                    state = DONE;
                    switch (currentChar) {
                        case EOF:
                            save = FALSE;
                            currentToken = END_FILE;
                            break;
                        case '=':
                            currentToken = EQ;
                            break;
                        case '<':
                            currentToken = LT;
                            break;
                        case '+':
                            currentToken = PLUS;
                            break;
                        case '-':
                            currentToken = MINUS;
                            break;
                        case '*':
                            currentToken = TIMES;
                            break;
                        case '/':
                            currentToken = OVER;
                            break;
                        case '(':
                            currentToken = LPAREN;
                            break;
                        case ')':
                            currentToken = RPAREN;
                            break;
                        case ';':
                            currentToken = SEMI;
                            break;
                        default:
                            currentToken = ERROR;
                            break;
                    }
                }
                break;
                
            case IN_COMMENT:
                save = FALSE;
                if (currentChar == EOF) {
                    state = DONE;
                    currentToken = END_FILE;
                } else if (currentChar == '}') {
                    state = START;
                }
                break;
            case IN_ASSIGN:
                state = DONE;
                if (currentChar == '=') {
                    currentToken = ASSIGN;
                } else {
                    unGetNextChar();
                    currentToken = ERROR;
                    save = FALSE;
                }
                break;
                
            case IN_NUM:
                if (!isdigit(currentChar)) {
                    unGetNextChar();
                    save = FALSE;
                    state = DONE;
                    currentToken = NUMBER;
                }
                break;
                
            case IN_ID:
                if (!isalpha(currentChar)) {
                    unGetNextChar();
                    save = FALSE;
                    state = DONE;
                    currentToken = ID;
                }
            case DONE:
                break;
            default:
                fprintf(listing, "Scanner Bug: state = %d\n", state);
                state = DONE;
                currentToken = ERROR;
                break;
        }
        
        if (save && tokenStringIndex <= MAX_TOKEN_LENGTH) {
            tokenString[tokenStringIndex++] = currentChar;
        }
        if (state == DONE) {
            tokenString[tokenStringIndex] = '\0';
            if (currentToken == ID) {
                currentToken = reservedLookup(tokenString);
            }
        }
        
    }
    
    if (TraceScan) {
        fprintf(listing, "\t%d: ", line_no);
        printToken(currentToken, tokenString);
    }
    
    return currentToken;
}












