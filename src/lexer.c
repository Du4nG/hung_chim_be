#include "lexer.h"

void hello_lexer() {
    printf("Hello from lexer");
}

FILE* read_file(char* path) {
    FILE *p_file = NULL;
    long size = 0;

    p_file = fopen(path, "r");
    fseek(p_file, 0, SEEK_SET);
    if (p_file == NULL) {
        printf(">>Cant open file!");
        return NULL;
    } else {
        printf("\n>>Reading file done!");
        return p_file;
    }
}

int show_file(FILE *ptr_file) {
    char ch;
    printf(">>Showing file ...");
    while ((ch = fgetc(ptr_file)) != EOF) {
        // printf("test1");
        putchar(ch);
    }
    fclose(ptr_file);
    printf("\n>>Showing DONE!!!");
    return 0;
}

int32_t calc_file_size (FILE* p_file) {
    int32_t size = 0;

    fseek(p_file, 0, SEEK_SET);
    fseek(p_file, 0, SEEK_END);
    size = ftell(p_file);
    printf("\nthe size is: ");
    printf("%d", size);
    fseek(p_file, 0, SEEK_SET);

    return size;
}

/**
 * get source code to char pointer 
*/
Lexer* get_source(FILE *p_file) {
    char* source;
    char ch;
    int32_t size = 0;
    int32_t i = 0;

    Lexer* code = (Lexer*)malloc(sizeof(Lexer));
    size = calc_file_size(p_file);
    code->size = size;
    printf("\nthe size is: ");
    printf("%d", code->size);
    code->source = (char*)malloc(sizeof(char)*size);
    printf(">>Getting source to char pointer");
    while ((ch = fgetc(p_file)) != EOF) {
        // printf("test1");
        code->source[i] = ch;
        // putchar(ch);
        i++;
    }

    // fclose(ptr_file);
    printf("\n>>Getting DONE!!!");
    printf("\nthe size is: ");
    printf("%d", code->size);
    code->cur_pos = 0;
    code->cur_char = code->source[0];
    return code;
}

/**
 * parsing source code
*/
void next_char(Lexer* code) {
    // char ch_return = "";
    code->cur_pos++;
    if (code->cur_pos >= code->size) {
        code->cur_char = '\0'; /*end of file*/
    } else {
        code->cur_char = code->source[code->cur_pos];
    }
    // return ch_return;
}

/**
 * peek to next char, return EOF if end of file, else return next char
*/
char peek(Lexer* code) {
    char ch_return;
    if ((code->cur_pos + 1) >= code->size) {
        printf("\n>>Out of source code");
        ch_return = '\0';
    } else {
        // printf("\n>>test2");
        // printf("\ncurrent pos is: "); //code->cur_pos
        // printf("%d", code->cur_pos);
        // printf("char is: %c", code->source[code->cur_pos + 1]);
        ch_return = code->source[code->cur_pos + 1];
        // printf("char is: %c", code->source[code->cur_pos + 1]);
    }
    return ch_return;
}


Token* get_token(Lexer* code) {
    Token* token = (Token*)malloc(sizeof(Token));
    token->text = (char*)malloc(3*sizeof(char));
    // token = NULL;
    char ch;
    uint8_t start_string = 0;
    uint8_t size_string = 0;
    uint8_t start_number = 0;
    uint8_t size_number = 0;
    uint8_t start_ident = 0;
    uint8_t size_ident = 0;
    uint8_t i = 0;
    // char ch_return;

    ch = code->cur_char;
    while (ch == ' ' || ch == '\t' || ch == '\r') {
        next_char(code);
        ch = code->cur_char;
        // printf("\n -> whitespace");
    }
    // printf("\n>>token char: %c", ch);
    if (ch == '+') {            /*Plus*/
        strcpy(token->text,"+");
        token->kind = PLUS;
    } else if (ch == '-') {     /*MINUS*/
        strcpy(token->text,"-");
        token->kind = MINUS;
    } else if (ch == '*') {     /*ASTERISK*/
        strcpy(token->text,"*");
        token->kind = ASTERISK;
    } else if (ch == '/') {     /*Slash*/
        strcpy(token->text,"/");
        token->kind = SLASH;
    } else if (ch == '\n') {    /*Newline*/
        strcpy(token->text,"\\n");
        token->kind = NEWLINE;
    } else if (ch == '\0') {    /*EOF*/
        strcpy(token->text,"\\0");
        token->kind = EOF_t;
    } else if (ch == '=') {     /*  check if "==" or "=" */
        if (peek(code) == '=') {
            next_char(code);
            strcpy(token->text, "==");
            token->kind = EQEQ;
        } else {
            strcpy(token->text, "=");
            token->kind = EQ;
        }
    } else if (ch == '>') {     /*  check if ">=" or ">" */
        if (peek(code) == '=') {
            next_char(code);
            strcpy(token->text, ">=");
            token->kind = GTEQ;
        } else {
            strcpy(token->text, ">");
            token->kind = GT;
        }
    } else if (ch == '<') {     /*  check if "<=" or "<" */
        if (peek(code) == '=') {
            next_char(code);
            strcpy(token->text, "<=");
            token->kind = LTEQ;
        } else {
            strcpy(token->text, "<");
            token->kind = LT;
        }
    } else if (ch == '!') {     /*  check if "!=" or "!" */
        if (peek(code) == '=') {
            next_char(code);
            strcpy(token->text, "!=");
            token->kind = NOTEQ;
        } else {
            printf("\n>>Get '!' only");
        }
    } else if (ch == 34) { /* " - quotation : detect string*/
        next_char(code);
        start_string = code->cur_pos;
        while (code->cur_char != 34) {
            if (code->cur_char == '\r' || code->cur_char == '\n' || code->cur_char == '%' ||
                code->cur_char == '\t' || code->cur_char == '\\' ) {
                printf("\n>>Invalid character in string");
            }
            next_char(code);
        }
        size_string = code->cur_pos - start_string;
        token->text = (char*)realloc(token->text, sizeof(char) * (1 + size_string));
        for(i = 0; i < size_string; i++) {
            token->text[i] = code->source[start_string + i];
        }
        token->text[i] = '\0';
        token->kind = STRING;
    } else if (isdigit((int8_t)ch)) { /* check is digit? */
        start_number = code->cur_pos;
        while (isdigit((int8_t)peek(code))) {
            next_char(code);
        }
        if (peek(code) == '.') {
            next_char(code);
            if (!isdigit((int8_t)peek(code))) {
                printf("\n>>Invalid character in number");
            } else {
                while (isdigit((int8_t)peek(code))) {
                    next_char(code);
                }
            }
        }
        size_number = code->cur_pos - start_number;
        token->text = (char*)realloc(token->text, sizeof(char) * (1 + size_string));
        for(i = 0; i <= size_number; i++) { /* peek is not number */
            token->text[i] = code->source[start_number + i];
        }
        token->text[i] = '\0';
        token->kind = NUMBER;
    } else if (isalpha(ch)) {
        start_ident = code->cur_pos;
        while (isalnum((uint8_t)peek(code)) || '_' == peek(code)) {
            next_char(code);
        }
        size_ident = code->cur_pos - start_ident;
        token->text = (char*)realloc(token->text, sizeof(char) * (1 + size_ident));
        for(i = 0; i <= size_ident; i++) { /* peek is not number */
            token->text[i] = code->source[start_ident + i];
        }
        token->text[i] = '\0';
        if (iskeyword(token->text) == 0) {
            token->kind = IDENT;
        } else {
            token->kind = iskeyword(token->text);
        }
    } else {                    /*unknow token*/
        printf("\n>>Unknow token");
    }
    next_char(code);
    return token;
    
}

