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
	// All this does is feed the characters.
	bool validate(const std::string &string, bool throw_error = true)
	{
		using namespace tul::tb;
		TreeBuilder bdr;
		try
		{
			if (bdr.buildTree(string, throw_error))
				return true;
			return false;
		}
		catch (const std::string &string)
		{
			std::cout << string << std::endl;
			return false;
		}
		return false;
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
				if [3]
				{
					a + b;
				}
			}
		)"));
		REQUIRE(validate(R"(
			(:) enter
			{
				if [sys.Machine.memory_size]
				{
					a + b;
				}
				else
				{
					if [sys.Machine.processor_type]
					{
						a - b;
					}
					else
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
				if [sys.Sys.argv[1]]
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
				var 32u a, b, c;
				(32u a : 32u b : pure) d, e, f;
			}
		)"));
		REQUIRE(validate(R"(
			grant String
			{
				var 64u length;
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
						for ({32u i = 1; 32u j = 0;} i < 3; ++i;)
							sml.Out.print(:i);
						for (32u i = 1, j(:0); i < 3; ++i;)
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
		#define doInvalidation(x) REQUIRE(false == validate(x, false))
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
		doValidation(R"(
			(:) enter {
				var 32u a;
				var type[a] b = a;
			}
		)");
		doValidation(R"(
			(:) enter {
				var {
					[1, 32u] a;
					type[a] b(:a);
				}
			}
		)");
		doValidation(R"(
			restricted var { 32u a; 8s b = cast[8s](a); }
			var { 32u c; 8s d = cast[type[b]](c); }
			private var { sml.String e = "e"; }

			public (: 32u a(:5)) enter { }
		)");
		doValidation(R"(
			alias {}
			alias { Aa = type[a]; Bb = type[b]; }
			(:) enter {
				alias { sf = sfml; Aa = type[a]; Bb = type[b]; }
				alias abc = alphabet;
				alias Abc = abc.Alphabet;
			}
		)");
		doValidation(R"(
			var [10, 32u] a;

			(:) enter {
				a[1, 2, 3];
				a[1, 2, 3,];
				sml.Out.print(:a,);
			}
		)");
		doValidation(R"(
			var 32u a, b,;
			var { 32u a, b,; float c,; }
			static 32u a, b,;
			static { 32u a, b,; float c,; }

			(1u c,:1u a,) b { }

			grant Aa {
				public (:) a;
				restricted var 64u b;
			}
		)");
		doInvalidation(R"(
			grant Aa {
				private var 5u a;
			}
		)");
		doValidation(R"(
			(1u {a, b(:0)}, String e = "e",: type[e]{f, g, h,},) d {}
			(const ptr const 1u {a, b, c}, : ) d {}
		)");
		doValidation(R"(
			(:) enter {
				alias {
					St = sml.String;
					Ot = sml.Out;
				}
				var St x = "x";
				throw;
				throw OUT_OF_BOUNDS;
				try
					Ot.print(:x.at(:1));
				catch (enum[St].OUT_OF_BOUNDS)
					Ot.print(:name.serialize());
				catch (enum[St].OUT_OF_MEMORY)
					doSome();
				catch (enum[Af].WRONG_ALGORITHM) {
					debug cause;
				} catch (enum[What].SOMETHING_WRONG)
					debug 1 + 2 + 3;
			}
		)");
		doValidation(R"(
			(:) enter {
				var {
					const 32u a = 1;
					ref const 32u b = $a + 1;
				}
			}
		)");
		doValidation(R"(
			(113u out : 4u in) factorial {
				var type[out] out;
				out = cast[type[out]](in);
				--in;
				while (in > 1)
					out *= cast[type[out]](in);
				return out: out;
			}

			(:) enter {
				var String string = sml.In.read()~string;
				var 4u converted = cast[4u](string.to32u());
				sml.Out << "derp";
				alias Math = sml.Math;
				alias Out = sml.Out;

				Out << Math.pi + Math.e / Math;  // makes no sense but if the operators exist, it must be accepted.
			}
		)");
		doValidation(R"(
			(:) enter {
				a > b - c;
				while (a > b) {
					sml.Out << "Forever";
				} else {
					sml.Out << "Never";
				}
				@(a - 3) %= 3;
				@(a - 3) %= $axxa + 3;
			}
		)");
		doValidation(R"(
			(:) enter {
				var {32u a, b;}

				function({a: a, b: b,}: g(),);
				function({derp: herp, g: z,}: h() - j(), {a: a, b: b,}: g(),);
			}
		)");
		doValidation(R"(
			(: this, Type {x, z, y}) c
			{ }
			(:) enter {
				var type[this] self;
				var {32u a: a;} = f();
				var {32u a: a; Type b: from_f;} = f();
				var Type h = f({x: a,}: g())~header;
				d(t: f({x: a,}: g())~header);
			}
		)");
		doValidation(R"(
			(float out : float in) sin
			{
				var float x = in;
				var float y = x - x*x*x/6 + x*x*x*x*x/120 - x*x*x*x*x*x*x/5040;
				return :y;
			}

			(:) enter
			{
				var float x = sml.In.read().toFloat();
				var float x = sml.In.read()~out.toFloat();
				sml.Out << sin(:x);
			}
		)");
		doValidation(R"(
			(64u choose : 32u {any, name, here} : pure) sum
			{
				return choose: any + name + here;
			}

			(float out : float in) sin
			{
				var float x = in;
				var float y = x - x*x*x/6 + x*x*x*x*x/120 - x*x*x*x*x*x*x/5040;
				return :y;
			}

			(:) enter
			{
				sml.In.read().toFloat();
			}
		)");
		doValidation(R"(
			grant Aa {
				(:) kill;
				var {32u a, b, c; 64s d, e, f;}
				restricted static {Type g, h, i;}
				static Type j, k,;
				public var sml.String l,;
			}

			(:this) tell {
				var type[this] now(:this);
				var ptr Aa local = new[Aa];
				(@local).kill();
			}
			(:) %=;
			(:) %=
			{
				cool <<=-ber;
				cool >>= ber;
				cool += ber;
			}
		)");
		doValidation(R"(
			var 32u AAA = 1;
			var 8uo a;
			var {64so a, b, c;}
		)");
	}
}
