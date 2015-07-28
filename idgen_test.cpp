/*
 * idgen_test.cpp
 * Test of ID generator
 */

#include "idgen.h"
#include "test_case_env.h"
#include <iostream>
#include <sstream>
using namespace std;

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

class testee_simple_async_generator {
	public:
		simple_idgen_async *_g;
		unsigned char _color_code;
};

void *use_simple_idgen_async(void *arg)
{
	if(!arg)
		return NULL;
	testee_simple_async_generator *tg =
		(testee_simple_async_generator *)arg;
	for(int i = 0; i < 4; i ++) {
		int id = tg->_g->next_id();

		ostringstream oss;
		oss << "thread id: " << pthread_self()
			<< "\tgenerator ptr: " << tg->_g
			<< "\titeration: " << i
			<< "\tID: " << id;
		ostringstream oss_colorful;
		oss_colorful << "\033[1;" << int(tg->_color_code) << "m"
			<< oss.str() << "\033[0m\n";
		cout << oss_colorful.str();
	}
	return NULL;
}

void test_simple_gen_async_p(void)
{
	START_TEST;

	cout << "Note: Each thread is colored on its own color" << endl;

	/* First testee generator */
	simple_idgen_async g1;

	/* Second testee generator */
	simple_idgen_async g2;

	test_job_mgr mgr;
	unsigned int start_color = 30;
	const int threads_cnt = 3;
	testee_simple_async_generator tgs[threads_cnt * 2] = { 0 };
	for(int i = 0; i < threads_cnt; i ++) {

		/* Run the first generator in this thread */
		tgs[2 * i]._g = &g1;
		tgs[2 * i]._color_code = start_color + (2 * i) % 16;
		mgr.add_job(use_simple_idgen_async, &tgs[2 * i]);

		/* Run the second generator in this thread */
		tgs[2 * i + 1]._g = &g1;
		tgs[2 * i + 1]._color_code = start_color + (2 * i + 1) % 16;
		mgr.add_job(use_simple_idgen_async, &tgs[2 * i + 1]);
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

class testee_async_generator {
	public:
		idgen<unsigned int,
			idgen_basic_algorithm<unsigned int>,
			mutex_object> *_g;
		unsigned char _color_code;
};

void *use_idgen_async(void *arg)
{
	if(!arg)
		return NULL;
	testee_async_generator *tg = (testee_async_generator *)arg;
	for(int i = 0; i < 3; i ++) {
		int id = tg->_g->next_id();

		ostringstream oss;
		oss << "thread id: " << pthread_self()
			<< "\tgenerator ptr: " << tg->_g
			<< "\titeration: " << i
			<< "\tID: " << id;

		ostringstream oss_colorful;
		oss_colorful << "\033[1;" << int(tg->_color_code) << "m"
			<< oss.str() << "\033[0m\n";
		cout << oss_colorful.str();
	}
	return NULL;
}

void test_custom_gen_async_p(void)
{
	START_TEST;

	cout << "Note: Each generator is colored on its own color" << endl;

	/* First testee generator */
	idgen<unsigned int,
		idgen_basic_algorithm<unsigned int>,
		mutex_object> g1;
	testee_async_generator tg1;
	tg1._g = &g1;
	tg1._color_code = 31;

	/* Second testee generator */
	idgen<unsigned int,
		idgen_basic_algorithm<unsigned int>,
		mutex_object> g2;
	testee_async_generator tg2;
	tg2._g = &g2;
	tg2._color_code = 32;

	/* Third testee generator */
	idgen<unsigned int,
		idgen_basic_algorithm<unsigned int>,
		mutex_object> g3;
	testee_async_generator tg3;
	tg3._g = &g3;
	tg3._color_code = 34;

	test_job_mgr mgr;
	for(int i = 0; i < 4; i ++) {
		mgr.add_job(use_idgen_async, &tg1);
		mgr.add_job(use_idgen_async, &tg2);
		mgr.add_job(use_idgen_async, &tg3);
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
