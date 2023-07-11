#include <stdio.h>
#include <stdint.h>
#include "lexer.h"
#include "parser.h"
#include "emitter.h"

int main() {
   // printf() displays the string inside quotation
   printf("Hello, World!");
   hello_lexer();
   char* path = "../code.txt";
   char* p_text = NULL;
   int32_t size = 0;
   FILE* p_file;
   Lexer* code;
   char ch;
   Token* token;

   FILE *fp = fopen("emitted.txt", "w");
   emit(fp, "hahahahahaha");
   emit(fp, " ha viet hung ");
   emit(fp, " hihihihi");
   fclose(fp);
   //Lexer* code = (Lexer*)malloc(sizeof(Lexer));
   ParserToken* parser_token = (ParserToken*)malloc(sizeof(ParserToken));

   p_file = read_file(path);

   code = get_source(p_file);
   // print_code(code);
   printf("\n>>Printing test:\n");

   // token = get_token(code);
   // printf(">>token char: %s", token->text);
   // while (token->kind != EOF_t) {
   //    // printf("\n>>test1");
   //    printf("\nToken: %-10s", token2string(token->kind));
   //    printf("%-4s %-20s", "->", token->text);
   //    token = get_token(code);
   // }
   printf("\n>>Adding db for testing ...\n");



   printf("\n>>Starting parser ...\n");

   parser_program (parser_token, code);


     
   // add_declared_var(parser_token, "dcm");
   // add_declared_var(parser_token, "dcm_thangloz");
   // add_declared_var(parser_token, "ha viet hung");

   // IsDeclaredVariable(parser_token, "dcm");

   // add_gotoed_var(parser_token, "dcmmmmm");
   // add_gotoed_var(parser_token, "dcmmmm_thangloz");
   // add_gotoed_var(parser_token, " hunggg");




   // ch = peek(code);
   // // print_code(code);
   // // putchar(ch);
   // while (ch != '\0') {
   //    // printf("\n>>test1");
   //    ch = peek(code);
   //    printf("%c", code->cur_char);
   //    // putchar(code->cur_char);
   //    next_char(code);
   // }

   // size = get_file_size;
   // p_text = get_source(p_file);
   // size = calc_file_size(p_file);
   // printf(size);
   // print_char(p_text);

   // char* p_test = "hahahahaha";
   // printf(p_test);

   // if (p_file != NULL) {
   //    show_file(p_file);
   // } else {
   //    return 0;
   // }
   
   return 0;
}