#include "CrossTerminalParser.hpp"
#include "CrossTerminalToString.hpp"

#include "libraries/catch.hpp"

#include <iostream>
#include <vector>


TEST_CASE("Test cross-terminal output", "[test-CrossTerminalParser]")
{
  /*
    Here we test whether the language is accepted.
  */

  using namespace tul::parser::dependency;
  using namespace tul::protocols;
  CrossTerminalParser cross_terminal_parser;

  std::vector<CrossTerminal> parse_stack;
  parse_stack.emplace_back(CrossTerminal::ENTER);

  std::vector<CrossTerminal> input_token_types;

  #define addT(name) input_token_types.emplace_back(CrossTerminal::name)
    addT(KEYWORD_PRIVATE);
    addT(IDENTIFIER_CLASS);
    addT(IDENTIFIER_VARIABLE);
    addT(SYMBOL_SEMICOLON);

    addT(KEYWORD_PRIVATE);
    addT(IDENTIFIER_CLASS);
    addT(IDENTIFIER_VARIABLE);
    addT(SYMBOL_SEMICOLON);

    addT(KEYWORD_PRIVATE);
    addT(IDENTIFIER_CLASS);
    addT(IDENTIFIER_VARIABLE);
    addT(SYMBOL_SEMICOLON);

    addT(KEYWORD_PRIVATE);
    addT(IDENTIFIER_CLASS);
    addT(IDENTIFIER_VARIABLE);
    addT(SYMBOL_SEMICOLON);

    addT(KEYWORD_RESTRICTED);
    addT(IDENTIFIER_CLASS);
    addT(IDENTIFIER_VARIABLE);
    addT(SYMBOL_SEMICOLON);

    addT(KEYWORD_PUBLIC);
    addT(IDENTIFIER_CLASS);
    addT(IDENTIFIER_VARIABLE);
    addT(SYMBOL_SEMICOLON);
  #undef addT

  while (parse_stack.empty() == false && input_token_types.empty() == false)
  {
    ParseReturn<CrossTerminal> returned
      = cross_terminal_parser.parseSymbol(parse_stack.front(), input_token_types.front());
    /*std::cout
      << "top: " << CrossTerminalToString::convertToString(parse_stack.front())
      << " input: " << CrossTerminalToString::convertToString(input_token_types.front())
      << std::endl;*/
    switch (returned.desired_action)
    {
      case ParseReturn<CrossTerminal>::Action::REMOVE_TOP: // Match, also remove from the input stack
        REQUIRE(parse_stack.size() > 0);
        parse_stack.erase(parse_stack.begin());
        REQUIRE(input_token_types.size() > 0);
        input_token_types.erase(input_token_types.begin());
      break;
      case ParseReturn<CrossTerminal>::Action::EPSILONATE:
        REQUIRE(parse_stack.size() > 0);
        parse_stack.erase(parse_stack.begin());
      break;
      case ParseReturn<CrossTerminal>::Action::CONTINUE:
        parse_stack.erase(parse_stack.begin());
        for (int i = returned.child_symbols->size() - 1; i >= 0; --i)
          parse_stack.insert(parse_stack.begin(), returned.child_symbols->at(i));
        continue;
      break;
      case ParseReturn<CrossTerminal>::Action::OBSERVE_ERROR:
      {
        /*
        std::vector<CrossTerminal> expected_crossterminals = cross_terminal_parser.calculateExpectedTokens(parse_stack.front());

        if (expected_crossterminals.empty())
        {
          std::cerr
            << "Error: expected token: '"
            << CrossTerminalToString::convertToString(parse_stack.front())
            << "' but got '"
            << CrossTerminalToString::convertToString(input_token_types.front())
            << '\'';
        }
        else
        {
          if (expected_crossterminals.size() == 1)
          {
            std::cerr
              << "Error: expected token: '"
              << CrossTerminalToString::convertToString(expected_crossterminals[0])
              << "' but got '"
              << CrossTerminalToString::convertToString(input_token_types.front())
              << '\'';
          }
          else
          {
            std::cerr << "Error: expected tokens: '";
            for (int i = 0; i < expected_crossterminals.size() - 1; ++i)
              std::cerr << CrossTerminalToString::convertToString(expected_crossterminals[i]) << "' , or '";
            std::cerr << CrossTerminalToString::convertToString(expected_crossterminals.back()) << '\'';
          }
        }*/
        REQUIRE(false);
      }
      break;
      default:
        REQUIRE(false);
      break;
    }
  }
  REQUIRE(input_token_types.size() == 0);
}