uint8_t iskeyword (char* string) {
    uint8_t return_val = 0;
    if (strcmp(string, "LABEL") == 0) { /*equal*/
        return_val = LABEL;
    } else if (strcmp(string, "GOTO") == 0) { /*equal*/
        return_val = GOTO;
    } else if (strcmp(string, "PRINT") == 0) { /*equal*/
        return_val = PRINT;
    } else if (strcmp(string, "INPUT") == 0) { /*equal*/
        return_val = INPUT;
    } else if (strcmp(string, "LET") == 0) { /*equal*/
        return_val = LET;
    } else if (strcmp(string, "IF") == 0) { /*equal*/
        return_val = IF;
    } else if (strcmp(string, "THEN") == 0) { /*equal*/
        return_val = THEN;
    } else if (strcmp(string, "ENDIF") == 0) { /*equal*/
        return_val = ENDIF;
    } else if (strcmp(string, "WHILE") == 0) { /*equal*/
        return_val = WHILE;
    } else if (strcmp(string, "REPEAT") == 0) { /*equal*/
        return_val = REPEAT;
    } else if (strcmp(string, "ENDWHILE") == 0) { /*equal*/
        return_val = ENDWHILE;
    } else {
        return_val = 0;
    }
    return return_val;
}

void print_code(Lexer* code) {
    printf(">>Printing code ...");
    uint32_t i = 0;

    printf("\nthe size is: ");
    printf("%d", code->size);
    printf("\ncurrent pos is: "); //code->cur_pos
    printf("%d", code->cur_pos);
    printf("\n>>Printing: ...\n");
    for(i = 0; i < code->size; i++) {
        putchar(code->source[i]);
    }
}

void print_char (char* p_text) {
    int i = 0;

    for(i = 0; p_text[i] != '\0'; i++)
        printf("str[%d] = %c\n",i,*(p_text+i));
}

char* token2string (int16_t token) {
    switch (token) {
        case EOF_t: return "EOF_t";
        case NEWLINE: return "NEWLINE";
        case NUMBER: return "NUMBER";
        case IDENT: return "IDENT";
        case STRING: return "STRING";
        /*Keywords*/
        case LABEL: return "LABEL";
        case GOTO: return "GOTO";
        case PRINT: return "PRINT";
        case INPUT: return "INPUT";
        case LET: return "LET";
        case IF: return "IF";
        case THEN: return "THEN";
        case ENDIF: return "ENDIF";
        case WHILE: return "WHILE";
        case REPEAT: return "REPEAT";
        case ENDWHILE: return "ENDWHILE";
        /*Operators*/
        case EQ: return "EQ";
        case PLUS: return "PLUS";
        case MINUS: return "MINUS";
        case ASTERISK: return "ASTERISK";
        case SLASH: return "SLASH";
        case EQEQ: return "EQEQ";
        case NOTEQ: return "NOTEQ";
        case LT: return "LT";
        case LTEQ: return "LTEQ";
        case GT: return "GT";
        case GTEQ: return "GTEQ";
        default: return "unknow!";
    }
}

// char* init_source (Lexer* source) {
//     source->source = 
// }

// char* next_char (Lexer* source) {
    
// }


