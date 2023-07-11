#include "parser.h"

uint8_t parser_check_token (ParserToken* parser_token, TokenType kind) {
    if (parser_token->cur_token->kind == kind) {
        return 1;
    }
    return 0;
}

uint8_t parser_peek_token (ParserToken* parser_token, TokenType kind) {
    if (parser_token->pek_token->kind == kind) {
        return 1;
    }
    return 0;
}

uint8_t parser_match (ParserToken* parser_token, Lexer* code, TokenType kind) {
    uint8_t return_val = 0;
    if (parser_check_token(parser_token, kind) == 0) {
        printf("\n! NOT match token");
        return_val = 0;
    } else {
        return_val = 1;
    }
    parser_next_token(parser_token, code);
    return return_val;

}

void parser_next_token (ParserToken* parser_token, Lexer* code) {
    // Token* lexer_token = (Token*)malloc(sizeof(Token));
    parser_token->cur_token = parser_token->pek_token;
    // lexer_token = get_token(code);
    // printf("char is: %s", lexer_token->text);
    // parser_token->pek_token = *lexer_token;
    parser_token->pek_token = get_token(code);
    // printf(" -> char is: %s", parser_token->pek_token->text);
    // parser_token->pek_token.kind = lexer_token->kind;
    // if (0) {
    //     printf("\nToken Lexer: %-10s", token2string(lexer_token->kind));
    //     printf("\nToken Current: %-10s", token2string(parser_token->cur_token));
    //     printf("\nToken Peek: %-10s\n", token2string(parser_token->pek_token));
    // }
    
}

void parser_init (ParserToken* parser_token, Lexer* code) {
    /*init*/
    // parser_token->db_variable->size = (uint32_t*)malloc(sizeof(uint32_t));
    /*mem allocation for database*/
    parser_token->db_variable = (DeclaredVariable*)malloc(sizeof(DeclaredVariable));
    parser_token->db_variable->decl_size = 0;
    parser_token->db_variable->declared_var = (char**)malloc(sizeof(char*));
    parser_token->db_variable->goto_size = 0;
    parser_token->db_variable->gotoed_var = (char**)malloc(sizeof(char*));
    /*****************************/

    printf("\n>> ...Initialize parser...");
    parser_next_token(parser_token, code);
    parser_next_token(parser_token, code);
}

uint8_t parser_program (ParserToken* parser_token, Lexer* code) {
    printf("\nPROGRAM");
    parser_init(parser_token, code);
    /*parser all the statements in the programs*/
    while (!parser_check_token(parser_token, EOF_t)) {
        parser_statement(parser_token, code);
    }

    /*Check that each label referenced in a GOTO is declared*/
    add_declared_var(parser_token, "dcm");
    add_declared_var(parser_token, "dcm_thangloz");
    add_declared_var(parser_token, "ha viet hung");

    add_gotoed_var(parser_token, "dcm");
    // add_gotoed_var(parser_token, "dcm_thanglozz");
    add_gotoed_var(parser_token, "ha viet hungg");

    uint32_t i = 0;
    uint32_t j = 0;
    uint32_t counter_checker = 0;
    for (i = 0; i < parser_token->db_variable->goto_size; i++) {
        for (j = 0; j < parser_token->db_variable->decl_size; j++) {
            if (strcmp( parser_token->db_variable->declared_var[j], 
                        parser_token->db_variable->gotoed_var[i]) == 0) {
                            counter_checker++;
                        }
        }
        if (counter_checker == 0) { /*not have any matching*/
            printf("\n>>GOTO and DECLARED not match!!!");
            printf("    >>in GOTO: %s", parser_token->db_variable->gotoed_var[i]);
            return 0;
        } else {
            counter_checker = 0;
        }
    }

    // parser_statement(parser_token, code);
    // parser_statement(parser_token, code);
    // parser_statement(parser_token, code);

}

