/*
 * scope_mutex.cpp
 * Implementation of Scope Mutex class
 */

#include "scope_mutex.h"
#include <iostream>
using namespace std;

mutex_object::mutex_object()
{
	do {
		pthread_mutex_init(&_m, &_attr);
		pthread_mutexattr_settype(&_attr, PTHREAD_MUTEX_RECURSIVE);
		/*int error = pthread_mutex_init(&_m, &_attr);
		if(error) {
			cout << "ERROR! mutex_init: " << error << endl;
			break;
		}
		error = pthread_mutexattr_settype(&_attr, PTHREAD_MUTEX_RECURSIVE);
		if(error) {
			cout << "ERROR! mutexattr_settype: " << error << endl;
			break;
		}*/
	} while(false);
	/*cout << "ERROR! mutex_object constructor failed!" << endl;*/
}

mutex_object::~mutex_object()
{
	pthread_mutex_destroy(&_m);
	pthread_mutexattr_destroy(&_attr);
	/*int error = pthread_mutex_destroy(&_m);
	if(error)
		cout << "ERROR! mutex_destroy: " << error << endl;

	error = pthread_mutexattr_destroy(&_attr);
	if(error)
		cout << "ERROR! mutexattr_destroy: " << error << endl;*/
}


mutex_object::operator pthread_mutex_t *()
{
	return &_m;
}

scope_mutex::scope_mutex(pthread_mutex_t *m)
	: _m(m)
{
	if(_m) {
		pthread_mutex_lock(_m);
		/*const int error = pthread_mutex_lock(_m);
		if(error)
			cout << "ERROR! pthread_mutex_lock: " << error << endl;*/
	}
}

scope_mutex::~scope_mutex()
{
	if(_m) {
		pthread_mutex_unlock(_m);
		/*const int error = pthread_mutex_unlock(_m);
		if(error)
			cout << "ERROR! pthread_mutex_unlock: " << error << endl;*/
	}
	_m = NULL;
}

void scope_mutex::halt()
{
	if(_m) {
		pthread_mutex_unlock(_m);
		/*const int error = pthread_mutex_unlock(_m);
		if(error)
			cout << "ERROR! pthread_mutex_unlock: " << error << endl;*/
	}
	_m = NULL;
}
