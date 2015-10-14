// Copyright © 2015 Kevin Robert Stravers
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

namespace tul { namespace protocols { 

enum class CrossTerminal
{
	ACCESS_SPECIFIER,
	ACCESS_SPECIFIER_READABLE,
	ADDITIVE_EXPRESSION,
	ALIAS_CORE,
	ALIAS_STATEMENT,
	ALIAS_TRAIL,
	ALIAS_TRAILS,
	AND_EXPRESSION,
	ARGUMENT,
	ARGUMENT_LIST,
	ARRAY,
	ARRAY_ACCESS_EXPRESSION,
	ASSIGNMENT_EXPRESSION,
	ATTRIBUTE_LIST,
	ATTRIBUTE_LIST_AFTER_CONST,
	BASIC_TYPE,
	BITWISE_AND_EXPRESSION,
	BITWISE_OR_EXPRESSION,
	BITWISE_XOR_EXPRESSION,
	CAPTURE_LIST,
	CAST_EXPRESSION,
	CLASS_MEMBER,
	CLASS_MEMBER_EXPRESSION,
	CODE_BLOCK,
	DATA_DECLARATION_LIST,
	DATA_DECLARATION_STATEMENT,
	DATA_DECLARATION_TYPE,
	DATA_NAMES,
	DATA_NAMES_LIST,
	DEFER_STATEMENT,
	DELETE_EXPRESSION,
	DO_STATEMENT,
	ELSE_STATEMENT,
	ENTER,
	ENUMERATION_MEMBER_EXPRESSION,
	EPSILONATE,
	EQUALITY_EXPRESSION,
	EXPRESSION_EXPRESSION,
	EXPRESSION_LIST,
	FOR_DATA_DECLARATION,
	FOR_STATEMENT,
	FUNCTION_NAMES,
	FUNCTION_SIGNATURE,
	GOTO_STATEMENT,
	GRANT_LIST,
	GRANT_LIST_ACCESS,
	HACK_STATEMENT,
	IF_STATEMENT,
	ITER_STATEMENT,
	LABEL_STATEMENT,
	MEMBER_DEFINITION,
	MEMBER_EXPRESSION,
	MULTIPLICATIVE_EXPRESSION,
	NAMEKIND,
	NEW_EXPRESSION,
	NO_SEMICOLON_STATEMENT,
	OPTIONAL_ADDITIVE_EXPRESSION,
	OPTIONAL_AND_EXPRESSION,
	OPTIONAL_ARGUMENT_LIST,
	OPTIONAL_ARGUMENT_LIST_AFTER_COMMA,
	OPTIONAL_ARRAY_ACCESS_EXPRESSION,
	OPTIONAL_ARRAY_ACCESS_EXPRESSION_AFTER_COMMA,
	OPTIONAL_ASSIGNMENT,
	OPTIONAL_ASSIGNMENT_EXPRESSION,
	OPTIONAL_ATTRIBUTE_LIST,
	OPTIONAL_BITWISE_AND_EXPRESSION,
	OPTIONAL_BITWISE_OR_EXPRESSION,
	OPTIONAL_BITWISE_XOR_EXPRESSION,
	OPTIONAL_CAPTURE_LIST,
	OPTIONAL_CONSTRUCTOR_LIST,
	OPTIONAL_DATA_DECLARATION,
	OPTIONAL_DATA_DECLARATION_AFTER_COMMA,
	OPTIONAL_DATA_NAMES_LIST,
	OPTIONAL_DATA_NAMES_LIST_AFTER_COMMA,
	OPTIONAL_DATA_NAME_LIST,
	OPTIONAL_EQUALITY_EXPRESSION,
	OPTIONAL_EXPRESSION,
	OPTIONAL_EXTRACTOR_EXPRESSION,
	OPTIONAL_FUNCTION_NAME_LIST,
	OPTIONAL_GRANT_LIST,
	OPTIONAL_MEMBER_EXPRESSION,
	OPTIONAL_MULTIPLICATIVE_EXPRESSION,
	OPTIONAL_NEW_LIST,
	OPTIONAL_OR_EXPRESSION,
	OPTIONAL_PARAMETER_LIST,
	OPTIONAL_PARAMETER_LIST_AFTER_COMMA,
	OPTIONAL_RELATIONAL_EXPRESSION,
	OPTIONAL_TEMPLATE,
	OPTIONAL_TEMPLATE_LIST,
	OR_EXPRESSION,
	PARAMETER_LIST,
	RELATIONAL_EXPRESSION,
	RELATIONAL_OPERATOR,
	RESOURCE,
	RETURN_STATEMENT,
	SINGLE_STATEMENT_OR_CODE_BLOCK,
	STATEMENT,
	STATEMENT_LIST,
	STATIC_ELSE_STATEMENT,
	STATIC_IF_STATEMENT,
	TEMPLATE_LIST,
	TYPE,
	TYPE_AFTER_CONST,
	TYPE_AFTER_PTR,
	TYPE_AFTER_REF,
	TYPE_AFTER_REF_CONST,
	TYPE_EXPRESSION,
	UNARY_EXPRESSION,
	UNARY_OPERATOR,
	VARIABLE_NAMES,
	VAR_OR_STATIC,
	WHEN_EXPRESSION,
	WHILE_STATEMENT,
	END_OF_MODULE,
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
	KEYWORD_ALIAS,
	KEYWORD_CAST,
	KEYWORD_CONST,
	KEYWORD_DEFER,
	KEYWORD_DELETE,
	KEYWORD_DO,
	KEYWORD_DOUBLE,
	KEYWORD_ELSE,
	KEYWORD_FLOAT,
	KEYWORD_FOR,
	KEYWORD_GOTO,
	KEYWORD_GRANT,
	KEYWORD_HACK,
	KEYWORD_IF,
	KEYWORD_LABEL,
	KEYWORD_LAMBDA,
	KEYWORD_NEW,
	KEYWORD_PRIVATE,
	KEYWORD_PTR,
	KEYWORD_PUBLIC,
	KEYWORD_PURE,
	KEYWORD_REF,
	KEYWORD_RESTRICTED,
	KEYWORD_RETURN,
	KEYWORD_STATIC,
	KEYWORD_STATICS,
	KEYWORD_TYPE,
	KEYWORD_VAR,
	KEYWORD_WHEN,
	KEYWORD_WHILE,
	PRIMITIVE_SIGNED,
	PRIMITIVE_UNSIGNED,
	STRING,
	SYMBOL_AMPERSAND,
	SYMBOL_AMPERSAND__AMPERSAND,
	SYMBOL_APETAIL,
	SYMBOL_APETAIL__APETAIL,
	SYMBOL_BAR,
	SYMBOL_BAR__BAR,
	SYMBOL_CARET,
	SYMBOL_COLON__PRUNE,
	SYMBOL_COMMA__PRUNE,
	SYMBOL_DOLLAR,
	SYMBOL_DOLLAR__DOLLAR,
	SYMBOL_DOT__PRUNE,
	SYMBOL_EQUAL,
	SYMBOL_EQUAL__EQUAL,
	SYMBOL_EXCLAMATION_MARK,
	SYMBOL_EXCLAMATION_MARK__EQUAL,
	SYMBOL_EXCLAMATION_MARK__EXCLAMATION_MARK,
	SYMBOL_FORWARD_SLASH,
	SYMBOL_FORWARD_SLASH__EQUAL,
	SYMBOL_GREATER_THAN,
	SYMBOL_GREATER_THAN__EQUAL,
	SYMBOL_GREATER_THAN__GREATER_THAN,
	SYMBOL_GREATER_THAN__GREATER_THAN__EQUAL,
	SYMBOL_LESS_THAN,
	SYMBOL_LESS_THAN__EQUAL,
	SYMBOL_LESS_THAN__LESS_THAN,
	SYMBOL_LESS_THAN__LESS_THAN__EQUAL,
	SYMBOL_MINUS,
	SYMBOL_MINUS__EQUAL,
	SYMBOL_MINUS__MINUS,
	SYMBOL_PERCENT,
	SYMBOL_PERCENT__EQUAL,
	SYMBOL_PLUS,
	SYMBOL_PLUS__EQUAL,
	SYMBOL_PLUS__PLUS,
	SYMBOL_SEMICOLON__PRUNE,
	SYMBOL_STAR,
	SYMBOL_STAR__EQUAL,
	SYMBOL_TILDE__PRUNE,
	UNIDENTIFIED,
	ENUM_END
};
}}
