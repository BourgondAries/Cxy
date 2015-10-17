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
#include "protocols/CrossTerminalTools.hpp"
#include "TreePruner.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>


namespace tul { namespace tp {

void TreePruner::pruneTree(protocols::ConcreteSyntaxTree *ct)
{
	// Assume ct is the root of the tree.
	// Perform the following actions.
	// For each node, decide which can stay, recurse
	for (protocols::ConcreteSyntaxTree *child : ct->children)
		pruneTree(child);

	ct->children.erase
	(
		std::remove_if
		(
			ct->children.begin(), ct->children.end(),
			[](protocols::ConcreteSyntaxTree *child)
			{
				assert(child != nullptr);
				#define caze(x) child->node_type == protocols::CrossTerminal::x ||
				bool predicate =
					caze(GROUPER_LEFT_BRACE)
					caze(GROUPER_LEFT_PARENTHESIS)
					caze(GROUPER_RIGHT_BRACE)
					caze(GROUPER_RIGHT_BRACKET)
					caze(GROUPER_RIGHT_PARENTHESIS)
					caze(KEYWORD_IF)
					caze(KEYWORD_WHILE)
					caze(KEYWORD_ELSE)
					caze(KEYWORD_WHEN)
					caze(SYMBOL_CARET)
					caze(SYMBOL_COLON)
					caze(SYMBOL_SEMICOLON)
					false;
				#undef caze
				if (predicate)
					delete child;
				return predicate;
			}
		),
		ct->children.end()
	);

	using namespace protocols;
	auto popfirst = [&](CrossTerminal cte) {
		for (std::size_t i = 0; i < ct->children.size(); ++i)
		{
			ConcreteSyntaxTree *child = ct->children.at(i);
			if ( child->node_type == cte) {
				assert(child->children.size() == 1);
				decltype(child) trans_child = child->children.at(0);
				child->children.clear();
				ct->children.at(i) = trans_child;
				delete child;
			}
		}
	};
	// Pull the NO_SEMICOLON and STATEMENT upward
	popfirst(CrossTerminal::NO_SEMICOLON_STATEMENT);
	popfirst(CrossTerminal::STATEMENT);

	auto popup = [&](CrossTerminal left, CrossTerminal right) {
		for (std::size_t i = 0; i < ct->children.size(); ++i) {
			ConcreteSyntaxTree *child = ct->children.at(i);
			if ( child->node_type == left ) {
				for (std::size_t j = 0; j < child->children.size(); ++j) {
					ConcreteSyntaxTree *child2 = child->children.at(j);
					if ( child2->node_type == right ) {
						for (std::size_t x = 0; x < j; ++x)
							child2->children.push_front(child->children.at(x));
						for (std::size_t x = j + 1; x < child->children.size(); ++x)
							child2->children.push_back(child->children.at(x));
						ct->children.at(i) = child2;
						child->children.clear();
						delete child;
					}
				}
			}
		}
	};
	popup(CrossTerminal::RELATIONAL_OPERATOR, CrossTerminal::SYMBOL_GREATER_THAN);
	popup(CrossTerminal::OPTIONAL_RELATIONAL_EXPRESSION, CrossTerminal::SYMBOL_GREATER_THAN);
	popup(CrossTerminal::RELATIONAL_EXPRESSION, CrossTerminal::SYMBOL_GREATER_THAN);
	// Prune all expressions that have one child or the second child as an epsilonate.
	for (
		std::size_t i = 0;
		i < ct->children.size();
		++i
	) {
		if (
			tul::protocols::CrossTerminalTools::isExpression(
				ct->children[i]->node_type)
			&& (
				(
					ct->children[i]->children.size() == 1
				) || (
					ct->children[i]->children.size() == 2
					&& ct->children[i]->children[1]->node_type
						== protocols::CrossTerminal::EPSILONATE
				)
			)
		) {
			assert(ct->children[i]->children[0]->node_type != protocols::CrossTerminal::EPSILONATE);
			// How do we bring that child up? We must make it a proper child of this node. How do we do this?
			// Detach it from the child:
			protocols::ConcreteSyntaxTree *transitive_child = ct->children[i]->children[0];
			ct->children[i]->children.erase(ct->children[i]->children.begin());
			// Now make sure that that child doesn't get deleted
			for (std::size_t j = 0; j < ct->children[i]->children.size(); ++j)
			{
				//protocols::ConcreteSyntaxTree *child = ct->children[i]->children[j];
				// std::cout << protocols::CrossTerminalTools::toString(child->node_type) << j << std::endl;
				delete ct->children[i]->children[j];
			}
			ct->children[i]->children.clear();
			// delete your own child
			delete ct->children[i];

			// Now assign the transitive child to this child.
			ct->children[i] = transitive_child;
		}
	}
}

}}
