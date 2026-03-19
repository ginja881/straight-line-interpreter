#include "ast.h"

A_Stm compound_stm(A_Stm stm1, A_Stm stm2) {
   /* 
    * @brief constructor for compound statement
    * @param stm1 first statement for compound statement
    * @param stm2 second statement for compound statement
    */
   A_Stm new_stm = (A_Stm) checked_malloc(sizeof(struct A_Stm_));
   new_stm->kind = CompoundStm;

   new_stm->u.compound_stm.stm1 = stm1;
   new_stm->u.compound_stm.stm2 = stm2;
   
   return new_stm;
}

A_Stm print_stm(A_ExpList exp_list) {
   /*
    * @brief constructor for print statements
    * @param exp_list expression list for print statement
    */

   A_Stm new_stm = (A_Stm) checked_malloc(sizeof(struct A_Stm_));
   new_stm->kind = PrintStm;

   new_stm->u.print_stm.exp_list = exp_list;
   
   return new_stm;
}

A_Stm assign_stm(A_Exp id_exp, A_Exp exp) {
   /*
    * @brief constructor for assignment statements
    * @param id identifier for variable for assignment
    * @param exp expression to make value to be stored in id variable
    */ 

   A_Stm new_stm =(A_Stm) checked_malloc(sizeof(struct A_Stm_));
   new_stm->kind = AssignStm;

   new_stm->u.assign_stm.id = id_exp->u.id_exp.id;
   new_stm->u.assign_stm.exp = exp;
   
  
   return new_stm;
}

A_Stm expression_stm(A_Exp exp) {
      /*
       * @brief constructor for expression statements
       * @param exp expression for expression statement
       */

       A_Stm new_stm = (A_Exp) checked_malloc(sizeof(struct A_Stm_));
       new_stm->kind = ExpStm;
      
       new_stm->u.expression_stm.exp = exp;
       return new_stm;
}

A_Exp id_exp(string id) {
    /*
     * @brief constructor for id expressions
     * @param id identifier for variable with identifier of "id"
     */

    A_Exp new_exp = (A_Exp) checked_malloc(sizeof(struct A_Exp_));
    new_exp->kind = ID_Exp;

    new_exp->u.id_exp.id = id;
    
    return new_exp;
}

A_Exp num_exp(int num) {
   /*
    * @brief constructor for numerical expressions
    * @param num numerical value of numerical expression
    */

   A_Exp new_exp = (A_Exp) checked_malloc(sizeof(struct A_Exp_));
   new_exp->kind = Num_Exp;

   new_exp->u.num_exp.num = num;

  
   return new_exp;
}

A_Exp eseq_exp(A_Stm stm, A_Exp exp) {
   /*
    * @brief  constructor for expression sequence
    * @param stm statement for expression sequence 
    * @param exp expression for expression sequence
    */

   A_Exp new_exp =(A_Exp) checked_malloc(sizeof(struct A_Exp_));
   new_exp->kind = Eseq_Exp;
   
   new_exp->u.eseq_exp.stm = stm;
   new_exp->u.eseq_exp.exp = exp;

   return new_exp;
}

A_Exp op_exp(A_Exp exp1, BinOp op, A_Exp exp2) {
  /*
   * @brief constructor for operation expressions
   * @param exp1 first expression for operation expression
   * @param exp2 second expression for operation expression
   * @param op operator enum
   */

  A_Exp new_exp = (A_Exp) checked_malloc(sizeof(struct A_Exp_));
  new_exp->kind = Op_Exp;
 
  new_exp->u.op_exp.exp1 = exp1;
  new_exp->u.op_exp.exp2 = exp2;
  new_exp->u.op_exp.op = op;

  return new_exp;
}

A_ExpList exp_list(A_Exp exp) {
    /*
     * @brief constructor for expression list
     * @param exp expression for expression list
     */

    A_ExpList new_explist = (A_ExpList) checked_malloc(sizeof(struct A_ExpList_));
    new_explist->exp = exp;
    new_explist->tail = NULL;

    return new_explist;
}

// Miscellaneous functions

int max_args_stm(A_Stm stm) {
     if (stm == NULL)
        return -1;
     
     if (stm->kind == PrintStm) {
        return max_args_exp(stm->u.print_stm.exp_list);
     }
     return -1; 
}
int max_args_exp(A_ExpList exp_list) {
    if (exp_list == NULL)
       return 0;

    A_Exp exp = exp_list->exp;
    acase SEMIssert(exp);

    if (exp->kind == Eseq_Exp && exp->u.eseq_exp.stm != NULL) {
         return 1 + max(max_args_stm(exp->u.eseq_exp.stm), max_args_exp(exp_list->tail));
    }
    return 1 + max_args_exp(exp_list->tail);
   
}

// Recursive Descent

int match(Token token_type, Lexer lexer) {
      return (lexer->token_head->token == token_type ? TRUE : FALSE);
}
A_Exp parse_exp(RawToken current_token, Lexer lexer) {
         if (current_token->token == ID)
	    return id_exp(current_token->text);
	 if (current_token->token == NUM)
	    return num_exp(atoi(current_token->text));

}
A_Stm parse_statement(RawToken current_token, Lexer lexer) {
      // Assignment
      A_Stm main_stm  = NULL;
      if (current_token->token == PRINT) {
           dequeue_token(lexer);
           A_ExpList main_list = parse_explist(current_token, lexer);
	   main_stm = print_stm(main_list);
      }
      else if (current_token->token == ID && match(ASSIGN, lexer) == TRUE) {
          A_Exp id_exp = parse_exp(curren_token, lexer);

	  dequeue_token(lexer);

	  A_Exp main_exp = parse_exp(peek(lexer), lexer);
	  
          main_stm = assignment_stm(id_exp, main_exp);
      }
      else {
           A_Exp main_exp = parse_exp(current_token, lexer);
	   main_stm = expression_stm(main_exp);
      }
      return main_stm;
}

A_Stm parse_source_code(Lexer lexer) {
    /*
     * @brief main implementation of parser
     * @param lexer lexical analyzer object for token stream
     */
    // Root of AST
    A_Stm root = NULL;
    A_Stm new_statement = NULL;
    
    // Current token
    RawToken current_token = peek(lexer);
    
    // Condition based off of EOF and running out of space
    while (is_queue_empty(lexer) == FALSE && current_token->token != END_OF_FILE) {
          // Consume token
          current_token = dequeue_token(lexer);
          
	  // Make new statement and check to see if next 
	  new_statement = parse_statement(current_token, lexer);
	 
          // Check to make new compound statement
          if (match(SEMI_COLON, lexer) == TRUE)
	      current_token = dequeue_token(lexer);
	  else 
              error(SYNTAX_ERROR, current_token->pos);
          
	  // Check if root is null
	  if (root == NULL) {
	      root = compound_stm(left, NULL);
	  }
	  else {
	      // Construct through cascading
	      root = compound_stm(root, new_statement);
	  }

    }
    return root;
}