uint8_t parser_statement (ParserToken* parser_token, Lexer* code) {
    /*check the first token: what is statement*/
    /* statement ::= "PRINT" (expression | string) nl */
    if (parser_check_token(parser_token, PRINT)) {
        printf("\nSTATEMENT - PRINT");
        parser_next_token(parser_token, code);
        if (parser_check_token(parser_token, STRING)) {
            parser_next_token(parser_token, code);
        } else {
            expression(parser_token, code);
        }
    } 
    /*"IF" comparison "THEN" nl {statement} "ENDIF" nl*/
    else if (parser_check_token(parser_token, IF)) {
        printf("\nSTATEMENT - IF");
        parser_next_token(parser_token, code);
        comparison(parser_token, code);
        parser_match (parser_token, code, THEN);
        parser_nl(parser_token, code);
        /* 0 or more than 1 statement in body*/
        while (!parser_check_token(parser_token, ENDIF)) {
            parser_statement(parser_token, code);
        }
        parser_match(parser_token, code, ENDIF);
    }
    /*"WHILE" comparison "REPEAT" nl {statement nl} "ENDWHILE" nl*/
    else if (parser_check_token(parser_token, WHILE)) {
        printf("\nSTATEMENT - WHILE");
        parser_next_token(parser_token, code);
        comparison(parser_token, code);
        parser_match (parser_token, code, REPEAT);
        parser_nl(parser_token, code);
        /* 0 or more than 1 statement in body*/
        while (!parser_check_token(parser_token, ENDWHILE)) {
            parser_statement(parser_token, code);
        }
        parser_match(parser_token, code, ENDWHILE);
    }
    /* "LABEL" ident */
    else if (parser_check_token(parser_token, LABEL)) {
        printf("\nSTATEMENT - LABEL");
        parser_next_token(parser_token, code);
        if (IsDeclaredVariable (parser_token, parser_token->cur_token->text)) {
            printf("\n>>Label already exists: %s", parser_token->cur_token->text);
            return 0;
        } else {
            add_declared_var(parser_token, parser_token->cur_token->text);
        }
        parser_match(parser_token, code, IDENT);
    }
    /* "GOTO" ident "=" */
    else if (parser_check_token(parser_token, GOTO)) {
        printf("\nSTATEMENT - GOTO");
        parser_next_token(parser_token, code);
        add_gotoed_var(parser_token, parser_token->cur_token->text);
        parser_match(parser_token, code, IDENT);
    }
    /* "LET" ident */
    else if (parser_check_token(parser_token, LET)) {
        printf("\nSTATEMENT - LET");
        parser_next_token(parser_token, code);
        /*if not declared*/
        if (IsDeclaredVariable(parser_token, parser_token->cur_token->text) == 0) {
            add_declared_var(parser_token, parser_token->cur_token->text);
        } 
        parser_match(parser_token, code, IDENT);
        parser_match(parser_token, code, EQ);
        expression(parser_token, code);
    }
    /* "INPUT" ident */
    else if (parser_check_token(parser_token, INPUT)) {
        printf("\nSTATEMENT - INPUT");
        parser_next_token(parser_token, code);
        if (IsDeclaredVariable(parser_token, parser_token->cur_token->text) == 0) {
            add_declared_var(parser_token, parser_token->cur_token->text);
        }
        parser_match(parser_token, code, IDENT);
    } else {
        printf("\nInvalid statement at ");
        printf(" %s ", parser_token->cur_token->text);
    }

    parser_nl(parser_token, code);
}

void comparison (ParserToken* parser_token, Lexer* code) {
    /* comparison ::= expression (("==" | "!=" | ">" | ">=" | "<" | "<=") expression) */
    printf("\nCOMPARISON");
    expression(parser_token, code);
    if (IsComparisonOperator(parser_token, code)) {
        parser_next_token(parser_token, code);
        expression(parser_token, code);
    } else {
        printf("Expected comparison operator at: %s", parser_token->cur_token->text);
    }
    while (IsComparisonOperator(parser_token, code)) {
        parser_next_token(parser_token, code);
        expression(parser_token, code);
    }
}

uint8_t IsComparisonOperator (ParserToken* parser_token, Lexer* code) {
    if (parser_check_token(parser_token, EQEQ)   
    ||  parser_check_token(parser_token, NOTEQ)     
    ||  parser_check_token(parser_token, LT)        
    ||  parser_check_token(parser_token, LTEQ)      
    ||  parser_check_token(parser_token, GTEQ)     
    ||  parser_check_token(parser_token, GT)) {
        return 1; 
    }
    return 0;
}

void expression (ParserToken* parser_token, Lexer* code) {
    /* expression ::= term {( "-" | "+" ) term} */
    printf("\nEXPRESSION");
    term(parser_token, code);
    while (parser_check_token(parser_token, MINUS) || parser_check_token(parser_token, PLUS)) {
        parser_next_token(parser_token, code);
        term(parser_token, code);
    }
}

