// Copyright © 2015 Kevin Robert Stravers
/*
This file is part of Unnamed-Language Compiler Reference Implementation (ULCRI).

ULCRI is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

ULCRI is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ULCRI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "KeywordMatcher.hpp"


namespace tul { namespace dependency { 

protocols::TokenType KeywordMatcher::getKeyword(const std::string &lexeme)
{
    using namespace protocols;
    if (lexeme == "restricted") return TokenType::KEYWORD_RESTRICTED;
    else if (lexeme == "private") return TokenType::KEYWORD_PRIVATE;
    else if (lexeme == "global") return TokenType::KEYWORD_GLOBAL;
    else if (lexeme == "glocal") return TokenType::KEYWORD_GLOCAL;
    else if (lexeme == "public") return TokenType::KEYWORD_PUBLIC;
    else if (lexeme == "return") return TokenType::KEYWORD_RETURN;
    else if (lexeme == "static") return TokenType::KEYWORD_STATIC;
    else if (lexeme == "const") return TokenType::KEYWORD_CONST;
    else if (lexeme == "defer") return TokenType::KEYWORD_DEFER;
    else if (lexeme == "label") return TokenType::KEYWORD_LABEL;
    else if (lexeme == "while") return TokenType::KEYWORD_WHILE;
    else if (lexeme == "cast") return TokenType::KEYWORD_CAST;
    else if (lexeme == "else") return TokenType::KEYWORD_ELSE;
    else if (lexeme == "goto") return TokenType::KEYWORD_GOTO;
    else if (lexeme == "hack") return TokenType::KEYWORD_HACK;
    else if (lexeme == "pure") return TokenType::KEYWORD_PURE;
    else if (lexeme == "when") return TokenType::KEYWORD_WHEN;
    else if (lexeme == "for") return TokenType::KEYWORD_FOR;
    else if (lexeme == "ptr") return TokenType::KEYWORD_PTR;
    else if (lexeme == "ref") return TokenType::KEYWORD_REF;
    else if (lexeme == "var") return TokenType::KEYWORD_VAR;
    else if (lexeme == "do") return TokenType::KEYWORD_DO;
    else if (lexeme == "if") return TokenType::KEYWORD_IF;
    else return protocols::TokenType::UNIDENTIFIED;
}

}}
