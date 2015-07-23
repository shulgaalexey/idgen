/*
 * idgen_test.cpp
 * Test of ID generator
 */

#include "idgen.h"
#include "test_case_env.h"


void test_simple_gen_p(void)
{
	START_TEST;

	simple_idgen g;
	int id = g.get_id();
	assert(id == 0);

	id = g.next_id();
	assert(id == 1);

	g.set_id(13);
	assert(g.get_id() == 13);
	assert(g.next_id() == 14);
}

void test_simple_gen_p02(void)
{
	START_TEST;
}

void test_simple_gen_n(void)
{
	START_TEST;
}

void test_simple_gen_async_p(void)
{
	START_TEST;
}

void test_simple_gen_async_n(void)
{
	START_TEST;
}

void test_custom_gen_p(void)
{
	START_TEST;

	idgen<int> g(10, 20, 15);
	assert(g.get_id() == 15);
	assert(g.next_id() == 16);

	bool except = false;
	try {
		g.set_round(false);
		g.set_id(20);
		g.next_id();
	} catch(exception& e) {
		/*cout << e.what() << endl;*/
		except = true;
	}
	assert(except);

}

void test_custom_gen_n(void)
{
	START_TEST;
}

void test_custom_gen_async_p(void)
{
	START_TEST;


}

void test_custom_gen_async_n(void)
{
	START_TEST;
}

int main(void)
{
	test_simple_gen_p();
	test_simple_gen_n();
	test_simple_gen_async_p();
	test_simple_gen_async_n();
	test_custom_gen_p();
	test_custom_gen_n();
	test_custom_gen_async_p();
	test_custom_gen_async_n();
	return 0;
}
