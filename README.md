# idgen
ID Generation Service Library
===================================

Overview
--------
This is a library with a family of ID generation service classes, applicable in
a wide range of simple and complex tasks and conditions.
The simplest class of generators is a primitive solution on the base of
increment of unsigned integer, while other classes allow to customize the
algorithm of ID generation, set-up the ranges of IDs, use generators in the infinite
loops, provide the tread safety etc.

Class View
----------
<img src="https://github.com/shulgaalexey/idgen/blob/master/idgen.png" alt="ID generation class view" style="width:250px"/>



How to use
----------
To use the ID generation, include the header file:

	#include "idgen.h"

All ID generators provide a basic interface, comprising three methods:
 * get_id() to get the currently generated ID;
 * next_id() to generate the next ID and get it;
 * set_id() to set the current ID, which would be used further for generating
 the next IDs.

For the simplest case, when You need a regular non-negative integer ID
generation, it is recommended to use single_idgen class, as following.

	/* Issue an instance of IDs generator */
	simple_idgen g;

	/* Get the firstly generated ID */
	int id = g.get_id();
	assert(id == 0);

	/* Get the next IDs */
	assert(g.next_id() == 1);
	assert(g.next_id() == 2);
	assert(g.next_id() == 3);

	/* Set the starting point of ID */
	g.set_id(13);

	/* Get a couple more IDs */
	assert(g.get_id() == 13);
	assert(g.next_id() == 14);

Note, that the type of IDs in this class is set to unsigned int.

For the cases when the thread safety is required, use simple_idgen_async.

When it is required to configure the ID generator with the specified type of IDs,
to limit the ID value range, as well as set up the first ID value, or assign a
specific algorithm of ID generation, it is recommended to use class idgen.
When it is required to generate IDs in an infinite cycle, You can set the
"round-robin" generation mode ON, so when the generator reaches the end of ID
value range, it starts again from the minimal ID value. If the "round-robin"
mode is off, the std::out_of_range exception will be thrown.

Note, that by default, the parameters of generator are assigned as following:
 * ID type: unsigned int
 * first ID value: 0
 * maximum ID value: UINT_MAX
 * algorithm of ID generation: simple increment
 * "round-robin" mode: ON

The type of IDs and the algorithm of ID generation can be assigned as
parameters of idgen class constructor while issuing instances of generator.
You can set the range of IDs and the first ID value in parameters of idgen
constructors. Another way to set the first ID value is the set_id() method.
To turn ON or OFF the :round-robin" mode, use set_round() method.

Example

	/* Issue an instance of ID generator */
	idgen<unsigned int> g(10, 20, 15);

	/* Get a couple of IDs */
	assert(g.get_id() == 15);
	assert(g.next_id() == 16);

	bool except = false;
	try {
		/* Turn OFF "round-robin" mode in order to get the exception */
		g.set_round(false);

		/* Set the starting value of ID on the higher limit of ID range */
		g.set_id(20);

		/* Request the ID out of range, and ... */
		g.next_id();
	} catch(exception& e) {
		/* Hooray, exception! */
		cout << e.what() << endl;
		except = true;
	}
	assert(except);


How to build
------------
 * Debug

g++ -Wall -g -o idgen_test_app scope_mutex.cpp test_case_env.cpp idgen_test.cpp -lpthread


 * Test Code Coverage

g++ --coverage -g -o idgen_test_app scope_mutex.cpp test_case_env.cpp idgen_test.cpp -lpthread

./idgen_test_app

gcov idgen_test.cpp


 * Release

g++ -Wall -o idgen_test_app scope_mutex.cpp test_case_env.cpp idgen_test.cpp -lpthread

How to run
----------
./idgen_test_app

Issues
------
 * -

TODO
----
 * Design classes
 * Design test cases
 * Update comments
 * Update docummentation
 * Consider creating C API
 * Support mutex_object for WIN OS
 * Check source coverage by tests (gcov)

