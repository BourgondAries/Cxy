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

#include "protocols/ParseReturn.hpp"

#include <map>
#include <set>
#include <vector>


namespace tul { namespace libraries {

/**
	The goal of this class is to hold data about the production rules and
	act as a map between rule and lookahead terminal.

	This simply means that this class - when given a 'current symbol' and a
	lookahead symbol - will decide which production of an LL(1) grammar to
	return. The production comes in the form of a struct. This struct is
	called "ParseReturn". It contains a pointer to a vector of children for
	the 'current symbol'.

	A function or class that invokes this class is one that implements
	a stack of tokens.

	A rule should not be added after parseSymbol is called. This is because
	the returned vector is a pointer and may be invalidated.
*/
template <typename SYMBOL>
class LLOneProductionDeducer
{
public:

	////////////////////////////////////////////////////////////
	#define MapIteratorType typename std::map<SYMBOL, std::vector<SYMBOL>>::iterator
	#define thisSymbolHasAnEpsilonRule() epsilonable_symbols.find(top_of_stack) != epsilonable_symbols.cend()
	#define thereExistsNoProduction() iterator_over_map == production_rules[top_of_stack].cend()
	#define getEntry() production_rules[top_of_stack].find(symbol_to_parse)

	protocols::ParseReturn<SYMBOL> parseSymbol(const SYMBOL &symbol_to_parse, const SYMBOL &top_of_stack)
	{
		using namespace protocols;
		if (symbol_to_parse == top_of_stack)
			return {nullptr, ParseReturn<SYMBOL>::Action::REMOVE_TOP};

		MapIteratorType iterator_over_map = getEntry();
		if (thereExistsNoProduction())
		{
			if (thisSymbolHasAnEpsilonRule())
				return {nullptr, ParseReturn<SYMBOL>::Action::EPSILONATE};
			return {nullptr, ParseReturn<SYMBOL>::Action::OBSERVE_ERROR};
		}
		return {&iterator_over_map->second, ParseReturn<SYMBOL>::Action::CONTINUE};
	}

	#undef getEntry
	#undef thereExistsNoProduction
	#undef thisSymbolHasAnEpsilonRule
	////////////////////////////////////////////////////////////

	void addRule(const SYMBOL &left_side, const SYMBOL &transition_symbol, const std::vector<SYMBOL> &right_side)
	{
		production_rules[left_side][transition_symbol] = right_side;
	}

	void addEpsilon(const SYMBOL &left_side)
	{
		epsilonable_symbols.emplace(left_side);
	}

	bool doesRuleExist(const SYMBOL &left_side, const SYMBOL &transition_symbol)
	{
		MapIteratorType iterator_over_map = production_rules[left_side].find(transition_symbol);
		return iterator_over_map != production_rules[left_side].cend();
	}

	#undef MapIteratorType

	std::vector<SYMBOL> getTransitionTokens(const SYMBOL symbol) const
	{
		auto iterator = production_rules.find(symbol);
		if (iterator == production_rules.end())
			return {};
		auto &transition_map = iterator->second;
		std::vector<SYMBOL> to_return;
		for (auto &prod : transition_map)
			to_return.emplace_back(prod.first);
		return to_return;
	}

private:

	std::set<SYMBOL> epsilonable_symbols;
	std::map<SYMBOL, std::map<SYMBOL, std::vector<SYMBOL>>> production_rules;

};

}}
