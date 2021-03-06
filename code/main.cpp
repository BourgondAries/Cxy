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
#define CATCH_CONFIG_RUNNER  // Only in one cpp file
#include "libraries/catch.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

#include "arg/Argument.hpp"
#include "ast/AbstractSyntaxTreeGenerator.hpp"


int main(int argc, char *argv[])
{
	int result = 1;
	ttl::Argument argument(argc, argv);
	if (argument.isPassed("help")) {
		std::cout
			<< "Cxy Compiler Reference Implementation v0.1.0-draft.3\n"
			<< "--test\t\tPerform self-tests\n"
			<< "--check-syntax file\t\tCheck the syntax of a file\n"
			<< std::endl;
		return 0;
	}
	if (argument.isPassed("test"))
		result = Catch::Session().run(1, &argv[0]);
	else if (argument.isPassed("check-syntax"))
	{
		std::string file = argument.getArgument("check-syntax");
		if (file == "")
		{ std::cout << "--check-syntax takes one file" << std::endl; }
		using namespace tul::ast;
		AbstractSyntaxTreeGenerator astg;
		try {
			std::fstream stream(file.c_str(), std::ios::in);
			if (stream.good()) {
				std::stringstream ss;
				ss << stream.rdbuf();
				astg.buildTree(ss.str());
			} else {
				std::cerr << "File `" << file << "' does not exist.\n";
			}
		} catch (std::string string) {
			std::cout << string << std::endl;
			return 1;
		}
	}
	else
	{

		std::cerr << "cxyc: fatal error: no input files\ncompilation terminated.\n";
	}
	return result;
}
