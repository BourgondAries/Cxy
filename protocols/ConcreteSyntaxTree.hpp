#pragma once

#include "CrossTerminal.hpp"
#include "Token.hpp"

#include <vector>

namespace tul
{
  namespace protocols
  {
    /**
      The concrete syntax tree is a data structure generated by the parser.
      It contains the type of node, token information, and productions (
      children_).
    */
    struct ConcreteSyntaxTree
    {
      Token token_;
      CrossTerminal node_type;
      std::vector<ConcreteSyntaxTree *> children_;
    };
  }
}