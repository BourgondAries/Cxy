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
#pragma once
#include "lexer/Lexer.hpp"
#include "parser/Parser.hpp"
#include "protocols/SyntaxTree.hpp"

#include <string>
#include <vector>


namespace tul { namespace tb {

class TreeBuilder
{
public:

	bool buildTree(const std::string &string, bool throw_error = true);
	bool buildTree(char character);

	bool endInput();

	std::unique_ptr<protocols::SyntaxTree> getSyntaxTree();

	std::vector<std::string> getExpectedTokens() const;
	std::string getExpectedTokensGrammar() const;
	std::size_t getLine() const;
	std::size_t getColumn() const;
	std::string getCurrentLexeme() const;

	std::string formulateError() const;

private:

	lexer::Lexer lexer_object;
	parser::Parser parser_object;

};

}}
