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
#include "TreeBuilder.hpp"
#include "protocols/CrossTerminalTools.hpp"


namespace tul { namespace tb {

bool TreeBuilder::buildTree(const std::string &string, bool throw_error)
{
	bool ret_val = true;
	for (auto chara : string)
	{
		if (buildTree(chara) == false)
		{
			ret_val = false;
			break;
		}
	}
	if (ret_val) ret_val = endInput();
	else if (throw_error) throw formulateError();
	else return false;
}


bool TreeBuilder::buildTree(char character)
{
	if (lexer_object.insertCharacter(character))
	{
		std::vector<protocols::Token> &token_stack = lexer_object.getTokenStack();
		for (std::size_t i = 0; i < token_stack.size(); ++i)
		{
			if (false == parser_object.parseSymbol(token_stack[i]))
				return false;
		}
		token_stack.clear();
		return true;
	}
	else
		return false;
	return false;
}


bool TreeBuilder::endInput()
{
	buildTree(' '); // Use a space to clear out the last symbol

	protocols::Token token;
	token.token_type = protocols::TokenType::END_OF_MODULE;

	if (false == parser_object.parseSymbol(token))
		return false;
	return parser_object.isEmpty();
}


std::unique_ptr<protocols::SyntaxTree> TreeBuilder::getSyntaxTree()
{
	return parser_object.getSyntaxTree();
}


std::vector<std::string> TreeBuilder::getExpectedTokens() const
{
	return parser_object.formulateExpectedTokens();
}


std::string TreeBuilder::getExpectedTokensGrammar() const
{
	std::vector<std::string> tokens = getExpectedTokens();
	if (tokens.empty())
		return "No productions expected";
	std::string result(tokens.at(0));
	if (tokens.size() > 1)
	{
		result += ": [\n";
		for (std::size_t i = 1; i < tokens.size(); ++i)
		{
			result += "\t" + tokens[i] + ",\n";
		}
		result.erase(result.end() - 1);
		return result + "\n]";
	}
	return result;
}


std::size_t TreeBuilder::getLine() const
{
	return lexer_object.getLine();
}


std::size_t TreeBuilder::getColumn() const
{
	return lexer_object.getColumn();
}


std::string TreeBuilder::getCurrentLexeme() const
{
	if (lexer_object.getTokenStack().empty() == false) {
		return (lexer_object.getTokenStack().cend() - 1)->accompanying_lexeme;
	} else {
		return "";
	}
}


std::string TreeBuilder::formulateError() const
{
	std::string error("On line ");
	error
		+= std::to_string(getLine())
		+ ", column " + std::to_string(getColumn())
		+ std::string("\nExpected\n`")
		+ getExpectedTokensGrammar()
		+ "'\nBut got `"
		+ getCurrentLexeme()
		+ "'";
	return error;
}

}}
