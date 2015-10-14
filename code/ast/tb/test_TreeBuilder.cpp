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
#include "TreeBuilder.hpp"
#include "libraries/catch.hpp"
#include "protocols/CrossTerminal.hpp"
#include "protocols/CrossTerminalTools.hpp"

#include "protocols/ParseReturn.hpp"

#include <cassert>
#include <iostream>


////////////////////////////////////////////////////////////////////////////////
// This is only a small utility validator... if you want to see how
// TreeBuilder works just go down to "TEST_CASE"
////////////////////////////////////////////////////////////////////////////////
namespace
{
	bool validate(const std::string &string, bool print_error_if_exists = true)
	{
		using namespace tul::tb;
		TreeBuilder builder_object;
		bool ret_val = true;
		char current = '\0';
		for (auto input_character : string)
		{
			if (builder_object.buildTree(input_character) == false)
			{
				ret_val = false;
				break;
			}
			current = input_character;
		}
		if (ret_val)
			ret_val = builder_object.endInput();
		if (ret_val == false && print_error_if_exists)
		{
			std::cout << "last char: " << current << std::endl;
			std::vector<std::string> expected = builder_object.getExpectedTokens();
			std::cout << "\nError: expected:\n";
		}
		return ret_val;
	}
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// The API is quite simple. It is demonstrated and tested here.
////////////////////////////////////////////////////////////////////////////////
TEST_CASE("TreeBuilder must validate input", "[test-TreeBuilder]")
{
	using namespace tul::tb;

	SECTION("Attempting to validate a small program")
	{
		// Create an instance and feed characters into it.
		// Every time a character is fed the return value
		// tells you if the input is still valid.
		TreeBuilder builder_object;
		std::string input_string = "var {ClassName variable_name; String identifier_name; } ";
		// Note the trailing space in the string. This causes the TreeBuilder to be able
		// To turn ; into a token, and thereby finish the input. If we left it out
		// The ; would merely be in a "current_working" state. Nothing would be invalidated,
		// It's just that we would not see it in the tree.
		for (auto input_character : input_string)
		{
			// Note that each character is validated, and aded to the tree if it completes
			// a token. Data structure:
			/*
				string: current building token. (characters are appended)
				TokenTree
				| --> TokenTree
							| --> TokenTree
				| --> TokenTree --> TokenTree

				The tokentree is a tree of tokens. Once the current building token
				is finished, it's appended to the tree by the internal parser.
			*/
			REQUIRE(builder_object.buildTree(input_character));
		}

		// Eventually, we may want to get that tree
		REQUIRE(builder_object.getConcreteSyntaxTree() != nullptr);
	}
	////////////////////////////////////////////////////////////////////////////////
	SECTION("Parse the start of a module: declarations. Some permutations.")
	{
		REQUIRE(validate(R"(
			var {
				1u b;
				1u a;
				1u c;
			}
		)"));
		REQUIRE(validate(R"(
			var {
				1u a;
				1u c;
				1u b;
			}
		)"));
		REQUIRE(validate(R"(
			var {
				1u c;
				1u b;
				1u a;
			}
		)"));
	}
	////////////////////////////////////////////////////////////////////////////////
	SECTION("Parse the start of a module: declarations and assignments")
	{
		REQUIRE(validate(R"(
			var {
				1u b = 1;
				1u a = 2;
				1u c = 3;
			}
		)"));
		REQUIRE(validate(R"(
			var {
				1u a(value: 4);
				1u c(value: 5);
				1u b(value: 6);
			}
		)"));
		REQUIRE(validate(R"(
			var {
				1u c(value: 7);
				1u b = 8;
				1u a;
			}
		)"));
	}
	////////////////////////////////////////////////////////////////////////////////
	SECTION("Check non-data class with expressions")
	{
		REQUIRE(validate("(:) enter { }"));
		REQUIRE(validate("(:) enter { a = \"Hi!\"; }"));
		REQUIRE(validate("(:) enter { a = \"Hi!\"; b = 300; }"));
		REQUIRE(validate("(:) enter { a = \"Hi!\"; b = 300; b_a =  b; }"));
		REQUIRE(validate("(:) enter { a = \"Hi!\"; b = 300; b_a = 10 * b; }"));
		REQUIRE(validate("(:) enter { b_a = 100 && 10 * b + 5 / 3 - another_identifier; }"));
		REQUIRE(validate("(:) enter { b_a = 100 && 10 * b + 5 / 3 - another_identifier | \"Cool m8\"; }"));
		REQUIRE(validate("(:) enter { b_a = sampleFunction()~a; }"));
		REQUIRE(validate("(:) enter { b_a = 100 && 10 * b + 5 / 3 * sampleFunction()~a - another_identifier | \"Cool m8\"; }"));
		REQUIRE(validate(R"(
			(:) enter
			{
				b_a = 100 && 10 * b + 5 / 3 * sampleFunction()~a - another_identifier | "Cool m8";
				q_a_d = 100 && 10 * b + 5 / 3 * sampleFunction()~a - another_identifier | "Cool m8";
				b_a = 100 && 10 * b + 5 / 3 * sampleFunction()~a - another_identifier | "Cool m8";
				b_a = 100 && 10 * b + 5 / 3 * sampleFunction()~a ^sampleFunction()~a - another_identifier | "Cool m8";
				var Type b_a = 100 && 10 * b + 5 / 3 * sampleFunction()~a ^sampleFunction()~a - another_identifier | "Cool m8";
				var 800u b_a = 100 && 10 * b + 5 / 3 * sampleFunction()~a ^sampleFunction()~a - another_identifier | "Cool m8";
				var 8s b_a = 100 && 10 * b + 5 / 3 * sampleFunction()~a ^sampleFunction()~a - another_identifier | "Cool m8";
				var AnotherClassName b_a = 100 > 5 && 10 * b + 5 / 3 * sampleFunction()~a ^sampleFunction()~a - another_identifier | "Cool m8";
				var AnotherClassName b_a = when (a) b else c;
			}
		)"));
		REQUIRE(false == validate("(:) enter { var const ClassName ", false));
		REQUIRE(validate("(:) enter { var const ptr ClassName alpha; }"));
		REQUIRE(false == validate("(:) enter { var const const ClassName alpha; }", false));
		REQUIRE(validate("(:) enter { var const ptr const ClassName alpha; }"));
	}
	////////////////////////////////////////////////////////////////////////////////
	SECTION("A class using different statement types")
	{
		REQUIRE(validate(R"(
			(:) enter
			{
				var 32u max = 1000;
				var 32u counter = 0;
				while (counter < 1000)
				{
					if (counter > 500 + 20)
					{ printSomething(); }
					for (const 33s cuda = 10; cuda < 20; ++cuda; --cuda;)
					{
						++global_var;
					}
				}
			}
		)"));
	}
	////////////////////////////////////////////////////////////////////////////////
	SECTION("Multiple function definitions")
	{
		////////////////////////////////////////////////////////////
		REQUIRE(validate(R"(
			var 32u variable = 0;

			(:) enter
			{
				++variable;
			}

			(:) performCalculation
			{
				--variable;
			}
		)"));
		////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////
		REQUIRE(validate(R"(
			var 32u variable = 0;

			(:) enter
			{
				++variable;
			}

			(:) performCalculation
			{
				--variable;
			}
		)"));
		////////////////////////////////////////////////////////////
	}
	////////////////////////////////////////////////////////////
	SECTION("Unary operators")
	{
		REQUIRE(validate(R"(
			(:) enter
			{
				var 32u a = 1000;
				var ptr 32u b = $a;
			}
		)"));
		REQUIRE(validate(R"(
			(:) enter
			{
				var 32u a = 1000;
				var ptr 32u b = $a;
				++@b;
			}
		)"));
		REQUIRE(validate(R"(
			(:) enter
			{
				var 32u a = 1000;
				var ptr 32u b = $$a;
			}
		)"));
		REQUIRE(validate(R"(
			(:) enter
			{
				var 32u a = 1000;
				var ptr const 32u b = $$a;
			}
		)"));
		REQUIRE(validate(R"(
			(:) enter
			{
				var 32u a = 1000;
				var ptr const 32u b = $$a;
				var 32u c = @@b;
			}
		)"));
		REQUIRE(validate(R"(
			(:) enter
			{
				var 32u a = 1000;
				var ptr 32u b = $a;
				++ @b;
			}
		)"));
		REQUIRE(validate(R"(
			(:) enter
			{
				var 32u a = 1000;
				var ptr 32u b = $a;
				if (!! (@b > 100))
				{

				}
			}
		)"));
		REQUIRE(validate(R"(
			(:) enter
			{
				var 32u a = 1000;
				var ptr 32u b = $a;
				if (!! (@b > 100))
				{

				}
			}

			(: : pure) funcTion
			{

			}
		)"));
		REQUIRE(validate(R"(
			(:) enter
			{
				var 32u a = 1000;
				var ptr 32u b = $a;
				if (!! (@b > 100))
				{

				}
			}

			(: : const) strToInt
			{}

			(: : const pure) funcTion
			{}


			(:: const) strToInte
			{}

			(:: const pure) funcTione
			{}
		)"));
	}
	////////////////////////////////////////////////////////////
	SECTION("Argument Lists")
	{
		REQUIRE(validate(R"(
			(32u a:32u b) myFunction
			{}
		)"));
		REQUIRE(validate(R"(
			(32u a, Class b:32u c) myFunction
			{}
		)"));
		REQUIRE(validate(R"(
			(32u a, Class b : Class c, 32u d) myFunction
			{}
		)"));
		REQUIRE(validate(R"(
			(32u a, 32u b : Class c, Class c2, 32u d, 32u e) myFunction
			{}
		)"));
		REQUIRE(validate(R"(
			([10, 32u] a, [10, 32u] b : Class c, Class c2, 32u d, 32u e) myFunction
			{}
		)"));
	}
	////////////////////////////////////////////////////////////
	SECTION("Array types")
	{
		REQUIRE(validate(R"(
			(:) enter
			{
				var [3, 32u] arr;
				arr[1] = 200;
			}

			(:) anotherFunction
			{
				var [2, [3, [4, [5, 8u]]]] szt;
			}
		)"));
	}
	////////////////////////////////////////////////////////////
	SECTION("Putting variables in the middle is not allowed")
	{
		REQUIRE(false == validate(R"(
			(:) enter
			{}

			32u var;
		)", false));
	}
	////////////////////////////////////////////////////////////
	SECTION("Type casting")
	{
		REQUIRE(validate(R"(
			(:) enter {
				var 32u a = 100;
				var 16u b = cast[16u](a) * 5;
				var 32u c = when (a > 100) a else b;
				var [8, 32u] y;
			}
		)"));
	}
	////////////////////////////////////////////////////////////
	SECTION("Member expression")
	{
		REQUIRE(validate(R"(
			(:) enter
			{
				var SomeClass a = SomeClass.createInstance()~instance;
				var SomeClass b = SomeClass.something()~ck;
				var SomeClass c = sys.Cool.something()~ck()~x[3 + 8];
				var Qs d = q.Qz.ctOr()~obj;
			}
		)"));
		REQUIRE(validate(R"(
			(:) enter
			{
				var Aa a = Aa();
			}
		)"));
		REQUIRE(validate(R"(
			(:) enter
			{
				var a.Aa a = Aa();
				a = b;
			}
		)"));
		REQUIRE(validate(R"(
			(:) enter
			{
				{

				}
				var a.Aa a = a.Aa();
			}
		)"));
		REQUIRE(validate(R"(
			(:) enter
			{
				var a.Aa a = a.b.Aa();
			}
		)", false));
		REQUIRE(false == validate(R"(
			(:) enter
			{
				var a.b.Aa a = a.Aa();
			}
		)", false));
		REQUIRE(validate(R"(
			(:) enter
			{
				var sys.Dir dir(start: "/");
				var sml.String paths;
				while (dir.hasChild()~yes)
				{
					 paths += dir.getCurrent()~child[1 + f()~g[a.Aa.h()~j()~k[l]]];
				}

				a.Aa.g += f;
			}
		)"));
		REQUIRE(false == validate(R"(
			(:) enter
			{
				a = b = c;
			}
		)", false));

		REQUIRE(validate(R"(
			(:) enter
			{
				a[0][1][2];
			}
		)"));
		REQUIRE(validate(R"(
			(:) enter
			{
				a[0, 1, 2];
			}
		)"));
		REQUIRE(validate(R"(
			(:) enter
			{
				sys.StdOut.printLine(string: callChild())~alpha;
			}

			(23u nm, 23u top_kek : 800u kek) topKek
			{
				return nm: kek + 200;
			}
		)"));
		REQUIRE(validate(R"(
			(:) enter
			{
				if (a)
					aAa();
				else if (b)
					bBb();
				else if (c)
					cCc();
				else
					dDd();
			}
		)"));
		REQUIRE(validate(R"(
			(:) enter
			{
				if (a)
				{  aAa();  }
				else if (b)
				{  bBb();  }
				else if (c)
				{  cCc();  }
				else
				{ dDd();   }
				if (e)
					what;
			}
		)"));
		REQUIRE(validate(R"(
			(:) enter
			{
				if (a)
					if (b)
						what;
					else
						while (c)
							inside_while;
			}
		)"));
		REQUIRE(validate(R"(
			(:) enter
			{
							hack
				(
							assembly: "mov %eax, %ebx"
				);
			}
		)"));
		REQUIRE(validate(R"(
			(:) enter
			{
				statics if (3)
				{
					a + b;
				}
			}
		)"));
		REQUIRE(validate(R"(
			(:) enter
			{
				statics if (sys.Machine.memory_size)
				{
					a + b;
				}
				statics else
				{
					statics if (sys.Machine.processor_type)
					{
						a - b;
					}
					statics else
					{
						a * b;
					}
				}
			}
		)"));
		REQUIRE(validate(R"(
			(:) enter
			{
				defer a * 4 + someStuff()~a;
				var Object object;
				object.initObject();
				defer
				{
					object.destroyObject();
				}
			}
		)"));
		REQUIRE(validate(R"(
			(:) enter
			{
				statics if (sys.Sys.argv[1])
				{
					var 32u x = computeSomething(in: 100, sec: 100 * 3)~value;
					sys.Out.printLn(val: x);
				}
			}

			(32u value : 64s in, 64s sec : const pure) computeSomething
			{
				var SomeClass object(arg: in);
				if (sec > 100)
				{
					defer object.doAbove();
					object.doSense();
				}
				return value: object.getVariable()~gotten;
			}
		)"));
		REQUIRE(validate(R"(
			grant Name {
				32u a, b, c;
				(32u a : 32u b : pure) d, e, f;
			}
		)"));
		REQUIRE(validate(R"(
			grant String
			{
				64u length;
				(String out : String left, String right) +;
			}

			(:) a
			{
				var String b;
				b.length == 0;
			}
		)"));
		REQUIRE(validate(R"(
			(:) enter
			{
				var ptr (:) a = lambda [](:){ sml.Out.print(:"Hi"); };
			}
		)"));
		REQUIRE(validate(R"(
			(:) enter
			{
				var 32u a = 1;
				var ptr (:) b = lambda [a $a @a $$a @@a](:){ sml.Out.print(:"Hi"); };
			}
		)"));
		REQUIRE(validate(R"(
			(:) enter
			{
				var 32u a = 1;
				var ptr (:) b =
					lambda [a$a@a$$a@@a](:)
					{
						sml.Out.print(:"Hi");
						for (32u i = 1; i < 3; ++i;)
							sml.Out.print(:i);
					};
			}
		)"));
		REQUIRE(validate(R"(
			(: Aa in, Aa b) doSomething {
				// ...
			}
			(:) enter {
				var Aa a = 1, b = 2;
				doSomething(: a, b: b);
				var float x = sin(:32);
			}
		)"));
		REQUIRE(validate(R"(
			(:) enter {
				sml.Out.print(
					: sml.Math.integrateNumericallySimpsons(
						: lambda [](double out : double in){ sml.Math.exp(:-in*in); },
						elements: 800,
						from: -1,
						to: 1
					)
				);
			}
		)"));
		REQUIRE(validate(R"(
			(:) enter {
				var sml.Vector accumulator;
				accumulator.map(
					: lambda (double out : double in){ sml.Math.exp(:-in*in); }
				);
			}
		)"));
		REQUIRE(validate(R"(
			(:) enter {
				var sml.Vector[Xx : float] accumulator;
				var sml.Vector[Xx : sml.String[:8u]] accumulator;
			}
		)"));
		REQUIRE(validate(R"(
			alias c = complicatedlibrary;
			(:) enter {
				alias d = anotherlonglibraryname;
				var c.Curve curve(
					:lambda(float out:float in){return :sml.Math.sin(:in);},
					from: -3,
					to: 3
				);
				sml.Out.print(:curve.getGraph());
				var ptr ref ptr const ptr const [80, ptr ([3, 8u] out:)] d;
				var ref ptr ptr ptr const ptr const [80, ptr ([3, ref ptr 8u] out:)] d;
				var This[Gg:Yy] me;
				if (me.doSomething(:d)~error) {
					sml.Out.print(:"Something went wrong");
					label x;
					goto x;
				}
			}
		)"));
		#define doValidation(x) REQUIRE(validate(x))
		doValidation(R"(
			(:) enter
			{
				alias sf = simplefastmedialibrary;
				var sf.RenderWindow window(title: "Main window", width: 800, height: 600);
				var sf.Circle circle(radius: 5.f, x: 0, y: 0);
				window.clear();
				window.draw(:circle);
				sml.sleep(seconds: 5);
				window.display();
			}
		)");

		doValidation(R"(
			var {
				float x1;
				32u y1 = 2;
				sml.String z1(:"z");
			}

			(:) enter
			{
				var {
					float x2;
					32u y2 = 2;
					sml.String z2(:"z");
				}
			}
		)");

		doValidation(R"(
			alias Aa = [100, ptr ref const 32u];
			(:(:) in) call { in(); }

			(:) enter {
				call(:lambda(:){ sml.Out.print(:"Hey!");});
			}
		)");
		doValidation(R"(
			(:) enter {
				alias Out = sml.Out;
				var (: String in) print = cast[(: String in)](Out.print);
				print(:"Anything");
			}
		)");
		doValidation(R"(
			(:) enter {
				alias Long = [5, ptr [8, ref const 32u]];
				var Long a, b, c;
				// ...
			}
		)");
		#define doInvalidation(x) REQUIRE(false == validate(x))
		doInvalidation(R"(
			(:) enter {
				alias Long = ptr 1;
			}
		)");
		doValidation(R"(
			(:) enter {
				var ptr 8u a = new[8u](1024);
				sml.Out.print(:delete(a));
			}
		)");
		doValidation(R"(
			(:) enter {
				// Placement new on an array
				var ptr 8u a = new[8u](1024, delete(new[8u](1024)));
				sml.Out.print(:delete(a));

				// Raw new
				var ptr Type b = new[Type];

				// Initializing new
				var ptr 32u c = new[32u]{:3};

				// Initializing new on an array
				var ptr 32u d = new[32u](4){:4};

				// Initialize a two-dimensional array
				var ptr ptr 32u e = new[ptr 32u](5){:new[32u](5){:0}};
			}
		)");
		doValidation(R"(
			var 32u a = [1, 2];
		)");
		doValidation(R"(
			// Check overloadable operators
			// Binary operators
			(:) + {}
			(:) ++ {}
			(:) - {}
			(:) -- {}
			(:) * {}
			(:) / {}
			(:) % {}

			// Compound operators
			(:) += {}
			(:) -= {}
			(:) *= {}
			(:) /= {}
			(:) %= {}
			(:) <<= {}
			(:) >>= {}

			// Comparison operators
			(:) == {}
			(:) != {}
			(:) > {}
			(:) < {}
			(:) >= {}
			(:) <= {}

			// Shift operators
			(:) << {}
			(:) >> {}
		)");
	}
}
