#ifndef __ERROR_MESSAGES__
#define __ERROR_MESSAGES__

#include <map>

enum ErrorMessage {
  INVALID_TYPE=0,
  ARRAY_BAD_INDEX,
  NON_CONSTANT,
  INVALID_SYMBOL,
  SYMBOL_CAST,
  WHILE_PREDICATE_ERROR,
  IF_PREDICATE_ERROR,
  PROCEDURE_RETURN_ERROR,
  FUNCTION_BAD_RETURN_TYPE,
  FUNCTION_NO_RETURN,
  NEGATION_TYPE_ERROR,
  ELSIF_PREDICATE_ERROR
};

static std::map<ErrorMessage,const std::string> ErrorMap{
  {INVALID_TYPE, "Invalid variable type"},
  {ARRAY_BAD_INDEX, "Array, invalid index provided"},
  {NON_CONSTANT,"Non constant symbol called for constant symbols"},
  {INVALID_SYMBOL,"Invalid Symbol type detected"},
  {SYMBOL_CAST,"Unable to cast to desired Symbol type"},
  {WHILE_PREDICATE_ERROR,"While predicate must be of integer type"},
  {IF_PREDICATE_ERROR,"if predicate must be of integer type"},
  {PROCEDURE_RETURN_ERROR, "Procedures are not allowed to return a value"},
  {FUNCTION_BAD_RETURN_TYPE,"Bad return type from function"},
  {FUNCTION_NO_RETURN, "Functions must return a value"},
  {NEGATION_TYPE_ERROR,"Only integers are allowed after a negation"},
  {ELSIF_PREDICATE_ERROR,"elsif predicates must be of integer type"}
};

#endif