void term (ParserToken* parser_token, Lexer* code) {
    /* term ::= unary {( "/" | "*" ) unary} */
    printf("\nTERM");
    unary(parser_token, code);
    while (parser_check_token(parser_token, SLASH) || parser_check_token(parser_token, ASTERISK)) {
        parser_next_token(parser_token, code);
        unary(parser_token, code);
    }
}

void unary (ParserToken* parser_token, Lexer* code) {
    /* unary ::= ["+" | "-"] primary */
    printf("\nUNARY");
    if (parser_check_token(parser_token, PLUS) 
    || parser_check_token(parser_token, MINUS)) {
        parser_next_token(parser_token, code);
    }
    primary(parser_token, code);
}

uint8_t primary (ParserToken* parser_token, Lexer* code) {
    /* # primary ::= number | ident */
    printf("\nPRIMARY");
    if (parser_check_token(parser_token, NUMBER)) {
        printf(" ( %s )", parser_token->cur_token->text);
        parser_next_token(parser_token, code);
    } else if (parser_check_token(parser_token, IDENT)) {
        printf(" ( %s )", parser_token->cur_token->text);
        /*Ensure variable is exist in database*/
        if (IsDeclaredVariable(parser_token, parser_token->cur_token->text) == 0) {
            return 0;
        }
        parser_next_token(parser_token, code);
    } else {
        printf("\n>>ERROR!!!");
        // return 0;
    }
    
}

void parser_nl (ParserToken* parser_token, Lexer* code) {
    printf("\nNEWLINE");
    parser_match (parser_token, code, NEWLINE);
    while (parser_check_token(parser_token, NEWLINE)) {
        parser_next_token(parser_token, code);
    }
    
}

//returns the size of a character array using a pointer to the first element of the character array
uint32_t size(char *ptr)
{
    //variable used to access the subsequent array elements.
    uint32_t offset = 0;
    //variable that counts the number of elements in your array
    uint32_t count = 0;

    //While loop that tests whether the end of the array has been reached
    while (*(ptr + offset) != '\0')
    {
        //increment the count variable
        ++count;
        //advance to the next element of the array
        ++offset;
    }
    //return the size of the array
    return count;
}

void add_declared_var (ParserToken* parser_token, char* input_var) {
    uint32_t index = parser_token->db_variable->decl_size;
    // parser_token->db_variable->size++;
    
    uint32_t size_word = ++parser_token->db_variable->decl_size; //size = size + 1
    printf("\n>> size: %d: ", parser_token->db_variable->decl_size);
    
    parser_token->db_variable->declared_var = (char**)realloc(parser_token->db_variable->declared_var, 
    (sizeof(char*) * size_word));
    parser_token->db_variable->declared_var[index] = (char*)malloc(sizeof(char) * size(input_var));
    parser_token->db_variable->declared_var[index] = input_var;
    printf(">> index %d: ", index);
    printf(" %s ", parser_token->db_variable->declared_var[index]);
}

void add_gotoed_var (ParserToken* parser_token, char* input_var) {
    uint32_t index = parser_token->db_variable->goto_size;
    // parser_token->db_variable->size++;
    
    uint32_t size_word = ++parser_token->db_variable->goto_size; //size = size + 1
    printf("\n>> size: %d: ", parser_token->db_variable->goto_size);
    
    parser_token->db_variable->gotoed_var = (char**)realloc(parser_token->db_variable->gotoed_var, 
    (sizeof(char*) * size_word));
    parser_token->db_variable->gotoed_var[index] = (char*)malloc(sizeof(char) * size(input_var));
    parser_token->db_variable->gotoed_var[index] = input_var;
    printf(">> index %d: ", index);
    printf(" %s ", parser_token->db_variable->gotoed_var[index]);
}

uint8_t IsDeclaredVariable (ParserToken* parser_token, char* input_var) {
    uint32_t size =  parser_token->db_variable->decl_size;
    uint32_t i = 0;

    for (i = 0; i < size; i++) {
        if (strcmp(parser_token->db_variable->declared_var[i], input_var) == 0) {
            printf(">>FOUND");
            return 1;
        }
    }
    printf(">>NOT FOUND");
    return 0;
}