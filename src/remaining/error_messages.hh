#ifndef __ERROR_MESSAGES__
#define __ERROR_MESSAGES__

#include <map>

enum ErrorMessage {
  INVALID_TYPE=0,
  ARRAY_BAD_INDEX,
  NON_CONSTANT,
  INVALID_SYMBOL,
  SYMBOL_CAST
};

static std::map<ErrorMessage,const std::string> ErrorMap{
  {INVALID_TYPE, "Invalid variable type"},
    {ARRAY_BAD_INDEX, "Array, invalid index provided"},
      {NON_CONSTANT,"Non constant symbol called for constant symbols"},
        {INVALID_SYMBOL,"Invalid Symbol type detected"},
          {SYMBOL_CAST,"Unable to cast to desired Symbol type"}
};

#endif
