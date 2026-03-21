#ifndef __EVAL_H__
#define __EVAL_H__



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "util.h"
#include "ast.h"
#include "symbol.h"

struct ExpressionResult_ {
       int val;
       HashTable table;
};

typedef struct ExpressionResult_ ExpressionResult;


ExpressionResult interpExp(A_Exp exp, HashTable symbol_table);
HashTable interpExpList(A_ExpList expression_list, HashTable symbol_table, int print);
HashTable interpStatement(A_Stm statement, HashTable symbol_table);

HashTable interpProgram(A_Stm AST_root, HashTable symbol_table);

#endif
