/*
 * test_case_env.h
 * Test Case Einvironment class
 */

#include <iostream>
#include <assert.h>

#ifndef _TEST_CASE_ENV_H_
#define _TEST_CASE_ENV_H_

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

#define START_TEST test_case_env _test_case_env(__FUNCTION_NAME__);

#endif /* _TEST_CASE_ENV_H_ */
