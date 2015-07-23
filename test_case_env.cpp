/**
 * test_case_env.cpp
 * Test Case Environment class implementatio
 */

#include "test_case_env.h"

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
