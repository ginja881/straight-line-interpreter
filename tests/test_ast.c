#include "util.h"
#include "ast.h"
#include "lex.h"

int main(void) {
     char* f_path = "tests/source_files/first.straight";
     FILE* fd = fopen(f_path, "r");
     Lexer lexer = make_lexer();
     lexer = read_tokens(lexer, fd);

     A_Stm root = parse_source_code(lexer);

     if (root->kind == AssignStm) {
           
           printf("\n %s \n", root->u.assign_stm.id);
	   if (root->u.assign_stm.exp->kind == Num_Exp)
	       printf("\n  IS NUM_EXP \n");
	   else if (root->u.assign_stm.exp->kind == ID_Exp)
	       printf("\n IS ID_EXP\n");
     }

     fclose(fd);
     return EXIT_SUCCESS;
}
