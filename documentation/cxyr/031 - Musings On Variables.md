## Musings On Variables ##

*Description*: Question the fundamentals of the declaration of variables and assignments.

*Discussion*: The common way to assign some object to a variable is to state it like
so:

	a = b;

The problem with this becomes apparent when assigning to multiple variables using
function return values.

	a: a, b: b = f();

First of all, we can't be certain what production we're in by looking at the first
symbol. An alternative is to use

	var {32u a: a, 5u b: b} = f();

This works just fine, but it's limited to declarations only. How can this be made
truly dynamic as well as powerful?
An option is to say that: well, since expressions are always first anyways, we just
force the declarations second:

	f() => a: a, b: b;

This could work very well, in addition, it avoids the need for the var keyword, even
though it would break a lot that's already implemented.

	f() => 32u a: a, b: b;

What about just raw declarations though?

	=> 32u a;

Function arguments currently have the potential form:

	f({a: b, c: d}: h())

There's something very clean and neat about that. I wish it were possible to have
this in a normal run-scope as well. The problem is that {} clashes with the scope
operator.

	{a: b, c: d} = h();

Just like:

	var {32u a: b, 5u c: d} = h();

Maybe it's even necessary to remove var altogether and just use {} in this specific
context.

	{32u a: r, Type b: b} = f();
	{8u char};
	{a: normal, char: color, b: some} = g();

That is indeed a possibility, but is it desired? It would be really cool to see compound
assignment as it makes the language a lot cleaner. Unfortunately, I can't use python's

	a, b, c = f()

since that would violate the idea of having statements. I could have the function
in the beginning though:

	f() => a: a, b: b, c: d;

I like the idea, operator => is not yet used. Really considering this. The grammar
would then still be LL(1) without sacrificing anything important. The grammar rules
would look like

	EXPRESSION '=>' NAMELIST ';'

We gotta remember that expressions are often far longer:

	f(:g()~a, control: true, expr: SYMBOLIC_EXPRESSION) + 3
		=> 32u d;

What if this is used to express assignment? It would make the language far more elegant,...
I kinda,.. like this idea. It would also avoid the use of the 'var' statement. Although
that kinda goes against the idea of making the code readable. I guess we can always
just search for => if necessary. I like this idea a lot. It fixes the issues that we're
currently facing. Removing the var keyword and all.

	plot(:f, style: DASHED_LINES, color: [127, 0, 0]) => math.Plot g: out;

Oh wait, isn't there a problem with math.Plot there? The namespaces will collide with
the actualy variable names! This means that we don't have LL(1) parsability. To solve
this issue, let's examine how var can be used.

	var math.Plot a;  // Generate an empty plot object
	{a: out} = plot(:f, style: DASHED_LINES, color: [127, 0, 0]);  // Generate the plot

The problem is that {} is a 'code-block', unless we make those only possible inside
while/for/if,... The problem is that the following is ambiguous:

	if (a > b)
		{a: out} = f();

To rememdy this problem, we can use the => but prevent object creation therein. Let's
also look at something else.

	extract {a: out} = f();

The exception here is the expression evaluating to a single outcome:

	a = f();

My problem with this is that it is not at all elegant. It's a hacky solution. I don't
particularly enjoy it. We can also make everything an extraction. Python's way is nice
and we want explicit (named) returns for readability reasons.

	= {a: out}: f();
	var {32u a: out}: f();
	[5, 32u].size;

