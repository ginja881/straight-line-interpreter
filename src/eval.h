#ifndef __EVAL_H__
#define __EVAL_H__



#include <stdio.h>
#include <string.h>
#include <ctypes.h>
#include <stdlib.h>
#include "util.h"
#include "ast.h"
#include "symbol.h"

struct ExpressionResult_ {
       int val;
       HashTable table;
};

typedef struct ExpressionResult_* ExpressionResult;

ExpressionResult interpExp(A_Exp exp);
HashTable interpExpList(A_ExpList expression_list);
HashTable interpStatement(A_Stm statement);

void interpProgram(A_Stm AST_root);

#endif
