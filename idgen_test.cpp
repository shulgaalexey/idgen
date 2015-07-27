/*
 * idgen_test.cpp
 * Test of ID generator
 */

#include "idgen.h"
#include "test_case_env.h"


void test_simple_gen_p(void)
{
	START_TEST;

	/* Issue an instance of ID generator */
	simple_idgen g;

	/* Get the firstly generated ID */
	int id = g.get_id();
	assert(id == 0);

	/* Get the next ID */
	assert(g.next_id() == 1);
	assert(g.next_id() == 2);
	assert(g.next_id() == 3);

	/* Set the starting point of ID */
	g.set_id(13);

	/* Get a couple more IDs */
	assert(g.get_id() == 13);
	assert(g.next_id() == 14);
}

void test_simple_gen_n(void)
{
	START_TEST;
	/* TODO: */
}

void *use_simple_idgen_async(void *arg)
{
	if(!arg)
		return NULL;
	for(int i = 0; i < 5; i ++) {
		simple_idgen_async *g = (simple_idgen_async *)arg;
		/*int id =*/
		g->next_id();
		/*cout << pthread_self() << ":\t" << g << ",\t" << i << ":\t" << id << endl;*/
	}
	return NULL;
}

void test_simple_gen_async_p(void)
{
	return;
	START_TEST;

	simple_idgen_async g1;
	simple_idgen_async g2;

	test_job_mgr mgr;
	for(int i = 0; i < 3; i ++) {
		mgr.add_job(use_simple_idgen_async, &g1);
		mgr.add_job(use_simple_idgen_async, &g2);
	}
	mgr.run();
}

void test_simple_gen_async_n(void)
{
	START_TEST;
	/* TODO: */
}

void test_custom_gen_p(void)
{
	START_TEST;

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

		/* Request the ID out of range */
		g.next_id();
	} catch(exception& e) {
		/* Hooray, exception! */
		/* cout << e.what() << endl; */
		except = true;
	}
	assert(except);

}

void test_custom_gen_n(void)
{
	START_TEST;
	/* TODO: */
}

void *use_idgen_async(void *arg)
{
	if(!arg)
		return NULL;
	for(int i = 0; i < 3; i ++) {
		idgen<unsigned int, idgen_basic_algorithm<unsigned int>, mutex_object> *g =
			(idgen<unsigned int, idgen_basic_algorithm<unsigned int>, mutex_object> *)arg;
		/*int id =*/
		g->next_id();
		/*cout << pthread_self() << ":\t" << g << ",\t" << i << ":\t" << id << endl;*/
	}
	return NULL;
}

void test_custom_gen_async_p(void)
{
	START_TEST;

	idgen<unsigned int, idgen_basic_algorithm<unsigned int>, mutex_object> g1;
	idgen<unsigned int, idgen_basic_algorithm<unsigned int>, mutex_object> g2;

	test_job_mgr mgr;
	for(int i = 0; i < 5; i ++) {
		mgr.add_job(use_idgen_async, &g1);
		mgr.add_job(use_idgen_async, &g2);
	}
	mgr.run();
}

void test_custom_gen_async_n(void)
{
	START_TEST;
	/* TODO: */
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