We can't use [] to denote an assignment list. Neither can {} or () be used. What if
the operators are reversed?

	] 8u y [;
	] 32u x: a, y: b [ = f();

	var {Type x: a, y: b; 32u z: c} = f();

An alternative is to just force the => notation, but I'm afraid that this is not how
we start writing an assignment nor think about the logic.

	f() => var 32u x: a, y: b;
	sin(0.3) => total;
	total += sin(0.3);

As can be seen, it doesn't fit that nicely. How do we solve this yet keep the grammar
LL(1)? Can the left side be made into an actual expression?

	x: a = f();
	32u y: b;
	Type z: c, w: d = g();

In the last two cases, the types notify that we're in a data declaration statement.
What is the grammar?

	EXPR [':' name] {',' EXPR ':' name} ['=' EXPR] ';'

The problem with this is that it appears to allow all kinds of weird grammars. For
example:

	a + b: c = f();

Wait, that's not weird at all, that makes perfect sense! Why not allow an assignable
expression on the left side?! Madness! So how can it be made even better? Let's try
out something:

	EXPR [EXPR] [':' name] {',' EXPR [EXPR] ':' name} ['=' EXPR] ';'

So in this case, we can think of any type expression to be an expression in itself.

	Type x: a, Type y: b = f();

A preferable situation is to allow Type to be mentioned once.

	Type {x: a, y: b} = f();

This fits very well with the currently used method for arguments in functions. The
grammar would need to look like.

	{EXPR '{' RETLIST '}'}+ ['=' EXPR] ';'

This allows:

	32u {x: a, y: b}, z: c, Type w: d = f()~g()~h()~j(: 10);

The problem with this kind of assignment statement that it becomes impossible to decide
the production if we aren't already done with most of the production. We'd need to
scan to the '=' in order to know that this is an assignment. That's the biggest issue
with this idea. We need an LR(1) parser for this. I much prefer not to.

Of course, all statements can be generalized as this. This can only be done by merging
the {} and aforementioned method into a single grammar:

	EXPR ('{' NAMELIST '}'|[EXPR] [':' name]) {',' EXPR ('{' NAMELIST '}'|[EXPR] ':' name)} ['=' EXPR] ';'

This should conclude the truly general expression. It's short, LL(1), concise, and
elegant. I like it! Is this it? Is this the end?

	a: v, b: w += f();

The compound operators also work on it. That is because the 'name' section doesn't
at all bind to the += expression. The nice thing about this grammar is that we can
simply add multiple variables in the same statement in this elegant manner. Can operators
outside the compound assignments be used here?

	32u x = 0, y = 1;
	32u {z: a, w: b} = x: a, y: b - f();  // Still uses the names 'a' and 'b'.

I guess you could say x - f()~a and y - f()~b are called, returning a new 'return set'.
I'm not so sure about this. I'm thinking "let's keep it in the realm of practicality".

	f({x: a, y: b}: x: a...

I'm starting to think that the above is just too impractical. The grammar will hardly
be remembered and it's just not easy to read. There need to be limitations. Just like
the limitation on the depth of conceptualization (1). A nice and practical depth.
This depth saves us from overly engineered complexity. Hell, if you really need an
advanced feature you can use a codegenerator, that's what this language should excell
at. The thing is though, we need named multiple returns as well as multiple arguments.
The argument problem is resolved in a very nice manner, but the variable creation
problem is not. An idea is to use 'var', but it makes the language ugly. Another is
to generalize the expression itself to support it. Instead of allowing one to write
a multitude of complexities like above, let's do something simpler:

	32u {x = 0, y = 1};
	32u {z: a, w: b} = f();
	z: a, w: b -= x, y;

Hmmm... that shouldn't work either. I could say that static methods are simply avoided
altogether, then types can not be used in expressions without some surrounding information.

	32u.size;  // No
	size[32u];  // Yes
	Type.method  // Yes

The parser breaks down at Type.method, because we don't know if the current line is
a variable declaration or part of an expression. As stated, expressions could just
be generalized and we can use that. Semantic analysis then needs to check each expression.

	f({x: a, y: b, z: c}: g(:10 + 3 * 2 - u)~h());

The above is nice, it works and it's very general. In many cases, maybe we want to
process the returning data just a little. What if c needs an added integer? I'm thinking
of adding named lists - in a sense:

	f({x: a, y: b, z: c}: {c: 1} + g(:10 + 3 * 2 - u)~h());

This matches the c value with the corresponding set returned, and adds one to it.
I think this is beautiful. Absolutely beautiful. Terse. Powerful. Fierce. The ideas
of named lists are born. They're static and at compile-time only. Nevertheless, useful.
The issue at hand is the use of {}. It's overused, and named lists just can't be used
anywhere outside function arguments because it's unknown whether either production
is to be used.

To remedy this, we can think of using the diamond <> operator instead. Maybe. Maybe,..

	Type.run() = 0;

could be used together with the general form of an expression. A full expression.
An fexpr! Finally. Am I starting to see? Am I starting to wake up?

	32u {x: a, y: c} = {out: -100} - {c: 1 + 3} * {c: 100} + {a: 200};
	sml.Out << x;

The problem with this is that it differentiates the expression on the left from the
expression on the right. I think it fits the generalized expression form though. That's
a good thing...
From there, the semantic analyzer can find out that the statement actually does. This
will not be too easy. There needs to be a fexpr in the basic grammar. fexprs can not
be nested. The fexprs are truly elegant! Such elegance!

	FEXPR ::= EXPR ('{' NAMELIST '}'|name [':' name]|) {',' EXPR ('{' NAMELIST '}'|name [':' name]|)} ['=' EXPR] ';'
	NAMELIST ::= {name [':' name|'=' EXPR]}  // Names are optional.

basically implies that a second (non-epsilon) expression becomes the new name. That's
actually very clever.

	a + b + c + d;  // Just fill in the first EXPR, leave all optionals empty.
	Type.doSomething();  // Just fill out the first expr.
	Type myname;  // Both filled out, declare a new variable.
	Type {a = x, b = y};  // Both filled out, declare.
	Type {a: x, b: y} = {x: Type(:1).get()} + f();  // Three elements filled out. Declare.
	a: x, b: y = {x: Type(:1).get()} + f();  // Two elements filled, just assign.
	32u {x: a, y = 100} = f();

This plays out very nicely. Expressions can be expressions. 32u.size can be used freely.
And above all, we avoid 'var' or 'static'. The part inside `{x: Type(:1).get()}` is
to be an atom. It must never consist of multiple variables. This way, we manage complexity.
The distinction between = and : in the {} lists is to distinguish an expression and
direct assignment. This is useful in larger expressions.

An idea that popped into my head just now is that I need to imagine a library where
a method returns some structure or class. In this case, both the callee and caller
need to have that structure as visible space:

	Main.cxy
	a/
	|-- Aa.cxy
	|-- b/
	    |-- Bb.cxy

Suppose A contains a method that returns the class Bb. How is this made visible to
Main? One method is to put it in the global scope with respect to the program itself.

	Main.cxy
	a/
	|-- Aa.cxy
	|-- Bb.cxy

Both elements are now visible since Bb is a root object. What if we have some deeper
hierarchy where we don't want that?

	Main.cxy
	a/
	|-- Aa.cxy
	|-- b/
	    |-- Bb.cxy
	    |-- c/
	        |-- Cc.cxy

Bb wants to return Cc to Aa, but it can't! Aa doesn't want to let Main know what it
uses internally, so Cc has to remain hidden to Main. How does Aa do this? We can use
concepts for this:

	Main.cxy
	a/
	|-- Aa.cxy
	|-- b/
	    |-- Bb.cxy
	    |-- Cc.cxy

With the code in Bb being:

	grant Cc {
		(:) performOperation;
		// ...
	}

Aa:

	(:) myAOperation {
		Bb[Cc: Cc] b;
		Cc c = b.getSomething();
	}

With the idea of allowing super expressions, the meaning of class names diminishes
somewhat. Maybe it isn't that useful. I'd still enforce it for the reader's sake though.
Another thing is that the language doesn't have implicit casting like Java, C, or
C++. There is no `(void *)` cast.

Regarding exceptions, let's just idealize them as long unwinding gotos, this way they
can be used for control flow without the pesky overhead. If you need error handling,
create a glocal and assign some string during runtime. The rules still stand.

	CAPITAL for constants
	UNDERSCORED_CAPITALS for anonymous enumerations
	lowercase for namespaces, variables, methods
	camelCase for methods
	PascalCase for classes

	(:) enter {
		alias {
			Key = crypto.Key;
			Encrypt = crypto.Encrypt;
		}
		while (true) {
			try {
				sml.Out << "Write your new key: " << sml.Out.FLUSH_BUFFER;
				sml.String read = sml.In.getLine();                         // Read a single line
				Key key;                                                    // Create a new key object
				key.set(read);                                              // read the key into a key object

				sml.File file("to_encrypt.txt");                            // Open the file to encrypt
				sml.String encrypted = Encrypt[Stream: sml.File].encrypt(   // Encrypt the file into a string
					key: key, stream: file);
				file.open("encrypted.txt");                                 // Open a new file, this closes the other file
				file.write(encrypted);                                      // Write the encrypted data
			}
			catch Key.NO_KEY_GIVEN
				sml.Out << "The key was empty, please try again";
			catch Key.KEY_TOO_SHORT
				sml.Out << "The key given was too short, please try again";
			catch sml.In.END_OF_FILE {
				sml.Out << "End of the stream reached, aborting";
				sml.Application.exit(1);
			}
			catch sml.File.FILE_NOT_FOUND {
				sml.Out << "The input file is not present";
				sml.Application.exit(1);
			}
			catch sml.File.NO_PERMISSION {
				sml.Out << "There are no sufficient permissions to operate on files";
				sml.Application.exit(1);
			}

			// Warnings will show up for uncaught exceptions
		}
	}

Note how I've removed the `:` from function calls with a single argument. I'm thinking
of allowing expressions in arguments to be of the form:

	ARG ::= EXPR [':' EXPR]
	ARGS ::= ARG {',' ARG} | []

This will remove the ugly `:` from calls where it is simply not needed. We can let
the semantic analyzer figure this out for us. I think in most cases, a comma is not
even necessary. I need to add some experimental features. To avoid conflicts, the
parser can just use a lookahead for now. Later on, this lookahead can be removed.
This extends the classes into expressions. Quite interesting. The catch expression
is also quite obnoxious. Scala uses the => operator, but I'd rather not. I'm thinking:

	try {
		// Code here
	} catch {
		// Common catch code
		Key.NO_KEY_GIVEN {
			sml.Out << caught.toString();
		}
		sml.File.FILE_NOT_FOUND
			sml.Out << caught.toString();
	} catch {
		// Other common catch code
		sml.File.NO_PERMISSION
			;
	}

This just has the grammatical form:

	CATCH_BLOCK ::= 'catch' '{' {ENUM BLOCK_OR_STATEMENT} '}'

This is much more elegant and allows using a single line as well. It also allows you
to put common catch code. I like the idea.

Name mangling is important. With name mangling, a lot can be achieved. The programmer
gets notified if a name mangles. That's ultimately a good thing. I like the idea of
underscores and big letters being used for enumerations, all the while enumerations
are also used to jump in code. This allows nonlocal gotos to exist. I really enjoy that.
Something I've never seen in a language is the ability to throw in such a way that
you return from multiple functions. That's extremely pleasant. I think it would be
interesting to construct a kernel language. The current problems that are still in
the language is that the grammar is too large to handle for a normal human being.
Maybe this is inevitable, but it's rather annoying. How would be construct a kernel
language?

	START ::= { FUNCTION }
	FUNCTION ::= '(' { PARAMS } ':' { PARAMS } [ ':' ATTRIBS ] ')' fname ( STAT | '{' STAT '}' )
	PARAM ::= TYPE EXPR

Gets big pretty fast. We have static and strong typing, so types will make so much
more difficult. I think the grammar is fine. A nice cleaning should make it nice. Hell,
I bet I could change most type thinys to EXPR and get away with it. Let's keep the
grammar specific though. So it's pretty clear that enums must have underscores. What
about completely CAPITAL values? We could make those 'constant', but it seems rather
boring. We can do a whole lot of different things. What about glo(b|c)als? I like
that idea. It makes it clear what is global and what isn't. Do we differentiate between
glocals and globals though? There's a big difference. We might as well write:

	32u global[name] = 300;

	(:) enter {
		global[name] += 3;
		f(global[name]);
	}

Thus making the expression 'global' part of the name. This keeps globals very clean
in our code.

	// Class Person
	(: this) constructor {
		glocal[objectCounter] += 1;
	}

	(: this) destructor {
		glocal[objectCounter] -= 1;
	}

As used in

	32u glocal[objectCounter] = 0;

	(:) overFunction {
		Person person;
		sml.Out << glocal[objectCounter];
	}

Alright, so that's the name mangling for globals. Why not expand it to enums as well?

	Classname.enum[enumname];

Then, inside the class, we write:

	enum[enumname];

And that's it. Nothing more, nothing less. Even if it's only used somewhere. I'm not
entirely sure if this is desirable. On one hand we don't particularly enjoy name mangling.
On the other hand, name mangling is useful. I just wish I could make the language
simpler.

I have just devised a solution to the whole variable problem! Instead of letting the
parser do the work, we let the lexer do the work. We exploit the fact that namespace
accesses are no more than 1 in depth. A buffer is created with three slots. If the
first slot is a namespace, the second a dot, and the third a class, then that constitutes
a single type. Another slot can be added to make sure we don't have a pure type. The
reason for this is so that member expressions aren't mistaken for types.

	my.Type.getSomething();
	my.Type element;

So the entirety of `my.Type.` is turned into a single token. The reason we need to
do so is because the parser can't look ahead to the dot. In the former case, we have
TYPEEXPR, the second being a TYPENAME. This requires another lexer stage. This will
be awesome! There are no limitations now. Static overloads of operators are not allowed,
neither are static -> operator on a type. Maybe I need my::Type just like C++, but
I'm quite unsure.

	my::Type::getSomething();
	my::Type element;

It does look quite different though. I like the quickness of the dot operator. It's
fast to write compared to ::. Actually :: isn't too bad either. I like the idea. It
disambiguates clearly, although the compiler will have no problem recognizing the
correct variable. It's more friendly to the human eye. Maybe that's just my C++ bias
showing though. I have an idea:

	my::sin(0.3)

Can be valid, we create a standard class per namespace called "construct" or something.
This class will be implied by the :: sign. This allows the wonderful syntax:

	math::Standard::cos(math::Standard::pi);
	math::cos(math::pi);

Are equivalent. The name of the class though, we need to be able to send the class
into some kind of template:

	MyType[math::Standard] mytype;

Because that's useful. We can also call it the Default class:

	Accum[math::Default] accumulator;
	mytype.cos(math::pi);

I really like this idea. It gives the :: operator purpose. It allows for smooth libraries
to exist.

	sf::drawToWindow(sf::RenderWindow...);

I really enjoy the idea. The single depth of the namespaces also mandates that this
becomes completely possible. One issue I have with this is that the default class
can't really evaluate other members of the package. By that I mean that it can only
observe modules lower than itself. Those methods should probably be implemented in
the respective classes then. Say we have a collection package:

	col::Array[32u] a(7), b(9);
	a.fill(3);
	a <> b;  // Swap operator
	sml::out << b.toString();

Instead of using col::swap or anything, we could either do a '.swap' on the objects
or just implement a swapping operator. Another thing is that now, [] can be a type,
and we don't yet know if it will be an expression or not. That's a big problem, since
arrays can be nested indefinitely.

	[5, 3, 1u]::size;  // How can we know this is an expression and not a declaration?

We can use the parser to evaluate this. If we have:

	Type name  // We have a declaration
	Type ::  // We have a member method

The problem with this is that we can't decide this before we're inside the expression.
An LR(1) parser could handle it, but not our LL(1) parser, unfortunately. How can
this be remedied? The same goes for templated types btw, it's not possible to use
a simple 3-buffer to find out if we have a declaration or not. It seems like we have
to stick to the fexpr in order to avoid var. Another question pops up: what is the
standardized way of assigning variables? Surely we can allow the following:

	32u {a: x, b: y, c = 3}, 8u d: z = f();

Or should this be split up into

	32u c = 3;
	32u {a: x, b: y}, 8u d: z = f();

The idea is to create 'assignment lists'. This means that anything of the form `x: y`
will have x be assigned to the y in the rightmost expression. To disambiguate the
above example with `z = f()`, we ... oh, it's already good.

	8u a: b, 32u c = g() = f();

This will bind the result of g to c, but f is assigned to the entire group. I think
this looks rather confusing. Maybe there's a way to avoid this entirely. We don't
want newcomers to be utterly confused by complicated grammar. We don't want to limit
a single data type per declaration, that would not make you able to extract all return
parameters in one sentence. So what else can be done? Do we just roll with it?

I've just implemented this entire scheme and it appears to work quite well. No trouble
so far. It's a slightly complicated grammar, but I think it can be understood quite
easily. So this is how we create expressions. Quite simple. For scopes I think I'll
just use `label { ... }`. Seems to be the simplest way to do it. This way we'll avoid
the problem with the FIRST set of {} on the expressions. In addition, {} needs to
be added to expressions.

Instead of thinking about specific assignments, why not create a general syntax for
expression fetching?


	v: out, e: error = sin(:0.4328);
	sml.Out << v << "with error" << e;

Should be something like:

	(sml.Out << :out << "with error" << :error;): sin(:0.4328);

This avoids temporaries. Maybe something like EXPR =>?

	sin(:0.4328), {c: 33} => sml.Out << :out << "With error: " << :error << " message: " << :c;

This can keep function return names anonymous: they're not assigned to variables.
In addition, it is easily generalized to, say, assignment statements or declarations:

	sin(:0.4328), sin(:85) => 32u {x = out[0], e = error[0], x2 = out[1]};

We can just say that the return values shadow the surrounding values.

	sin(:0.4328), sin(:85) => sml.Out << out[0] << error[0] << out[1];

Looks nice to me. I like the idea of local manipulations like this. We can then avoid
having to assign the returns. How about naming the commas? Or what about naming the
returns as collectives?

	getWindow(): x, getViewport(): y => sml.Out << x.getSize()~width / y.getSize()~width + y.getSize()~height;

As you can see here, getViewport is only called once, assigns all returns to a struct
'y' which is later used in the expression. I like the idea because it's powerful. Maybe
all expressions can be of this form...

	100: x, 32 * 10: y => 32u {x, y};
	getWindow(): x => 32u {h = x.getHeight(), w = x.getWidth()}, 32f r = x.getRatio();

	// The classical approach
	ref Window x = getWindow(); 32u h = x.getheight(), w = x.getWidth(); 32f r = x.getRatio();

What this syntax does is that it allows you to use type inference. Something that
I consider somewhat dangerous. Luckily it's only for a single statement, this limits
the damage that can be done by recklessly inferring. I think I need to stick with
the 'experimental' method. It does not allow type inference. Neither does it allow
you to recklessly get values. I think all returned items should be checked against
some type. Unless you send it to another object directly. In that case, the above
method is actually pretty good. getWindow().getHeight() is legal, why not allow it
to be easier to write? We could say we limit a single depth of the function call,
but that seems rather extreme. This would not allow joys like

	sml.Out << currentWindow->getWindow().getHeight();

That's non-sensical. I therefore think it's fair to have the inferring syntax for
a single statement. The value will not be reused or changed. Maybe multiple returns
are a mistake... Maybe it just complicates too much. We could always just accept a
tuple back. Anyways, no, type inference can go away, we _need_ explicit types badly.
They keep code correct, and prevent bugs when erroneous edits are made. We could also
go overboard (I've talked about this before) and do not allow inline expressions.
That would be incredibly interesting. While not extremely safe, it would eliminate
all kinds of type errors.

	32u a = f() + 3129/31 + sin(923)~error;  // Expressions like this only allowed after = sign.
	sml.Out << a;  // Can't do anything other than use variables directly.

	// sml.Out << a + 1;  // Error!
	type[a] b = a + 1;
	sml.Out << b;

How practical is this? There's a fine line between type inference and being extremely
explicit. Maybe even too explicit! Let's look at some possibilities. Another possibility
is to avoid types in normal statements completely, but to allow them anonymously like
so:

	312/3 * 5: x => sml.Out << x;
	f(): y => 32u y;
	new ClassObject: obj => ptr ClassObject;
	vector.get(): v ==> v.x: x, v.y: y, v.z: z => sml.Out << x << y << z;
	1: a ==> a + 1: a ==> a * 3: a => sml.Out << a;  // Any idea what a prints? 6 sml.Out << (1+1)*3;

I don't know.
I don't know if I can like this. Doesn't seem too practical. It's clean though. That's
the one positive thing. Mhm. I can't enjoy this kind of syntax. I do wish for there
to be a limit to nesting of calls though. Doesn't seem very productive, and it's nigh
unreadable. If we ban any kind of recursion we are at risk for being too cocky about
it. If we have to store every intermediary... phew that's going to be a pain in the
ass I tell you what.

	label {
		ref Window window = getWindow();
		< 32u h = window.getHeight();
		32u w = window.getWidth();
		< 32f r = h.divide(w);  // Can't do h.divide(window.getWidth())
	}

Where the < operator exports the declaration out of the scope. We wanted to use height
and ratio, but never w. Basically, the whole idea with allowing expressions to be
nested is an implicit type inference. Although the types match, the programmer never
sees the actual types:

	give(buffer.getValue());  // What does getValue() return? What does give take?

While elegant and short, the following sets the types in stone:

	InterestingLong x = buffer.getValue();
	give(x);

My initial idea was to implement type checks inside expressions. The problem with
these is that they increase coding noise a lot. Something that distracts the programmer
and makes him work less efficient. I just can't decide. It's difficult. What can I
do? I know. I need to objectively measure. Certain traits need to be quantified. Then
I can make informed choices.

Definitions:
* Noise - The amount of characters-to-goal ratio (code-golfing languages win here)
* Readability/Clarity: Inverse of Noise
* Simplicity: One divided by the amount of possible sentences (152 sentences => 0.658%)

I want to minimize noise but also have high readability. These two go against eachother.
Maybe passing around function returns directly is not that bad. Considering that the
callee will type check the incoming messages. This means that the caller doesn't need
to worry about the types. What if the caller calls a method on the returned item?
It would be very beneficial for the caller to be aware of the type, so that it is
always sure that the correct type is given. Consider the case where a different type
is given that has the same methods. Would this be acceptable?

Ideally, we'd love to remove redundancy as much as possible. For example, some new
ideas:

	new-statement: newptr[3] newptr[4] 32u x;

This implicitly creates a variable without having the syntax like:

	ptr ptr 32u x = new[ptr 32u](6){new[32u](8){0}};

The anonymous object:

	anon x = object.getSomething();
	otherobject.sendSomething(x);

The anonymous object can't be manipulated from this context. It's anonymous. No known
operations on it, the size is unknown, everything is unknown. The compiler will figure
out the type. The anon object is just a message that you can pass on. That's all.
The internet agrees that type inference is only useful where the type is already redundant.
Therefore, we need a new-statement (instead of new-expression) to remove redundancy.
The anonymous objects can be used so a higher class can pass around data without knowing
what it is.

With the addition of the anonymous object, the whole language becomes a lot easier.
What about extracting elements though? We have this pseudo-type with tuples when returned.
The same goes for arguments to functions. We could just say the following:

	anon x = f();
	g(x~a);

This would solve all problems, although it wouldn't allow an easy change in value.
I'm a fan of the fexpr types. They're powerful and versatile! Let's keep those. What
about anonymous tuples, just like anon? Can that be used? I don't know to be honest.
I wonder what can be done. Would we like some kind of type inference on the tuples?
That's an interesting question.

	ptr ptr 32u x = new(100){new(30)}

I like this, new basically infers the type from its context. The outermost new infers
the type to be ptr ptr 32u, the inner one has one ptr stripped. That's a very interesting
syntax. It's simple. Elegant. Nice. This does however posit that these must be on
the exact same line as a declaration.

One thing I'd like to have - which consistently fails - is to have a unified grammar.

	ENTER ::= { UNITS }

	UNITS ::=
		ACCESS '(' [PARAM {',' PARAM}] ':' [PARAM {',' PARAM}] [':'  ATTRIB ] ')' functionName '{' { STAT } '}'
		| ACCESS TYPE variableName = EXPR ';'
		| 'granted' ClassName ( '{' { TYPE name ';' } '}' | 'pass' )
		| 'granted' variableName TYPE
		;

	ACCESS ::= [ 'public' | 'private' | 'restricted' ] [ 'global' ];
	PARAM ::=
		TYPE '{' { variableName [',']} '}'
		| TYPE variableName
		;
	ATTRIB ::= [ 'const' ] [ 'pure' ] [ 'safe' ];
	STAT ::= FEXPR | IF | GOTO | CONSTRUCT;

	TYPE ::=
		primitive                                                  // 32u, 64s, 1u
		| '[' EXPRESSION ',' TYPE ']'                              // [8, [3, 8u]] arrays
		| ('const' | 'ref' | 'ptr') TYPE                           // const/ref/ptr prefixes
		| [ namespace '::'] class [ '[' GARG {',' GARG} ']' ]      // my::Type[Con: 32u]
		;

	GARG ::=
		TYPE
		| className ':' TYPE
		| variableName ':' EXPR
		;

The big idea is to allow a similar setup to Java and C++ classes. Methods need to
be added, as well as data. The data follows a similar pattern of TYPE name = EXPR.
The thing is that function signatures are types as well. Ideally, such signatures
should signal that we have an actual method and not a 'function value' or function
pointer.

	alias St = sml::out;
	grant Name {
		32u a, b, c;
		(:) control;
	}

	grant va 32u;

	// Per-object member
	ptr (:) x lambda { sml::out << "x!" << sml::endl; ++counter };

	// Per-object member, e meaning 'exception' on overflow.
	64ue counter = 0;

	// Per-class member
	(:) enter {
		x();  // print 'x!'
		x = lambda(:){ sml::out << "y!" << sml::endl; };
		x();  // print 'y!'
	}

Maybe the declaration statement can be unified.

	MEMBER ::=
		TYPE name '{' { STAT } '}'
		| TYPE name ['=' EXPR] ';'
		;

This simplifies the parser a bit. To solidify the inline-redundancy-avoidance: we
can specify the proper grammar.

	DECL ::=
		TYPE name = EXPR

I'm thinking that when we process the right side, a context is sent to the expr. This
context will bind to operators as 'new' or lambda such that they do not need to be
stated explicitly:

	ptr MyClass a = new(20);
	ptr (ref MyClass b : ) f = lambda [old] {
		return b: old;
	};
	ptr MyClass {x: a, y: b} = {a: new} + f();

One problem that is still not fully resolved is that I may want an expression in the
name section:

	32u x: a + 3 = f();

I'm thinking that this is rather confusing, I like the idea of keeping the left side
clean from expressions.

	32u x: a = {a: 3} + f();

Will add 3 to a. I'm really not sure what's preferable. The former seems more terse.

	32u {x: a, y: b} = f() + {a: 1, b: 3};

Not liking it too much.

	32u {x: a, y: b} = f();
	x += 1;
	y += 3;

That's a lot more clear to be honest. The problem of casting also comes up. How do
I do the following?

	8u {x: a, y: b} = f();

Should I cast inside the left side? Should I create a special cast operator?

	8u {x: cast[8u](a), y: cast[8u](b)}, 32u z: c = f();
	x: cast[8u](a), b: cast[8u](b), z: error += g();

	is the same as
	anon o = f();
	8u x = cast[8u](o~a);
	8u y = cast[8u](o~b);
	32u z = (o~c);

	anon o = g();
	x += cast[8u](o~a);
	y += cast[8u](o~b);
	z += o~error;

Maybe that is the most elegant solution! Because we turn multiple variables into single
variables on the left side... but it does obfuscate the code a lot. The readability
is just horrendous.

	32f {x, y} = {0, 0};
	x, y += {1, 3};

This idea uses placed names instead. It's syntactic sugar for:

	32f {x: 1, y: 2} = {1: 0, 2: 0};

Not sure if I like this. Maybe a Vector class is more useful in this case. I need to
make up my mind on this. I can simplify this by a lot. I think... Let's say we have
the following syntax:

	32u {x: a, y: b} = cast[32u](f());

Which for each case translates to the name wherever it is encountered

	32u x: a = cast[32u](f()~a);
	32u y: b = cast[32u](f()~b);

and NOT

	32u x: a = cast[32u](f())~a;
	32u y: b = cast[32u](f())~b;

The latter makes no sense, you can't cast an entire list of variables. What if we have
multiple types of variables and do not want to cast them all? Well too bad I guess.
Same goes for decls of this kind:

	32u x: a = f() + g();

is equivalent to

	32u x = f()~a + g()~a;

It doesn't allow me to asses each variable individually.

	32f {value: out, error: error} = sin(math::PI);
	if (error == 0)
		debug sml::err << "Algorithm doesn't correctly calculate error";
	out += 3;

I kinda like the idea of allowing expressions on the left side. It feels like the
arguments:

	f({x: cast[32u](a), y: b}: g());

On the other hand, the KISS principle tells me I should only use it to extract values
and that's it, so the above becomes

	64u {a: a, b: b} = g();
	32u n = cast[32u](a);
	f(x: n, y: b);

I think I need to stick with the KISS principle. The only thing I definitely like
is that function arguments can be directly substituted by expressions from the returns
from other functions. Or should that be illegal as well? The entire issue lies with
the 'anonymous' tuples. How we define operations on these. How these are created in
expressions. Maybe I should just do away with the whole complication of the grammar.
I don't particularly enjoy it myself. It's terse but it seems like a waste of effort.

I mean, what language actually does this in a practical manner? In most cases, we
define a class and the operations on it. The class is basically a very well-defined
tuple with well-defined operations.

	64f {a = 0, b = 0}, 64f c = 0;
	label { a: x, b: y, c: z += diag(); }  // Add the vector.

Yeah, in most cases, specific classes will be made to describe the return value type.
The above will most likely be:

	Vector vector;
	vector += diag();

Much simpler to understand. Where no such thing is desired is e.g. the calculation
of the sine or cosine values, where we may be interested in the error of the approximation.
In other cases, some kind of flag can also be passed down the returns, which is useful.

	1u t: timeout, 32u count: out = performActions();
	if (timeout)
		return ...;

This prevents us from making classes for every single return. C++ solves the conundrum
using std::tuple. I'd rather stay away from such functionality. Let's just say we
do it as follows: we can declare-and-extract values (using assignment and other compound
operators. And we leave it at that. No special syntax. There's just extraction, then
you can do whatever you want. with the results. A tuple has the following semantics:

All operations on a tuple redirect to the variable named 'out'.
An exception is the ~ extractor. It extracts the proper name.

That should be sufficient.

	// Manual extraction
	32f a = math::sin(math::PI);
	32f e = math::sin(math::PI)~error;

	// Terse extraction
	32f {a: out, e: error} = math::sin(math::PI);

For an extractor, we can apply a rule that states that only extractions can occur inside
the extractor, assignments can not occur.

	32u {value, counter: change, nice = 3} = f();

Instead, the correct form will be:

	32u {value, counter: change} = someobj.getDiffs();
	32u nice = 3;
	// 32u nice(3);

So all values are assigned when we perform an extraction. It does mean that only a
single function call is allowed on the right side. We can't really operate on the tuple
directly. What if we want to return by proxy?

	(32f {out, error} : 32f in : pure) sin {
		out, error: error = sin(in, error: 0f);
	}

	(32f {out, error} : 32f {in, error} : pure) sin {
		// Use maclaurin to compute the value
		out = in - pow(in, exponent: 3)/6 +
			pow(in, exp: 5)/120 - pow(in, exp: 7)/5040;
		error = pow(x, exp: 9)/326880;
	}

In the case of the sine function, the error is taken to be the largest possible error.
Ideally, it ought to be computed from x itself, which is an expression of cosine, which
translates into a sine (so back into this expression). The above shows an interesting
return method. We just let the values be assigned. The end of the scope returns. This
also allows returning unallocated objects. For example, an unallocated integer:

	(32u {out, other} : : pure) get {
		out = 1;
	}

Will not assign a value to other. This means that the callee:

	32u {out, a: other} = get();
	sml::out << a;  // Undefined behavior

This may be a good argument for making returns explicit, but there's a very good use
for not making them so. This specifically allows us to return unallocated PODs, something
that can save time.

So if you can only use a single call on the right side of an extraction, we get some
interesting grammar...

	// Using a different get
	{32u {out, other: something}, 8u e: error} = get();

This can in fact be done with a { lookahead, but that will clash with the single-statement
branches. So it's undesirable. The grammar becomes a lot more straightforward though.

	extract 32u {out, other: something}, 8u e: error = get();

I don't like extract, it isn't elegant. I like the former fexpr method better than
this.

	32u out, a: something = get().doSomething();

Works nicely in combination with both new variables and replacing old variables. This
looks stunning! Allowing arbitrary expressions should be fine, as they merely act on
the 'out' variable anyways. Yeah, should be just fine. The question that remains is
how the semantics of get().doSomething() are. Suppose both return tuples of (Class out, 32u a).
Then we should get multiple a. The first out (from get) is used when we did .doSomething.
Let's just say that the call that didn't use the .. hold on. Isn't + technically a
member call? Well, should that exclude the following?

	32f {a, error: error} = sin(PI) + 1f;

Since the 1f is the last call, no other items are returned. This is difficult. What
can be done? We can limit the right side to a single member expression. The `+` expression
will return a single 'out' that can be used. So since it has the highest prevalence,
that out will be used.

	32f a = sin(PI) + 1f;  // Single out there anyways

	32f {a, e: error} = sin(PI);
	a += 1f;

If you still want the error, you can extract it like the above. Hey I can't handle
every single use case. There are too many of them. Should the same be done for function
calls? As in:

	f(g());

I'm thinking this just binds g's out to f's in. That's fine.

	f({x: a, y: b}: g());

I'm wonderinf if the above should instead be:

	Type {x: a, y: b} = g();
	f(x: x, y: y);

This does impose one limitation that may be useful: we get to know what the types are.
The equivalent in C++:

	TypeA a = g1();
	TypeB b = g2();
	f(a, b);

The latter has one more character. Mainly due to named-x of my language. So what should
be done? You know, the return value should be quite obvious. An idea is to create an
inline-documentation generator. For example what types some arguments are. This aids
development and readability, whilst avoiding writing this tedious documentation during
coding. Hand-written documentation is often times lacking and is neglected when changes
occur. It's important to let the compiler do a lot of the work.

	32u {a, r: remainder} = 300 \ 200;
	sml::out << a << ' ' << r << sml::endl;

Should output `1 100`. This will probably be fine. So we've got fexpr, functions, modules,
methods, access, statics, purity, io,... seems like everything is in place. At least
I hope so. The semantics of constexprs and enums need to be made. All enums are `LIKE_THIS`.

	MY_ENUM;
	ERROR_IN_DEPTH;
	IO_READ_ERROR = 3;

	(:) enter {
		try {
			throw IO_READ_ERROR;
		} catch {
			IO_READ_ERROR {
				debug sml::err << "io reading error, aborting";
				sml.abort();
			}
		}
	}

All enums are thusly declared in the same class. Parents can access them by writing

	SubClass.IO_READ_ERROR

Which can be thrown. The subclass's enum is not visible to superclasses. A program
wide mechanism should be in place. The enums can also be placed in the grant list.

	grant Aa {
		IO_READ_ERROR;
	}

	(:) myFunc {
		throw Aa.IO_READ_ERROR;
	}

The enums have to be generated in the class where they are defined, of course. You
can't state `sml::Main::IO_READ_ERROR = 5`. It would be nice to automatically propagate
a class to the lower classes such that reading is easy, but this breaks demeter's
law. It would be really useful to have internal enumerations for - say - a library.
Then again, it would also be incredibly useful if you can nest the library where it
is needed instead of making it global. Grants solve this issue. A possiblity is to
allow grants to 'pass' at all times:

	grant MyClass;

This will just pull out whatever's needed without stating the requirements. If the
class does not present any requirements, compilation fails. This can be useful for
further propagation into the system, using successive grants. We could also state that
the sml library is always automatically granted, and not allow global access to the
top level.

	::Main::doSomething();

is not allowed anymore. Should this be changed to be so? I like the purity of it. That's
for sure. Eh, grant merely becomes a comment then.

	// Grant the class Aa, it uses .substring(from: 32u, to: 32u);

Then, the callee (creator parent) uses:

	ThatClass[Aa: sml::String] myclass;

Should grants always be forced? By not allowing global access, we can actually force
a very clean paradigm of programming. Where copying one folder structure simply doesn't
affect the correctness of all containing classes. As long as all grants are given,
the program will behave correctly. What can be done is to let the compiler generate
a message upon "ungranted class". The compiler can probably also state what the grants
are using static analysis. Hmm, is there then any use for the grant list? Well, it
can be used to actually see what the grants are. It is redundant information isn't
it? Why not just allow the compiler to do something like

	cxyc --grant lib/something/deeper/My.cxy

And get a list over the grants. It can even read deeper into other classes that get
grants. So you get all the information necessary. Yeah it's redundant. Out with it!
The only problem is that the semantics of the grants is still unexplained. Other items
as purity luckily come through. This also destroys the idea of globals by global positioning.
What happens instead is that we just send in a class with a static variable. What if
we want to send the variable directly?

	static 32u myglobal = 3;

	(:) enter {
		SubClass[global: $myglobal] a;
		a.increment();
		sml::out << myglobal;
	}

	// SubClass (granted)
	(: this) increment {
		@global += 1;
	}

Should output 4. Wait, doesn't that make everything pure? Can we send in a package?
I think that's wrong to do at the moment. That seems too powerful. In most cases, who
needs the entire package? Even if, maybe the design should've been different. Another
thing is that the type of the `myglobal` variable or `global` is not specified in the
SubClass class. That's actually quite nice. Because we don't mandate specifics of the
type, the type can be a wide range of different variables. For example in the above
I sent in the pointer to the `myglobal` variable. This means that 'global' is a variable
of that class of the exact same type 'ptr 32u'. This seems extremely powerful. Just
imagine what you can send in there. You can send any references with similar semantics
as the 32u.

	static math::SuperInteger myglobal = 3;

	(:) enter {
		// SubClass[global: myglobal] Fail because you can't derefence.
		SubClass[global: $myglobal] a;
		a.increment();
		sml::out << myglobal;
	}

The interesting thing is that this can only be permitted when using compile-time static
variables. For normal class-local variables, this is not permitted. Is the function
using it still pure though? It gets a variable as an extra parameter, in a sense. Interestingly,
this allows you allows you to substitute pretty much all variables. So what is an impure
function? Anything using a static granted? That seems pretty accurate actually. sml::out
is an automatically static-granted variable. We all know that it changes state. So
are static variables that are sent in also impure? This would suggest yes. Are there
any cases where sending in variables via concepts is not desired? A deeply nested structure
where the second-top and the deepest levels both need something. You now need to feed
the entire chain downwards. That's rather tedious. Luckily, you should specify what
you need from the start (bottom-up coding). Even then, it's only a one-time-thing.

So what is there to do with the variables that should be CONSTEXPR? Well I suppose
making them all-caps is the only option. It isn't a bad decision. Hell, the templates
can even take in class-local variables. Then we have glocals!

	32u counter = 0;
	(: : pure) constructor {
		SubClass[name: counter] object;
	}

This solves the entire glocal/global problem. Yes, we do have to pass them on, but
once they are in the objects, they're pretty much good. Hell you could create an object
and just call the static methods (`this` is just ignored).

	(:) enter {
		32ue counter = 0;
		math::Mathematics[Float: 64f, counter: counter, MAXTIME: 100] m;
		m.sin(math::PI);
		m.calculateSomething();
		// ...
		sml::out << counter;  // Could be non-zero!
	}

All kinds of nifty optimizations may lie here. The most important one is that the
static variables can just be replaced in the code generators. That makes the class
instantiation extremely powerful. One thing is that the lvalue must not use dynamic
placement:

	(:) enter {
		1u choice = 1;
		[2, 32ue] counter = [0, 0];
		Class[counter: counter[choice]] c;
		// ...
	}

The problem is that choice can't be read inside the `Class` object. This implies that
choice must also be given. Instead of explicitly giving it, why not implicitly do so?
Of course, in this case counter has to be an lvalue. What the templates do is directly
replace the name by the expression. The natural question to ask is; what is pure?
I don't know anymore. I really really like the fact that everything seems so pure.
So are given references impure to touch now? They're explicitly given. Maybe we need
to limit purity to IO. Builtin IO, that is. Anything using hack is not pure? Sounds
reasonable... actually that sounds perfectly reasonable. Considering how hack allows
IO in the first place by calling the system. Finally! We have something concrete. Another
interesting tidbit is the standard module library (sml). This will be passed implicitly
everywhere. Its IO 'global' variables will be impure to access. This separates truly
pure Cxy from the hacky Cxy. I like it!

	// Main.cxy
	32u counter = 0;
	(:: pure) constructor {
		construct counter(0);
	}

	(:) enter {
		32u CO = 3;
		sub::Class[Type: This, AFF: CO] value;
		value.go();  // Would fail if CO != 3
	}

	// sub/Class.cxy
	if [AFF == 3] {
		(:) go {
			sml::out << "Woah, what's going on?!";
		}
	}

Would be cool if we could send in namespaces. That'd make a big difference I think.
Unfortunately, namespaces have the same names as variables. Most of the time at least.
The problem with allowing namespaces is that it obfuscates local folders. What if one
overlaps with the given namespace? No, that's ugly and you then need to consider the
subfolders. That is ugly. Let's not do that. So we stick with normal variables, classes,
and constexprs. Any pure function should be computable during compile-time. This allows
the compiler to make sure that constexprs solely use other constexprs and pure functions.

	ComplicatedClass CLASSNAME;  // Valid as long as the constructor is pure

This is often not so trivial in C++, where basically only PODs can be constexpr. So
what can be done now? I think I just need to shear off the excess fat from the grammar.
Currently, the grammar is quite big. I'd like to make it more elegant. Where do I start?
I guess we could take a look at the type expressions and expressions in general. Hopefully
the lexer can aid us here. What I actually not really like is the non-separation of
variable names, functions, and namespaces. There's just something that irks me deeply.
A possibility is to prefix all namespaces with :: or another operator. Methods are
variables so those are probably fine, although they have wildly different semantics.

	static (:) something lambda { sml::out << "hey you"; };
	static ptr 32u value new(10){3};

	method (:) enter {
		something();
		value[3] += 4;
		sml::out << value[3];
	}

Yeah that's the nice thing about variable declaration. It's that you have a single
name followed by something else. This can easily be exploited. I think the grammar
needs a beautification of some sort. How do we cleanly define methods and values of
a class?

	method (:) enter { }
	member (:) renter lambda { };

It makes sense though, although 'method' and 'member' are quite annoying to have to
write all the time. That's why I like the more context-sensitive syntax.

	Signature (Type)
	|   Name
	|   |          Method code
	|   |          |
	(:) methodName { /* code */ }

	Signature
	|   Name     Initializer
	|   |        |
	32u variable = 0;

As can be seen, the method code can be considered an implicit lambda-initializer.
This seems far-fetched. I'd like to have a general syntax that can also infer the
type.

	ptr 32u a = new(10){0};
	(:) m = lambda[a]{ sml::out << a[9]; };

I like this more. It's cleaner and doesn't require any special syntax. All that's
needed is for the right side to be aware of the right side. It just infers the type
from the left side. I hope that's manageable. Now, as for methods. What should be done
there? We know that we always have one class per file. This makes programming easier.
We know that top-level types will be class members. Now, there ought to be a distinction
between methods and variables. Scala uses 'var' (or val) and 'def' to distinguish them.
My main issue is that the syntax seems to be incompatible. For example, for normal
variables we use '=' or a constructor. For methods we use a direct { sign. I think
this needs to be changed

	32u a = 3;
	(:) enter = {

	};

The problem with this is that it's actually quite ugly. It requires a semicolon and
treats methods as variables (which they are not). We could summarize using the following
rules:

	Each member that has a direct function type is a method.
	Each member that is not a direct function type is a variable.

Ideally, the distinction between variable and method should be minimized.

	MY_ENUM; ANOTHER_ENUM = 3;
	32u CONST = 391 + ANOTHER_ENUM;

	static 32u varname = 3;
	(:) enter {
		++varname;
	}

What about static and non-static members? We could just say that: if never accessed
in a certain way, they are static/non-static. I think the best thing to do is to move
`(:)` up in the 'type' hierarchy. Let it be its own 'function signature'. With this,
we can easily distinguish between methods and members. Let's look at an example:

	static ptr (:) fptr = {};  // Function type inferred

	(:) enter {
		fptr = lambda[](:){};  // Function type specifically stated

		// Illegal, no expressions in type[name]!
		// fptr = lambda[]type[@fptr]{};
	}

The lambda should always return a ptr to the signature it's using. This way, we can
ensure that the correct type is given. This also implies that the signature isn't a
type directly, but merely a specific side-thingy. I don't know how to explain it. It's
nice really. I'm thinking static may also be used on functions. The reason is that
'this' really makes it more ambiguous. Hell if you wanna cast it you can just add the
'this' variable if you want:

	(::pure) doStuff {
		debug sml::out << "Hello!" << sml::endl;
	}

	static (:) enter {
		This object;
		object.doStuff();
		ptr (: this : pure) fptr = doStuff;
		fptr(this: object);
	}

There's something I don't really like here. First of all we're disconnecting the signature
from its original intent: to convey the actual signature. Yeah, static just belongs
inside the signature.

	(: this : pure) doStuff {
		debug sml::out << "Hello!" << sml::endl;
	}

	(:) enter {
		This object;
		object.doStuff();
		ptr (: this : pure) fptr = doStuff;
		fptr(this: object);
	}

There, much better. Not the most elegant solution, but pretty elegant. Like in the
middle of elegance on the scale of elegance,.. right? Anyways, this is how I like it.
What if the class is from a subclass?

	(:) enter {
		SubClass subclass;
		subclass.doStuff();
		ptr (: SubClass this : pure) fptr = SubClass::doStuff;
		fptr(this: subclass);
	}

That works out quite wonderful. Yeah, I'm starting to really like this. It's also easily
parsable. We can always distinguish between TYPE and SIGNATURE. That's really freaking
nice. I hope I can continue with this. Okay, so that's out of the way. What about actual
static variables though? Those aren't addressed yet. Do we still need the static keyword?
Perhaps. I'm not sure. It doesn't really affect the type of the variable. Maybe name
mangling could work for static variables... Imagine a mangled static name:

	32u {MANGLED, local};

	(: this) run {
		local = MANGLED;
	}

	(:) run2 {
		MANGLED = 34;
	}

Just that the CAPITALS are already taken by constexprs. If only there was an alternative.
Yeah maybe static is the best bet for variables.

	static 32u unit_counter = 30;
	32u current_id;

	(:) constructor {
		++unit_counter;
		sml::out << unit_counter << sml::endl;
		construct current_id = unit_counter;
	}

Looks pretty much fine actually. And I can always add `static { ... };` if needed.
Yeah, compromise. There's no name mangling for statics apparently. I guess that's fine.
The only name manglers are pretty much enums and CONSTEXPRs. Well, classes and variables
also mangle their names, but not as strictly. We could also just say to hell with static
variables and just create an object instead. That's actually very elegant! Just imagine:

	32u counter = 0;

	(: this) constructor {
		construct counter = 0;
	}

	(:) enter {
		This local;  // Always alive during the program anyways.
		SubClass[counter: local.counter] subclass;
		// ...
	}

That is so elegant... I love it! That's absolutely fantastic. It also allows scalability
since we can just clone the object and repeat the previous run. I think this concludes
the search for 'statics'. Any other topics to consider? We've removed grants altogether
and now just use direct substitution. There are variables and methods, functions can
also exist. CONSTEXPR and `ENUMERATED_VALUES`. What more? Alias of course. I think
it's for the best to just do away with alias altogether. I mean it may be nice for
a short time but we can hardly do any direct substitution with it. Well, it kinda removes
the hell of large namespaces/class names, which is nice really, but I don't think that
it will pay off in the end. The reason is that we just cheat ourselves of the true
variables. Especially with the alias statements that allow this:

	alias ClassName = namespace::ClassNameName;

We'd start looking at the granted ClassName whenever we'd see it. That's what I don't
like about it. Secondly if we allow

	alias nm::Cn = namespace:: ClassNameName;

then whenever we encounter "nm::Cn", we'll be looking for that folder, it may even
clash with the folder name. That's definitely not desirable in a language that is
supposed to be easily readable. Yeah, let's avoid that ugly mess! Then, the language
becomes quite simply actually...

	methods
	variables
	grants
	static ifs

Using static ifs for code generation seems very appealing, but it doesn't bring out
the full capacity of code generators. I'd love to see the code generator that was discussed
in the beginning in place. But that comes later. First we need to implement this. Well,
basically de-plement what has been implemented. Expressions need to be cleaned. What
ties types to expressions is the ability for arrays to contain expressions of CONSTEXPR
types. They can't contain non-constexpr expressions. So yeah, I just need to deplement
some of the stuff and that should be the end of the grammar.

	8ue counter = 0;

	(:) enter {
		sml::out << "sample program" << sml::endl;
		This object;
		try {
			while (true) {
				object.counter += 1;
			}
		} catch {
			sml::UNSIGNED_OVERFLOW
				sml::out << "Unsigned integer overflow detected"
					<< sml::endl;
			sml::SIGNED_OVERFLOW
				sml::out << "Signed integer overflow detected"
					<< sml::endl;
			any
				sml::out << "Caught an unknown signal!" << sml::endl;
		}

		sub::Class[counter: object.counter] object2;
		try {
			object2.check();
		} catch {
			sml::UNSIGNED_OVERFLOW
				sml::out << "Exactly what we expected" << sml::endl;
			any
				sml::out << "Caught an unknown signal!" << sml::endl;
	}

	// sub/Class.cxy
	(: this : pure) check {
		counter += 1;
	}

I really like the idea. It's clean and tractable. I really like it. Really really like
it.

	private 32u {a, b, c}, [100, 8u] buffer, sml::String holder;
	public ...

Those look nice as well, goes hand-in-hand with the variable declaration syntax. Even
the extraction syntax!

	restricted 32f {value, error: error} = sml::sin(sml::PI);

	(: this, 1u test: pure) constructor {
		if (test)
			construct value, error: error = sml::cos(sml::PI);
	}

I've been thinking. I don't like restricted/public/private that much actually. So an
idea popped into my mind: every non-method is private. Every method is public. However,
every `ptr SIGNATURE CONSTEXPR = { ... };` is considered a private method. Hmmm...
it does look kinda ugly to be honest.

	ptr (32f out : this : pure) COMPUTEINTERNAL = {
		out = sml::sin(0.1);
		out += 1.0;
	}

	(32f out : this) get {
		out = this.COMPUTEINTERNAL();
	}

Nah, there has to be something better than this.


	private (32f out : this : pure) computeInternal {
		out = sml::sin(0.1);
		out += 1.0;
	}

	public (32f out : this) get {
		out = this.computeInternal();
	}

Actually this looks pretty much fine... doesn't look bad at all, but perhaps it'd be
best to put the access specifier somewhere else. I'm thinking after the type signature.
But honestly, it doesn't look bad. We can just say everything is public by default
such that enter becomes easy to learn (upon being introduced to the language). The
nice thing about not putting access in the signature is that it doesn't actually matter
to the semantics of the program:

	(:) enter {
		(::private) name = This::staticFunction;
		name();
	}

If you can return the function, then it loses all forms of specification. It's absolutely
unnecessary. Alright.

	ACCESS TYPE namelist [More] = EXPR ';'
	ACCESS SIGNATURE name '{' code '}'

Is the basic setup of a class. Simple? Yes. Elegant? Yes. Short? Yes I'd say so. For
a strongly and statically typed language. Then, the bodies of functions are like:

	STATEMENT
	{ EXPR [name ':' name] }+ ['=' EXPR] ';'

I need to really capture the essence of the grammar, let's try again:

	DATA ::=
	ACCESS { TYPE (
	               name [ ':' name | '=' EXPR ]
	               | '{' { name [ ':' name | '=' EXPR ] }+ \{ ',' } '}'
	              )
	       }+ \{ ',' } [ = EXPR ] ';'

Good, that's out of the way. I think that's exactly what I want! Now for the methods

	SIGNATURE ::= '('
	                (
	                 { TYPE (
	                         name
	                         | '{' { name }+ \{ ',' } '}'
	                        )
	                 } \{ ',' } [ ',' ]
	                )
	                 ':' $1 ':' ['const'] ['pure']
	              ')'
	ACCESS SIGNATURE name STATEMENT

Our enums:

	[ ACCESS ] { ENUMNAME ['=' EXPR] }+ \{ ',' } [ ',' ] ';'

Statements:

	STATEMENT ::=
		'if' '(' EXPRESSION ')' STATEMENT
		| '{' { STATEMENT } '}'
		| 'goto' name ';'
		| 'label' name ';'
		| 'try' STATEMENT 'catch' ...
		| 'while' '(' EXPRESSION ')' STATEMENT
		| 'for' '(' STATEMENT ';' EXPR ';' STATEMENT ')' STATEMENT

Where $1 denotes the first capturing group. The regex, not what has actually been
captured. So the regex is just copied there. `\{}` means to interpolate the previous
`{}` group wtih the sequence within. `{ 'a' } \{ ',' }` would accept:

	(nothing)
	a
	a, a
	a, a, a

And so on. The trailing comma is not allowed since we don't have lists like most other
programming languages. C++ for example allows the trailing comma in enums:

	enum A {
		a,
		b,
		c,
	};

But we don't have enumerations in that sense. Our enumerations are ended by a ';',
which makes the use of trailing commas not worth anything. And trailing commans in
arguments are just ugly. The trailing comma is not necessary. The only reason they
are in enums is that you can easily copy the last line and add an entry. For expressions
containing raw arrays, we may allow a trailing comma. Let's look at the use case:

	[8, 32u] a = [
		1,
		2,
		4,
		8,
		16,
		32,
		64,
		0,
	];

Longer lists are even more complicated. It's always nice to have the ability to trail
the comma. But only for lists like these. We don't need them in arguments. Or do we?

	myFunctionName(
		a: this.something()~height,
		somecomplicatedname: alfa + beta / gamma \ THETA,
		[3, 4, 5, 6, 7],
		control: [1, 2, 3],
	);

Yeah,... that's actually pleasant to have. Just in case you need to add another argument.
Are commas really necessary for lists though? I suppose they can always be optional.
I still have unanswered questions. For example, isn't [ the start of a type? And types
can be evaluated in expressions for having static members. Hold on... wait a second...
can it be solved by... no. Creating an object?! This would create a name but solve
the entire conundrum... hold the fk on man :O.

	ClassName name;
	name.doSomething();

That doesn't really aid readability though. As in, you have a static local object
and you're not using it that much. What about a class-local constexpr one?

	Classname NAME;

	(:) enter {
		NAME.doSomething();
	}

No I think it's more useful to be able to call statics directly. But how do we reconcile
the use of [] inside an expression? [8, 32u]::size could be useful... right? C solves
it by using the sizeof operator. I think it's an interesting approach. It works on
any class. '::size' looks more like some static method. I think we need an operator
for this just to solve the entire problem. Secondly, we need to look at other topics.

	[3, 32u] a = [size[type[a]], 4, 6];

Should give an array that's `[96, 4, 6]`, because `3*32` is 96 bits of information.
I mean, what information can you get out of primitives? Well you can get the maximum
number from 32f, or 32u, or 50u. But you can also calculated that using size:

	32u MAX = math::pow(2, size[type[MAX]]);
	32u MIN = cast[type[MIN]](cast[32uo](MAX) + 1);  // Overflow back to the least

We'd like to know the size of something, the maximum, minimum, whether it's a primitive,
what kind of primitive. This is useful for generating code.

	if [sml::primitive[GIVEN]] {
		// ...
	}

It also makes me think that perhaps, it'd be a good thing to have nestable grants.
With this I mean that you can send in partial grants. I'd love for unit tests to be
incorporated in the compiler. The idea is that every class that gets instantiated has
a set of tests:

	test myTest {
		if [size[GrantedType] < 8]
			fail "Granted Type has a size less than 8 bits";
	}

The compiler can run the test for each distinct subtype. This can be very useful in
removing errors and applying your own semantic checks to the class. We should obviously
run the lowest classes first.
I guess that's solved then, the primitive types don't really fit into expressions.
So we can still have the array syntax without a problem!

	if [has[Granted, (:) enter]];
	if [has[Granted, public 32u a]];

I'm not sure about the above. Weird things happen in metaprogramming. Instead of SFINAE
I'd like to use something that's more direct (and takes considerably less time). So
how about metaprogramming? Most issues have now been cleared up, grants are easier,
expressions are simpler, statics are resolved. I'm still wondering if we can eliminate
types from expressions and encapsulate them in "size" or other operators.

	# Main
	CLASS ::=
		{ DATA | ENUM | METHOD | STATIC_IF }*;

	DATA ::=
		[ ACCESS ] { TYPE (
		                   name [ ':' name | '=' EXPR ]
		                   | '{' { name [ ':' name | '=' EXPR ] }+ \{ ',' } '}'
		                  )
		           }+ \{ ',' } [ = EXPR ] ';'

	ENUM ::=
		[ ACCESS ] { ENUMNAME ['=' EXPR] }+ \{ ',' } [ ',' ] ';'

	METHOD ::=
		[ ACCESS ] SIGNATURE name (
		                           '{' { STATEMENT }* '}'
		                           | STATEMENT
		                          )

	STATIC_IF ::=
		'if' '[' EXPR ']' '{' CLASS '}' { 'else' 'if' '[' EXPR ']' '{' CLASS '}' }* 'else' '{' CLASS '}'

	# Auxiliary:
	SIGNATURE ::= '('
	                (
	                 { TYPE (
	                         name
	                         | '{' { name }+ \{ ',' } '}'
	                        )
	                 } \{ ',' } [ ',' ]
	                )
	                 ':' $1 ':' ['const'] ['pure']
	              ')'

	STATEMENT ::=
		FEXPR
		| WHILE_STATEMENT
		| IF_STATEMENT

I've got an idea for type information! WOOT! Let's go:

	type[Name]~size
	type[Name]~name
	type[Name]~max
	type[Name]~is_primitive
	type[Name]~is_class

That's absolutely beautiful! The static ifs allow us to conditionally exclude and include
code. This is very useful.

	32u ISVALUE = 33, OUTVAL = 5;

	if [ISVALUE == 34] {

		(:) enter {
			sml::out << "It's 34" << sml::endl;
			if [OUTVAL == 5]
				sml::out << "Also outval is 5" << sml::endl;
			else
				sml::out << "Outval isn't 5" << sml::endl;
		}

	} else {

		(:) enter {
			sml::out << "It's 33" << sml::endl;
		}

	}

As can be seen, conditional compilation simply removes anything that isn't worth adding.
This means that there are no semantic checks on the code that is being eradicated.
There are however syntactic checks, since that never changes. `type` being used for
type information is awesome.

	EXPR ::=
		'(' EXPR ')'
		| EXPR BINOP EXPR                                # Arithmetic: a + b, c % d, e * f, precedence implied  P: 1
		| UNOP EXPR                                      # @dereference, $$const_ptr, -negative                 P: 2
		| EXPR '(' ARGLIST ')'                           # Calling a method, very high precedence               P: 3
		| [ namespace '::' ] [ classname '::' ] member   # Static member expression
		| EXPR '.' member                                # Getting a member                                     P: 5
		| EXPR '->' member                               # Getting a member                                     P: 5
		| EXPR '~' name                                  # Extracting from a tuple                              P: 4

I'll continue the grammar later.
Interestingly enough, new can be considered an automatic function class:

	(ptr Type out : ) new {
		32u {bits, rem: remainder} = type[Type].size \ BITSPERWORD;
		hack("malloc " + (bits + rem));
		out = hack("last");
	}

Something along those lines. I need some kind of pseudo-pure class for new. Anything
that returns a ptr to something could be considered pure, if it calls new it's no
problem. Just comparing the pointer in a non-equality way should be impure. New is
an operator so perhaps it's cool, but I'd still like to have a class for this.

	(ptr Type out :: pseudopure) new ...

Could be it, a method that we declare 'pure' solely on the basis of trusting us (the
programmer). Another idea is to let exceptions be pure. This way, exceptions that
aren't pure are only caught in the impure section. Normally, if a pure method throws,
then it's based on the input arguments. However, new may throw as it implicitly links
itself to a global store or memory. We could say that memory is implicit and always
supplied automatically though. This would make new pure. Yeah, we don't need specific
pure/impure enums, since the impure functions can only throw in such a way that enums
are already caught in impure code. Let's just say that new is indeed pure.

	private POINTERS_TRUE;

	(:: pure) enterPure {
		try {
			ptr 32u {a = new{0}, b = new{0}};
			// if (a > b)  // Impure
			if (a == b)  // Pure
				throw POINTERS_TRUE;
		} catch {
			sml::OUT_OF_MEMORY {
				throw;
			}
		}
	}

	(:) enter {
		try {
			enterPure();
		} catch {
			POINTERS_TRUE {
				sml::out << "Pointers have the same values" << sml::endl;
			}
			sml::SIGINT {
				sml::out << "Interrupt signal caught" << sml::endl;
			}
	}

I like the cleanliness of the above. Only issue I have is that I can't assign a variable
to the enum. The enum has no name. Perhaps `This::Enum` could do. Or `enum[This]`
with `enum` being an alias for `enum[This]`.

	private SYSTEM_ON, SYSTEM_OFF, FAILED_TO_START;
	(:) enter {
		enum a = SYSTEM_OFF;
		try
			startSystem($a);
		catch {
			FAILED_TO_START
				sml::out << "Failed to start the system" << sml::endl;
		}

		if (a == SYSTEM_OFF)
			sml::abort();

		// ...
	}

Creating an object in order to use static functions seems very appealing actually.

	(:) enter {
		math::MathematicsEnginge me;
		sml::out << me::sin(me::PI) << sml::endl;
	}

It'd remove the grammar lookahead ambiguity as well. It also solves the idea of using
different subspaces:

	(:) enter {
		sml::Mathematics m;
		sml::out << m::sin(m::PI);
	}

I like the form of the assignment operator using captures:

	expr: name, expr: name, exrp: name = expr;

It's very powerful and simple. With types it becomes slightly more complex. Although
I suppose with a type in the beginning we know what the form of the code is going
to be. So we don't need an fexpr like the extremely versatile one. For expressions
starting with a namespace name:

	(:) enter {
		sml::Mathematics m;
		m::sin(m::PI);
	}

Using a three-sized lexer buffer we can identify [name '::' name]. And state that
this is in fact a 'static member', and turn that into a single token. I'm a little
unsure. To remedy the problem, we could say that sml is both an object as well as
a package. This way we can allow using sml::out, whilst also support sml::Vector.

	math::Mathematics math;
	math::computeSomething(math::PI);

Should be allowed. Depending on the token directly after the name, we can just see
if it's a type or not. What I don't like about it is the following:

	math::Mathematics math;  // How do I know this doesn't hold state?
	(:) enter {
		math::doSomething();
	}

I need to actually look at all invocations of math::function in order to see if there
are any '.' operators used on it. The biggest problem for me is that if the object
has any constructors, those will be called on the declaration of the object.

	(:) enter {
		sml::Mathematics M;
		M::sin(M::PI);
	}

Oh,.. oh. Doesn't look bad. I like this. It uses the object as a constexpr. This
means that non-static `(: this)` methods can't be called on it. It should also
make all members static. The members that are used, at least. Hell I bet I could get
away with just using the dot operator, but that won't show I'm using a static method
if we are using an actual object.

	(:) enter {
		sml::Mathematics M, m;
		m::sin(M::PI);
		M::cos(m::PI);
	}

The semantics of a constexpr variable are such that you can't change any member variables.
What happens to sml::out though? It doesn't exactly hold state, it just outputs to
the given system stream.

	(:) enter {
		OUT << "Hello World!";
	}

Where OUT is a constexpr granted to the main class. That could be powerful. OUT could
also be a granted object like:

	out << "...";

or even

	sml.out << "...";

Where sml is a global object that contains the out object. I think this is the most
correct version. This makes me consider whether it'd be good to just say we pass
everything down, even sml classes.

	// Main.cxy
	(:) enter {
		sub::Class[String: sml::String, Vector: sml::Vector] subclass;
		sml::out << subclass.doSomething();
	}
	// sub/Class.cxy
	Vector[32u] control;

	(:) constructor {
		construct control([1 2 3 4]);
	}

	(String out :) doSomething {
		out = control.toString();
	}

I don't know, seems a little tough. It does keep each class very sustainable though.
As in, you can easily find out requirements for any class. Luckily you can use `type[name]`
to alleviate some of the troubles.

	(:) enter {
		sub::Class[
			String: sml::String,
			Vector: sml::Vector,
			Control: sml::Logger,
			Graphics: gl::GraphicsHandler,
			]
			SUBCLASS;

		type[SUBCLASS] alpha;
	}

This does give quite a convincing case for whole-namespace insertions though. I'd
love to be able to make the language as clean as possible. The problem I have with
C++ is that you need to define a global library. I like it when it's more local.
Imagine classes just giving arbitrary types downward. That's so awesome. The primitives
are of course very important. One thing that can be done is to make sets, maps, and
similar utilities primitives.

	(:) enter {
		set[32u] a;
		a += 3;
		a += 6;
		if (a[6]) {
			sml::out << "a contains 6";
		}
		a -= a;  // Remove all elements

		map[32u, 1u] b;
		string c;
	}

Another idea:

	private LOG_BOUNCE;
	(:: pure) enterPure {
		32u a = 5;
		bounce LOG_BOUNCE a;
		++a;
		bounce LOG_BOUNCE a;
	}

	(:) enter {
		try {
			enterPure();
		} bounce {
			LOG_BOUNCE name {
				sml::out << name;
			}
	}

What this does is that it allows enterPure to to continue after the bounce without
directly calling any impure functions.

I mean, just think of the classes when they have their grants. You just compile --grant
and get a list of all necessities. You know that this class will work. Of course,
100% likely when all the methods in the class are pure. This is excellent for building
libraries. What if a library wants to use a vector internally? They can just pass
it from the top level downwards. It doesn't seem very practical though. I mean I
get that primitives are defined everywhere, but there are so many important data
structures that should also be everywhere. An example is the array. Incredibly important
to have that accessible at all times. The thing is though, most library authors use
an internal version of their own array, because the provided one wasn't good enough.
Heard that story time and time again. Another thing is the allocator that will be
used. For example, having a fixed-size array for std::vector instead of a dynamically
growing one. Of course, one could also just create a specific version of vector using

	(ptr Type out :: new) create {
		32u {size, remainder: remainder} = type[Type]~size \ OCTETSIZE;
		if (remainder)
			size += 1;
		hack("void *a = malloc(" + size + ");");
		hack("assign", "out a");
	}

We can say this is an allocator, it just allocates some memory. It could also be
a cyclic allocator that overwrites previous bytes, but I'm a little uncertain. Perhaps
it's best if the allocator is implemented in a custom class instead. This will solve
a lot of trouble. Even then, a custom allocator appropriating class can easily be
built.

	[1024, 1u] bits;
	10u at = 0;

	(ptr Type out : this, 32u in, ptr any location : pure) allocate {
		if (location < at)
			throw INACCESSIBLE_LOCATION;
		out = cast[type[Type]](at);
		at += Type~size;
	}

	(:) enter {
		col::Vector[Alloc: This] vector;
		col::Map[Alloc: This] map;
	}

This way, we can create cheap and simple allocator classes and pass them on to the
appropriate subclasses. In a loop, we can do:

	Allocator[32u] allocator;
	while (some_condition) {
		ptr 32u a = allocator.allocate();
		@a = 130 + condition_variable;
		// ...
	}

Basically a garbage collected loop. We allocate a large amount on the stack, and
use the allocator as an abstraction for 'allocating' objects.

An interesting aspect of any sorted array is that the addresses of the objects could
be used to sort the elements. This is currently an impure operation. new has always
been pseudopure, but I'm considering just making it pure. Really keeping it to 'hack'
to be impure:

	SortedArray[ptr 32u] sorted;
	(: this : pure) myfun {
		for (32u i = 0; i < 1000; ++i) {
			ptr 32u x = new{0};
			sorted.insert(x);
		}
	}

Could give different arrays each time. That should be fine. Hell we accept side effects
due to glocals, why not allow this indeterminism? The action of taking a value's
pointer and comparing it shows something that is not deterministic. One can base
decisions on those values. We can just imply that the system is given as an argument.
Yeah, that solves it. We just give an instance of the memory to the program. If the
same instance is given, then we should also get the same values. Of course we never
get the same instance. Oh well. Compromise.

I think the semantics for functions {} need to be inspected.

	new[32u](10, address: delete(old)){0 + 1};
	lambda[a](:){ /* code */ }

I think the syntax for these expressions could just be special.

	(:) name { /* code */ }
	32u name2 {0};
	32u {x: a, y: b} {a: 0, b: 0};

I think perhaps these rules are for themselves. Do I have everything then? Is this
sufficient?

	public (: this : pure) inc {
	}

Oh, idea, instead of having some functions, why not just use 'restricted' on methods?

	restricted (: this : pure) myMethod {

	}

If a method is restricted, that means that it can not be changed. This makes all
methods true lambdas. The problem with that is that the methods can be changed from
the inside. In that case, the method would need to be const.

	const (:) myFunction = {
		// Why use an equals? Because it's seen as a variable.
	}

I really would like to unify the whole semantics game into a simple system. Every
file is a class, every instantiation creates an object. The object contains data
and methods. Or it contains just data, but methods are specialized data.

	(:) enter {
		32u a = 0;
		ptr (:) x = lambda [$a] { @a = 10; };
	}

There has to be something called "class-scope", for which the semantics are slightly
different than statement-scope.

	name::Type {x: a, y: b}, 32u z: c = f();
	(: this) method {
		this.z = 10;
		label name;
			--z;
		if (this.z > 3)
			goto name;
	}

I'd like to get to the core of the language. This would remove a lot of statements.
The only ones left are assignment, declarations, expressions, and if/goto/label.

Man I just wanna have this all neat and finished. I feel like it's come a long way
already. Just an idea about minimalism though. Why not remove enums altogether and
instead just throw random names as you like?

	throw whatever_i_want_dude;
	throw myError;

Or just do not find it necessary to declare enums. This would simplify catching the
errors as well. How is the grammar fully LL(1)?

	Type member;

	(: this) constructor {
		construct {
			member(1 * 8);
		}
	}

	const ptr (: this) function = lambda (: this) {
		// Statements
	};

The interesting thing is, making methods constants, then we have a very 'pure' model,
meaning that the approach is minimal. We can idealize the signature-first syntax
as syntactic sugar :).

	sub::TypeA {a, aa}, sub::TypeB b;
	32u length = 0;

	(: this) constructor {
		// Statements
	}

Yeah, I think that's about right.

	try {
		32ue a = 32ue~max + 1;
	} catch {
		NUMERIC_OVERFLOW
			sml::out << "overflown integer!";
	}

I think I can roll with this. The syntactic sugar, yeah. Looks fine. Just need to
polish the semantics a little and the language will be done. Especially the enumerations.
Those are important. So an enumeration is associated with some specific class. What
about primitives? I've though about this a bit. I think it's best too keep primitives
just the basic units (integers, floats), and add arrays to the mix. Pointers are
also included. So if you have a super awesome custom vector, then just return the
pointer to the array with the size, or turn it into a suitable array. Hell you can
even return a nested array. Should be easy. Yeah that's feasible.

I've been thinking about purity. It's nice to have, absolutely. The problem in making
hack impure is that we can't do awesome floating point optimizations in some cases.
For example, the fast inverse square root may want to use hack in order to speed
up the computation. In addition, the code is already 'pure' since things are explicitly
granted to the lower class.

	(:) enter {
		32u value;
		hack("asm
			movl $10, %eax;
			movl $20, %ebx;
			addl %ebx, %eax;
			out: value, %eax"
		);
		print value;
	}

Maybe we just need the most basic of io, just printing, errorring, and file handling.
It is desirable to be able to do some assembly programming deeper down. Also, the
grants kinda destroy some of the purity that was thought of. In C++ you can include
whatever you want and it will work. I'm thinking that's not in line with Demeter's
laws. How is this solved? Knowing that an algorithm is pure is a very nice thing.

I just want to see a library with a top class being 'pure'. That would be very comforting.
It lets me know that this library, when given the same input, never diverges from
that input. This is why I believe bounce handlers should be given. No. Instead, let's
make impure grants be callable from pure code! Genius? Let's examine.

	(:) enter {
		name::Class INVERSE;
		sub::Class[fast: INVERSE::fsqrt] myclass;
		myclass.run();
	}

That looks actually pretty neat. You inject impure code where it is requested. Not
only does this keep the impurity at the top level, it also allows you to inject optimizations
and other niceties. As long as the subclass doesn't use hack directly, it's fine.
It's just like going out to enter, getting the result, and then inputting that result
back into the class.

This means that the subtree is pure, it doesn't use anything machine-dependent, meaning
that cutting the subtree off and using it elsewhere should always work. I think that's
it. Are we done? Defaults of course. So the [] allows us to override definitions?
Seems like something nice. I mean, if it's desirable then sure, why not? Meaning
that not using the fast inverse square root will just fallback on the pure algorithm.
I like that idea. It's very clean. Hmm, you could basically inject anything then.

	private (:) cool {
		sml::out << "Hey!";
	}

	(:) enter {
		sub::Class[myFunction: cool] a;
		a.myFunction();
	}

That would be scary, I suppose it's best to make any grants only accessible inside
that class. If you want a reference, just create a function for it.

	project/
		Main.cxy
		code/
		impure/

Having projects like this basically makes code your free world. Impure code is separated
from pure code. I think this will work out very nicely. The compiler also needs to
grant something to Main:

	(:) compilerEnter {
		project::Enter[File: internal::File] ENTER;
		ENTER.enter();
	}

We could idealize the output stream as a file too.

	(:) enter {
		Io a(Io::STDOUT_FLAG);
		a << "output";
		Io b(Io::STDIN_FLAG);
		b.readLine();
	}

Wait, are grants of Io downward pure? Io can just be labeled impure. But does this
make printing and file operations in granted classes 'pure'? Well yes! The hacks
were granted. I guess enter can also be pure then! So long it doesn't have hack in
its hierarchy, all is well. This seems so clean. I love it! Io is a granted class
then. Nice. Granted by the compiler for a specific machine. This makes the writers
of the compiler only liable for a tiny IO library. That's it. The Io library should
support stdout, stdin, and files. stdout/in are files, essentially.
We just define a simple io library and we're done. Cool.

So to finalize the grammar,... let's bring back what was written before. A final
grammar. Let's do it.

	// Class-Scope
	CLASS ::= { [ ACCESS ] ( DATA | ENUM | METHOD ) } ;
	ACCESS ::= 'private' | 'public' | 'restricted' ;
	DATA ::=
		{ TYPE (
		        name [ ':' name | '=' EXPR | ARG ]
		        | '{' { name [ ':' name | '=' EXPR | ARG ] }+ \{ ',' } '}'
		       )
		}+ \{ ',' } [ ',' ] [ '=' EXPR ] ';' ;
	ENUM ::= { enum  ['=' EXPR] }+ \{ ',' } [ ',' ] ';' ;
	METHOD ::= SIGNATURE name STATEMENT ;
	SIGNATURE ::= '('
	                (
	                 { 'this' | TYPE (
	                                  name
	                                  | '{' { name }+ \{ ',' } '}'
	                                 )
	                 } \{ ',' } [ ',' ]
	                )
	                 ':' $1 [ ':' [ 'const' ] [ 'pure' ] ]
	              ')' ;
	ARG ::= '(' { EXPR [ ':' EXPR ]
	            | '{' { name ':' name }* \{ ',' } '}' ':' EXPR
	            }* \{ ',' } [ ',' ]
	        ')'

	// Type and Expressions
	TYPE ::=
		[ TYPE ] 'type' '[' name ']' | 'ptr' TYPE | 'ref' TYPE | 'const' TYPE
		| '[' { EXPR }+ \{ ',' } ',' TYPE ']'
		| 'enum' '[' TYPE ']
		| typename '[' { TYPE [ ':' TYPE ] | EXPR [ ':' EXPR ] } ']'
		| primitive ;                     // 32u, 8ue, 16so, 3sb, 32f
	STATEMENT ::=
		'if' '(' EXPR ')' STATEMENT
		| '{' { STATEMENT } '}'
		| 'goto' name ';'
		| 'label' name ';'
		| 'try' STATEMENT 'catch' '{' { STATEMENT } { enum STATEMENT } '}'
		| '++' EXPR ';' | '--' EXPR ';' | 'hack' '(' STR ')' ';'
		| ;
	FEXPR ::=
		{ EXPR [ [EXPR] ':' name | '{' { name [ ':' name ] } '}' ] } \{ ',' } [ '=' EXPR ] ';' ;
	EXPR ::= // Precedence decreasing
		EXPR '.' name
		| EXPR ( '+' | '-' ) EXPR
		| EXPR ( '*' | '/' | '\' ) EXPR
		| EXPR '..' EXPR ;

So ideally, we'd have everything pre-determined by using an LL(1) lookahead, instead
of determining things after-the-fact. Maybe ':' needs to be brought back. We aren't
an LR(1) parser! Ech! Post-processing. Post processing fixes it. A current problem
is that type expressions with type~something aren't processed correctly. Man that
automatic passing mechanism gives a lot of butthurt. It's just annoying to work with.
Let's say you can't use types directly in expressions, this means that whatever starts
with a type becomes a valid assignment. I'm considering not allowing field-initializers
since I'm afraid they can cause confusion. Say you have a variable set to 1, but
the constructor says to construct with 0. What is used? How does it relate to other
operations and impure operations? Let's just say the constructors must not branch
and will simply create a new object. In the beginning, the object is not constructed
at all. This allows us to save computing time. The nice thing about field initializers
is that they're so well understood, languages use them. People love them. They're
fast and easy. What about the ambiguity:

	a, b = f();

We could disallow the = per unit. I really don't want the := operator in the language.
My solution is to not allow combinations of ':' and '='. The method of using:

	EXPR '=>' { TYPE name [ ':' name] | EXPR } ';'

Could be interesting, but we get the same problem with EXPR/type confusion. This
is the core of the issue. EXPR can have the same FIRST set as TYPE. How is this solved?
One could have a universal type prefix for types inside exprs.

	type[[32, 64se]]~
	::[32, 5, 1, 8u]~size + ...

I like the use of '::'

	::MyClass::coolFunction(300);
	::Io::write(::MyClass::coolFunction(300));
	::name::Space~size + 9;

Some kind of element that differentiates the FIRST sets. '::' is rather large. It
can be shortened to '?', or some other token.

	<MyClass>~size
	<name::Space[32u]>

I kinda like that. You're never going to nest these, the only trouble are the <>
operators, these will clash. Yeah I can just say that types aren't allowed in expressions
and that everything must be an object in an expression. The initial solution. I really
wish to be able to separate types from expressions. That would be beautiful. The
problem is just having this FIRST set being incompatible. Ugh,..

Now ARG is clashing with the lists {}.... I really need to revise the grammar so
it doesn't have these lookahead clashes. Hopefully the grammar can be augmented.
It's the idea that matters (the design of the language). Not so much the syntax.
At the heart of the issue lie expressions and types. Types can be a part of an expression,
types may include expressions (arrays, grants). Expressions may include types. This
leads to a clash. Yeah I think the solution is just to remove types from expressions
altogether. Let's just use CONSTEXPR objects instead.

	auto x = f();
	32u a = x~a, b = x~d;
	type[a] A;
	A + 3;  // Not a type because of the CONSTEXPR

Now the remaining conflict is the {} and {} list in arguments. Using [] would clash
with arrays:

	[ ...  // Array declaration or expression?
	array[32u, size: 10]  // Clear as day.
	[] 100 20 [] 5 32u a;
	[10, 32u] a = array { 1 3 9, 9 };

I like that last one. It's a special operator of sorts. Why not allow {}? Because
it clashes with scope. Maybe expressions should not be allowed all alone on a line.
Maybe var is the true solution to all problems. But it does not justify extraction.

	= f() =>
		Type {a, b}, c: d;
	[3, 32u] a = {1 2 3};

I really wish to make all operations easy to parse by using an appropriate lookahead.

	var 32u d;
	var {Type {a: x, b: y}, my::Class c: z}, d = f();

This kinda does var a favor. By forcing var, we're giving a strong enough hint. So
we could just idealize using that.

	10u a, b(130), c;
	a, b, c = f();  // { EXPR }+ \{ ',' } [ '=' EXPR ] ;
	a: derp, b: out, c: control = f();  // { EXPR [ ':' name ] }+ \{ ',' } [ '=' EXPR ] ;

I really like that last one. It's so clean. It also filters initialization from actual
expressions (assignment). By disallowing '=',... we could still salvage the fexpr:

	Type {a, b: c}, d: y, 10u e: x = f();

I think that's quite hard to read. I'm skilled in reading Cxy but I find this hard
to decode

	Type {a, b: c},
		d: y,
		10u e: x
		= f();

No, I don't like this at all.

	f() {
		Type a = control;
		32u b = out;
		c = d;
	};

What about that extractor syntax? Just ... no? `EXPR '{' { STAT } '}'`. Nah man.
The extraction syntax is very much liked:

	x[3 + i]: my_name, extractor.name: controller = name.getClassMember()~ground()~holdUp();

I really enjoy that syntax. It's just python but with names. I want to make the language
more simple. Especially with the variable declaration syntax as it is.

	Type mytype, yourtype(0.3), ourtype(399, count: 50);
	32u a(30);
	ptr 800u(new(20){123});

This would make the grammar a whole lot simpler. Also we can prefix types with '::'
to make them usable in expressions:

	::Type::CONSTANT  ...
	for (32ue i(0); i < ::name::Space[Aa]::CONSTANT; ++i) {
	}

To be honest, using i(0) feels awkward. Not being allowed to assign upon creation
feels slightly awkward. I think I can get used to it though. It's better practice.

	Type mytype;
	mytype = ::Type(0.3);

So what about arrays? `[` vs { vs (. array{} is something I like.

	#GRAMMAR

	// Class-Scope
	CLASS ::= { 'test' STAT | [ ACCESS ] ( DATA | METHOD | cnamep TYPE ')' ) } ;
	ACCESS ::= 'private' | 'public' | 'restricted' ;
	DATA ::= TYPE { name [ ARG ] }+ ';' ;
	METHOD ::= SIGNATURE mname STATEMENT ;
	SIGNATURE ::= '(' ( { DATA | 'this' ';' } ) ':' $1
		[ ':' [ 'const' ] [ 'comp' ] [ 'ctor' ] [ 'dtor' ] [ 'pure' ] ] ')' ;
	ARG ::= '(' { FEXPR }* \{ ';' } ')' ;
	FEXPR ::= { EXPR [ ':' mname ] }+ \{ ',' } [ COMPOUND EXPR ] ;

	UNOP ::= '@'|'$'|'$$'|'\'|'!' ;
	BINOP ::= '+'|'-'|'%'|'^'|'&'|'|'|'*'|'>='|'<='|'=='|'!='|'>'|'<'|'/'|'<<'|'>>' ;
	COMPOUND ::= OP . '=' ;
	OP ::= 'op' '-' name | BINOP ;
	UN ::= 'un' '-' name | UNOP;

	EXPR ::= UNA_EXPR [ OP EXPR ] ;
	UNA_EXPR ::= { UN } MEM_EXPR ;
	MEM_EXPR ::= CALL_EXPR [ ( '~' | '.' | '->' ) MEM_EXPR ] ;
	CALL_EXPR ::= RES_EXPR TRAIL ;
	TRAIL ::= [ ( ARG | LIST ) TRAIL ] ;
	RES_EXPR ::= [ '::' TYPE '::' ] ( name | mname | ename )  | '::' TYPE '::' ARG | '(' EXPR ')' | string | integer | float | LIST
		| 'lamda' [ '[' { name } ']' ] [ SIGNATURE ] '{' { STAT } '}'
		| 'cast' '[' TYPE ']' '(' EXPR ')'
		| 'new' [ '[' TYPE ']' ] ARG [ '{' ARG '}' ]
		| 'del' '(' EXPR ')' ;
	LIST ::= '[' { EXPR }+ ']' ;

	TYPE ::= [ 'ref' ] TYPEREF ;
	TYPEREF ::= [ 'const' ] TYPECONST ;
	TYPECONST ::= [ 'ptr' ] TYPE | 'ptr' SIGNATURE
		| 'array' [ '[' { EXPR } ']' ] TYPE | primitive | 'enum' TYPE
		| [ pname ] cname [ '[' { ( TYPE | cnameeq TYPE | FEXPR ) ';' }+ ']' ] ;

	// Statements
	STAT ::=
		'goto' name EXPR ';' | 'label' name ';' | 'try' STAT | 'default' name ARG ';'
		| 'catch' '{' { case [ TYPE '::' ] ename STAT | STAT } '}' | 'raise' ename ';'
		| 'hack' '(' string ')' ';' | DATA ';' | FEXPR ';' | '{' { STAT } '}' ;

	// Lexer, names are given in lowercase.
	name ::= lower { lower | upper } ;
	cname ::= upper lower { lower | upper } ;
	cnamep ::= cname '(' ;
	cnameeq ::= cname '=' ;
	pname ::= { lower }+ '::' ;
	digit ::= '0'|'1'|'2'|'3'|'4'|'5'|'6'|'7'|'8'|'9' ;
	integer ::= { digit [ ' ] }+ ;
	float ::= integer '.' integer ;
	string ::= '"' { any | '\"' } '"'
	primitive ::= integer ('s'|'se'|'so'|'u'|'ue'|'uo') ;
	lower::='a'|'b'|'c'|'d'|'e'|'f'|'g'|'h'|'i'|'j'|'k'|'l'|'m'|'n'|'o'|'p'|'q'|'r'|'s'|'t'|'u'|'v'|'w'|'x'|'y'|'z' ;
	upper::='A'|'B'|'C'|'D'|'E'|'F'|'G'|'H'|'I'|'J'|'K'|'L'|'M'|'N'|'O'|'P'|'Q'|'R'|'S'|'T'|'U'|'V'|'W'|'X'|'Y'|'Z' ;

Some of the changes that are planned. As can be seen, typedecls are very simple now.
What I don't like is that signatures may contain `Type {a, b, c}`. This is dissimilar
compared to the `DATA` clause.

	(Type a, b, c; 32u out; 32f e(0.34);  :  anon in, second; 8u cool;) doSomething {
		cool: out, out: something = passOn(anon);
	}

I like it! The only ugliness is that the colon is barely visible. Also this means
I can use the 'data' declaration inside the signature. Awesome! Oh, and guess what,
the constructor is included in that, so standard variables! Smooth. That's really
smooth man. I'd suggest just putting in two spaces around the ':' marker. To make
it clear where it is. That's easier to scan. Another idea I have is to let everything
you pass as a [] argument to classes - passthrough automatically, such that libraries
are easier to integrate. Nobody likes just passing around mindlessly. However, it's
still possible to overwrite the classes in the lower subclasses:

	Type my type from(0.9);  // Local type decl.
	32u control variable three(50) ABSOLUTE(10);  // Local and constexpr variable
	HAS_CONTROL NO_CONTROL LOST_CONTROL TOTAL_CONTROL(4);  // Enumeration

	(:: pure) enter {
		Io out(::Io::STDOUT_FLAG);
		out << "Simplify!";  // Simplify is of type `ptr 8u`, and is null-terminated.
		String myString("Simplify!");
		out << myString;  // Simplify is of type `String`, and is not null-terminated.
		out << myString * 2;
	}

	(32f out;  :  32f a b c x y z;) dotProduct {
		out = a*x + b*y + c*z;
	}

Looks pretty nice without the commas as well, maybe I'll keep that, and replace the
semicolons with commas? Maybe. No. This is beautiful. I absolutely love it. It looks
so clean and refreshing compared to C++. Just spaces. Easier to write, easier to
read, easier to organize. So arrays, what about them? `{}` is out of the question
as it clashes. `[` is out of the question, it clashes. `(` is out of the question,
it clashes. What's left? Any groupers? Anybody?

	[10, 8u] wow hey you;
	(:) enter {
		wow = ('a' 'b' 'c');
		hey = ('h' 'e' 80 10+48*3 0-3*8);
		you = (0-3 0-6 0-9);
	}

Only thing I dislike is that - expressions clash with the unary minus operator. It's
still pretty darn clean though. I like that.

	EXPR ::= '(' { EXPR }+ ')' ;

Becomes a rule. Maybe '--' can be a prefix operator. But that mixes badly with the
iterative -- already. `%-` can be seen as the 'blocking negation'.

	[10, 8u] wow hey you;
	(:) enter {
		wow = ('a' 'b' 'c');
		hey = ('h' 'e' !+80 !-10+48*3 !-3*8);
		you = (0-3 0-6 0-9);
		wow = 'a' 'b' 30 0 !-3;
	}

Using % is kinda ugly though. Maybe \ will do. Hey! It's also an escape sequence?
But it doesn't do well for expressions, because we expect a binary operator. Perhaps
something like # could be used. I like ! though. It also gives a nice delimiting
feel to the expressions.

	(: ptr 8u ar; 32u length; 8u in) doSomething {
		// do something
	}

	(:) enter {
		doSomething(ar, length: length = 1 2 3 4 5 6 7 8; 8+9;);
		32f a, b;
		a, b: error = sin(0.321;);
		ptr 32u a; 32u l;
		a, l: length = 1 2 9 7 !-2384 'd';
		for (32ue i(0); i < l; ++i)
			::Io << a[l];
	}

Using FEXPR inside arguments... I don't know anymore. It's ... beautiful.

	[10 32u] a(1 2 3 4 A+B f() c()~d+3/GNUM ::math::exp(::math::EULER, power: 3) 0 0);

I kinda like it. It's easy to type. I suppose it's not recommended to put several
big expressions inside. Also what about the array size type? Use zero for automatic
detection? Or just leave out the number altogether?

	[32u] a ( 1 2 3 );  // Size 3.
	(:: pure) enter {
		a[1], a[2]: second = f();
	}

The comma in the expression list is to avoid writing an array (since that's a valid
expression). I think we're pretty far now. FEXPR inside arguments. I like it! This
will be so clean holy shit. I think I need to write some more example code in it
though:

	public (113ue out : 5ue in) factorial {
		out = in;
		--in;
		while (in > 1)
			out *= in;
	}

	private (String out : 113u in) toString {
		out = ::String::parse(cast[128u](in));
	}

	private (32ue out : Array[32ue] in) sum {

		out = 0;
		Array[32ue] partial_sum(size=in.size);
		partial_sum[0] = 0;
		for (32ue counter(0); counter < in.size; ++counter) {
			out += in[counter];
			if (counter > 0)
				partial_sum[counter] = partial_sum[counter - 1] + in[counter];
		}

	}

	(:) enter {
		try {
			sml.out << factorial(30);
			debug "what's going on man";
		} catch {
			::Sml::INTEGER_OVERFLOW {
			}
		}
	}

Where sml is a granted object. Since grants are automatically down-propagated, there
is no problem in just using this all over the place. So we got everything that's
needed for a practical language! Good. Though, I find it kinda verbose to be honest.
Maybe I should compare code to the C++ equivalent or Python equiv.

	# The above
	fun factorial(n):
		m = n
		n -= 1
		while (n > 1)
			m *= n
		return m

	fun sum(arr):
		out = 0
		arr = [0] * len(arr)
		partial_sum = [0] * len(arr)
		for i in range(0, len(arr)):
			out += arr[i]
			if i > 0:
				partial_sum[i] = partial_sum[i-1] + arr[i]
		return out, partial_sum

My code is somewhat bigger in size. It's somewhat more verbose. Then again, it has
types. Types add a lot of information. Now to polish the grammar. We have statements,
these are independent of the 'core' of the language, which only has 3 statements:

	STAT ::= FEXPR ';' ;
		| 'label' name ';'
		| 'goto' name EXPR ';'

Where the expr in goto is an integer or boolean result. This allows all kinds of
things like loops to exist. So the core of the language is really:

	Data declaration + Granting
	Method declaration
	Assignment/FEXPRs
	Gotos

That's pretty minimal if you ask me. I just need to work a little on the FEXPRs.
I really like them, that's for sure. Now let's look at function calls:

	object.myFunctionName(name="do whatever you want"; size=3; control, inside: out = cool());

That looks really smooth actually. Really smooth. I like it. What about default arguments?
These tend to look like:

	(: 32u a(1023) b(40), ptr Type c(new)) doSomething { ... }

Now, what's unnerving is that the initialization is inside the type of the function.
I'd really like to have it on the outside, but it gives us such useful information
about the function. In addition, it's really useful when calling a function and you
just want to add a single parameter, suppose I start with the following:

	(32f out : (32f out : 32f in) in, 32f from, to ) integrate {
		...
	}

Which is called using

	integrate(
	          lambda (32f out : 32f in) { out = ::Math::exp(!-::Math::pow(in, power: 2)); };
	          from=0;
	          to=10
	         );

But one day we want to add a parameter which is the following

	(32f out : (32f out : 32f in) in, 32f from, to, enum[Math] method) integrate {
		...
	}

Now you can't use the function because method has to be given. Instead, this is desirable:


	(32f out : (32f out : 32f in) in, 32f from, to,
		enum[Math] method(::Math::SIMPSONS_METHOD)) integrate {
		...
	}

This makes the default argument be simpsons method (it's a method to numerically
integrate a function. I really like the idea of having this ultra simple unifying
syntax. The thickness of it! Maybe overloads will do though. How will casts affect
overloads?

	ptr (: 32u takes, these, arguments; 32f with, these, names)
		a(cast[(: ...)](myObj.memberFun));

Where the cast expansion ... is the same as in the signature of a. Should a be auto?
My intuition says 'yes!'. How do I formalize the method of automated casting for
initializers? Any initializer will cast to the appropriate type (or try to). Problem
with that is that I could assign an integer to the type and it would fail at call
time. Difficult decisions... What if we don't allow overloading? What if we just
say that each function has only one signature. But, you can of course avoid setting
an argument. This means that the argument wasn't passed. The function can use that
information to specialize certain cases. This does however bar us from using different
types for the function signature. That's rather sad, I'd love to see the following:

	(: 32f in) name ...
	(: 64f in) name ...

Just depending on the in parameter. I like the idea of overloading. Just gotta keep
in mind how casting affects it. So overloading is fine. Is it sufficiently elegant?
Anyways, as we know, the top-level class-signature is syntactic sugar for just declaring
a CONSTEXPR member variable to the object. That's the equivalent of what it does.
Granting is a powerful system as well. You can't really fight it. I'm concerned about
the manageability though. The thing is, sometimes you won't really know where a class
comes from. It's just there. You know it doesn't come from directly above, it may
come from multiple levels up. You gotta scout the class out. Rather annoying actually.
What about constructor semantics? I'd like to see that come to fruition. Just call

	MyClass myobject;
	myobject.construct();  // Actually initialize the object

This will require no 'construct' syntax or anything similar. I like it. It's simple.
But what about the destruction? Will destruction always take place even if the object
has not been constructed? That's rather antiintuitive. An explicit destructor goes
against the point of having a destructor at all. I think it's fair to say that all
new objects call the default constructor. What about const-initialization in the
constructor though? Well, we could do like Java does and just leave that the constructor
free to change any const member. Also, do we need to call all composite constructors
before the constructor starts? What if we want to construct them with something interesting?
Say a const or something like that. How is that done? Do we just need to re-call
the appropriate constructor on it? Wouldn't that be a waste?

The whole language is syntactic sugar for a class-definition language with lambdas.
That's the deal. DDL, Data-Definition Language.

	DATA ::= TYPE { name [ ARG ] }+ ';' ;

Simple. Then we have the MDL, Method-Definition Language.

	METHOD ::= SIGNATURE mname STATEMENT ;

Also quite easy to grasp. Really, I just need to define the semantics of enumerations,
constructors and various other entities. Going back to constructors. The default
constructor is always called for a local object (or new). For PODs, it'll just default
to a garbage value. Shouldn't be a problem. Ok. Now, the body of the constructor.
Do we allow 'construct' statements? I think it's fine. As long as they're not in
branches. In a sense C++ has the right idea about initializing _all_ variables in
the beginning. This means that you won't use initialized variables in the constructor.
Although I reckon it's easy to catch, if we use anything involving that name before
its 'construct' invocation, then we have an error. One option is to simply say we
individually 'init' every composition.

	Array[32u] a(size: 10);
	(: this; 32u newsize(10)) construct {
		a.resize(newsize);
	}

This is a vastly simplified model. Does not require special semantics or syntax.
Well except for allowing setting of references and consts inside the constructor.
A possibility is to call whatever method or set whatever whilst being a const method.
We could introduce the 'ctor' word:

	Type member(0);
	(: this : ctor) {
		member.setConstMember(1000);
	}

With that, a 'create' function can only be called by a data declaration or by another
create function. This allows cascading downward in compositional problems. I suppose
the dtor doesn't need this functionality. The dtor can just look at its own class
and decide. Perhaps it can do some internal cleanup, but that should not be necessary.
The dtor does need to be called implicitly though. 'delete' won't do, but using a
method might.

	3912u a(cast[3192u](::Object()));

Maybe we have a big class and just cast it into an unsigned. Not really sure if this
is possible though. I suppose the 'dtor' tag could be introduced. I suppose a constructor
'returns' an object, although it more so builds it in-place. I guess the ctor has
an implicit this on both in and outputs. So using a 'dtor' tag just gets called at
the end of an object's lifetime. Objects casted into, say, unsigneds, won't destruct
at all. That is, if the bits are totally exchanged. Or are they? Since interchanging
them essentially means to completely destroy the old object. If a ctor method has
a name, then it is not a constructor, but it needs to be called by a ctor method.

So we've got everything I think. const-member assignment... wait. Why not just call
the ctor and dtor 'construct' and 'destruct'? This would mean that the ctor and dtors
are callable from anywhere. Full freedom. This brings a dilemma though. Where does
the chain start? I guess the var decl invokes. Then, that ctor can call different
ctors. That seems reasonable I suppose.

What about enumerations? Do we use ::Class::enum or something to denote the type?
I like `enum[ClassName]`. I suppose each class has its own 'signals' that it can
throw. Or it can throw any other signal. But I think it's preferred if a class throws
its own signals. Should we allow throwing others' signals?

I really like the simplified language. It gives me a kind of peace that I did not
have before. It's really nice... I just need to internalize the language as it is.

	Type a b c(damage=10) d(enchanted=::Type::RARE_ITEM);
	ptr Array[32u] e(new{in, size: size=1 3 5 7 9 11});
	a: control, b = myFunction();
	try {
		b.runTests(c);
	} catch {
		::Type::RARE_ITEM {
			b.increment();
			c.strengthen(2);
			retry;  // Jump back to the start of the try block
		}
	}

	(:) enter {
		::Io << "This is a message";
	}

One thing that does annoy me is that there is no way of getting a ~size from an array
expression. Should be possible with parentheses. I suppose if I want an empty double
array I'd just write

	(() ())~size

Where we expect the outer array to contain only one element, and be castable from
and to the expected types. Maybe C did it right. It doesn't try and protect you using
consts constructors like I have. I like ctors and dtors and all, but it seems to
be a waste to spend too many resources on something. I like to see the current enums
as 'signals'. We're back at 'undeclared enums' again. I'm thinking let every class
throw what it wants, and it'll be seen as a signal. What about enums? Enums are very
useful, but I think they deserve a different spot in the language. We can use constants
for enums. The problem with that is that there is no designated enum type then. There
are only integers. I want to make coding in this go as buttery-smooth as possible.
Not having to declare the signal is pretty nice. Of course, if you want to raise
a different signal you'll have to trigger that signal. Probably by going into a class
and just raising it from there. Suppose we don't have enums, only signals. What now?
What's left to add?

Enums? Enums are so incredibly useful for programmers. I really enjoy them.

	(:) myMethod {
		raise CAN_NOT_CONTROL_THIS_AWESOME;
		raise I_JUST_CREATED_A_NEW_SIGNAL_NOTHING_YOU_CAN_DO;
	}

It's an interesting concept, but it removes enums from the equation. I'm thinking
a code generator is perfect for an enum. Just imagine the following:

	#Enum(VALUE NAME NAME2 NAME3 NAME4 NAME5)

Just expands into:

	private 3u value;
	public This VALUE(0) NAME(1) NAME2(2) NAME3(3) NAME4(4) NAME5(5);

	private (: 3u in : ctor pure) constructor {
		value = in;
	}

	public (: This in : ctor pure) constructor {
		value = in.value;
	}

	(1u out : this; This rhs : const pure) == {
		out = this.value == rhs.value;
	}

	(String out : this : const pure) name {
		switch (this.value) {
			case 0: out = "VALUE"; break;
			case 1: out = "NAME"; break;
			case 2: out = "NAME2"; break;
			case 3: out = "NAME3"; break;
			case 4: out = "NAME4"; break;
			case 5: out = "NAME5"; break;
		}
	}

	(: this : pure) setVALUE ...

Etc. Should be working. I just wanna clean up the grammar a bit. We've got a pretty
simple language with few productions. I like the simplicity of it. Although some
of the semantics irk me just slightly.

	def x(32u a b c  :  32f d e f)
		sml.out << "Control message" + d + e + f;

We could create a 'macro' like this:

	def <a> <sig>
		<statement>

Gets turned into

	const ptr const <sig> <a> = lambda [@] <sig> <statement> ;

I like the idea. But I also like the cleanliness of the former method of just writing

	<sig> <a> <statement>


	Set[String] set;
	set.add("cat");
	set.add("dog");
	set.add("human");
	if set.has("cat")
		::Io << "set has the cat element";

Hmm... because expressions can't really spill into anything, we can just avoid the
() around ifs at all. They can be completely optional. I like that.

	while 1 > 4 and set.has("cat")~out
		debug "I think I may prefer 'and' over '&&'";

I do enjoy 'and' and 'or'. They're far easier to write as well.

	if a and b and c and d
		::Io << "cool!";

The problem with an expression like this is that array expressions must be parenthesized.

	if 1 2 3
		::Io << "what";

That's a problem, so how can it be resolved? Perhaps it's best to just add an array
operator of some sort.

	call(1 2 3);
	if 1 2 3
		call(1 2 3);

	if #(#(1 2 3) #(4 5 6) #(7 8 9))
		::Io << "I think [] can be much nicer actually.";
	::[::[ 1 2 3 ] ::[ 4 5 6 ]];

I just want to remove the :: from them though. Maybe we need to rethink the entire
array declaration syntax. Perhaps it's best to just write:

	Array[32u, 10] a([1 2 3]);
	Array[Array[32u, 10], 10] b([[1 2 3 4 5 6 7 8 9 10] [...] ...]);

What I like about this is that it allows us to write the [] freely in expressions.

	if 1 2 3
		debug `won't work because 2 and 3 are taken to be a statement, which is syntactically
		flawed.`

	ptr 32u a; 32u len;
	a, len: length = [1 2 3 8 9+3 getSome()];
	[10; 32u] c;

So we don't allow cross-linking operators, which prevents the need for a comma. What
I mean is that we don't allow dual unary and binary operators. This allows us to
write lists without using commas. Something I find very pleasant. There's just this
clash between actual data declaration and type specification.

	array 100 array 10 MyType a b c d;

I like 'array'. It assumes that arrays are of the form: 'number type' This simplifies
the array type declaration significantly. What can also be done is to allow any number
of expressions to be under array:

	array 10 3 10 MyType a;

This allows you to define multidimensional arrays.

	array[10 30 10 ptr MyType]

The above looks too much like a grant. I'd rather avoid it. So suppose we use this
syntax for arrays. Then we can use [] anywhere else

	myCall(in=[1 2 3]);
	myCall(in, length: size=[1 2 3]);

Wonderful. Is this it? We have arrays, we have ::Type to avoid expr and statement
overlap. What else is there to do? The thing is though, var would prevent the need
for both of these.

	var [8 32u] a([1 2 3 4 5 6 7 8]);
	[1 2 3] + [3 4 5];
	array 8 32u a([1 2 3 4 5 6 7 8]);

We also need an automated cast from the array to an unnumbered ptr.

	ptr 32u a; 32u len;
	a, len: length = [18 19 !-32 !+39 !+get()];
	32u sum(0);
	for (32ue c(0); c < len; ++c)
		sum += a[c];

Is size inference a desirable thing? Perhaps not. Perhaps it is. Maybe we should
just use ptr if the size is indeterminate. The above syntax is actually pretty nice
isn't it? It's short and not that complicated. Contrary to previous endeavours. The
current paradigm is for simplicity. Oh, and for a case where we want to manually
call the ctor/dtor:

	ptr 1u a(new(::MyType~size));
	cast[ptr MyType](a)->construct();
	// ...
	cast[ptr MyType](a)->destruct();

This can be useful, when we create some kind of factory pattern. There is a collision
between the :: and the expression syntax though. Take for example:

	[::A::b() ::C::d()]

How do we know :: doesn't bind to the previous element? We could enfore is in the
grammar by making :: Type just like that, since type only contains :: internally,
it won't clash with other elements.

	32u a(2) b(1);
	32u c(::32u::+(this: $$a; right: $$b));

We could also say:

	@Type::CPTR;

But would that fall under @() or @(Type)::CTPR? I'd prefer the latter, but it mixes
badly with the former. In that case, @ needs a higher precedence than ::. Also,
. needs a higher precedence than @, which makes . a higher precedence than ::. Isn't
that odd?

	@MyType::A.b.c()
	(@MyType)::A.b.c()

No man I think @ will just be confusing to people. :: also nicely encloses types:

	::ptr const ptr array 100 32u::SIZE
	::namespace::Type[Control; name="Raven"; EDGE=23]::SIZE

I really like that one. It acts as a delimiter.

	debug ::String::getInfo();

I really like that one. Okay, so are we do I agree with myself on the array syntax?

	if ::array 1000 32u::SIZE == 32000
		debug "Test correct";

This means that arrays are created by writing []. Arrays are declared by writing
the keyword 'array'. How can we cast arrays to run-time objects?

	array 32u a( [1 2 3 exp expr() intef()~g] );
	array 6 32u a( [1 2 3 exp expr() intef()~g] );

What's the difference? The former stores the size of the array. a.length is possible
to use. The second type is a definitely-static array. It will always have the same
size. I guess we can allow partial array setting:

	array 10 32u a([1 2 3 4]);
	sml.out << a[4];  // Undefined behaviour, as a[4] is not set yet.

The array can't grow. We could make array an object. The thing is, it'd be best if
it were a class. The problem is we need a basic type for [] to use. I like the array
syntax, but maybe it's best to check out other languages.

	array 32u a([1 2 3]);  // Normal initializer
	array 32u a[1 2 3];  // Array-initializer
	MyType[] a = new MyType[123]();  // Java
	a = [1, 2, 3]  // Python
	a = [1 2 3];  // Matlab
	int a[] = {1, 2, 3};  // C++
	$a = array(1, 2, 3);  // PHP
	A = [1 2 3]  // Oz
	a = (1, 2, 3);  // PERL
	(make-array '(1 2 3))

I kinda like the [] start. It's shorter than array and easy to type []. array is
probably fine too. I suppose we can poll the type of a after creation to check the
size. The size is static so it should be fine. Can we pass an array into a method?
An array is a pointer to the start of a set of elements. It contains size data as
well (static, compile-time). We can't always know the length of the array given to
a method. So a method assumes the length zero.

	(: array 32u a) control {
		for (32ue i(0); i < ::type[a]::LENGTH; ++i) {
			sml.out << a[i];
		}
	}
	(:) enter {
		control([1 2 3]);
	}

Should print by using the size 0. An uninitialized array is just an empty array.
It's a pointer. array 32u = ptr 32u. What about signatures?

	(: 32u a(10) b c(0)) fun { ... }

	(:) enter {
		ptr (: 32u a b) a(fun);  // assumes argument c is automatic (zero).
	}

In this case, the function overload is not called, but the default argument is cast
away. Maybe default arguments should be a thing, and not overloading. I like that
idea. The cool thing about it is that static code analysis can remove code that isn't
called anyways.

	(: 1u a b c(true) d(false) e(true)) something {
		if a
			...;
		if c
			...;
	}

The nice thing is that the function `(: 1u a b)` casted from the above will remove
the if c clause completely and just run the code. What is the type of an overload
though? Well not an overload but more so standard params. This also makes the entry
point unambiguous. What about types though? Different type overloads would be necessary
I think... Man this is hard. I realize that methods like print may benefit from overloading,
but on closer inspection we can just write "print(mynumber.toString())". Since you
can only print strings. What about args of the same name?

	(: this; 32u in; 32f in) add {
		this.sum += cast[32u](in);
	}

I don't know how this is supposed to work. Code generators?

	#print(a);

Just gets turned into:

	print((a).toString);

Or basically:

	#print(a + b);

is

	sml.out << (a+b).toString();

I'm not sure if overloading is the way to go. C doesn't use it and that language
does pretty fine. Overloading can be extremely useful, suppose I have a class called
matrix and I want to either multiply by a vector or a constant. Both have different
semantics but it's still a multiplication. My main concern is that it'll be hard
to find the overloaded function, because you need to look at all the types of the
inputs. Maybe it brings more trouble than it solves. Hell you can still 'overload':

	(: 1u a() b() c(); Type d() e() f()) myFun {
		if d.is_default {
			// Do stuff without d, e, and f
		} else {
			// Do stuff with d, e, and f
		}
	}

	(:) enter {
		myFun(a=1; b=0; c=false);
		myFun(d=::Type(0.3); e=::Type(0); f=::Type(0.6));
	}

The nice thing is that static checking can remove the ifs, so no real value is lost.
This does not apply to operators though. I'd love to have operator overloading for
operators. Maybe operators should just keep to their own types. Where it makes sense.

	(Type out : const this; ref const Type in : const) + {
		32u partial_sum = this.computeTotal() + in.computeTotal();
		out.setCumulativeSum(partial_sum);
	}

I like the idea. Keeps the language clean. No need to abuse operators. Plus the method
names hint what's going on:

	datatype.addVector(vectorname);
	datatype.addSet(mys);
	// Alternatively
	datatype.add(set=mys);
	datatype.add(vector=myv);

I think this can work. I think I'm almost done with the entire language. Is there
anything else? I've worked through arrays, overloading, expressions, grants,....
I could inspect statements. As of now, the only statements are fexpr and datadecl.
The core of the statements have been added. It's actually really simple. ifgoto,
and labels. Mix in the FEXPRs and DATA declarations, and the language is complete.
Also added the scope. So you can have multiple consecutive statements. I'm wondering
if lambdas are really necessary. They're big and bloaty. They do stay confined to
a single method though. What I mean is that if they are defined therein, you know
that the lambda is used in that method.

	ref (64ue out(0) : array 64u in; 64u length) sum(lambda {
		for (64u i(0); i < cast[64u](length); ++i) {
			out += in[i];
		}
	});
	sml.out << sum(in, length: length=[1 2 9 8 4 3 6 7]).toString();
	// Prints "40"

Could be kinda useful. Maybe I should write a full grammar. But am I satisfied? Is
everything in-place? Am I missing anything? We've got construction, destruction,
purity, const-correctness, references, ptrs, const variables, arrays, type and size
of a type, signals, hacks, loop constructs, objects, default parameters. CONSTEXPRs,
no enums... We can add some conveniences to the language. While, for, static ifs.
Wait, what kind of statements does C++ have? Expressions, declarations, calls (part
of expressions), for/while/do, switch, return, break, continue, label.

I just noticed that I'm retarded. I haven't noticed this before. The right-recursion
for most of the expressions are actually forcing right-associativity. This is annoying.
Luckily, it's nothing that tree-reordering can't solve! The algorithm for reordering
summing negatives is as follows:

	1) Make the end of the summation chain the root.
	2) Make all right children the parent
	3) Make all right children's right node the current right node

	-
		1
		-
			2
			3

Is a tree for 1 - 2 - 3, it must become

	-
		3
		-
			1
			2

Likewise for 1-2-3-4-5+6-7+9-1

	-
	1 -
	  2 -
	    3 -
			  4 +
				  5 -
					  6 +
							7 -
								9 1

It's annoying that this happened. Oh well. I almost feel like going full LISP-style
again.

	a, b: c = d();
	a += 34;
	b.something(a: b);
	a.+(b).*(3)

This would get parsed very easily, because exprs would be reduced to very simple

	memberexpr = member [ '.' memberexpr ]

It's just not practical. So how can left-associativity be handled? It can be observed
that the tree can be evaluated top-to-bottom wise. This will create a stack. However,
it doesn't represent the correct semantics. Is it possible to find the correct tree
after parsing with an LL(1) parser?

	*[+[1 2 3 -[8]] 100]  // -2 * 100

	(:) enter {
		32u a(+[100 1 2 3 4 5]);
		a = +(1 2);
		a +=
	}

What if we only allow compound operators?

	(:) enter {
		a, b = f();
		b = !-100;
		b += g();
		a -= b;
		// a, b = f();
		// a -= !-100 + g();
	}

That's actually really nasty. Maybe longer variable names are more useful. It also
muddies the distinction between '.' expressions and other expressions.

	(:) enter {
		32u sum(0);
		sum += f(10);
		sum += ((1-3)+2);
		sum += 1.-(3).+(2)
	}

I feel lost. Maybe using them as members is the best thing. Associativity will be
rather destroyed though. A right child of the same order, that child must become
the parent. The child's left child, will become the new child's right child. Tested
it out for a bit. Seems to work... Maybe expressions are safe. I think they are.
I just require post-processing of the parse tree. Well that was easy. What's left?
Maybe there's a collision with lists and actual expressions. You see:

	[a [b]];  // Clash of a[b] with a [b]
	[[1] [2]];

Maybe it's not severe, but imagine a function returning an array, and suddenly you
get `[a() [1 2]]`. That should fail catastrophically. The parser doesn't take into
account that this may happen. How is this solved? The same goes for function calls
`[a() (b+c)]`. What is to say a doesn't return a function? Then b+c would be an argument
into that returned function. This is obviously a problem. While I like space delimited
arrays, maybe it's time to bring out the comma again. [1,3,5,7,3,6]. Maybe it's just
fine. `[a(),(3+5),19]`. I like this. How can array access be made recursive?

	AR ::= RES AR
	RES ::= name
	AR ::= [ '[' EXPR ']' AR ]

	>-
	a <= b;
	a = a < b;
	a leq b;
	a geq b;

Just messing around.. How do I want to code?

	(1u out intersects : const ptr Polygon left right; ptr Axis axis : const pure) findCollision {
		Vector mainvector(computeSupport(left, axis: axis) -
			computeSupport(right, axis: -axis));
		Simplex simplex($mainvector);
		Vector inverse(-mainvector);
		while true {
			mainvector = computeSupport(left, axis: inverse) -
				computeSupport(right, axis: -inverse);
			if dot(left: mainvector, right: inverse) < 0
				intersects = false;
			simplex.unionAdd(mainvector);
			simplex, inverse: inverse, out: origin = nearestSimplex(simplex);
			if out
				break;
		}
	}

I almost feel like expressions should just be removed. It's just something that can
be mistaken by humans. In addition, it's a whole lot of complexity for the entire
language added. At least the base of the language seems pretty solid. Not having
space delimited arrays makes the idea of having space-delimited type declarations
a little more foreign. `[1 2+4/6 3**4/2.control() 932]`. So much could be simplified
by using LISP-expressions.

	32u result(+ (obj (fun name:1 control:2 nothing:3)) 3);
	(= result 100);
	[10 ptr Type] a(new(10));
	result, a: checkpoint = (f name: "control" world: 15);
	hack("C++: std::cout << \"Hello er\"");
	if (< (abs (- x (* guess guess))) tol) {

	}

Expressions do become super simple then. Maybe only make expressions lisp-styled.
Hmm... That's interesting. Making only expressions lisp-styled kinda fits neatly
in the language now. Let's experiment a little more.

	(128ue out : 128ue in) factorial {
		out = in;
		--in;
		while (> in 1)
			out *= in;
	}

	(32f out : 32f in) qrsqrt {
		64s i;
		32f x2 y;
		const 32f treehalfs(1.5);
		x2 = (* in 0.5);
		y = in;
		i = (@ cast[ptr 64s]($y));
		i = (- 0x5f3759df (>> i 1));
		y = (@ cast[ptr 32f]($ i));
		y = (* y (- threehalfs (* x2 y y)));
		out = y;
	}

	(:) enter {
		This obj;
		(<< (sml.out) (obj.factorial in: 10)))
		sml.out << obj.factorial(10);
		(sml.out.<< "cool");
		(sml.out.<< (qrsqrt 12));
	}

There's something otherworldly about this. It's somewhat longer and more tedious.
But how else can precedence be written like this? The above code simply can't be
misinterpreted. The () exprs also fit nicely with the idea of constructors. The grammar
becomes `DATA ::= TYPE { name [ EXPR ] }+ ';'`. The expr simply is the constructor.
I'm not sure... It looks kinda neat. It also destroys all possible ambiguity. I like
that. So we keep all statements. It's just that expressions themselves are s-expressions.
So how can arrays be declared or indexed?

	(:) enter {
		[10 32u] a([] 1 2 3 4 5 6 7 8 9);
		(a.[] 9);
		(a.[] 3) = 100;
		(createWindow name: "My Window" xsize: 300 ysize: 200
			x: (/ view.xsize 2) y: (/ view.ysize 2));
	}

I like it. It does mix the dot operator with a lot of the other operators though.
Not a fan of that. We could say that all operators except the '.' operator are actual
operators. This should also count for the builtin $$, $, and @ operator. There is
precedence if we have `@a.b`. What is evaluated first? @a or a.b? For this, we need
to make one of them a function: (@ a.b). That looks way better. What about the dot
operator though? (. a b) ? That just doesn't look nice nor practical.

	(:) enter {
		32u a(> 3 1);
		a = (~ (factorial number: (+ 1 5)) derp);
		Type b(name: "five");
		Type c(Type::constructor in: 10 control: 0);
		Type[CONTROL: (+ 1 2)] d;
		signal OUT_OF_PROGRAM;
	}

I like the idea. It makes the LL(1) language complete. There is no further need for
tree post-processing either. Hell, it's not even necessary to prune that much. Expressions
used to be highly prunable due to precedence. Now it's a cakewalk.

	(:) enter {
		ptr (100ue out : 100ue in) factorial(lambda { out = when (== n 0) 1 else (factorial (- n 1))});
	}

How does it mix with the when expression? (((a == b) + 3) * 5240). Another possiblity
is to simply allow infix expressions, but not expand on their precedence without
any parentheses.

	(:) enter {
		32u a(1 + ((3 * 9).size(mult: 10)) );
		ptr (100ue out : 100ue in) factorial(lambda {
			out = when (n == 0) 1 else factorial(n - 1)});
	}

The grammar for such a syntax would simply be `EXPR ::= ATOM [ op EXPR ]`. This would
be right-associative, but () can group inside ATOM. Very powerful and simple.

	Type name name2(arg);
	Type2 name3(myarg; control=10) name4 name5;
	Type3[MyTemplate] name6;
	Type4[get=myfunction] name7;
	Type5[MyClass=OtherClass; get=myfunction] name8;

	(:) myfunction {
		32u a(name.getSomething()) b c;
		a, b: special, c = getMore();
		a += 3;
		ptr 32u d([1,2,8,f(),10]);
		label x; {
			array 10 32u e;
		}
		goto x 10 < 3;
	}

It's tempting to just let operators be syntactic sugar for right-to-left evaluated
`a.+(b).*(c)`. Or should that be done from left-to-right? It's also tempting to just
allow operators, and fix the tree.
An idea I've been entertaining is coming from my original implementation of Cxy.
It's the simple 'operation operands' statement list. Very minimalistic and requires
basically no parser. I'm thinking that that idea may come back again, as a bootstrapping
mechanism. Here's an outline

	// Core declaration/operations
	number name  // Declare a new stack-variable. Removed upon exiting scope.
	operation left right  // Any binary operation, mathematical, or otherwise. Returns to 'ans'
	// Flow control
	label name  // Labels this location, to jump/call
	goto name cond  // Jump to the label 'name' if the condition is not zero
	call name  // Call the label, returns here.
	// Dynamic memory
	new number  // Allocate number bits on the heap.
	del ref  // Delete the allocated bits
	// IO
	read  // Read 1 bit from the standard input.
	write name  // Write the bits of 'name' to the standard output.

This allows you to create a recursive factorial function:

	label factorial
	113 myvar
	= myvar ans
	-- ans
	call factorial
	* ans myvar
	return

113 myvar allocates 113 bits on the stack. It then copies 'ans' (a global) into the
variable. It decrements ans, and calls itself recursively. The result of the recursion
is multiplied by myvar. This result is already stored in 'ans'. The function returns.
Simple and bulletproof. The language can be made so that overflows fail the program
with a very clear message. m4 can be used to place other files in there. Why create
this? Well, maybe it's useful. It'll be a fun little experiment. Hell, even object-
oriented programming is possible. The idea is to let the programmer decide how many
bits a data structure uses. He then performs operations on that data structure as
he wishes. This gives a huge amount of power to the programmer. First, pointer and
reference semantics have to be defined. There is also the case of special optimizations
like vector mathematics. If we intend to perform that operation, the compiler should
be able to optimize it.

I think the language is in its final stage of preprocessing. I don't know what more
to improve. I can isolate specific algorithms. Especially the reordering one. That
will be a fun task.

Is the grammar finished? Is everything that has been desired implemented? Expressions
make up the largest part of the grammar in terms of sentences. I need to make ARG
and array access recursive. The difficulty lies in that. The use of [] and () are
in themselves left-recursive, so should we view [ and ( as an operator in that context?

	CALL ::= UNDER_CALL [ '(' ARGS ')' CALL ]

This could work. The '(' is merely regarded as an operator just like all the others.
This gives us

	CALL_EXPR ::= ACC_EXPR { ARG } ;
	ACC_EXPR ::= RES_EXPR { LIST } ;

Will this work? Imagine the code:

	a[4]()[b];
	((a[4])())[b];
	(((a(1))[2])[3])

The parentheses denote parse tree. Oh. I also resolved the problem with !! and !.
Now 'not' is uesd instead. likewise, 'xor' has been added. The words are operators
used for booleans. They can be created for other types too though. Ugh I'm still
pondering operator precedence. I really like to have it, but it requires post-processing.
It's the post-processing that I don't like. I wish the grammar to be easily semantically
correct. What about simply putting all operators on the same level?

	a + b * d;

Due to right-recursion, the right-most elements will be computed first.

	not a and b;

would be wrong, since not (a and b) would occur. I suppose if we want, we can simply
do:

	not(a) and (b);

Let's just expand on the idea:

	10 + 3 * 8 - 4 . max;

4 is an integer of size 2s (standard type is signed). Then, the first operation is
.max. This results in '7'.

	10 + 3 * 8 - 7
	10 + 3 * 1
	10 + 3
	13

This would be simple to implement, and also very friendly. Commoners do not remember
operator precedence. Professional programmers may remember them. For the longest
time I have not. People I have worked with have not. Maybe it's for the best. This
removes ambiguity and possible misinterpretations. How are method parameters affected?
So far only the unary operators take precedence over everything else. That does shorten
the grammar significantly.

	3 + @myptr() / 4;

@ binds more strongly, and () binds even stronger. This means that @ will dereference
the return value out from the myptr() call. It shall be as such. From lowest to highest
precedence:

	Binary operators
	Unary operators
	Type prefixing
	member/extraction
	argument/array access

Since everything is right-recursive anyways, there should be no problem with this
precedence.

	32u a;
	a = @val[3] + 4;

Where 'val' is of the type `::array 4 ptr 32u::`. Still, lists are not space-delimited.
The nice thing about all this is that we can still implement operator precedence
(as in, * before +) post-production, independently of the grammar. Overloading unary
operators with binary operators is still overloading of methods. This is something
that was not desired. Should a special function name be created? What about the address
of an address? Well an address is itself just an rvalue. It has no address except
some random value on the stack. It therefore makes no sense. I think I've gone over
this before. Anyways, let's examine the grammar. What about compound assignments?

	if ((a >= b) && c())
		doSomething();
	a += 3;

Operators are just methods. Wait, what if names are also operators? What if names
can take left and right arguments as well?

	Matrix a(inverse=true; eye=true;)
		b(eye=true) c(100);
	(3 - 1 * a matmul c())
	if (a contains 103)
		.sml.out << "a contains an invalid number";

I'm toying with some interesting stuff here. Allowing one to create custom operators
is incredibly exciting. Instead of being scala and making operators >>=<< weird symbols,
the operators are simply words.

	3 mul 8;
	8 mod 3;

Without precedence anyways, this should be fine. Operators are simply methods. There
is however one problem. It's pretty useful to have overloading. Without overloading,
the operators will be a mess. You mean I will have to define two multiplications?
One for scalar and the other for matrices.

	Matrix a(eye=true);
	32f b(3102.0);

	a * b;  // Ok, overload for 32f
	b * a;  // Err, no overload for floats
	b * b;  // Ok, matrix multiplication with self.

One part of me tells me that it should be fine to define other operators. How can
the above be resolved?

	a scalmul b;
	b * b;

It does solve the specificity of operators. I kinda like that. The question is: should
operators be type-overloadable? Let's examine.

	Mat4 quat(rot=45.0; [1.0,1.0,1.0]);
	Vec3 pt([2.0,3.4,5.0]);
	Mat4 newmat(quat vecmul pt);

the case for overloading appears strong. It seems just so useful... I'd think that
the specific names convey information that is actually important. I really like the
idea of custom operators. It opens up a whole new avenue of creativity. Without overloading,
we need to be specific in our method names.

	::Math::colmax(b matscal 1.3 + a matscal 10.0);

In the above cases, only binary operators are eligible for operator use. What about
unary and even ternary operators? I suppose we need to create a method taking zero
arguments:

	8.neg() + 3;

Which fits quite naturally. I just dislike that method names can not be used for
unary operators. This limits quite a lot. It's always the expressions getting in
the way. It's quite annoying. What about this method of expressions though? Binary,
named expressions. It sounds cool. `@getMat().matmul(@getOtherMat());` Why don't
I just roll with that? I need something simple to implement. Maybe create a lexer
instead. Not sure anymore.

	op(neg) alpha op(plus) beta op(times) gamma;

Everything except expressions are in place. What if operators are just kept like
they are? What if we do not allow  defining named operators? Only symbolic operators.
Any symbolic operators? No, we already concluded that was bad. Named operators seem
really interesting though. It's basically such that an operator takes the entire
right side as an argument, whilst a call nests.

	a.plus(b.plus(c.plus(d)));
	a op(plus) b op(plus) c op(plus) d;

That's actually seemingly useful. Maybe 'op' can be used... That may be a solution.
When do you actually use these operators? Rarely I reckon.

	a op(matmul) b * 100.0;

Maybe do as python does, by not naming methods the direct symbols, but instead __eq__.
This avoids any confusion... Maybe 'op - ' is better.

	@myValue() op-matmul secondMatrix();

I like it. This also allows the use of:

	op-square @myMatrix(start=1; increment=3; eye=true) op-matmul beta_matrix;

That is in fact incredibly clear. An identity matrix, created by a pointer, returned
squared, and matrix multiplied with `beta_matrix`. Yeah I like this. The language
is extremely simple now. It's easy to discern operators. Operations have to be static
as well. We can't allow expressions inside the op-name signature. It is merely an
alias:

	a op-matmul b;
	a.matmul(b);

Now that that is resolved: how do we declare a new unary - method? -- could do the
trick, but it's already used for iteration.

	inc a;
	dec b;
	--4 + 2

We can instead make inc and dec the incrementors and decrementors. This resolves
any potential ambiguity. This makes -- and ++ the respective negation and positivation?
operators. I like the idea.

	op-dec a;
	op-inc c;

Alright. That's now solved. Let's look at some sample code:

	(:) enter {
		sml::String string;
		sml.in op-read string;
		sml.out op-print factorial(string.to32u()).toString() + " is the number";
	}

	(113ue out : 5ue in) factorial {
		out = ::5ue::(in);
		dec in;
		while in > 1
			out *= in;
	}

! is the binary not, !! is the boolean not. I really like the possibility of coding

	if myset op-has 2
		debug "Op contains two!";

It really allows the programmer to write beautifully expressive and clear code. That
is something to strive towards. Now, unary operators are not explicit, so we still
need the comma in the list. I wish to remove the comma. Maybe have something like
'uop' or 'up'? This will 'unify' (not exactly, but the word seems nice here) lists
with the data declaration statement, as it requires no commas either.

	up-inv $getMyMatrix() op-matmul other_matrix;

This seems fitting, as unary operators are supposed to bind more strongly. They have
higher precedence. Should $$ be changed? I like the consty stuff of it, but it looks
a lot like !!, --, and ++. What I don't like is that the double characters are both
in the binary and unary operators. What about the semantic requirement for operators?

I would like to think it automatically takes a const-reference to the argument. That
seems very useful, because we don't want to write:

	sml op-print $a - $b;

We want to write:

	sml op-print a-b;
	sml.print($a.-($b));
	sml op-print a.--();
	sml op-print --a;

Let ',' not be an operator. That would be incredibly annoying. So should we just
remember the types of operators? What about smart pointers? C++ allows redefining
'->' and * (dereference) that allows you to create a class that acts as a smart pointer.

	std::unique_ptr<int> a(new int(0));
	*a = 4;
	sml::Sptr[32u] a(new[32u]{0});
	// @a = 4;  // Not allowed
	up-dref a = 4;

Instead of using @ we can define up-dref to dereference. If @ is not allowed to be
overridden, then we have to define our own dereferencing method. I think that may
be the best.

	array 32u a([1 --4 32 un-square 43]);

Maybe we should just be satisfied with remembering the different operators. Maybe
that's the way. Perhaps it's fine.

	private 32u a b c;

	(:) enter {  // CLASS
		This my_object;
		my_object.a = 1;
		my_object.b = 2;
		my_object.c = 3;
		debug my_object.a + 1;
		debug my_object.c + 1;
		sml.out.print(my_object.a.toString());
		Matrix a(type=::Matrix::TRIANGULAR; grow=2) b;
		b = ::Matrix::eye();
		a = a op-matmul b;
		sml.out op-print a.toString();
	}

Unary operators. Should they all be of a similar 'kind' to remember them correctly?
Hell, we don't even need boolean or, and, and xor, since using 1u will work the same.

	32u a(100);
	1u b(a > 50);
	if b & 1
		debug "hey!";

Since no lists are allowed, we could just say that ifs do not require a () around
the expression. The problem with that is that we still have potential binary operators..
maybe.. let's try it out:

	if (a + b - c.getSomething op-matmul d) > 320
		debug --my_value + 3;

Wouldn't it be nice to have a method that can take in expressions and arbitrarily
call the toString method? Perhaps that's what # can be used for:

	#sml::print(a + b);

It will just call toString around the expression. What should be done,.. I think,
is to copy the arguments verbatim as a string.

I just need to solidify the whole idea. Especially now that everything is starting
to feel like one language.

	Type name name2(args) name3(otherargs; my=23);
	(Atype a(args) b; Btype c : Ctype d; Etype e f) g {
		// Recursively calling self
		g(a=a; b=b; c=c;);  // recursion
		// Assigning to the output parameters
		d = ::Ctype::();
		e = ::EType::();
		f = e;

		// Add a small, artificial delay
		util::Counter alpha(0);
		while alpha.value < 100
			un-inc alpha;
	}

The part I don't like is the `::Ctype::()` part, it looks like something that should
not be. On one hand, I'd love if it were a cast. On the other, this could be a constructor
call. Either is great. How will that be conveyed in the grammar? Well, there is only
one constructor. It must automatically return a value that is `This` itself. That
means that we can simply write:

	a = ::MyType::constructor();
	// I prefer
	a = ::MyType::();

So how is this done? We can add a simple grammar rule. Either an ARG or `MEM_EXPR`.
Unfortunately, this creates a collision of lookaheads with `MEM_EXPR`... So how is
that resolved?

Why not keep the original grammar? We can detect if the right child of `PAC_EXPR`
is `(` or not. How elegant. So be it. `::Type::( ... )` is the object creation expression.
I like it. We already have 'cast' anyways, and casts are more rare than creations.

	32u a;
	a = cast[32u](::4u::(10));
	a = 10;  // Also works.

Works perfectly with the grammar, as `::Type::().something();` is perfectly fine.
This makes `->` not that useful in this case. That is invalid, unless you take the
pointer of it, which is invalid because it's an rvalue.
Another interesting aspect is allowing expressions inside calls. That's rather odd.
What I mean is that the following is perfectly legal Cxy

	(:) enter {
		32u b(10);
		:: somewhere::There ::method(a[1] = b);
	}

That's rather odd,... we can assign to an array? Well, maybe it's okay, since we
can idealize the whole ordeal as if we put the arguments right at the top of the
function. Maybe that's okay. However, those assignments can only use locally defined
parameters, and nothing more.

	(:) enter {
		game::Game[Physics=lib::Physics] instance;
		instance.enter();
	}

	// game/Game.cxy:
	private util::FrameLimit frames(60);
	private 32u state(START_STATE);

	(: : pure) constructor { }
	(: : pure) enter {
		while true {
			handleInput();
			doPhysics();
			draw();
			frames.limit();
	}

	(:) doPhysics {
		switch (state) {
			START_STATE {
				...
			}
			default signal UNKNOWN_GAME_STATE;
		}
	}

We kinda need some strongly typed enumerations. That would make the above easier.
Maybe `Enum` as a type that is always local... `enum[This]` seems good enough.

	enum[This] game_state(START_STATE);

Is this it? Is it over? I feel content. I see no further issues. Perhaps only the
associativity of the operators:

	a * b;

What if b is the matrix and a is the scalar? a has no member called * that takes
in a matrix.

	a ro-* b;

Where ro stands for 'Right Operator', so the right element is the 'this' value. Whilst
the left is the 'in' value. I think that this overcomplicates things unnecessarily.
Instead, we can just reorganize a bit and keep things simple. I think that's a much
better solution!

I just need to internalize the current core language. I think this tree will be easy
to translate into C or C++. I'll just need to polish the type system and everything
will be complete.

	(:) enter {
		sml op-print "Hello world!";
		sml::String my_string("My string") your_string("Your string");
		sml op-print my_string + your_string;

		Matrix my_matrix(rows=10; columns=10)
			eye(identity=true; size=10)
			other_matrix(my_matrix op-matmul eye);

		32u value(10);
		64ue other_value(value + 10 >> 2);

		// Pointer manipulation
		ptr 64ue value_ptr($other_value);
		@value_ptr = 10;

		sml op-print value->toString();
		sml op-print (@value).toString();
		sml op-print other_value;
	}

I'm rather satisfied. I see no problems. What about a standard value for granted
classes? I think we should avoid that. Instead, you can create a non-pure glue part
that will glue the parts together. Then you can override if you choose so... the
problem with that is that we can't override. We need information on how to override.
Why is this useful? Say we're given a math library. It implements the sin function.
What if our processor has a sin function?

	hack("cpu=sin %s0");  // Likely ARM processors, s0 is a register.

Should static ifs be used? Then purity will be broken. Only overriding using....
wait..

	(32f out : 32f in) sqrt {
		static if ! cpu_has_sin {
			32s casted(cast[32s](in));
			casted = (1 << 29) + (casted >> 1) - (1 << 22);
			out = cast[32f](casted);
			out = out + in / out;
			out = (0.25 * out) + in / out;
		} else {
			hack("sqrt");
		}
	}

Maybe the ones implementing it should statically if and hack what they're doing.
That avoids the need for overriding grants. What about new APIs that know an efficient
sqrt implementation exists, but wish to implement it themselves as a placeholder?

	(:) enter {
		x::Maths[Sqrt=my::Sqrt] viable;  // This is annoying!
		x::Maths var;  // The is nice, we can override Sqrt later
	}

It would be very useful if x::Maths could just implement its own sqrt module so that
we can focus on optimizations later.

	Sqrt(internal::Sqrt);
	// methods...

Seems like a nice syntax. It's simple and fits well with the current syntax of the
language. But it's one more rule. I don't like that. Is the rule ambiguous?

	CLASS ::= { [ ACCESS ] ( DATA | METHOD ) | cname '(' TYPE ')' } ;

The new part is `| cname '(' TYPE ')'`. I think this is rather elegant. It does not
clash with DATA because the lexer can use a lookahead preprocessor that looks for
`cname '('`. Heh, that makes our whole parsing solution basically LL(2) or something.

	Sqrt(int::Sqrt);
	Exp(int::Exp);

	(32f out : 32f in) exp {
		out = ::Exp::exp(in);
	}

What other parts of the grammar are LL(2)? Type declarations are! name:: could however
be considered a 'namespace accessor'. This avoids the LL(2) problem. What about the
grants?

	Sqrt=internal::Sqrt;  // ?

That doesn't look like an initialization, but rather an assignment. Standard params
are supposed to be initialized and not assigned, as seen in methods... So how is
this solved without using LL(2)? Do we say that `cname '('` is a single token? That
could solve it. When do we see that at all? Let's see... `::Type::()` is a constructor
call anyways. That means we have an LL(1) grammar with a smart tokenization algorithm.
Well, not 'smart' per se. More so... useful only for this language.

C++14 has cool stuff like '[[deprecated]]'. Should this be added to the language?
I think most problems can be solved via statements. One statement I see viable is
the 'warning' statement. It is used inside a method. Whenever the method is called
from anywhere, a warning is invoked from the callee's line where it calls the warning
method.
'error' can also be a statement, failing compilation. This is useful in partial compilation
(like static if).

I absolutely abhor -- for the unary minus sign. It's ugly. It disgusts me. I like
'neg' better.

	[neg 3 neg 5+3 neg 8]
	[--3 --5+3 --8]

The problem with 'neg' is that it's bigger because it requires a space. `--` also
displays the space more clearly. Is there anything that can replace the current unary
minus? \? No that's division... right?

	[\3 \5+3 \8]

It's the shortest version. Do I really need \ as another division?

	a, rem: remainder = 3.14 \ 1.2;  // This could be useful... right?

I like the idea of being able to do floating point division in this manner. Wait.
This is already encoded in the floating point.

	a = 3.14 / 1.2;
	b = a - un-floor(a);  // () is an EXPR, so it works just the same.
	a -= b;

Now, a is the whole number, and b is the fraction. Problem solved. Let's use \ as
the negation operator! Woohoo! Awesome. What else is there to do? Oh yes, fix the
lexer. But, am I satisfied with the language? Let's see. Removed the filthy --, oh,
probably need to reimplement iterator statements with ++ and --. Or just turn them
into operators anyway... Operators can have side effects, so hmm...

	Type a;
	f(\a; x=a);

What are the semantics if \ actually modifies a? Do operators always need to take
in const-references? Is there a case where we need to give them non-const references?
With all the operators currently in use, I see no use for taking in non-const references.
So... Hmmm. Wait! Operators have defined... no. Order doesn't count in arguments.
We could say arguments follow their logical direction. This is a bug source though.
A developer changes the order and boom, a bug is introduced. Let's avoid that. Instead,
we can do... Hmm... No we should allow changes. The reason is that arbitrary methods
can also invoke changes and return something else.

	f(a.pop(); a);

Ouch! What can be done? Making methods take variables is too drastic a change. That
implies a simpler grammar though... The problem is; people really really like not
having to name things. It also avoids bugs to be somewhat declarative. Especially
with the for as given in python or java/c++ `for (auto it : expr)`. So I just can't
allow removing expressions from method invocations. Even if I did, we'd need to do
the following before each call:

	32u parama;
	parama = 1 + 3;
	f(parama);

The problem with that is that it destroys the original idea of having named parameters.
Niceties with forcing precalculation is that we avoid any ambiguous order of changes.
Since expressions are always in a predefined order, and so are statements, we avoid
the whole problem. I guess I'll have to compromise and say that the statements as
listed in the method invocation is how the operators are run. That seems fine.

I'm still a little annoyed by the type of arrays. Arrays are of a compile-time constant
size. Yet we induce their sizes from constexpr arrays. But we also pass this type
information on. But array size can't change... Hmm. That's interesting. Why not make
the entire array size implicit? This avoids the ugly syntax too. What about methods
using arrays to iterate? Well, they can use the provided size. This avoids a lot
of trouble.

	array 32u a([1 \2 3+3 4 \5]);

This seems fine. It avoids the lookahead with `[` in expressions. This is nice. Let
the compiler instead infer what the size is. This used in conjunction with methods.
I feel like I need to define a lexer as well. But am I satisfied with the grammar?
operators can take in non-const references. Grants can be partial classes. Seems
like everything is fine. The grammar is pretty short too. Anything else? Well, the
current grammar needs a big change. It's good for now. A few new sequence recognizers
need to be added. One for 'cname' '(' in sequence. Another for... Hmm
Maybe '::' '(' for the constructor call. What about,.. instead of using 'array',
we use '::' '[' instead for expressions containing arrays? Or is that too ugly? Maybe...

	[[32u]] a(::[::[1*factorial(3) 2 3] ::[4 5 6*f()] ::[7 \8 9-1]]);
	array array 32u a([[1*factorial(3) 2 3] [4 5 6*f()] [7 \8 9-1]]);
	ar ar 32u a([[1*factorial(3) 2 3] [4 5 6*f()] [7 \8 9-1]]);

Interestingly enough, this example gives us the exact same size in line length. Is
either desired over the other? I dislike the ::. It does not look clean. In addition,
the trailing ]] on the type isn't very appealing. So should I stick with 'array'.
It probably is the easiest, isn't it? It's clear enough too. Should be anyways. Hmm...
I wonder if the grammar is done now... For arrays we just imagine as if the size
is implicitly passed. Oh, arrays of a large size that are not pre-initialized, but
instead initialized in the runtime. However, the size remains fixed. What about those?

	array[100, 100] 32u my_array;

seems like a nice notation. A comma-separated list denoting the dimensions of the
array. The same would go for any 'grant', although this being a pseudo-grant. A method
specifying what grant it wants, will only accept classes that have been granted the
exact class:

	(: Class[Aa=Xx] a) method {
		...
	}

	(:) enter {
		Class[Aa=Xx] b;
		method(a=b);
		Class c;
		method(a=c);  // Illegal.
	}

This allows a wide variety of specialization,.. well, using static-ifs that is. Let's
translate StringFilter from the compiler to Cxy:

	array[2] 8u outstream;
	8u can_fetch;
	enum[This] previous(NOTHING_STATE) state(NOTHING_STATE);

	(: : ctor) constructor {}

	(8u out(0) : 8u in : pure) hexToBits
	{
		switch (hex)
		{
			case '0': out |= 0b0000; break;
			case '1': out |= 0b0001; break;
			case '2': out |= 0b0010; break;
			case '3': out |= 0b0011; break;
			case '4': out |= 0b0100; break;
			case '5': out |= 0b0101; break;
			case '6': out |= 0b0110; break;
			case '7': out |= 0b0111; break;
			case '8': out |= 0b1000; break;
			case '9': out |= 0b1001; break;
			case 'A': out |= 0b1010; break;
			case 'B': out |= 0b1011; break;
			case 'C': out |= 0b1100; break;
			case 'D': out |= 0b1101; break;
			case 'E': out |= 0b1110; break;
			case 'F': out |= 0b1111; break;
			default: raise INVALID_CODE_POINT; break;
		}
	}


	(: this; 8u in) push
	{
		switch (state)
		{
			case NOTHING_STATE:
				switch (character)
				{
					case '\'':
						state = SINGLE_STATE;
						outstream[0] = '`'; can_fetch = 1;
					break;
					case '"':
						state = MULTIPLE_STATE;
						outstream[0] = '`'; can_fetch = 1;
					break;
					case '`':
						state = VERBATIM_STATE;
						outstream[0] = '`'; can_fetch = 1;
					break;
					default: outstream[0] = character; can_fetch = 1; break;
				}
			break;
			case SINGLE_STATE:
				switch (character)
				{
					case '\'':
						state = NOTHING_STATE;
						outstream[0] = '`'; can_fetch = 1;
					break;
					case '`':
						can_fetch = 2;
						outstream[0] = '`';
						outstream[1] = '`';
					break;
					case '\\':
						previous = state;
						state = ESCAPE;
					break;
					default: outstream[0] = character; can_fetch = 1; break;
				}
			break;
			case MULTIPLE_STATE:
				switch (character)
				{
					case '"':
						state = NOTHING_STATE;
						outstream[0] = '`'; can_fetch = 1;
					break;
					case '`':
						can_fetch = 2;
						outstream[0] = '`';
						outstream[1] = '`';
					break;
					case '\\':
						previous = state;
						state = ESCAPE;
					break;
					default: outstream[0] = character; can_fetch = 1; break;
				}
			break;
			case VERBATIM_STATE:
				switch (character)
				{
					case '`':
						state = ESCAPE_VERBATIM;
					break;
					default: outstream[0] = character; can_fetch = 1; break;
				}
			break;
			case ESCAPE_VERBATIM:
				switch (character)
				{
					case '`':
						state = VERBATIM_STATE;
						can_fetch = 2;
						outstream[0] = '`';
						outstream[1] = '`';
						break;
					default:
						state = NOTHING_STATE;
						can_fetch = 2;
						outstream[0] = character;
						outstream[1] = '`';
					break;
				}
			break;
			case ESCAPE_STATE:
				switch (character)
				{
					case 'a': state = previous; can_fetch = 1; outstream[0] = '\a'; break;
					case 'b': state = previous; can_fetch = 1; outstream[0] = '\b'; break;
					case 'c': state = CODE; break;
					case 'f': state = previous; can_fetch = 1; outstream[0] = '\f'; break;
					case 'n': state = previous; can_fetch = 1; outstream[0] = '\n'; break;
					case 'r': state = previous; can_fetch = 1; outstream[0] = '\r'; break;
					case 't': state = previous; can_fetch = 1; outstream[0] = '\t'; break;
					case 'v': state = previous; can_fetch = 1; outstream[0] = '\v'; break;
					case '"': state = previous; can_fetch = 1; outstream[0] = '"'; break;
					case '\'': state = previous; can_fetch = 1; outstream[0] = '\''; break;
					default: raise CAN_NOT_ESCAPE_THIS; break;
				}
			break;
			case CODE_FIRST_STATE:
				outstream[0] = 0;
				outstream[0] |= hexToBits(character) << 4;
				state = CODE2;
			break;
			case CODE_SECOND_STATE:
				outstream[0] |= hexToBits(character);
				if (outstream[0] == '`')
				{
					outstream[1] = '`';
					can_fetch = 2;
				}
				else
					can_fetch = 1;
				state = previous;
			break;
		}
	}

	(1u out : this : const pure) available
	{
		out = can_fetch > 0;
	}

	(8u out : this : pure) pop
	{
		--can_fetch;
		out = outstream[can_fetch];
	}

	(: this) end
	{
		switch (state)
		{
			case ESCAPE_VERBATIM:
				outstream[0] = '`';
				can_fetch = 1;
			default: break;
		}
	}

Seems like it expresses just fine. I see no problem with this. The include of the
string can be modelled as `String(something::String)` if it is to be granted. This
makes me thing 'why not make everything like this?'. What I mean is that we disallow
direct type usage of the form `type::Name`, and just make it a 'grant'. Then, by
using private or public, outsiders can override the types. This solves a whole class
of trouble. Then again, it's rather annoying to having to name something again. I
think I'll just keep it as-is. Seems like there about 27 keywords in the language.
Everything is in place it seems. The lookaheads do not clash. Now it's time to make
the lexer compliant to the grammar. The question is: is there absolutely nothing
that can be changed in the grammar that makes it better? Hold up. We don't actually
have assignment in... wait we do. Ok good. I was thinking that ARG didn't actually
assign to a parameter, but that information is encoded in the FEXPR... Let's clean
up types instead.

	const (ref ptr (some::Thing)) something;
	_ (_ ptr ((:))) function;

`PAC_EXPR` currently accepts `::Aa::(1 + 3)`, which doesn't make semantic sense.
The idea is to make this a constructor, but I imagine it may be better to remove
the requirement for the trailing '::' altogether. Maybe it needs to be put lower
in the chain, near `RES_EXPR`? This new system appears to be incorrect at first,
but semantics lurks in the shadows...

	::(:)::(lambda(:){})();

The trailing () is a valid executor of this lambda. So the grammar is correct. Semantics
make a similar statement incorrect.

	::Aa::()();

So this seems correct. It also avoids the expr-after-::-problem. This is now resolved.
Do the other semantics still work correctly? It seems so. Cool. Now to clean up any
small errors.

Lambdas will force the use of {} since we can then discern () exprs from ( signatures.
cast requires the type inside brackets, and the expr explicitly inside (), because
its precedence is pretty ambiguous. So that's complete. Changed the grammar for granting
classes slightly. `'Class' '='` is parsed as a single token called 'cnameeq'. It
stands for "class name equals". This expression occurs nowhere else in the grammar,
so it is safe to make this new token. Anything else falls under TYPE and will be
referenced to the 'In' type.

	Vector[32u] a b c;
	Vector[In = 32u] a b c;

Are equivalent declarations. Cool. Let's review expressions just to be sure:

	(@(a.c[0])) + (b->d) op-something (@((::Aa::fun)().a.b.c)) * ($(name))

Looks like the precedence is fine. The types also seem fine. What else? Let's see...
I won't be expanding on the statements, just keeping it to the core language now.
I'm afraid I'm becoming satisfied with the grammar. Maybe I need to ask my friends
to take a look at it. Just looking through the enum productions, not entirely satisfied.

	try
		String a("This is my string");
	catch {
		// Common entry code
		// Handler code
		case ::String::OUT_OF_MEMORY
			handleCase();
		case ::Something::ELSE_IS_HAPPENING
			handleOtherCase();
		// Common exit code
	}

The nice thing is that this generalizes enums as part of expressions. That is, actually
accessing the enum values. However, what about the enum "type"? We have 'enum' for
those currently, but I'm not a huge fan of it.

	enum String a(::String::CONTROL_ENUMERATION);

Oh... I like that. It fits well with array already... also with ptr, const, ref,...
wow. That's elegant. I like it. I think I may replace the grammar for enums now..
Another thing is that catch now uses the 'case' keyword. I'm a little skeptical about
it actually. Maybe a new keyword should be used. The potential confusion is there.
When someone confuses a case with a catch.

	(:) enter {
		try {
			switch (var) {
				case INVALID_STATE
					doSomething();
				case SOMETHING_ELSE
					doSomethingElse();
			}
			raise INVALID_MEMORY_SEGMENT;
		} catch {
			debug "Caught an error at" + #LINE;
			case INVALID_MEMORY_SEGMENT
				handleError();
		}
	}

Alright, catch should be fine now. case denotes an exception case. Any case in-between
will be executed. That said, types seem to be complete... In the production grammar,
it will be slightly different due to limits on the parser generator. Just forgot
del and new there. Important operators. What about threading? Just make that a hack
library implementation? Sounds fair to me. What about shared state? I guess we could
send in a pointer to some shared state. That works, no doubt. Even global state is
possible anyways using ALLCAPS variables. Alright. Anything more I'm missing?

	for (64ue new_token(getTokenStack().size() - amount_of_new_tokens);
		new_token < getTokenStack().size();
		++new_token)
	{
		ref protocols::Token curr_token(getTokenStack()[new_token]);
		{
			protocols::Token potential_next;
			potential_next.column = 0;
			potential_next.line = 0;
			potential_next.entry_type = ::protocols::EntryType::OTHER_SYMBOL;
			potential_next.token_type = ::protocols::TokenType::UNIDENTIFIED_TOKEN;
		}

		while ((curr_token.token_type == protocols::TokenType::UNIDENTIFIED_TOKEN)
			&& curr_token.entry_type == protocols::EntryType::OTHER_SYMBOL)
		{
			if (curr_token.accompanying_lexeme.empty())
				return false;
		}
		8u last(curr_token.accompanying_lexeme[-1]);
		curr_token.accompanying_lexeme.popLast();
		potential_next.accompanying_lexeme.insert(
			begin=potential_next.accompanying_lexeme.begin(),
			end=last);
		identifyToken($curr_token);
	}
	if (potential_next.accompanying_lexeme.empty() == false)
		getTokenStack().insert(begin=getTokenStack().begin() + new_token + 1, end=potential_next);
	ref protocols::Token newtok(getTokenStack()[new_token]);
	64ue colcount(position_counter.getCurrentColumnNumber())
		lncount(position_counter.getCurrentLineNumber());
	if (colcount == 1)
	{
		newtok.line_number = lncount - 1;
		newtok.column_number = last_line_length;
	}

That works quite well actually. This is a small translation of a part of the current
lexer. Checking types on their own will be useful too. The majority of the grammar
is segmented. I actually wish to separate the parts so the grammar is modular. Unfortunately
this makes the grammar difficult to work with. The operator [] should also exist...
right? Or should only raw arrays or ptrs be accessed using that?

	Vector[32u] a([1 2 3 4]);
	sml op-print a[3].toString();
	sml op-print a.get(3).toString();

I'm a little divided. If there's a method definition, it should be:

	(Type out : 123s in) [] { ... }

I don't know. I don't want to allow a definition of operator '('. That just feels
wrong. But []? I've used the std::vector<> overloads so much in C++. I think they're
tremendously useful. On the other hand, it makes the semantics misleading.

	obj[1];  // Is this a ptr or an object of a class, or array?

You can't really know this. That's the problem. It could be implemented... maybe
even with an overload of types in the '[]', so you can supply something that is not
an integer. `case 1+2 un-deref a();`. Operators @ and $$ are already 'inalienable'.
They can not be redefined. Making [] un-redefinable... hmmm. Tough choice. We can
already not define operator (), but []? Do we in addition to these allow other 'standard'
yet redefinable operators? Maybe a keyword like 'at' that acts like []:

	Vector[32u] a([1 2 3]);
	a at 2 = 100;

This will make it obvious what we mean. Although it does not allow the following:

	Vector[Vector[32u]] a([::Vector::([1 2 3])]);
	a at 1 at 2 = 100;

Since the right-associativity will try to execute '1 at 2', which makes no sense.
A tough decision indeed. Overloading [] seems like a nice feature but it just departs
from the orthodoxy of C defining a[b] as `*(a+b)`. I don't know. I was never really
bothered by Java's use of 'get'. Hell that even works when chaining them. Yeah, let's
keep it only for pointers and arrays.

	(:) enter {
		array 32u a([1 2 3 9+3 \9-3]);
		ptr 32u b(a);
		b[2] = a[2];

		Vector[32u] c(a);
		c.get(2) = b[2];
		#asgn(b[2], a[2], c.get(2));
	}

I can live with that. I kinda like it already. Simple and it works. Keeps the semantics
cleanly separated. `(un-deref a).something();`. Hmm... I just really don't like that
C++ allows redefinitions of the -> operator or the dereferencing operator. So what
will be decided? Hmmmmm... I think I'll just keep this. Only some binary and unary
operators are overloadable, but fundamentally important operators can not be overloaded.
The semantics of @ is that it _always_ changes a 'ptr x' to 'ref x', and it changes
a 'ref ptr x' to 'ref x'. $ always turns a 'ref x' into a 'ref ptr x'. Simple enough.
Those operators are important. [] does pointer addition. () calls a callable. Where
is the line drawn? Which operators do we overload and which do we not overload? What
about just allowing definitions of op- and un-? Leaving all other operators implemented
for the builtins.

	Matrix[32f] a(random=true) b(random=true);
	a = a op-matadd b;  // a = a + b;
	b = a op-matmul b;  // b = a * b;
	if un-det a op-neq 0.0 {  // if un-det a != ::Number(0)
		a = un-inv a;
		b = b op-matmul a;  // b = b * a;
		b = b op-mul 10.0;
	}

You know I kinda like the idea. It makes sure methods can not be symbols. This clashes
with my inherent idea about operators. I really enjoy C++'s ability to just state

	std::vector<int> a, b;
	b.push_back(10);
	a = b;

Where a is a copy, and not a reference to b. If we say all members get copied, then
a would refer to b's internal array. The destructors of both would destroy both objects.
In the matmul operations, a new matrix is created and returned. This sets all member
items, all the way to primitives. We could say all all basic operators just work
on each element of a class. Given both sides of the operators are of the same type.
I don't know how this would be useful though. Anyways, deep copying is useful. How
can it be implemented if = just assigns all primitives?

	Vector[32s] a b;
	b.add(10);
	a = b.copy();

This may work... Then again, it would be very nice to just overload these operators.
On another note, it's kinda misleading that a method is called for operator =. This
may have unexpected side-effects. If the struct-copying way of C is used, then we
don't get this. However, can we safely assign a ptr class to another? How will it
interfere with destructors? What should it mean? I like the explicitness though.

Just using a copy method. We can let the compiler issue a warning when a class has
no copy method. Will have to think about that. Reading a book about collision detection.
Be back in a bit.

So, since we have method arguments, we can bind names and ':' symbols, or '=' symbols.

	f(name=10; x:=1);

A little sketchy... Yeah. It's probably better to just use FEXPRs there and it should
be fine. As for assignment '=' and other binary operators, what do we allow? Java
just sets the reference. C just copies recursively. The problem with that is that
the destructor could deallocate copied memory regions. So this is dangerous. Only
if we write `x = y.copy()`... No. This makes things more complex. The complexity
stems from when the expression on the right goes off the stack. When does the right
expression go off the stack? `a = b + 3;`. If b + 3 is a new object, it is sent to
the = method. That method takes a const reference in, and when it returns, the reference
is put off the stack. Hmmm. If we have a '=' primitive assignment, then the destructor
of the temporary will destroy the internal data such that the new object will be
in an invalid state. So it can't be allowed. So operators should be overloadable?
Is there a solution? `a.copy(b)`. This could be a solution, but do we allow operator
redefinition then? `a op-copy b`. The new lexer though,.. makes me think a reversed
grammar may be possible:

	EXPR '=>' TYPE name=name other=name, EXPR, EXPR;

So let's examine (re-evaluate) this grammar.

	EXPR '=>' { [ ':' name ] EXPR } ';'
	TYPE name name2 name3,... ';'

	(128u out : 5u in) factorial
	{
		cast[128u](in) => out;
		--in;
		while in > 1
			out *= cast[128u](in);
	}

	32u a(1) b(2) c(3);
	a + b + c => a :rem b;

This way, ':' acts as both a separator and name assigner.

	// Breadth-first search
	(ref const Tree out : ref const Tree in; 8u search) bfs {
		Vector[ref const Tree] level newlevel;
		newlevel.add(in);

		while newlevel.size > 0 {
			newlevel => level;
			for 64u i(0); i < level.children_count; ++i {
				if level[i].data == search {
					level[i] => out;
				}
				newlevel.add(level[i]);
			}
		}
	}

	(Tree out() : ref Tree in(); 5u depth(random())) createRandom8Tree {
		in.createRandomChildren(random() % 10);
		for Tree child in out {
			createRandom8Tree($child; depth=depth-1);
		}
		out = in;
	}

	(:) enter {
		Tree a;
		createRandom8Tree() => a;
		ref const Tree(bfs($$a; search: 'a'));
	}

I think I got it. We can force the _first_ argument to be the 'in' value. But this
forces methods to have 'in', doesn't it? No. Let's examine.

	expr() => expr(), name: expr(), name2: expr();
	func(expr(), name: expr(), name2: expr());
	func(expr() => name, name: name2, name3: name4);

Hmm... this is easier to parse. But I'm confused about the functions.

	func() => expr() :name expr() :name2 expr();
	func(expr() => name :other name);
	expression [ '=>' expr():name expr2():name2 ... ]
	expression [ '=>'

	Type a b c;  // typedecl
	expr() => a, name: b, rets: c;

This can be tokenized as: `',' name ':'`. A single token. It's very interesting.
What about calling a method?

	func(expr(); expr() => a, name: b, rets: c);

The grammar is very LL(1) friendly then. Due to lexing, we can potentially implement
this instead of the previous grammar.

	32u a b;
	modulo(left: 13123231; right: 321) => a, remainder: b;

Ok. In the bfs algorithm it was just so intuitive to use the = assignment in methods.
It's already fleshed out, so why not roll with it? For the semantics of '=': the
choice is to make all operators overloadable. This never confused me in C++ and provides
a standard interface for object copying. If no operator '=' is provided, the standard
operator like in C will be applied. Good. I kinda like expressions in method arguments.
This allows you to add to a vector that is in the argument. That's a nice idea...
What about methods having default parameters whilst casting them to a lambda? How
are the semantics defined on these? The problem is that the default parameters are
by definition implementation details. They should not be part of the signature. However,
it should be noted that some arguments may be default inside the signature. So how
do we notify the compiler? Hmmm... Let's see... we have default arguments inside
the signature as of now. But the values of the default arguments are also in the
signature. The implementation detail is inside the signature. This is wrong. It was
decided that signatures should not contain implementation details. Where is the information
put to define default arguments?

	(32u out : 32u in()) name {
		out = in;
	}

No, we can't put it there! How is this cast into a fptr? C++ simply says that fptrs
can't have default arguments. In Cxy, a different paradigm is used for functions.
You basically inject one assignment statement for each variable. Hell, you could
even go as far as to say you just inject the statements you wish to inject. So how
can I specify a default argument there? Just change to overloading? But I don't like
overloading. It is nice in some cases but overall creates a mess. We could say default
arguments are the exception to the signature. Is there a different solution? It'd
be really cool if lambdas could have default arguments.

	(: default 32u a) something {
		default a(20);
	}

The problem is that 'default' is not part of a's type. I think I've got it:

	(: 32u a()) something {
		default a(20);
	}

This avoids the implementation detail, in addition to separating type from extra
information. It also adds zero new keywords. I like it. Let's use that.

	template<class FwdIt, class Compare = std::less<>>
	void quickSort(FwdIt first, FwdIt last, Compare cmp = Compare{})
	{
		auto const N = std::distance(first, last);
		if (N <= 1) return;
		auto const pivot = std::next(first, N / 2);
		std::nth_element(first, pivot, last, cmp);
		quickSort(first, pivot, cmp);
		quickSort(pivot, last, cmp);
	}

Can be turned into:

	FwdIt();  //
	Compare(internal::Less);  //

	(: FwdIt first last) quickSort {
		const 64u n(from op-distance last);
		if n <= 1 return;
		ptr const FwdIt pivot(first op-next n/2);
		::FwdIt::nthElement(from=first; to=pivot; end=last);
		quickSort(first=first; last=pivot);
		quickSort(first=pivot; last=last);
	}

	// Usage:
	(:) enter {
		Sort[FwdIt=OurForward; Compare=OurCompare] sort;
		OurArray a([1 2 3 8 9 \4 \3 \2 3 94 \3 \1321 394]);
		sort.quickSort(first=a.forward(); last=a.back());
	}

Or something similar. The CMP is already given as a grant. Grants are rather powerful,
as they cascade downward.
I think the grammar may be complete then. Is there anything left to do? Let's review
the original goals.

	* General purpose, yes.
	* Comparatively quick compile-time (LL(1), no SFINAE)
		* LL(1), yes
		* Deterministic memory management
		* Zero-cost abstractions (grants)
		* Optional run-time bound checking (implemented in class, static if)
		* Easy parallelization, via #codegenerators
		* Compile-time runnable code, has to be implemented.

The 'safe' tag is not yet added. What does it do? Hmm. I don't know how useful that
will be now that the language is developed. I think everything is covered. Only thing
that would be awesome if $$ can be replaced by another operator that makes it look
cleaner. What about $$$? I think we should explicitly disambiguate. Letting symbols
be simply wrong in the case where we have 'a==\1' ... or... hmmm. The former case
does look interesting. Either way, $$$ does not make a lot of semantic sense. Well,
here goes nothing. Just have to rewrite the grammar. The grammar won't work without
the proper lexer. So I have to hack it together. I can use a stream machine to detect
certain tokens. I also need to formalize the tokens. And fix that pesky line-number
issue for groupers, since they break prematurely. Let's make an image to visualize
the lexer's mode of operators

	Character -> Comment Filter -> String Filter -> Classify Entry
	-> Check if pop -> Classify Token Type -> Assign current line

It's rather bulky as it attempts to check a lot of things. Especially the algorithm
that takes clumped symbol tokens apart and attempts to match them. How can it be
made so that it recognizes the break? The mealy machine can be enlarged. But I don't
like that. I could create a generator for it though... Let's do that. Much simpler.
The question is: where do we separate actual lexing from replacement? We could say
the lexer is purely a classifier, not changing its input except for ignoring or pruning
the occasional unneeded information. On the other hand, we have a stringfilter that
acts as a way to replace certain elements inside strings. Well. Maybe we can take
that for granted and be content. Hell, the string filter can even run after the lexer
so that we don't need to encode the lexer's rules. That sounds like a nice solution.
So how are the rules constructed? From 0, all alphas go to the alpha state. From
0, all symbols go to respective symbol states. Wait. Isn't the lexer fine as it is?
It's just a little clumsy. Or that's how it feels anyways. Hmm. I'd love to implement
Cxy-like functionality and a standard library. For example, a C32ue class. this will
throw an error when it overflows. Anyways, to complete the language, the basics need
to be solid first. So we work on the lexer. Since the grammar is no longer in a state
of flux, we can write most elements manually. I also notice the comment ignorer does
not take into account the different strings. Both are necessary, so how do we keep
track?
One way is to use a 'global' recognizer that recognizes both strings and comments.
How are the semantics when we have `/* "/* " */` ? Obviously, the string recognizer
must not take the " as a string in this context. But the above inside a string must
be accepted. It's like it's recursive. I think strings need to be recognized first.
The reason is that we want to allow comments like `/* There is the "/*" comment opener */`
to be valid. The problem with that is non-closed strings. What if there is an error
where the string is not closed? Why not instead use # as a comment? Since code generators
can be used to just output nothing. I like the idea. What are the desired semantics
of `"#()"` though. Do code generators take precedence over strings? One could embed
generated text into a string by using simple addition. In any case, how could one
write a raw '#' inside a string given the # has a higher precedence? Maybe something
like `"\h"` where h stands for hashtag. Letting the code generator run first, this
resolves all problems. The code generator can run first. It run anywhere. Including
inside text. But. What about the following? `#(There is an extra ')' inside...)`?
Won't this prematurely end the code generator? I need better rules for this. We must
allow writing an `)` inside the text. What about making it a string? `#("There is an extra ')' inside...")`.

	(:) enter {  #("Don't worry about this line yet")
		sml op-print "Hello world";  #(`Prints "Hello world" to the output`)
	}

I can't find myself liking it. It is more general but I enjoy // more. So how can
the conundrum be solved? Well,... '//' inside a string is not a comment. Neither
is `/*`. Come to think of it. What about code generators and their limitations on
the right parenthesis? We can also make the comment ignorer 'dumb', in that it just
recognizes '//' and blindly filters, regardless of string status.

	sml op-print "A // B does not exist";

Will yield the error that the string is unterminated. Avoiding this problem is simply
done via:

	sml op-print "A /\/ B does not exist";

Another potential solution is to send the state back into the comment ignorer. This
makes having a state machine inside the comment ignorer simpler. The hashtag comment
is interesting though. Its verbosity disinterests programmers from writing comments.
Comments should be rare. Code should be explanatory. `#()`. Say we use this as a
commenting mechanism. How do we treat `#("#()")`? Nesting is important. How is the
string affected? How does escaping affect the nesting?

	#("My comment #("Nested Comment")")

If the code generators come before strings, then they simply don't care about strings.
This means that you can embed them inside strings, and they should work. Should the
strings obey the same rules? Let's use the exact same rule for escaping strings inside...
Wait. If those rules are used,... ugh. Let's say the precedence of #() is the highest.
The preprocessor will process it. Only strings can be used in it. Nothing else. `("`
and `")` are the delimiters. They nest. No expressions are allowed... What about
using `#(Stuff here)#`? using #( and )# as delimiters... genius or sucks? I kinda
like it. It serves as delimiters for codegeneration.

	#Parser(a ::= b a | eps; b ::= 'e';)#

I like this version. It avoids confusion with the "" strings. It also allows nesting
and writing the ) character freely. Though, I question its usefulness. What if we
want to write the following:

	#(   )#   )#

How can )# in the middle be seen as a token that doesn't close the codegenerator?
I think nobody will write this. So it's not a big deal. Okay. So that's done. We've
got new style comments

	(:) enter {  #( Don't worry about this line. Focus on the second line )#
		sml op-print "Hello world";  #( This is where the words "Hello world" are printed )#
	}  #( This finishes the opening brace. This is where the program ends )#

Looks just fine actually. I like the potential uses. What about # single line comments?

Man. A language has so much to consider. I want it all to fit inside this neat package
called a 'language'. It needs to be simple. But not simpler. I'd love if I could
just do simple passes over the text. Simple, non-interfering passes. In that regard,
using `#()#` being processed first, regardless of location or context.

`# ` (note the space) will also be processed. Being a newline-terminated comment.

Is everything in place then? Can we be content with the following?

	#()\h)#

As a valid input for '#'? This implies that #( is a string of the form "". But \h
doesn't work because \ is an operator... Hmmm... Is there an alternative? #""# could
be used...

	#`This is a comment`#
	# This is also a comment
	#`Nested#`Comment`#`#
	#CodeGenerator`This is the code`#

I actually like this one. It makes strings more friendly.

	#Cg"asm: #Ga`sin`# "#
	#CodeGenerator(`This is a literal string`)#
	#CodeGenerator("This is a literal string")#
	#("This is a comment")#

Hmm... I don't know. Where is the line between code generation and language drawn?
A three-buffer can be used:

	[ ) # X ]
	[ # ( X ]
	[ ) # ( ]

What do we do in the last case? If strings are used inside the parentheses, is the
ending '#' really needed? The ending '#' is nice for disambiguating long sequences.
I like the strings that are inside. They make it look like a normal method call.
Hmmm, code generators need their own escaping mechanism for ')'. Maybe # can be self-
escaping:

	#( Comment here #)
	#(#( Nested #) comment #)

This simplifies the code generator. # followed by any character invokes the code
generator. # followed by a space or tab comments out the line. # followed by a left
parenthesis implies a multiline-comment... that's simple. That's elegant...

	#Expr(1+2^3##6#)

Where ## denotes a raw # character. Good stuff man. This is independent from strings.
What about line comments containing #(? The simplest thing is to ignore absolutely
everything until a newline character has been seen. What does maylab do with its
% comments? Let's find out. It uses the simplest approach. Let's roll with that.
Simple and elegant. So # denote an out-of-string escape sequence. The sequence is
only the single following character. This is to expand the range of characters there
are. #( means one thing, `# ` means another. This has made me think about the design
of my C++ class that handles this specifically. Should that class be a customizable
machine? I started out using a ringbuffer as a template. On second thought, I think
it may be best to implement the class to do exactly what it needs.

	(2u out : 8u in) put  # Puts a character on the stack. Outputs how many characters you can pick
	(8u out : 2u in) get  # Returns the `in`th element

I just implemented it. This is the current API:

	(8u a b; 1u ai bi : 8u in) put

This is simpler. Just booleans telling us whether the values of a or b should be
used. This is because raw strings can contain the null character. The extra boolean
information is thus needed. The state machine is a simple pushdown automaton. It
also correctly processes `#something()` such that the `#` still exists in the output.
This allows the second pass to generate code without having to worry about comments.
Strings do not need to worry about comments either, only about strings. Another routine
that can be added is `(1u out : this) isInsideComment`. This is useful for editors
that want to highlight syntax. The semantics of comments are thus formalized and
carved in code. What is the next topic? Strings are already covered. Oh wait! What
about the following?

	#Expr(#)

The '#)' will be seen as a comment terminator. This implies that there was an opener
before this. The comment filter currently throws an error. I guess it should just
let it pass instead...

Seems reasonable. This should be fixed now. One confusing aspect is the following:

	#Expr((##)#)
	##expr(This is my (expression#) and it's cool!##)

If there is an escape for # (being ##), then why doesn't #expr need an escape? How
does that make sense? The main issue is that the decision is context-based. I don't
enjoy that. Let's look at an example:

	#do(something##)#)

Why does #d not require escaping? ) does, but only if the #) is unmatched. This messes
with comments containing codegenerators

	#(
		#create(#) <--- ends the nested comment here.
	#)

So how is this solved?

	#(
		#create[some new ##]code#]
	#)

That would solve the issue. Unknown sequences are simply ignored. The problem with
this is that the codegenerator's pass will also interpret ##. So how is the issue
resolved? Ideally, #( is merely a special case of the code generator. Commenting
codegenerators themselves should still count the codegenerators inside.. right? Well

Hmm, C and C++ do not have the option to escape `*/`. Maybe that's not a terrible
trade-off. Anyways, the problem of code generators needs to be solved. Since comments
are ideally also code generators, the code generated inside must be commented out.
A proposed solution is to use [] for code generators. This is an interesting proposal.
Are there any alternatives? Comments should also ignore code generators that are
inside, right? What is right to do?

	#( #gen(#) #)

What if gen prematurely exits the comment by generating `#) this is outside the comment #(`?

Should this be allowed? I like it much better to think of comments as more prioritized
entities. Such that code generators happen after commenting out code. This avoids
potential disasters.

	#makeParser(grammar.file#)
	makeParser#[grammar.file#]

Another possibility is to make the codegenerator like

	#code(#)
	#( #codeGen(#) #)

	#(
		Author=`Name`
		Date=`4/1/2016 20:54`
		Description=`Class used to generate the productions for the parser.`
	#)

By just making the requirement for #)... no it's needed. Else you can't include stray
')' characters.

	#(This is some comment#)
	#(#generateCode(#)#)

	#comment( #)

An idea is to make all #[alpha] tokens a comment indenter IF the comment is >0.

This is now implemented. #[alpha] simply increments the counter. Otherwise it just
outputs since #a is an unknown escape. Can it be assumed to always be a code generator?

	#generateCode(#( #)#)
	#(#generateCode( #)#)

What are the exact semantics? Let's try to formulate them.

1. A stream is read from left-to-right.
2. Two characters are assessed at the same time.
3. `# ` ignores the buffer until '\n'. '\n' is included in the output.
4. `#(` starts a multiline comment. Commes nest.
5. `#a` where `a` is any lowercase character is ignored. If it is inside a multiline comment, it increments the nesting counter.
6. `#)` decrements the nesting counter. If it decrements a counter from zero, it just outputs it verbatim.
7. `##` outputs a single `#` and clears the buffer.

This defines the semantics for all cases. Commenting out a piece of code is now easy.

	#(
		(:) enter {  # The enter method
			#make(##Wacky ##stuff#);
		}
	#)

Come to think of it, #[ fits better, since the files that are going to be code generators
act as classes.
What if we have `#cg#)`? There is no opening parenthesis. Does this pass through
the comment system? I think the comment system should be straight forward instead
of complicated. Maybe a different system can be constructed...

	#( Multiline comment
	So it continues here
	#) # ends the multiline comment
	# a single-line comment

The code generator's operation is simple:

	# lower { upper | lower } '(' { any } '#' ')'

	#[myStuff()#]

I think we need to roll with #[ actually. The important part is that it disambiguates
long sequences where there's a tonne of comments. The comments could be mixed with
actual code generation

		...
		and generate more alpha x 2
	#)  # Is this a comment ending or a cg entry?

		...
		and generate more alpha x 2
	#]  # Clearly a code generator invocation.

	#generateCode[This is some code#]
	#[generateCode()#]
	#(
		My comment
	#)
	#[
		myGenerator()
	#]

The nice thing about the #[ and #( preprocessors is that the ## escape is only necessary
within those groupers.

	#[ doSomethingCool(Anything goes here.##]) #]
	#[gen(#[aws()#])#]

It doesn't look clean though.

	#anyName(code here)

Maybe that's acceptable. Code generators are not to be used very often anyways. Only
for certain niches.

	#[generateLexer(lexerfile.l)#]

	(ref Vector[Token] out : this; 8u in) lex {
		generatedLexer(in);
		out = $this.lexvector;
	}

Alright. I guess I've formed a concensus in my head. Let's roll with this. It's consistent
and it works.
Are there any criticisms of these comments and code generators? Hmm, let's see. When
do we start running into trouble? It's just ugly is all. I don't like ugliness. Yeah,
I need to change this. If I don't like its aesthetics, something will need to replace
it.

	#ideally(this would be the best)
	# Just comments
	/# Maybe this /# is #/ ok #/

Should it use parentheses or brackets? I'm not sure. Clasically square brackets have
been used to denote compile-time arguments. Well, this is also compile-time.. so...

	#ideally[this would be the best]
	# Just comments
	/# Maybe this /# is #/ ok #/

If you need a bracket inside the above, then you should write something like `#]`.
That seems like a reasonable escape. The problem is that it doesn't play nice with
any editor matching. What about # itself? Let's just consume those anyways. If your
code requires those tokens, perform a pass for \h for a # and \m for a right bracket.
That's the simplest method. Also, who even uses nested code generators? Even then, it'd
be best to just let the user define their own escapes. What if a code generator is
called with an argument another code generator? I think it would be cleanest to not
allow the inner code generator to break the outer encapsulation:

	#outer[#inner[]]

If inner returns ']', it should not be allowed to break the outer code generator.
But that's not very easy to make. The lexer would need to be aware of the tokens
that it needs to consume before a ']' makes it possible to close the generator.

What can be done is to process the input after the first '[' and feed it through
the same stages as normal compilation. The top level-lexer will not be able to see
the ']' if done correctly. So how do we consistently deduce where to end the stream?

We feed the input into a buffer. Once a #a is found, that is processed by the preprocessor.
Another idea is to let the output from the second preprocessor fill in another buffer
that is deeper than the current one. I can probably implement it that way. In fact,
it would be interesting to implement right now. Without the interpreting of course,
just copying a file. Should code generators' generated code be code generated? In
other words, should that generated code pass through a cg stack as well as a comment stack?
In any case, how is ']' escaped? It can be thought to escape all ']'. But, only when
the generated code is inside another code generator. This seems rather counter intuitive.
Only inside other cgs should outputted elements be buffered.

	#outer[#lb[]Doing something#rb[]]

So how can this work with comments? Let's see. Let `# ` be the line comment mechanism.
Obviously, the following would be really cool:

	String a(`this is  # The string does not end here
	my favorite string type.`);

The same should be true for code generators:

	#generateCode[#[My comment]]

I just need to formalize these rules.
Either way, it's nice. It works intuitively. The same should be applied to comments.
You see, a comment should ignore code generators that are inside, but it should also
ideally not allow the code generator to end the sequence.

	#[This is a comment #generate[]ed code]

	Parser a(#createParser[grammar]);

One way of doing the nested code generators is to create a branch when the codegenerator
is invoked.

	#a[#b[]]

Creates a new internal buffer for b. I think some simple methods can be used. First,
all we need to do is scan the input for a # and a subsequent non-#. ## is the global
escape meaning a single #.

If a `#a` is spotted, the stream is consumed until a `[` is spotted. So.. essentially,
all output from codegenerators is escaped. An imaginary final pass unescapes the
result. That result is fed into the compiler. Ideally, we can turn the input into
a list of strings.

	[0x0 0x1 0x2 0x3...]

Where each pointer points to a sequence of code. Each pointer could contain... I
need to rethink this thing.

	[ here is my code # and it continues here ]
	                  #generate [ other # ]
	                                    #code [ hey! ]

The idea is to let the # create a new scope. That scope captures the name of the
generator. It then feeds the contents into that generator. This means the top-level
generator is basically `#cxy[...]`. Can the same recursive ideals be defined for ##?

	#
	# [ # ]

Yeah, seems reasonable. ## is just another generator with a special syntax due to
how useful it is. So let that be the official semantics:

1. '#' starts the codegenerator. It is consumed.
2. A subsequent '#' returns '#' and ends the code generator.
3. All symbols are consumed until the first '['.
4. The first bracket creates a separate entity for strings.
5. Strings are concatenated until the closing ']'.

This means that `#a[#b[]]` will become

	#
	a [ # ]
	    b [ ]

This requires a recursive algorithm. Essentially, what happens is that we need a
preprocessor object. For each instance of '#', the preprocessor summons another preprocessor
to work on the rest of the stream. When the stream gets an ending character ']',
it sends the input to the appropriate function. This can probably be done by using
a simple depth recognizer. It picks out the alphanumerics after the #. All it needs
to do is to increment a counter for each #. What does the datastructure look like?

	String cgname inputstring;

So the cgname is captured after a '#', then the input string is captured character
by character. If another '#' shows up in the state where it is capturing characters
for the input string, then the inputstring should be a list where we can discreetly
point out other generators. In fact, since cgs are greedy and generate immediately,
their generated content is immediately appended to the output 'inputstring'. Such
that we only really need:

	String cgname inputstring;
	ptr CodeGenerator sub(null);

Now, whenever a '#' is encountered, the sub can be instantiated. Here, it can filter
out the cgname. If the sub is not null, then all input will be redirected towards
the sub. If the sub notifies that it is finished, it is deleted. Its output appended
to the inputstring, and execution continues. A very general substitution method can
be created. By calling the './', the OS can deduce the command to execute via the
shebang line. The stdouted text can then be substituted. That's actually quite beautiful.
Hold on. That's how Cxy was supposed to be initially. A powerful string manipulator.
Huh, cool. This can be created independently from Cxy itself. Should a callback be
used? Suppose we have... hmmm
Ideally we should support shebangs...

	algorithm:

		switch (characters_two) {
			case '##': out='#';
			case '# ': comment=true;
			case '#a': codeGenerator();
			case '#[': comment();
		}

I like it. This should be sufficient. Once that's implemented as a preprocessing
step, everything else should fall into place.

	(:) enter {  # Don't worry about this
		sml op-print "Hello world";  # Focus on this line
	}  # Not important

	#[Compute the factorial of a number]
	(128ue out : 5u in) factorial {
		out = in;
		--in;
		while in > 1
			out *= in;
	}

	(String out : this) doSomething {
		out = this.serialize();
	}

	#serializable[  # Creates this.serialize methods for types.
		32u a b c;
		MyType d;
	]

This means that if a code generator returns `#cg[]`, that won't be interpreted. I
think that's appropriate. The reason for this is that the code generator known as
`##` would not have a well-defined form if this were the case:

	##a[]

If the '#' is reinterpreted, then meaning of ## is useless. Hence, all code generators
output only the final form of what they produce. What about grants?

	Aa[Class=Name] b;

Can the Class name be a generator or not? Maybe that's not appropriate. When a class
No it would allow arbitrary redefinition of private parts of the class. Suppose I
have a lexer and a parser generator. They use the same file. How do I distribute
these files to the entities?

	Lexer[file="grammar"] lex;
	Parser[file="grammar"] parse;

An idea is to have a compile-time environment to manipulate. Starting at the top
node, going downward. For each entity, a map is kept that holds data. Names like
the above are available at compile-time. How will the semantics be defined? This
is an important question. Grants are extremely powerful. Granting constant data is
also powerful. Currently, we only really have running code.

	(:) enter {
		x = y op-extend w + z.getSomething()~three;
	}

A rather simple grammar. Would code generation be needed for cases where the resulting
constant can be deduced from constant variables?

	(:) enter {
		Parser parser;
		parser.useGrammar(MYGRAMMAR);
	}

Problem is that optimization isn't perfect, so code generation is still desired in
some cases.
What else? Well let's see....

Ideally, the lowest denominator of computing is bits. Having a table of bytes somewhere
is like having classes. Temporal locality would imply that having often-used information
packed together inside a class. We can't guarantee that data is thusly stored in
one particular fashion. Using the bit descriptor allows one to place bits very precisely
in Cxy, so alignas would potentially be superfluous. It seems fitting to make every
type listed as it is specified in a class, work like that in the class's table as
well. Basically, each member of a class is just put inside one big table, with the
table being the size of all members combined.

	Type a b;
	8u c d;

This would (given ::Type::SIZE == 10) give you a table of 36 bits. Makes sense man.
Is this everything? Is there anything else I need to review? The grammar is done...
although it will need some formal work. The lexer can be hand-crafted, and it will
be beautiful. Expressions are simple. Types are linear. Statements are LL friendly.

Is this it? Is this the entire language?

	(array 2 16u out : 32u in; 1u bigendian()) split32into16 {
		default bigendian(true);
		if bigendian
			@cast[ptr 32u](out) = in;
		else {  # Given 16u == ::Byte::SIZE;
			out[0] = cast[array 2 16u](out)[1];
			out[1] = cast[array 2 16u](out)[0];
		}
	}

This does make me ponder endianness of units. Little endian makes the most sense
to me. Either way, there's no clear answer, both can be justified, and both are equally
useful.
The problem is that endianness operates on a byte level. I think it would be best
if the language assumes little endianness just for the sake of logic.

	(1u little big:) endian {
		2u a(1);  # 01 in memory
		little = cast[ptr 1u](a)[1];
		big = cast[ptr 1u](a)[0];
	}

	if endian()~little { ... }

	array 20 array 20 32u a;

So as for semantics regarding grants. Grant(name::Stuff); will always be overridden
by grants of the form `Grant=name::Stuff`. Lower level grants will override top-level
grants. I do not know how ideal this is:

	# Main.cxy
	(:) enter {
		sub::Sub[Type=Math] sub;
	}

	# sub/Sub.cxy
	Type(local::MyMath);
	local::Under[Type=local::OtherMath] other;

This implies that other will still use OtherMath, but Sub will have `Type` be `Math`
instead of `MyMath`.

I'm thinking maybe method signatures should be changed to use | as a separator instead
of the ':'.

	(|) enter {}
	(:) enter {}
	(32u out | 32u in) shiftLeft {}
	(32u out : 32u in) shiftLeft {}

Probably for no good reason anyways... Also. I'm trying to formalize most of the
grammar as of now. There is a small fix... There it is. A nice thing about the bit
fields is that we can support psuedo-class syntax:

	16s a(31209);
	32s b(-1239);
	a = cast[type(a)](b);  # Cast interprets rawly, so sign bit is wrong
	# Correct algorithm:
	a = cast[type(a)](b);  # First a normal cast.
	a.set(15, value=b.get(31));  # Then a sign bit setting.

This will give wrong results for sufficiently high numbers.

	16s a(-2);
	32s b(0);
	b = cast[type(b)](a);
	b.setSign(a.getSign());
	# or
	b = a.to32s();

I have a busy year ahead of me now,... hopefully the grammar will evolve as more
will be learned. I need to ask myself: "Is there anything I don't like?". Whatever
it is, it's always removed. For example, I still don't really enjoy $$... I find
it misleading... still, it should be fine.

An interesting idea taken from Haskell is the $ operator. I'm just pondering a very
simple grammar scheme that would use context-independent meanings for symbols...
Allow me to elaborate:

	class ( ... )

Now, $ can be used and it means '(' and it ends at the end of the file.

	class $
		name ( "something" )
		method ( return ( 32u (a b) ) param (8u c) code ({
			equals (a b)
		})
		)

I don't know. Could be interesting. It's a very free grammar in that you can decide
where the tags go. The problem is that it's not compact. Another problem is that
programmers will have a bad time communicating with each other using this method.
They'll end up with their own way of writing things, making codebases inconsistent
and messy.

Is the grammar done now? Maybe review type signatures a little. Oh! I have an idea!
What is static if is actually utilized in the lexer?! That'd be amazing since it allows
truly variable compilation due to the following scenario:

	static if x { public } else { private } (:) method { #[My method] };

This requires no grammar changes, however, the lexer will need to filter out the
word 'static' in order to parse an expression. However! this won't test all possible
grammatical errors.

Just wondering what I can clean up here... Ah, static if as a statement isn't acceptable
since we'll use the lexer for static inclusion. But, the lexer already uses the the
codegenerators to avoid generating unnecessary code. But imagine:

	#IfARM86[hack("something");]

This could work, it avoids the static ifs, we'll need to build up a grant class list
such that subclasses can also use IfArm or whatever, since a cg is only within a
single file anyways. This seems like a reasonable solution. It also avoids the need
for using static ifs... I'm still somewhat divided. Can the cgs then use static data
to determine a condition?

	#If (a == true, ...)

The problem with that is that we don't have any syntax conformance since the If class
needs to split apart the argument into multiple sections. I don't like that. Maybe
if can be special or something. I don't know...

I just added the 'comp' signature value. This value adds the information that the
method must be compile-time calculable. It must not use any class or method that
is a hacked grant... or maybe it will. That will give inconsistent results though.

	public Options a b c;

	public (: this; 32u a()) cool {
		default a(100);
		this.a.setSomething(a);
	}
	public (: this) x {
		ptr (: this; 32u a()) b(cool);
		b();
		b(10);  # Should this work?
	}

We also need unit tests. These are incredibly useful and we can verify the semantics
of the grants. So how do we reconcile the idea of using lambdas with default arguments?
Maybe default arguments should be mentioned differently... I think it can work actually...

	test assert(::GrantName::control());

	(:) enter {
		plot(lambda(32f z : 32f x y){z=x*y;};
			x=[1 10]; y=[-10 10]);
		# Destructor waits until the handle is destroyed.
	}

	test assert(::Granted::someMethod() == 1);

So the current language contains many common utilities and shortens them to simple
relations. What about the current limitations? It's not possible to state that a
namespace ought to be overridden except for usages of said namespace (or renaming)
a folder. Operators are fine, except that 'op' and 'un' are words, which implies
that we need spaces between the operators.

	aop-expb  # should not work

This implies that builtin operators also require a space. That is unreasonable. op
and un can be exceptions to this rule. The colon is also underused now. Well except
for namespace accessing... Am I satisfied with this grammar (and semantics?).

	array 20 Type a b c;
	array Type d e f;

The arrays can infer length,... or... well in that case, all elements must be of
the same size. By that I mean, d, e, and f must be of the same size. For arguments
to functions, the length of the array can be inferred. That shouldn't be a problem.
The array will just not change size during runtime. This presents an issue during
runtime. Especially considering lambdas. What lambdas could do is point to several
methods. When calling a lambda that contains default arguments, the machine calls
the lambda with all names of the arguments given, and it calls the correct method...

What about concatenating op- methods with '='? That should be possible. It's an interesting
concept.

	(My out : this; ref const My in) exp {
		...
	}

	My a(2) b(10);
	a op-exp b;
	a op-exp= b;

We could say that we need to just create a new operator for it:

	My a b;
	a op-aexp b;

Where aexp is a method that assigns data to a, and doesn't take a const reference
to a. The problem with this is that it becomes quire a liability to take into consideration
each time. Maybe there's a better solution. A universal language would be highly
interesting. I'm thinking that multiple returns may provide more inconvenienve than
not... but I'm a little uncertain. It's definitely useful.

An idea tickling my mind for a while is complete-file preprocessing:

	#Cpp
	##include <iostream>
	int main() {
		std::cout << "something" << std::endl;
	}

That could put all tools in this language. I like the idea. However, the languages
will need to be compiled into Cxy, then compiled into the final executable.

	(32f:ptr(32f:32f);32u steps;32f initial step) euler {
		for (32u i(0);i<steps;++i) {
			initial+=step*in(initial);
		}
		out=initial;
	}
	(:) enter {
		ptr (32f:32f) f(lambda{out=\in;});
		euler(f,initial=10,step=1.0/200.0);
	}

The above should be terser. I like having a good type system, but terseness is nice.
Hell, even python (without static types) is very much readable because we can easily
infer most of the types. Haskell uses this kind of system. Inferring types from their
functional requirements is a nice feature, but it allows one to send in types that
do not semantically adhere to the callee's expectations. This is a good reason for
having static and strong types in the language.

A shortening sequence could be very useful. The above poses the problem: "What if
we want to write something in a shorter fashion?".

	#Step euler {
		sml.out << "could be done this way";
	}

Where the code generator #Step basically generates the signature of euler, knowing
that iterative signatures look alike. The use of code generation can mediate a tonne
of boilerplate to simple cg invocations. This requires that code generators be granted.
My issue with granting is that there is too much implicit information.

	#Re[32f] { }
	(32f out : 32f in) { }

What's nice about code generation is its versatility. The power of code generation
is unmatched. There needs to be a sort of formal and clean way to specify all of
Cxy. Let's just compare:

	// C++
	float euler(float (*function)(), float step, float initial) { }
	# Cxy
	(32f : ptr (32f:32f); 32f step initial) euler { }

It's shorter than the C++ version. By making names optional, they can be considered.
'in' or 'out' in methods. Depending on their location of course. C++ has the edge
with pointers because they use two fewer characters. They can also concat the * with
the name, so it becomes three character savings. I like the short form for methods
though.

	(: 1u; this) control {}

No need to write 'in' or 'out' there. It's a nice shortening form.

	StringMap map;
	map.add(from="Hello!"; to="There");
	sml.out << map["Hello!"];

Without multiple return-assigns, the operator = would just be another conventional
operator. In fact, that'd work very well with the right-associative operations imposed
by the LL(1) grammar.

	a = b = c;

There's an elegance to that. Still, I like the idea of multiple returns, so how can
it be conveyed without the use of the equality operator?

	anon a(f());
	b = a~d;
	c = a~e;

The problem is that this requires another temporary to store the intermediate result.
That is definitely not something to be desired. So how can it be implemented?

	b: d, c: e = f();

Is the current method of specifying multiple assigns from a method. What about operators
automatically taking in references? This clashes with the idea of using pointers
explicitly.

I've been thinking about a new idea to employ. Thinking about all the clutter that
my types would bring makes me think that it's maybe cool to let types be fulyl inferred.

	a = 32u(100);
	a = 64u(10);

The only problem is how the above can avoid re-declaring a type. I remember from
my professional days programming in Python that there wasn't a big problem in finding
out a type. All you did was dir(myvar) to get the answer. I'm thinking that we can
employ the same strategy.

	a = 32u(100);
	type[a];  // Issues compiler information on the type
	b = myFunc();
	type[b];  // More useful
	numb = Complex(10, i=1);
	extra = numb + 1;

This also allows function overloading by duck typing. I liek the idea. My only issue
is with the declaration of variables. We could say we use static types and do not
allow overriding the variable with another type, but this will be a problem...

	a := 32u(10);
	a = 100;
	a = 32s(-10);  // Error
	a := getSome();  // Error, a already declared.

If we avoid globals, we can simply search the code for the name...

	jumptable = array[10, 10] 32u;
	index = 32u;

	index = 3;
	jumptable[0, index] = 0;

Hmmm... that's interesting.

	sumComplex(out : in, a, b, c : pure):
		for i in c:
			b += i
		out = b + a - in
		out.removeImaginary()

I like the idea. Should a pythonic syntax be used instead of the classical C syntax?

	enter(out : argc, argv):
		if argc > 1:
			sml::print(argv[1])
		f = [1 2 3]  // Declare f as array 32s 3
		f = [1 2 3 4]  // Illegal, change of type
		out = 0

What about implicitly named methods?

	myFunc():
		control()

	get(a b c):
		a = 1
		b = "Hey"
		c = -200

	enter(out : argc argv):
		if argc > 10:
			myFunc()
		else:
			sml::print("Heyyy guys!")
			sml::print(100)
		a:a b:b c:c = get()
		f = -10
		f:c = get()

Multiple returns will be a slight challenge. I currently have

	Type expr:name = expr

	a = get()~a
	b = get()~b
	c = get()~c
	a~a b~b c~c = get()

The concept is very powerful. Hell I can even make lambdas so much more enjoyable:

	map(list, fun=(out:in): out = in + 3)

The question is to eliminate the return parameter problem. This is not easy. Fexprs
can be used

	value: out, error: error = sin(0.3)

It looks good to me.

I think Python uses a philosophy where each module is sufficiently small to avoid
types overwriting themselves. I like the idea... I really do. Maybe I should just
implement compilable python. With the add-on of various items. Maybe with semicolons
and more classical C-like syntax?

	while thereAreSufficientFunds()
		funds = fetchFunds()
		allocation = allocateFunds(funds)
		sendFunds(allocation)

That's absolutely beautiful.

	file = sml::file("control")
	while file.has()
		character = file.get()
		++character
		print character
		debug character

	counter = Counter[32u];
	while counter < 10
		++counter
	print counter

	Counter[32u] counter;
	while (counter < 10) {
		++counter;
	}
	sml::print(counter.getString());

The former is so much more terse and easy to read.

	showUnauthorizedAccess()
		title = "Unauthorized"
		unauthorized = showUnauthorizedAccess.render()

		return layoutBoxPage.render(title, unauthorized)

After having considered this and asked several programmers who are interested in
language design, I've decided to go with single-returns and inferred typing.

	approxAndError(: pure):
		approx = 3201.0
		error = 0.0123
		return approx, error

	result = approxAndError()
	print(result.approx + 3.0)

Here, all returns of multiple values are to be put inside a named tuple. This means
that the names are dependent on what the function returns.

An idea is to let the compiler find the following ): for each expression. It's basically
an LR parser.

I've been experimenting a lot as of late and I think I've come to understand more
and more. It's not efficient to use my intended packaging system. It's tedious and
gets in the way of the program. What's needed is functional purity. This will give
comfort that a subsystem just processes data.

	# io means 'impure'
	io print(in: 32u):
		for i = 0; i < in; ++i:
			sml:print("Hello!")  # An impure operation

	32s main():  # Main is assumed to be impure
		print(10)
		# return 0

We also need function signatures to have types. The reason is because we want simple
ABIs so other languages can link to this one. Like Rust, we can allow full inference
inside functions, since that doesn't concern the programmer. What about classes and
structs?

If a simple ABI is desired, then functions must not be overloadable.

	(32u) main:
		defer del a
		a = new 100
		sml:print("Hello World")
		sml:printPointer(a)
		sml:print32u(@a)

Basically, the language just becomes C with functional purity, const-correctness,
and a few different operators to avoid ambiguity. And of course named arguments!

	(io, 32u in, 32u add) f:
		sml:print32u(in + add)

	(32u) main:
		f(100, add=10)

Prints out `110`.

	(32u) main:
		hack("mov %eax, %ebx")

Stuff like op-matmul can still be implemented by creating a standard for structs
and their methods.

	# Matrix.cxy
	array: ptr 32f
	size: 32u
	rows: 32u
	cols: 32u

	(Matrix) Matrix:
		a = Matrix
		a.array = nullptr
		a.size = a.rows = a.cols = 0
		return a

	(Matrix, Matrix left right) Matrix_matmul:
		# ...
		# Your matrix multiplication code here
		return c

	(32u) main:
		a = Matrix()  # Note the (), this calls the ctor.
		# It also defers the destructor in this scope
		b = Matrix()
		a op-matmul b

That looks quite nice. A simple ABI to interface with C. Functional purity. Hell,
even classes are included.

Language design is difficult. Ideally, the language should be interpretable but also
have a stable and simple ABI for C to use.

	(float, float in) sin

	(32s) main:
		printf(sin(0.3))

The problem with purity is that we need to define an arbitrary boundary. Is the GPU
used inside the abstract machine? What about threads? What about logging? We could
say writing to the error stream is part of the machine. That's cool. But what else?
Maybe deeply inside a function call stack, some function needs to probe the fs for
a file. What then? Do we need to pass the file's contents at that time?

Reading Rust's rationale for the decision to drop pure had an impact on me. I understand
what they're saying I just wish it were false that we need to define this arbitrary
line.
Ideally, to avoid all these troubles, we should be able to instantly propagate downward
any required information to the most deeply nested function of the call stack. How?
Imagine the following generic algorithm that computes a sequence of FTCS (Forward-Time
Central-Space approximation) to a differential equation. It's expensive to keep all
intermediate results in RAM. It's also clumsy to return a value, keep state, and
subsequently re-enter. Instead, it calls a file iterator to write. This is pure since
that item was given as an argument.

	main:
		file = sml:fopen("output.txt", sml:READ)
		computeFTCS(file)

However, the ftcs function doesn't use the file iterator directly. Instead, it calls
a more deeply nested function, that again calls a more deeply nested function...
In essence, it's an implicit parameter:

	g(stream):
		stream.write("Hello!")

	f():
		# ...
		g()

	main():
		file = sml:file("output", sml:READ)
		f(stream=file)

The problem is that there may be more methods in the tree that also use the name
stream and end up colliding with this.
The act of acquiring resources outside the ideal machine should be considered impure.
This includes requesting a webpage, reading a file, or getting cpu registers. Even
talking to the GPU is impure. However, if a function takes as argument an already
allocated resource, then the impure part has occurred. But isn't writing to that
file impure? Well, the resource is acquired. It's not part of the state machine.
What about the side-effects it has on other variables?

I think I got it (shower thoughts work)! Note how acquiring an outside resource uses
pointers. Now note that these are global. So the simple solution is to make global
read/write impure. That solves the entire problem (for now, in my head). The access
to the GPU, files, and other non-abstract-machine values all go through pointers.
This means that we can implement purity.

After some consideration, I'm thinking about just creating a cleaner version of C.
Why? Because it seems appropriate. Also, functions need to have types in their signatures.
This way, we can do partial compilation (which is great!). How about duck typing?

I'm thinking it's actually quite beautiful to let users look at the data structure
and infer what's going on. Like Linus states how you need to know what the data looks
like in order to infer what's going on. Without knowing the data (encapsulation),
you end up with "This class does X but I'm not sure how".

Unfortunately the world isn't a tonne easier when encapsulated away. I'd say it does
more harm than good. This is why I think it may be to our benefit to just go with
structs like in C. Still, classes are tremendously useful. Maybe we'll end up with
the likes of python. Python also basically has structs. Although they call them classes.
Imagine a string:

	# String.cxy
	64u length
	8u array

	new(this):  # Type is known
		this.length = 0

	del(this):
		del this.array

	This cat(this, This in):
		newlength = this.length + in.length
		newarray = new[newlength] 8u()
		for i = 0; i < this.length; ++i:
			newarray[i] = this.array[i]
		for j = 0; j < in.length; ++j:
			newarray[i + j] = in.array[j]

	# Main.cxy
	32u main():
		str = String()
		str2 = String()
		str.array = "Hello "
		str.length = 6
		str2.array = "world"
		str2.length = 5

		str3 = str op-cat str2
		printf(str3)

Is full inference possible with partial compilation? We could recompile the entire
dependency list, but that'd incur too big of a complexity I think. It's also harder
to reason about the code without specific type information in the signatures. If
you then try to debug some deeply nested used class, then it may become incredibly
difficult to understand the semantics at play due to the types being used. I think
that may be useful. What about memory safety? Classes should alleviate this mostly
because we use RAII. Hell, RAII is the one thing that makes programs safe. No standard
argument values, no overloading, simple classes-as-in-C-structs-with-functions. What
about purity? I think it can be really nice. What about demeter's? Demeter's is probably
the most important thing to get right if we want to write software that is safe,
reliable, and fast. If Demeter's is enforced, then it may be difficult to design
an architecture.

Basically, the aim is to create a C that is cleaner...
The current idea is to create an easily ABI interfaceable language. One that can
compile into ELF such that it can be linked with C.

	# Main.cxy
	doSomething(void, 32u in) {
		printf("%d", in);
	} # Seriously, braces make finding matching braces much easier!

Gets compiled to `Main_doSomething`. This makes it easy to interface with C... or
any other language for that matter. To mix any binary format, you need consistency.
Why not go a step further? Any function not compiled with "this" will automatically
assume the standard ABI and create a symbol that is raw.

doSomething(void) {
	expr check(x) = x.getFile() != capture.controlVar();

	if check(1) {
		printf("Control!");
	}
}

Here are some improvements:
Structs with associates functions, so you don't have to write long names.
Local 'macros': type-inferred expressions that are not part of the preprocessor (improve
readability)
Same linking model as in C.
Forcing {} for if/while/do/elif
Clean up cdecl (ptr instead of `*`)
Allow `1_00_000` for better readability.
Named args

checkBounds(1u, Bounds in, Point point) {
	left(x) x.x > in.left;
	right(x) x.x < in.right;
	top(x) x.y < in.top;
	bottom(x) x.y > in.bottom;

	if left(point) and right(point) and top(point) and bottom(point) {
		return true;
	}
	return false;
}

Yes, I'll still allow using any variable names for types. It's absurd not to. We
do not have namespaces, so disallowing this would be a disaster:

	wxString
	cxString
	csString
	abString
	coolString

We need to allow this. We could also allow a single-nest namespace using a simple
namespace directive.

	cx:String

Should get translated to `cx_String` or something like that

	cx:String a;
	a.init();
	defer a.term();

We can add a () to a to automatically init and terminate the class.

	cx:String a();
	// Nothing needed here.

Using a 2-token lookahead, we can use an LL1 parser on the methods, as well as allow
types to disambiguate themselves.

	 main(32s, 32u argc, ptr arr 8u argv) {
		if 1 > 3 {
			printf("Hey guys!");
		}
		for let a = 0; a < argc; ++a {
		}
		myExpr(x) x.getStuff();
		MyStuff a();
		printf("%d", myExpr(a));
	}

The question is now, should the function type come before or after the function name?

It no longer makes any sense to have purity... everything eventually communicates.
This is a sad realization, but one that has to be overcome.

	import cstdio, cstdlib;

	main() {
		#test(x) { printf("%s", x.toString()); };
		test(10);
		test("Cool!");
	}

Looking at rust really makes me wonder if that's the language to be using in some
time. Go seems to be more focused on interprocess com. Hmm, this makes me think..
What if instead of functional polymorphism, we use type-this polymorphism instead?

	main() {
		"hey".print();
		123.print();
		whatever = getSomething();
		whatever.print();
	}

This prevents ABI clutter as well as presenting a semi-polymorphic API. This solves
the problem in a neat and orderly fashion, but it does delegate functionality to
other parts of the program... Even so, specializations for std::ostream in C++ do
exactly the same thing. I really want what Haskell has, using a non-( call:

	main() {
		x = Matrix;
		x multiply x;
	}

I really like the idea, it also allows

	main() {
		x = "my value";
		x print;
		x add x;
	}

By using simple Right-Left rules for evaluation, we can get expressive code. My main
problem with allowing `x add x` is that adding a single token to this may cause a
major headache, especially in longer expressions.

	x.add x

Would be more applicable. This means that no ( would default to just calling the
method. The problem with that is that: how do we disambiguate members from calls?

	x.add | x.mul | x;

I like the idea. It avoids the clumsy op- syntax.

	import matlib;
	main() {
		x = Matrix:identity();
		x = x.mul # x;
		x.print();
	}

This is nice. It looks clean. This way we can also keep the ABI super simple.  I
really like this solution. It's clean, it looks great, and it doesn't conflict with
any existing stuff, except code generation for #.

	32u a, b, c;

	new(this, this) {
		this->a = 0;
		this->b = 0;
		this->c = 0;
	}

	del(void, this) {
		this->a = 10;
	}

	main(32u, 32u argc, ptr ptr 8u argv) {
		macro local(x) x.print();
		temp = This();
		local(123);
	}

So anything starting with just a name is to be considered a method or function. Everything
else will be considered a member of the structure. What if you want a static member?
What if static members are made to be hacks? That could be interesting. We just don't
support global variables out of the box :P. Very interesting indeed. Problem with
that is that deeply nested structures need to update each call in the chain in order
to support some extra details. It probably isn't needed.

	a : 64s;
	macro append(vector, value) vector.append(value, size(value));
	main() {
		a = sml:Vector();
		b = 10;
		c : 32u = 1000;
		append(a, b);
	}

It'd be nice with typesafe vectors though. Allowing macros in the outer scope is
an interesting development. This way, we can import macros. Which is nice. So we can
also import the append macro for vectors.

	main() {
		a = sml:Vec();
		sml:Vec:append(a, 100);
	}

By not allowing operator overloading, the meaning of @, %^&,... remain the same.
That also counts for the = operator. This should blindly copy all members. If you
really want a copy, just do:

	object.copy();

Problem with vectors then is that members do not get a 'copy' call. You could give
the vector a copy function, but... Just the [Based] generics should work. In that
case, what will the ABI look like?

	namespace_ClassName_typespecialization_methodname

? The problem with this is that particularly friendly to specializations. Yeah, that's
unfortunate I guess. You'll just have to define a new class for the specific specialization
then.

	main() {
		a = sml:Bits();
		b = sml:Vec[Data=1u]();
		a.append(true);
		b.append(false);
	}

a is more efficient in storage space.

Looking at rust it can be seen that Rust uses a lot of keywords like impl, enum,
some pattern matching, and generally a lot of advanced techniques. I'd like to remove
the more advanced patterns. It would be ideal to allow the code to be manipulated
using various tools like awk, sed, and grep to gain a lot of power. I wonder if such
a language that's also practical is possible. I could also just use a tag like:

	#codegen[arguments come here, and match any regex?]

I like Rust's macros. They're powerful.
