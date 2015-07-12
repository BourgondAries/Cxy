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
#pragma once

namespace tul
{
	namespace protocols
	{
		enum class CrossTerminal
		{
			ACCESS_SPECIFIER,
			ADDITIVE_EXPRESSION,
			AND_EXPRESSION,
			ARGUMENT,
			ARGUMENT_LIST,
			ASSIGNMENT,
			BASIC_TYPE,
			BITWISE_AND_EXPRESSION,
			BITWISE_OR_EXPRESSION,
			BITWISE_XOR_EXPRESSION,
			CALL,
			CODE_BLOCK,
			COMMA_ARGUMENT_LIST,
			COMMA_SEPARATED_LIST,
			COMMA_SEPARATED_LIST_ITEM,
			COMMA_SEPARATED_TYPE_AND_NAME_LIST,
			DATA_DECLARATION,
			DECLARATION,
			DECL_OR_FUNC,
			DO_STATEMENT,
			ENTER,
			EPSILONATE,
			EQUALITY_EXPRESSION,
			EXIT,
			EXPRESSION,
			FIELD_DECLARATION_LIST,
			FOR_STATEMENT,
			FUNCTION_DEFINITION,
			FUNCTION_LIST,
			FUNCTION_SIGNATURE,
			GOTO_STATEMENT,
			IF_STATEMENT,
			ITER_STATEMENT,
			LABEL_STATEMENT,
			MEMBER_EXPRESSION,
			METHOD_DECLARATION_LIST,
			MULTIPLICATIVE_EXPRESSION,
			NONE,
			NO_SEMICOLON_STATEMENT,
			OPTIONAL_ADDITIVE_EXPRESSION,
			OPTIONAL_AND_EXPRESSION,
			OPTIONAL_ASSIGNMENT,
			OPTIONAL_BITWISE_AND_EXPRESSION,
			OPTIONAL_BITWISE_OR_EXPRESSION,
			OPTIONAL_BITWISE_XOR_EXPRESSION,
			OPTIONAL_CALL,
			OPTIONAL_EQUALITY_EXPRESSION,
			OPTIONAL_EQUALS,
			OPTIONAL_MEMBER_EXPRESSION,
			OPTIONAL_MULTIPLICATIVE_EXPRESSION,
			OPTIONAL_OR_EXPRESSION,
			OPTIONAL_RELATIONAL_EXPRESSION,
			OPTIONAL_STRING_PLUS,
			OR_EXPRESSION,
			RELATIONAL_EXPRESSION,
			RELATIONAL_OPERATOR,
			RESOURCE,
			STATEMENT,
			STATEMENT_LIST,
			SUBROUTINE,
			SUBROUTINE_LIST,
			TYPE_PREFIX,
			TYPE_PREFIX_AFTER_CONST,
			TYPE_PREFIX_AFTER_REF,
			TYPE_PREFIX_AFTER_REF_CONST,
			UNARY_EXPRESSION,
			WHILE_STATEMENT,
			GROUPER_LEFT_BRACE,
			GROUPER_LEFT_BRACKET,
			GROUPER_LEFT_PARENTHESIS,
			GROUPER_RIGHT_BRACE,
			GROUPER_RIGHT_BRACKET,
			GROUPER_RIGHT_PARENTHESIS,
			IDENTIFIER_CLASS,
			IDENTIFIER_ENUMERATION,
			IDENTIFIER_PACKAGE,
			IDENTIFIER_SUBROUTINE,
			IDENTIFIER_VARIABLE,
			INTEGER_LITERAL,
			KEYWORD_ASSEMBLY,
			KEYWORD_CONST,
			KEYWORD_DO,
			KEYWORD_FOR,
			KEYWORD_GOTO,
			KEYWORD_IF,
			KEYWORD_LABEL,
			KEYWORD_PRIVATE,
			KEYWORD_PTR,
			KEYWORD_PUBLIC,
			KEYWORD_REF,
			KEYWORD_RESTRICTED,
			KEYWORD_WHILE,
			PRIMITIVE_SIGNED,
			PRIMITIVE_UNSIGNED,
			STRING,
			SYMBOL_AMPERSAND,
			SYMBOL_AMPERSAND__AMPERSAND,
			SYMBOL_APETAIL,
			SYMBOL_APETAIL__APETAIL,
			SYMBOL_BACKWARD_SLASH,
			SYMBOL_BAR,
			SYMBOL_BAR__BAR,
			SYMBOL_CARET,
			SYMBOL_COLON,
			SYMBOL_COLON__COLON,
			SYMBOL_COMMA,
			SYMBOL_DOLLAR,
			SYMBOL_DOLLAR__DOLLAR,
			SYMBOL_DOT,
			SYMBOL_DOT__DOT,
			SYMBOL_EQUAL,
			SYMBOL_EQUAL__EQUAL,
			SYMBOL_EXCLAMATION_MARK,
			SYMBOL_EXCLAMATION_MARK__EXCLAMATION_MARK,
			SYMBOL_FORWARD_SLASH,
			SYMBOL_GREATER_THAN,
			SYMBOL_GREATER_THAN__EQUAL,
			SYMBOL_GREATER_THAN__GREATER_THAN,
			SYMBOL_LESS_THAN,
			SYMBOL_LESS_THAN__EQUAL,
			SYMBOL_LESS_THAN__LESS_THAN,
			SYMBOL_MINUS,
			SYMBOL_MINUS__MINUS,
			SYMBOL_PLUS,
			SYMBOL_PLUS__PLUS,
			SYMBOL_SEMICOLON,
			SYMBOL_STAR,
			SYMBOL_STAR__STAR,
			UNIDENTIFIED,
			ENUM_END
		};
	}
}