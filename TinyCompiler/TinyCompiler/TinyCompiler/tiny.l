%{
	#include "global.h"
	#include "util.h"
	#include "scan.h"

char tokenString[MAX_TOKEN_LENGTH + 1];

%}

digit			[0-9]
number			{digit}+
letter			[A-Za-z]
identifier		{letter}+
newline			\n
whitespace		[ \t]

%%

"if"			{ return IF; }
"then"			{ return THEN; }
"else"			{ return ELSE; }
"end"			{ return END; }
"repeat"        { return REPEAT; }
"until"         { return UNTIL; }
"read"          { return READ; }
"write"         { return WRITE; }
":="            { return ASSIGN; }
"="             { return EQ; }
"<"             { return LT; }
"+"             { return PLUS; }
"-"             { return MINUS; }
"*"             { return TIMES; }
"/"             { return OVER; }
"("             { return LPAREN; }
")"             { return RPAREN; }
";"             { return SEMI; }
{number}        { return NUMBER; }
{identifier}    { return ID; }
{newline}       { line_no++; }
{whitespace}    { }
"{"             {
                    char c;
                    do{
                        c = input();
                        if (c == EOF){
                            break;
                        }
                        if (c == '\n'){
                            line_no++;
                        }
                    } while(c != "}")
                }
.               { return ERROR; }

%%

TokenType getToken(void)
{
    static int firstTime = TRUE;
    TokenType currentToken;
    if (firstTime) {
        firstTime = FALSE;
        line_no++;
        yyin = source;
        yyout = listing;
    }
    currentToken = yylex();
    strncpy(tokenString, yytext, MAX_TOKEN_LENGTH);
    if (TraceScan) {
        fprintf(listing, "\t%d ", line_no);
        printToken(currentToken, tokenString);
    }
    return currentToken;
}















