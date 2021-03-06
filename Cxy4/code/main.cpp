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
#include <iostream>

#include "datru/EntryType.hpp"
#include "datru/Token.hpp"
#include "datru/TokenType.hpp"
#include "lex/Lexer.hpp"

int main(int argc, char *argv[]) {
	typedef datru::Token<datru::EntryType, datru::TokenType> CommonToken;
	typedef lex::Lexer<datru::EntryType, CommonToken, datru::TokenType, datru::Typifier> CommonLexer;

	auto lexer = CommonLexer();
	lexer.insert("Hello there 'my friend' - I mean, hi!");
	std::cout << "Test" << std::endl;
}
