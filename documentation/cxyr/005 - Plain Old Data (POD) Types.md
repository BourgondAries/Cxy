## Plain Old Data (POD) Types ##
*Description*: How do we specify plain old data types? These are the fundamental types
every turing machine uses. These are bit sequences interpreted as integers. In some
cases, floats are also considered PODs. Another POD is a pointer. Simplifying this:
anything that is not a compound data structure is a POD.

*Discussion*:
Let's look at a current proposal for POD types:
`<:denary:>`s:

	32s
	64s
	100s
	10s
	9s
	50s
	47s


Specifying an unsigned integer:
`<:denary:>u`:

	32u
	64u
	100u
	10u
	9u
	50u
	47u


Why not use u32 instead? Will the lexer be able to pick this up faster? Suppose:

	2000000000s // 2 signed gigabit
	s2000000000 // 2 signed gigabit

What reads more easily? I have to say both are tempting candidates. There is
just something that irks me about the second one. We know a variable name must
start with either an underscore or an alpha character. If the token starts with
a number, we know that we only need to check the last character to see  if it is
a type. What this clashes with is that `s2000000000` is a valid variable name.

Anyhow, utilizing massive bit integers is often not feasible. In most cases, the
highest values go into the thousands of bits. The length of the string that
describes the data type is so short that it should not be a problem for anyone
reading it to quickly find the "u" or "s".

Specifying a pointer type:
ptr Type

	ptr 32u
	ptr ptr ptr 8u


Specifying a reference type:
ref Type

	ref 32u
	ref ptr 9u

I'd like these references and pointer types to avoid the nastiness that cdecl has
given us. cdecl is difficult (and ugly) to parse whereas a keyword like ref and ptr
are easy and unambiguous. A nice thing about ref is that it can be considered a true
reference to the compiler, and hence be optimized. ref can even be used to alias local
variables without any overhead during run-time (refs can not be reassigned).

Another very import pseudo-POD is the plain array. It would be highly beneficial
if the code can be read left-to-right. Let's look at my proposal:

	[32u, 10]
	var [32u, 10] var_name;

Notice how the number of elements follows the type that is going to be in each
slot. This can be read as: `Create an array containing 32u with 10 elements`.
How does this nest?

	[[32u, 5], 10]
	var [[32u, 5], 10] var_name;

Becomes `an array of an array containing 32u with 5 elements with 10 elements`.

That seems confusing. Maybe we should switch flip the order?

    [5, [10, 32u]]
    var [5, [10, 32u]] var_name;

`Five elements of an array of 10 elements of 32u`. Wow! That rolls off much more
nicely. This should be a valid type and degenerate to a pointer to the start of
the array. The declaration above makes it easy to visualize as well, as we can
read and draw from left-to-right:

	      0-----1-----2-----3-----4
	      |     |     |     |     |
	0    32u   32u   32u   32u   32u
	1    32u   32u   32u   32u   32u
	2    32u   32u   32u   32u   32u
	3    32u   32u   32u   32u   32u
	4    32u   32u   32u   32u   32u
	5    32u   32u   32u   32u   32u
	6    32u   32u   32u   32u   32u
	7    32u   32u   32u   32u   32u
	8    32u   32u   32u   32u   32u
	9    32u   32u   32u   32u   32u


How do we access an array? Well since [] haven't been used before in an
expression, we can add them to any variable as a suffix:

    var [10, [20, 32u]] values;
    // Initialize the values...
    assert(values[3, 5] == values[3][5]);

Note that the comma-array-access-notation is syntactic sugar. If only that
notation is used then the compiler may be able to lay them in memory as a single
array. This often aids efficiency.

How do we declare floating point variables? This is a problem that needs to be addressed.
An initial idea is to simply allow one to say `32f`, but that doesn't include the
size of the mantissa and the exponent. How do we semantically define the operations
on any-sized floats? What is the essence of these data types? We could argue that
these floats are just rational with an exponent. They actually are (in a sense), but
does that help?

	var 24f8es myfloat;

The thing is that we also have decimal float types. Instead of creating a big type
out of it, we could simply class them:

	var Decimal[:32] myfloat;

The second option specifies the type of float. For consistency reasons, a float that
is of the standard IEEE754 type could just be denoted '16f', 32f, 64f, 128f,...

*Tangent*: See `Reference Types`.

*Conclusion*:

	Type ::= Number TypeOfInt
	Type ::= ref Type                 // Note that ref ref is not allowed.
	Type ::= ptr Type
	Type ::= [ Expression Type ]

Note that the lexer will perform the operation of recognizing the tokens. The
parser never observes a separate "64" and "u"... but this is for the compiler
writer to decide.

An example of the above:

	var ptr [5, 8u] a;
	var ref ptr [5, 8u] b = $a;
