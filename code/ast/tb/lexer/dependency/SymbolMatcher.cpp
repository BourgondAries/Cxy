// Copyright © 2015-2016 Kevin Robert Stravers
/*
This file is part of Cxy Compiler Reference Implementation (Cxy CRI).

Cxy CRI is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Cxy CRI is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Cxy CRI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "SymbolMatcher.hpp"


namespace tul { namespace dependency { 

protocols::TokenType SymbolMatcher::getSymbol(const std::string &lexeme)
{
	using namespace protocols;
	if (lexeme == "&&=") return TokenType::SYMBOL_AMPERSAND__AMPERSAND__EQUAL;
	else if (lexeme == "||=") return TokenType::SYMBOL_BAR__BAR__EQUAL;
	else if (lexeme == "!!=") return TokenType::SYMBOL_EXCLAMATION_MARK__EXCLAMATION_MARK__EQUAL;
	else if (lexeme == ">>=") return TokenType::SYMBOL_GREATER_THAN__GREATER_THAN__EQUAL;
	else if (lexeme == "<<=") return TokenType::SYMBOL_LESS_THAN__LESS_THAN__EQUAL;
	else if (lexeme == "&&") return TokenType::SYMBOL_AMPERSAND__AMPERSAND;
	else if (lexeme == "&=") return TokenType::SYMBOL_AMPERSAND__EQUAL;
	else if (lexeme == "||") return TokenType::SYMBOL_BAR__BAR;
	else if (lexeme == "|=") return TokenType::SYMBOL_BAR__EQUAL;
	else if (lexeme == "^=") return TokenType::SYMBOL_CARET__EQUAL;
	else if (lexeme == "$$") return TokenType::SYMBOL_DOLLAR__DOLLAR;
	else if (lexeme == "==") return TokenType::SYMBOL_EQUAL__EQUAL;
	else if (lexeme == "!=") return TokenType::SYMBOL_EXCLAMATION_MARK__EQUAL;
	else if (lexeme == "!!") return TokenType::SYMBOL_EXCLAMATION_MARK__EXCLAMATION_MARK;
	else if (lexeme == "/=") return TokenType::SYMBOL_FORWARD_SLASH__EQUAL;
	else if (lexeme == ">=") return TokenType::SYMBOL_GREATER_THAN__EQUAL;
	else if (lexeme == ">>") return TokenType::SYMBOL_GREATER_THAN__GREATER_THAN;
	else if (lexeme == "<=") return TokenType::SYMBOL_LESS_THAN__EQUAL;
	else if (lexeme == "<<") return TokenType::SYMBOL_LESS_THAN__LESS_THAN;
	else if (lexeme == "-=") return TokenType::SYMBOL_MINUS__EQUAL;
	else if (lexeme == "->") return TokenType::SYMBOL_MINUS__GREATER_THAN;
	else if (lexeme == "--") return TokenType::SYMBOL_MINUS__MINUS;
	else if (lexeme == "%=") return TokenType::SYMBOL_PERCENT__EQUAL;
	else if (lexeme == "+=") return TokenType::SYMBOL_PLUS__EQUAL;
	else if (lexeme == "++") return TokenType::SYMBOL_PLUS__PLUS;
	else if (lexeme == "*=") return TokenType::SYMBOL_STAR__EQUAL;
	else if (lexeme == "&") return TokenType::SYMBOL_AMPERSAND;
	else if (lexeme == "@") return TokenType::SYMBOL_APETAIL;
	else if (lexeme == "|") return TokenType::SYMBOL_BAR;
	else if (lexeme == "^") return TokenType::SYMBOL_CARET;
	else if (lexeme == ":") return TokenType::SYMBOL_COLON;
	else if (lexeme == ",") return TokenType::SYMBOL_COMMA;
	else if (lexeme == "$") return TokenType::SYMBOL_DOLLAR;
	else if (lexeme == ".") return TokenType::SYMBOL_DOT;
	else if (lexeme == "=") return TokenType::SYMBOL_EQUAL;
	else if (lexeme == "!") return TokenType::SYMBOL_EXCLAMATION_MARK;
	else if (lexeme == "/") return TokenType::SYMBOL_FORWARD_SLASH;
	else if (lexeme == ">") return TokenType::SYMBOL_GREATER_THAN;
	else if (lexeme == "<") return TokenType::SYMBOL_LESS_THAN;
	else if (lexeme == "-") return TokenType::SYMBOL_MINUS;
	else if (lexeme == "%") return TokenType::SYMBOL_PERCENT;
	else if (lexeme == "+") return TokenType::SYMBOL_PLUS;
	else if (lexeme == ";") return TokenType::SYMBOL_SEMICOLON;
	else if (lexeme == "*") return TokenType::SYMBOL_STAR;
	else if (lexeme == "~") return TokenType::SYMBOL_TILDE;
	else return protocols::TokenType::UNIDENTIFIED;
}

}}
