/**
 * test_case_env.cpp
 * Test Case Environment class implementatio
 */

#include "test_case_env.h"
#include <stdlib.h>

test_case_env::test_case_env(const string &func_name)
	: _status(0)
{
	cout << func_name << "..." << endl;
}

test_case_env::~test_case_env()
{
	if(_status == 0)
		cout << "OK" << endl;
	else
		cout << "ERROR: Test Case Exit status: " << _status << endl;
}


void test_job_mgr::add_job(test_case_routine_f test_func, void *arg)
{
	test_cases.push_back(test_case_descr(test_func, arg));
}

void test_job_mgr::run()
{
	for(size_t i = 0; i < test_cases.size(); i ++) {
		int ret = pthread_create(&test_cases[i]._thread,
				NULL,
				test_cases[i]._test_case,
				test_cases[i]._arg);
		if(ret) {
			cout << "Error! pthread_create() return code: "
				<<  ret << endl;
			exit(1);
		}
	}

	for(size_t i = 0; i < test_cases.size(); i ++)
		pthread_join(test_cases[i]._thread, NULL);
}
