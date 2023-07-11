#ifndef PARSER_H
#define PARSER_H
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

typedef struct DeclaredVariable_t {
    char** declared_var;
    uint32_t decl_size;
    char** gotoed_var;
    uint32_t goto_size;
} DeclaredVariable;

typedef struct ParserToken_t {
    Token* cur_token;
    Token* pek_token;
    DeclaredVariable* db_variable; //variables database
} ParserToken;




uint8_t parser_check_token (ParserToken* parser_token, TokenType kind);
uint8_t parser_peek_token (ParserToken* parser_token, TokenType kind);
uint8_t parser_match (ParserToken* parser_token, Lexer* code, TokenType kind);
void parser_next_token (ParserToken* parser_token, Lexer* code);
uint8_t parser_program (ParserToken* parser_token, Lexer* code);
uint8_t parser_statement (ParserToken* parser_token, Lexer* code);
void comparison (ParserToken* parser_token, Lexer* code);
uint8_t IsComparisonOperator (ParserToken* parser_token, Lexer* code);
void expression (ParserToken* parser_token, Lexer* code);
void parser_nl (ParserToken* parser_token, Lexer* code);
void term (ParserToken* parser_token, Lexer* code);
void unary (ParserToken* parser_token, Lexer* code);
uint8_t primary (ParserToken* parser_token, Lexer* code);
uint32_t size(char *ptr);
void add_declared_var (ParserToken* parser_token, char* input_var);
void add_gotoed_var (ParserToken* parser_token, char* input_var);
uint8_t IsDeclaredVariable (ParserToken* parser_token, char* input_var);

#endif