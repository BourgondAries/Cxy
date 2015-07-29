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
#include "libraries/catch.hpp"
#include "SymbolMatcher.hpp"


TEST_CASE("Test symbol matcher", "[test-SymbolMatcher]")
{
	SECTION("Attempting to match a few symbols")
	{
		using namespace tul::dependency;
		using namespace tul::protocols;
		auto checkReturn = [](const std::string &lexeme) -> bool { return  SymbolMatcher::getSymbol(lexeme) != TokenType::UNIDENTIFIED; };
		REQUIRE(checkReturn("++"));
		REQUIRE(checkReturn("-"));
		REQUIRE(checkReturn("^"));
		REQUIRE(!checkReturn("if"));
	}
}
