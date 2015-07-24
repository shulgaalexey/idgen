/*
 * test_case_env.h
 * Test Case Einvironment class
 */

#include <iostream>
#include <assert.h>
#include <vector>

#ifndef __TEST_CASE_ENV_H__
#define __TEST_CASE_ENV_H__

#ifndef __FUNCTION_NAME__
	#ifdef WIN32   /* WINDOWS */
		#define __FUNCTION_NAME__   __FUNCTION__
	#else          /* *NIX */
		#define __FUNCTION_NAME__   __func__
	#endif
#endif

using namespace std;

class test_case_env {
	private:
		int _status;
	public:
		test_case_env(const string &func_name);
		~test_case_env();
	public:
		void set_status(int status) { _status = status; }
};

#define START_TEST test_case_env test_case_env(__FUNCTION_NAME__);

typedef void *(*test_case_routine_f)(void *);

class test_job_mgr {

	class test_case_descr {
		public:
			test_case_routine_f _test_case;
			void *_arg;
			pthread_t _thread;
		public:
			test_case_descr(test_case_routine_f test_func, void *arg)
				: _test_case(test_func)
				  , _arg(arg)
				  , _thread(0) {}

	};

	private:
	vector<test_case_descr> test_cases;
	public:
	test_job_mgr() {}
	virtual ~test_job_mgr() {}
	public:
	void add_job(test_case_routine_f test_func, void *arg);
	void run();
};

#endif /* __TEST_CASE_ENV_H__ */
