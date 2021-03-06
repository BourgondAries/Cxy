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
#include "CrossTerminalTools.hpp"


namespace tul { namespace protocols { 

bool CrossTerminalTools::isExpression(const CrossTerminal &ct_)
{
	switch (ct_)
	{
		case CrossTerminal::ADDITIVE_EXPRESSION: return true;
		case CrossTerminal::AND_EXPRESSION: return true;
		case CrossTerminal::ARRAY_ACCESS_EXPRESSION: return true;
		case CrossTerminal::ASSIGNMENT_EXPRESSION: return true;
		case CrossTerminal::BITWISE_AND_EXPRESSION: return true;
		case CrossTerminal::BITWISE_OR_EXPRESSION: return true;
		case CrossTerminal::BITWISE_XOR_EXPRESSION: return true;
		case CrossTerminal::CAST_EXPRESSION: return true;
		case CrossTerminal::CLASS_MEMBER_EXPRESSION: return true;
		case CrossTerminal::DELETE_EXPRESSION: return true;
		case CrossTerminal::ENUMERATION_MEMBER_EXPRESSION: return true;
		case CrossTerminal::ENUM_EXPRESSION: return true;
		case CrossTerminal::EQUALITY_EXPRESSION: return true;
		case CrossTerminal::MEMBER_EXPRESSION: return true;
		case CrossTerminal::MULTIPLICATIVE_EXPRESSION: return true;
		case CrossTerminal::NEW_EXPRESSION: return true;
		case CrossTerminal::OPTIONAL_ADDITIVE_EXPRESSION: return true;
		case CrossTerminal::OPTIONAL_AND_EXPRESSION: return true;
		case CrossTerminal::OPTIONAL_ARRAY_ACCESS_EXPRESSION: return true;
		case CrossTerminal::OPTIONAL_ASSIGNMENT_EXPRESSION: return true;
		case CrossTerminal::OPTIONAL_BITWISE_AND_EXPRESSION: return true;
		case CrossTerminal::OPTIONAL_BITWISE_OR_EXPRESSION: return true;
		case CrossTerminal::OPTIONAL_BITWISE_XOR_EXPRESSION: return true;
		case CrossTerminal::OPTIONAL_EQUALITY_EXPRESSION: return true;
		case CrossTerminal::OPTIONAL_EXPRESSION: return true;
		case CrossTerminal::OPTIONAL_EXTRACTOR_EXPRESSION: return true;
		case CrossTerminal::OPTIONAL_MEMBER_EXPRESSION: return true;
		case CrossTerminal::OPTIONAL_MULTIPLICATIVE_EXPRESSION: return true;
		case CrossTerminal::OPTIONAL_OR_EXPRESSION: return true;
		case CrossTerminal::OPTIONAL_RELATIONAL_EXPRESSION: return true;
		case CrossTerminal::OPTIONAL_SHIFT_EXPRESSION: return true;
		case CrossTerminal::OR_EXPRESSION: return true;
		case CrossTerminal::RELATIONAL_EXPRESSION: return true;
		case CrossTerminal::SHIFT_EXPRESSION: return true;
		case CrossTerminal::SIZE_EXPRESSION: return true;
		case CrossTerminal::TYPE_EXPRESSION: return true;
		case CrossTerminal::TYPE_OR_EXPRESSION: return true;
		case CrossTerminal::UNARY_EXPRESSION: return true;
		case CrossTerminal::WHEN_EXPRESSION: return true;
		default: return false;
	}
}

bool CrossTerminalTools::isKeyword(const CrossTerminal &ct_)
{
	switch (ct_)
	{
		case CrossTerminal::KEYWORD_BREAK: return true;
		case CrossTerminal::KEYWORD_CAST: return true;
		case CrossTerminal::KEYWORD_CATCH: return true;
		case CrossTerminal::KEYWORD_CONST: return true;
		case CrossTerminal::KEYWORD_CONSTRUCT: return true;
		case CrossTerminal::KEYWORD_CONSTRUCTOR: return true;
		case CrossTerminal::KEYWORD_CONTINUE: return true;
		case CrossTerminal::KEYWORD_DEBUG: return true;
		case CrossTerminal::KEYWORD_DEFER: return true;
		case CrossTerminal::KEYWORD_DELETE: return true;
		case CrossTerminal::KEYWORD_DESTRUCTOR: return true;
		case CrossTerminal::KEYWORD_DO: return true;
		case CrossTerminal::KEYWORD_DOUBLE: return true;
		case CrossTerminal::KEYWORD_ELSE: return true;
		case CrossTerminal::KEYWORD_ENUM: return true;
		case CrossTerminal::KEYWORD_EXPERIMENTAL: return true;
		case CrossTerminal::KEYWORD_FLOAT: return true;
		case CrossTerminal::KEYWORD_FOR: return true;
		case CrossTerminal::KEYWORD_FOREACH: return true;
		case CrossTerminal::KEYWORD_GOTO: return true;
		case CrossTerminal::KEYWORD_HACK: return true;
		case CrossTerminal::KEYWORD_IF: return true;
		case CrossTerminal::KEYWORD_IN: return true;
		case CrossTerminal::KEYWORD_LABEL: return true;
		case CrossTerminal::KEYWORD_LAMBDA: return true;
		case CrossTerminal::KEYWORD_NEW: return true;
		case CrossTerminal::KEYWORD_PRIVATE: return true;
		case CrossTerminal::KEYWORD_PTR: return true;
		case CrossTerminal::KEYWORD_PUBLIC: return true;
		case CrossTerminal::KEYWORD_PURE: return true;
		case CrossTerminal::KEYWORD_REF: return true;
		case CrossTerminal::KEYWORD_RESTRICTED: return true;
		case CrossTerminal::KEYWORD_RETURN: return true;
		case CrossTerminal::KEYWORD_SIZE: return true;
		case CrossTerminal::KEYWORD_STATIC: return true;
		case CrossTerminal::KEYWORD_THIS: return true;
		case CrossTerminal::KEYWORD_THROW: return true;
		case CrossTerminal::KEYWORD_TRY: return true;
		case CrossTerminal::KEYWORD_TYPE: return true;
		case CrossTerminal::KEYWORD_VAR: return true;
		case CrossTerminal::KEYWORD_WHEN: return true;
		case CrossTerminal::KEYWORD_WHILE: return true;
		default: return false;
	}
}

bool CrossTerminalTools::isUselessSymbol(const CrossTerminal &ct_)
{
	switch (ct_)
	{
		case CrossTerminal::SYMBOL_AMPERSAND: return true;
		case CrossTerminal::SYMBOL_AMPERSAND__AMPERSAND: return true;
		case CrossTerminal::SYMBOL_AMPERSAND__AMPERSAND__EQUAL: return true;
		case CrossTerminal::SYMBOL_AMPERSAND__EQUAL: return true;
		case CrossTerminal::SYMBOL_APETAIL: return true;
		case CrossTerminal::SYMBOL_BAR: return true;
		case CrossTerminal::SYMBOL_BAR__BAR: return true;
		case CrossTerminal::SYMBOL_BAR__BAR__EQUAL: return true;
		case CrossTerminal::SYMBOL_BAR__EQUAL: return true;
		case CrossTerminal::SYMBOL_CARET: return true;
		case CrossTerminal::SYMBOL_CARET__EQUAL: return true;
		case CrossTerminal::SYMBOL_COLON: return true;
		case CrossTerminal::SYMBOL_COMMA: return true;
		case CrossTerminal::SYMBOL_DOLLAR: return true;
		case CrossTerminal::SYMBOL_DOLLAR__DOLLAR: return true;
		case CrossTerminal::SYMBOL_DOT: return true;
		case CrossTerminal::SYMBOL_EQUAL: return true;
		case CrossTerminal::SYMBOL_EQUAL__EQUAL: return true;
		case CrossTerminal::SYMBOL_EXCLAMATION_MARK: return true;
		case CrossTerminal::SYMBOL_EXCLAMATION_MARK__EQUAL: return true;
		case CrossTerminal::SYMBOL_EXCLAMATION_MARK__EXCLAMATION_MARK: return true;
		case CrossTerminal::SYMBOL_EXCLAMATION_MARK__EXCLAMATION_MARK__EQUAL: return true;
		case CrossTerminal::SYMBOL_FORWARD_SLASH: return true;
		case CrossTerminal::SYMBOL_FORWARD_SLASH__EQUAL: return true;
		case CrossTerminal::SYMBOL_GREATER_THAN: return true;
		case CrossTerminal::SYMBOL_GREATER_THAN__EQUAL: return true;
		case CrossTerminal::SYMBOL_GREATER_THAN__GREATER_THAN: return true;
		case CrossTerminal::SYMBOL_GREATER_THAN__GREATER_THAN__EQUAL: return true;
		case CrossTerminal::SYMBOL_LESS_THAN: return true;
		case CrossTerminal::SYMBOL_LESS_THAN__EQUAL: return true;
		case CrossTerminal::SYMBOL_LESS_THAN__LESS_THAN: return true;
		case CrossTerminal::SYMBOL_LESS_THAN__LESS_THAN__EQUAL: return true;
		case CrossTerminal::SYMBOL_MINUS: return true;
		case CrossTerminal::SYMBOL_MINUS__EQUAL: return true;
		case CrossTerminal::SYMBOL_MINUS__GREATER_THAN: return true;
		case CrossTerminal::SYMBOL_MINUS__MINUS: return true;
		case CrossTerminal::SYMBOL_PERCENT: return true;
		case CrossTerminal::SYMBOL_PERCENT__EQUAL: return true;
		case CrossTerminal::SYMBOL_PLUS: return true;
		case CrossTerminal::SYMBOL_PLUS__EQUAL: return true;
		case CrossTerminal::SYMBOL_PLUS__PLUS: return true;
		case CrossTerminal::SYMBOL_SEMICOLON: return true;
		case CrossTerminal::SYMBOL_STAR: return true;
		case CrossTerminal::SYMBOL_STAR__EQUAL: return true;
		case CrossTerminal::SYMBOL_TILDE: return true;
		default: return false;
	}
}

std::string CrossTerminalTools::toString(protocols::CrossTerminal cross_terminal)
{
	switch (cross_terminal)
	{
		case protocols::CrossTerminal::ACCESS_SPECIFIER: return "ACCESS_SPECIFIER";
		case protocols::CrossTerminal::ADDITIVE_EXPRESSION: return "ADDITIVE_EXPRESSION";
		case protocols::CrossTerminal::AND_EXPRESSION: return "AND_EXPRESSION";
		case protocols::CrossTerminal::ARGUMENT: return "ARGUMENT";
		case protocols::CrossTerminal::ARGUMENT_LIST: return "ARGUMENT_LIST";
		case protocols::CrossTerminal::ARGUMENT_LIST_WITH_THIS: return "ARGUMENT_LIST_WITH_THIS";
		case protocols::CrossTerminal::ARRAY: return "ARRAY";
		case protocols::CrossTerminal::ARRAY_ACCESS_EXPRESSION: return "ARRAY_ACCESS_EXPRESSION";
		case protocols::CrossTerminal::ASSIGNMENT_EXPRESSION: return "ASSIGNMENT_EXPRESSION";
		case protocols::CrossTerminal::ATTRIBUTE_LIST: return "ATTRIBUTE_LIST";
		case protocols::CrossTerminal::ATTRIBUTE_LIST_AFTER_CONST: return "ATTRIBUTE_LIST_AFTER_CONST";
		case protocols::CrossTerminal::BASIC_TYPE: return "BASIC_TYPE";
		case protocols::CrossTerminal::BITWISE_AND_EXPRESSION: return "BITWISE_AND_EXPRESSION";
		case protocols::CrossTerminal::BITWISE_OR_EXPRESSION: return "BITWISE_OR_EXPRESSION";
		case protocols::CrossTerminal::BITWISE_XOR_EXPRESSION: return "BITWISE_XOR_EXPRESSION";
		case protocols::CrossTerminal::BREAK_STATEMENT: return "BREAK_STATEMENT";
		case protocols::CrossTerminal::CAPTURE_LIST: return "CAPTURE_LIST";
		case protocols::CrossTerminal::CAST_EXPRESSION: return "CAST_EXPRESSION";
		case protocols::CrossTerminal::CATCH_LIST: return "CATCH_LIST";
		case protocols::CrossTerminal::CLASS_MEMBER: return "CLASS_MEMBER";
		case protocols::CrossTerminal::CLASS_MEMBER_EXPRESSION: return "CLASS_MEMBER_EXPRESSION";
		case protocols::CrossTerminal::CODE_BLOCK: return "CODE_BLOCK";
		case protocols::CrossTerminal::COLONIZED: return "COLONIZED";
		case protocols::CrossTerminal::COLON_LIST: return "COLON_LIST";
		case protocols::CrossTerminal::CONSTRUCT_LIST: return "CONSTRUCT_LIST";
		case protocols::CrossTerminal::CONSTRUCT_STATEMENT: return "CONSTRUCT_STATEMENT";
		case protocols::CrossTerminal::CONTINUE_STATEMENT: return "CONTINUE_STATEMENT";
		case protocols::CrossTerminal::DATA_DECLARATION_LIST: return "DATA_DECLARATION_LIST";
		case protocols::CrossTerminal::DATA_DECLARATION_STATEMENT: return "DATA_DECLARATION_STATEMENT";
		case protocols::CrossTerminal::DATA_DECLARATION_TYPE: return "DATA_DECLARATION_TYPE";
		case protocols::CrossTerminal::DATA_NAMES: return "DATA_NAMES";
		case protocols::CrossTerminal::DATA_NAMES_LIST: return "DATA_NAMES_LIST";
		case protocols::CrossTerminal::DEBUG_STATEMENT: return "DEBUG_STATEMENT";
		case protocols::CrossTerminal::DEFER_STATEMENT: return "DEFER_STATEMENT";
		case protocols::CrossTerminal::DELETE_EXPRESSION: return "DELETE_EXPRESSION";
		case protocols::CrossTerminal::DO_STATEMENT: return "DO_STATEMENT";
		case protocols::CrossTerminal::ELSE_STATEMENT: return "ELSE_STATEMENT";
		case protocols::CrossTerminal::END_OF_MODULE: return "END_OF_MODULE";
		case protocols::CrossTerminal::ENTER: return "ENTER";
		case protocols::CrossTerminal::ENUMERABLE_TYPE: return "ENUMERABLE_TYPE";
		case protocols::CrossTerminal::ENUMERATION_MEMBER_EXPRESSION: return "ENUMERATION_MEMBER_EXPRESSION";
		case protocols::CrossTerminal::ENUM_END: return "";case protocols::CrossTerminal::ENUM_EXPRESSION: return "ENUM_EXPRESSION";
		case protocols::CrossTerminal::ENUM_OR_NOTHING: return "ENUM_OR_NOTHING";
		case protocols::CrossTerminal::EPSILONATE: return "EPSILONATE";
		case protocols::CrossTerminal::EQUALITY_EXPRESSION: return "EQUALITY_EXPRESSION";
		case protocols::CrossTerminal::EXPRESSION_LIST: return "EXPRESSION_LIST";
		case protocols::CrossTerminal::EXP_STATEMENT: return "EXP_STATEMENT";
		case protocols::CrossTerminal::FOREACH_STATEMENT: return "FOREACH_STATEMENT";
		case protocols::CrossTerminal::FOR_DATA_DECLARATION: return "FOR_DATA_DECLARATION";
		case protocols::CrossTerminal::FOR_STATEMENT: return "FOR_STATEMENT";
		case protocols::CrossTerminal::FUNCTION_NAMES: return "FUNCTION_NAMES";
		case protocols::CrossTerminal::FUNCTION_SIGNATURE: return "FUNCTION_SIGNATURE";
		case protocols::CrossTerminal::GOTO_STATEMENT: return "GOTO_STATEMENT";
		case protocols::CrossTerminal::GROUPER_LEFT_BRACE: return "GROUPER_LEFT_BRACE";
		case protocols::CrossTerminal::GROUPER_LEFT_BRACKET: return "GROUPER_LEFT_BRACKET";
		case protocols::CrossTerminal::GROUPER_LEFT_PARENTHESIS: return "GROUPER_LEFT_PARENTHESIS";
		case protocols::CrossTerminal::GROUPER_RIGHT_BRACE: return "GROUPER_RIGHT_BRACE";
		case protocols::CrossTerminal::GROUPER_RIGHT_BRACKET: return "GROUPER_RIGHT_BRACKET";
		case protocols::CrossTerminal::GROUPER_RIGHT_PARENTHESIS: return "GROUPER_RIGHT_PARENTHESIS";
		case protocols::CrossTerminal::HACK_STATEMENT: return "HACK_STATEMENT";
		case protocols::CrossTerminal::IDENTIFIER_CLASS: return "IDENTIFIER_CLASS";
		case protocols::CrossTerminal::IDENTIFIER_CONSTEXPR: return "IDENTIFIER_CONSTEXPR";
		case protocols::CrossTerminal::IDENTIFIER_ENUMERATION: return "IDENTIFIER_ENUMERATION";
		case protocols::CrossTerminal::IDENTIFIER_PACKAGE: return "IDENTIFIER_PACKAGE";
		case protocols::CrossTerminal::IDENTIFIER_SUBROUTINE: return "IDENTIFIER_SUBROUTINE";
		case protocols::CrossTerminal::IDENTIFIER_VARIABLE: return "IDENTIFIER_VARIABLE";
		case protocols::CrossTerminal::IF_STATEMENT: return "IF_STATEMENT";
		case protocols::CrossTerminal::INTEGER_LITERAL: return "INTEGER_LITERAL";
		case protocols::CrossTerminal::ITER_STATEMENT: return "ITER_STATEMENT";
		case protocols::CrossTerminal::KEYWORD_BREAK: return "KEYWORD_BREAK";
		case protocols::CrossTerminal::KEYWORD_CAST: return "KEYWORD_CAST";
		case protocols::CrossTerminal::KEYWORD_CATCH: return "KEYWORD_CATCH";
		case protocols::CrossTerminal::KEYWORD_CONST: return "KEYWORD_CONST";
		case protocols::CrossTerminal::KEYWORD_CONSTRUCT: return "KEYWORD_CONSTRUCT";
		case protocols::CrossTerminal::KEYWORD_CONSTRUCTOR: return "KEYWORD_CONSTRUCTOR";
		case protocols::CrossTerminal::KEYWORD_CONTINUE: return "KEYWORD_CONTINUE";
		case protocols::CrossTerminal::KEYWORD_DEBUG: return "KEYWORD_DEBUG";
		case protocols::CrossTerminal::KEYWORD_DEFER: return "KEYWORD_DEFER";
		case protocols::CrossTerminal::KEYWORD_DELETE: return "KEYWORD_DELETE";
		case protocols::CrossTerminal::KEYWORD_DESTRUCTOR: return "KEYWORD_DESTRUCTOR";
		case protocols::CrossTerminal::KEYWORD_DO: return "KEYWORD_DO";
		case protocols::CrossTerminal::KEYWORD_DOUBLE: return "KEYWORD_DOUBLE";
		case protocols::CrossTerminal::KEYWORD_ELSE: return "KEYWORD_ELSE";
		case protocols::CrossTerminal::KEYWORD_ENUM: return "KEYWORD_ENUM";
		case protocols::CrossTerminal::KEYWORD_EXPERIMENTAL: return "KEYWORD_EXPERIMENTAL";
		case protocols::CrossTerminal::KEYWORD_FLOAT: return "KEYWORD_FLOAT";
		case protocols::CrossTerminal::KEYWORD_FOR: return "KEYWORD_FOR";
		case protocols::CrossTerminal::KEYWORD_FOREACH: return "KEYWORD_FOREACH";
		case protocols::CrossTerminal::KEYWORD_GOTO: return "KEYWORD_GOTO";
		case protocols::CrossTerminal::KEYWORD_HACK: return "KEYWORD_HACK";
		case protocols::CrossTerminal::KEYWORD_IF: return "KEYWORD_IF";
		case protocols::CrossTerminal::KEYWORD_IN: return "KEYWORD_IN";
		case protocols::CrossTerminal::KEYWORD_LABEL: return "KEYWORD_LABEL";
		case protocols::CrossTerminal::KEYWORD_LAMBDA: return "KEYWORD_LAMBDA";
		case protocols::CrossTerminal::KEYWORD_NEW: return "KEYWORD_NEW";
		case protocols::CrossTerminal::KEYWORD_PRIVATE: return "KEYWORD_PRIVATE";
		case protocols::CrossTerminal::KEYWORD_PTR: return "KEYWORD_PTR";
		case protocols::CrossTerminal::KEYWORD_PUBLIC: return "KEYWORD_PUBLIC";
		case protocols::CrossTerminal::KEYWORD_PURE: return "KEYWORD_PURE";
		case protocols::CrossTerminal::KEYWORD_REF: return "KEYWORD_REF";
		case protocols::CrossTerminal::KEYWORD_RESTRICTED: return "KEYWORD_RESTRICTED";
		case protocols::CrossTerminal::KEYWORD_RETURN: return "KEYWORD_RETURN";
		case protocols::CrossTerminal::KEYWORD_SIZE: return "KEYWORD_SIZE";
		case protocols::CrossTerminal::KEYWORD_STATIC: return "KEYWORD_STATIC";
		case protocols::CrossTerminal::KEYWORD_THIS: return "KEYWORD_THIS";
		case protocols::CrossTerminal::KEYWORD_THROW: return "KEYWORD_THROW";
		case protocols::CrossTerminal::KEYWORD_TRY: return "KEYWORD_TRY";
		case protocols::CrossTerminal::KEYWORD_TYPE: return "KEYWORD_TYPE";
		case protocols::CrossTerminal::KEYWORD_VAR: return "KEYWORD_VAR";
		case protocols::CrossTerminal::KEYWORD_WHEN: return "KEYWORD_WHEN";
		case protocols::CrossTerminal::KEYWORD_WHILE: return "KEYWORD_WHILE";
		case protocols::CrossTerminal::LABEL_STATEMENT: return "LABEL_STATEMENT";
		case protocols::CrossTerminal::MEMBER_DEFINITION: return "MEMBER_DEFINITION";
		case protocols::CrossTerminal::MEMBER_EXPRESSION: return "MEMBER_EXPRESSION";
		case protocols::CrossTerminal::METHOD_DECL_OR_DEF: return "METHOD_DECL_OR_DEF";
		case protocols::CrossTerminal::MULTIPLE_OR_SINGLE_CONSTRUCT: return "MULTIPLE_OR_SINGLE_CONSTRUCT";
		case protocols::CrossTerminal::MULTIPLICATIVE_EXPRESSION: return "MULTIPLICATIVE_EXPRESSION";
		case protocols::CrossTerminal::NAMEKIND: return "NAMEKIND";
		case protocols::CrossTerminal::NAMES_SYMBOL: return "NAMES_SYMBOL";
		case protocols::CrossTerminal::NEW_EXPRESSION: return "NEW_EXPRESSION";
		case protocols::CrossTerminal::NO_SEMICOLON_STATEMENT: return "NO_SEMICOLON_STATEMENT";
		case protocols::CrossTerminal::OPTIONAL_ADDITIVE_EXPRESSION: return "OPTIONAL_ADDITIVE_EXPRESSION";
		case protocols::CrossTerminal::OPTIONAL_AND_EXPRESSION: return "OPTIONAL_AND_EXPRESSION";
		case protocols::CrossTerminal::OPTIONAL_ARGUMENT_LIST: return "OPTIONAL_ARGUMENT_LIST";
		case protocols::CrossTerminal::OPTIONAL_ARGUMENT_LIST_AFTER_COMMA: return "OPTIONAL_ARGUMENT_LIST_AFTER_COMMA";
		case protocols::CrossTerminal::OPTIONAL_ARRAY_ACCESS_EXPRESSION: return "OPTIONAL_ARRAY_ACCESS_EXPRESSION";
		case protocols::CrossTerminal::OPTIONAL_ARRAY_ACCESS_EXPRESSION_AFTER_COMMA: return "OPTIONAL_ARRAY_ACCESS_EXPRESSION_AFTER_COMMA";
		case protocols::CrossTerminal::OPTIONAL_ASSIGNMENT: return "OPTIONAL_ASSIGNMENT";
		case protocols::CrossTerminal::OPTIONAL_ASSIGNMENT_EXPRESSION: return "OPTIONAL_ASSIGNMENT_EXPRESSION";
		case protocols::CrossTerminal::OPTIONAL_ASSIGNMENT_MULTIVARIABLE: return "OPTIONAL_ASSIGNMENT_MULTIVARIABLE";
		case protocols::CrossTerminal::OPTIONAL_ASSIGNMENT_OR_CAPTURE: return "OPTIONAL_ASSIGNMENT_OR_CAPTURE";
		case protocols::CrossTerminal::OPTIONAL_ATTRIBUTE_LIST: return "OPTIONAL_ATTRIBUTE_LIST";
		case protocols::CrossTerminal::OPTIONAL_BITWISE_AND_EXPRESSION: return "OPTIONAL_BITWISE_AND_EXPRESSION";
		case protocols::CrossTerminal::OPTIONAL_BITWISE_OR_EXPRESSION: return "OPTIONAL_BITWISE_OR_EXPRESSION";
		case protocols::CrossTerminal::OPTIONAL_BITWISE_XOR_EXPRESSION: return "OPTIONAL_BITWISE_XOR_EXPRESSION";
		case protocols::CrossTerminal::OPTIONAL_CAPTURE_LIST: return "OPTIONAL_CAPTURE_LIST";
		case protocols::CrossTerminal::OPTIONAL_COLON_LIST: return "OPTIONAL_COLON_LIST";
		case protocols::CrossTerminal::OPTIONAL_COLON_LIST_AFTER_COMMA: return "OPTIONAL_COLON_LIST_AFTER_COMMA";
		case protocols::CrossTerminal::OPTIONAL_CONSTRUCTOR_LIST: return "OPTIONAL_CONSTRUCTOR_LIST";
		case protocols::CrossTerminal::OPTIONAL_CONSTRUCT_LIST: return "OPTIONAL_CONSTRUCT_LIST";
		case protocols::CrossTerminal::OPTIONAL_DATA_DECLARATION: return "OPTIONAL_DATA_DECLARATION";
		case protocols::CrossTerminal::OPTIONAL_DATA_DECLARATION_AFTER_COMMA: return "OPTIONAL_DATA_DECLARATION_AFTER_COMMA";
		case protocols::CrossTerminal::OPTIONAL_DATA_NAMES_LIST: return "OPTIONAL_DATA_NAMES_LIST";
		case protocols::CrossTerminal::OPTIONAL_DATA_NAMES_LIST_AFTER_COMMA: return "OPTIONAL_DATA_NAMES_LIST_AFTER_COMMA";
		case protocols::CrossTerminal::OPTIONAL_EQUALITY_EXPRESSION: return "OPTIONAL_EQUALITY_EXPRESSION";
		case protocols::CrossTerminal::OPTIONAL_EXPRESSION: return "OPTIONAL_EXPRESSION";
		case protocols::CrossTerminal::OPTIONAL_EXTRACTOR_EXPRESSION: return "OPTIONAL_EXTRACTOR_EXPRESSION";
		case protocols::CrossTerminal::OPTIONAL_LOOP_NAME: return "OPTIONAL_LOOP_NAME";
		case protocols::CrossTerminal::OPTIONAL_MEMBER_EXPRESSION: return "OPTIONAL_MEMBER_EXPRESSION";
		case protocols::CrossTerminal::OPTIONAL_MULTIPLICATIVE_EXPRESSION: return "OPTIONAL_MULTIPLICATIVE_EXPRESSION";
		case protocols::CrossTerminal::OPTIONAL_NAME: return "OPTIONAL_NAME";
		case protocols::CrossTerminal::OPTIONAL_NEW_LIST: return "OPTIONAL_NEW_LIST";
		case protocols::CrossTerminal::OPTIONAL_NEW_TYPE: return "OPTIONAL_NEW_TYPE";
		case protocols::CrossTerminal::OPTIONAL_OR_EXPRESSION: return "OPTIONAL_OR_EXPRESSION";
		case protocols::CrossTerminal::OPTIONAL_PARAMETER_LIST: return "OPTIONAL_PARAMETER_LIST";
		case protocols::CrossTerminal::OPTIONAL_PARAMETER_LIST_AFTER_COMMA: return "OPTIONAL_PARAMETER_LIST_AFTER_COMMA";
		case protocols::CrossTerminal::OPTIONAL_RELATIONAL_EXPRESSION: return "OPTIONAL_RELATIONAL_EXPRESSION";
		case protocols::CrossTerminal::OPTIONAL_SHIFT_EXPRESSION: return "OPTIONAL_SHIFT_EXPRESSION";
		case protocols::CrossTerminal::OPTIONAL_TEMPLATE: return "OPTIONAL_TEMPLATE";
		case protocols::CrossTerminal::OPTIONAL_TEMPLATE_LIST: return "OPTIONAL_TEMPLATE_LIST";
		case protocols::CrossTerminal::OPT_ASGN: return "OPT_ASGN";
		case protocols::CrossTerminal::OPT_COLON_ASGN: return "OPT_COLON_ASGN";
		case protocols::CrossTerminal::OPT_DATA_NAMES: return "OPT_DATA_NAMES";
		case protocols::CrossTerminal::OPT_EXP_STATEMENT: return "OPT_EXP_STATEMENT";
		case protocols::CrossTerminal::OR_EXPRESSION: return "OR_EXPRESSION";
		case protocols::CrossTerminal::PARAMETER_LIST: return "PARAMETER_LIST";
		case protocols::CrossTerminal::PRIMITIVE_SIGNED: return "PRIMITIVE_SIGNED";
		case protocols::CrossTerminal::PRIMITIVE_SIGNED_WRAPPED: return "PRIMITIVE_SIGNED_WRAPPED";
		case protocols::CrossTerminal::PRIMITIVE_UNSIGNED: return "PRIMITIVE_UNSIGNED";
		case protocols::CrossTerminal::PRIMITIVE_UNSIGNED_WRAPPED: return "PRIMITIVE_UNSIGNED_WRAPPED";
		case protocols::CrossTerminal::REC_DATA_NAMES: return "REC_DATA_NAMES";
		case protocols::CrossTerminal::RELATIONAL_EXPRESSION: return "RELATIONAL_EXPRESSION";
		case protocols::CrossTerminal::RELATIONAL_OPERATOR: return "RELATIONAL_OPERATOR";
		case protocols::CrossTerminal::RESOURCE: return "RESOURCE";
		case protocols::CrossTerminal::RETURN_STATEMENT: return "RETURN_STATEMENT";
		case protocols::CrossTerminal::SHIFT_EXPRESSION: return "SHIFT_EXPRESSION";
		case protocols::CrossTerminal::SHIFT_OPERATOR: return "SHIFT_OPERATOR";
		case protocols::CrossTerminal::SINGLE_CONSTRUCT: return "SINGLE_CONSTRUCT";
		case protocols::CrossTerminal::SINGLE_STATEMENT_OR_CODE_BLOCK: return "SINGLE_STATEMENT_OR_CODE_BLOCK";
		case protocols::CrossTerminal::SIZE_EXPRESSION: return "SIZE_EXPRESSION";
		case protocols::CrossTerminal::STATEMENT: return "STATEMENT";
		case protocols::CrossTerminal::STATEMENT_LIST: return "STATEMENT_LIST";
		case protocols::CrossTerminal::STATIC_OR_DYNAMIC_CONTROL: return "STATIC_OR_DYNAMIC_CONTROL";
		case protocols::CrossTerminal::STRING: return "STRING";
		case protocols::CrossTerminal::SYMBOL_AMPERSAND: return "SYMBOL_AMPERSAND";
		case protocols::CrossTerminal::SYMBOL_AMPERSAND__AMPERSAND: return "SYMBOL_AMPERSAND__AMPERSAND";
		case protocols::CrossTerminal::SYMBOL_AMPERSAND__AMPERSAND__EQUAL: return "SYMBOL_AMPERSAND__AMPERSAND__EQUAL";
		case protocols::CrossTerminal::SYMBOL_AMPERSAND__EQUAL: return "SYMBOL_AMPERSAND__EQUAL";
		case protocols::CrossTerminal::SYMBOL_APETAIL: return "SYMBOL_APETAIL";
		case protocols::CrossTerminal::SYMBOL_BAR: return "SYMBOL_BAR";
		case protocols::CrossTerminal::SYMBOL_BAR__BAR: return "SYMBOL_BAR__BAR";
		case protocols::CrossTerminal::SYMBOL_BAR__BAR__EQUAL: return "SYMBOL_BAR__BAR__EQUAL";
		case protocols::CrossTerminal::SYMBOL_BAR__EQUAL: return "SYMBOL_BAR__EQUAL";
		case protocols::CrossTerminal::SYMBOL_CARET: return "SYMBOL_CARET";
		case protocols::CrossTerminal::SYMBOL_CARET__EQUAL: return "SYMBOL_CARET__EQUAL";
		case protocols::CrossTerminal::SYMBOL_COLON: return "SYMBOL_COLON";
		case protocols::CrossTerminal::SYMBOL_COMMA: return "SYMBOL_COMMA";
		case protocols::CrossTerminal::SYMBOL_DOLLAR: return "SYMBOL_DOLLAR";
		case protocols::CrossTerminal::SYMBOL_DOLLAR__DOLLAR: return "SYMBOL_DOLLAR__DOLLAR";
		case protocols::CrossTerminal::SYMBOL_DOT: return "SYMBOL_DOT";
		case protocols::CrossTerminal::SYMBOL_EQUAL: return "SYMBOL_EQUAL";
		case protocols::CrossTerminal::SYMBOL_EQUAL__EQUAL: return "SYMBOL_EQUAL__EQUAL";
		case protocols::CrossTerminal::SYMBOL_EXCLAMATION_MARK: return "SYMBOL_EXCLAMATION_MARK";
		case protocols::CrossTerminal::SYMBOL_EXCLAMATION_MARK__EQUAL: return "SYMBOL_EXCLAMATION_MARK__EQUAL";
		case protocols::CrossTerminal::SYMBOL_EXCLAMATION_MARK__EXCLAMATION_MARK: return "SYMBOL_EXCLAMATION_MARK__EXCLAMATION_MARK";
		case protocols::CrossTerminal::SYMBOL_EXCLAMATION_MARK__EXCLAMATION_MARK__EQUAL: return "SYMBOL_EXCLAMATION_MARK__EXCLAMATION_MARK__EQUAL";
		case protocols::CrossTerminal::SYMBOL_FORWARD_SLASH: return "SYMBOL_FORWARD_SLASH";
		case protocols::CrossTerminal::SYMBOL_FORWARD_SLASH__EQUAL: return "SYMBOL_FORWARD_SLASH__EQUAL";
		case protocols::CrossTerminal::SYMBOL_GREATER_THAN: return "SYMBOL_GREATER_THAN";
		case protocols::CrossTerminal::SYMBOL_GREATER_THAN__EQUAL: return "SYMBOL_GREATER_THAN__EQUAL";
		case protocols::CrossTerminal::SYMBOL_GREATER_THAN__GREATER_THAN: return "SYMBOL_GREATER_THAN__GREATER_THAN";
		case protocols::CrossTerminal::SYMBOL_GREATER_THAN__GREATER_THAN__EQUAL: return "SYMBOL_GREATER_THAN__GREATER_THAN__EQUAL";
		case protocols::CrossTerminal::SYMBOL_LESS_THAN: return "SYMBOL_LESS_THAN";
		case protocols::CrossTerminal::SYMBOL_LESS_THAN__EQUAL: return "SYMBOL_LESS_THAN__EQUAL";
		case protocols::CrossTerminal::SYMBOL_LESS_THAN__LESS_THAN: return "SYMBOL_LESS_THAN__LESS_THAN";
		case protocols::CrossTerminal::SYMBOL_LESS_THAN__LESS_THAN__EQUAL: return "SYMBOL_LESS_THAN__LESS_THAN__EQUAL";
		case protocols::CrossTerminal::SYMBOL_MINUS: return "SYMBOL_MINUS";
		case protocols::CrossTerminal::SYMBOL_MINUS__EQUAL: return "SYMBOL_MINUS__EQUAL";
		case protocols::CrossTerminal::SYMBOL_MINUS__GREATER_THAN: return "SYMBOL_MINUS__GREATER_THAN";
		case protocols::CrossTerminal::SYMBOL_MINUS__MINUS: return "SYMBOL_MINUS__MINUS";
		case protocols::CrossTerminal::SYMBOL_PERCENT: return "SYMBOL_PERCENT";
		case protocols::CrossTerminal::SYMBOL_PERCENT__EQUAL: return "SYMBOL_PERCENT__EQUAL";
		case protocols::CrossTerminal::SYMBOL_PLUS: return "SYMBOL_PLUS";
		case protocols::CrossTerminal::SYMBOL_PLUS__EQUAL: return "SYMBOL_PLUS__EQUAL";
		case protocols::CrossTerminal::SYMBOL_PLUS__PLUS: return "SYMBOL_PLUS__PLUS";
		case protocols::CrossTerminal::SYMBOL_SEMICOLON: return "SYMBOL_SEMICOLON";
		case protocols::CrossTerminal::SYMBOL_STAR: return "SYMBOL_STAR";
		case protocols::CrossTerminal::SYMBOL_STAR__EQUAL: return "SYMBOL_STAR__EQUAL";
		case protocols::CrossTerminal::SYMBOL_TILDE: return "SYMBOL_TILDE";
		case protocols::CrossTerminal::TEMPLATE_LIST: return "TEMPLATE_LIST";
		case protocols::CrossTerminal::THROW_STATEMENT: return "THROW_STATEMENT";
		case protocols::CrossTerminal::TRY_BLOCK: return "TRY_BLOCK";
		case protocols::CrossTerminal::TYPE: return "TYPE";
		case protocols::CrossTerminal::TYPE_AFTER_CONST: return "TYPE_AFTER_CONST";
		case protocols::CrossTerminal::TYPE_AFTER_PTR: return "TYPE_AFTER_PTR";
		case protocols::CrossTerminal::TYPE_AFTER_REF: return "TYPE_AFTER_REF";
		case protocols::CrossTerminal::TYPE_AFTER_REF_CONST: return "TYPE_AFTER_REF_CONST";
		case protocols::CrossTerminal::TYPE_EXPRESSION: return "TYPE_EXPRESSION";
		case protocols::CrossTerminal::TYPE_OR_EXPRESSION: return "TYPE_OR_EXPRESSION";
		case protocols::CrossTerminal::UNARY_EXPRESSION: return "UNARY_EXPRESSION";
		case protocols::CrossTerminal::UNARY_OPERATOR: return "UNARY_OPERATOR";
		case protocols::CrossTerminal::UNIDENTIFIED: return "UNIDENTIFIED";
		case protocols::CrossTerminal::VARIABLE_NAMES: return "VARIABLE_NAMES";
		case protocols::CrossTerminal::VAR_OR_STATIC: return "VAR_OR_STATIC";
		case protocols::CrossTerminal::WHEN_EXPRESSION: return "WHEN_EXPRESSION";
		case protocols::CrossTerminal::WHILE_STATEMENT: return "WHILE_STATEMENT";
		default: return "";
	}
}

}}
