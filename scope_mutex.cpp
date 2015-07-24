/*
 * scope_mutex.cpp
 * Implementation of Scope Mutex class
 */

#include "scope_mutex.h"

mutex_object::mutex_object()
{
	pthread_mutex_init(&_m,NULL);
}

mutex_object::~mutex_object()
{
	pthread_mutex_destroy(&_m);
}


mutex_object::operator pthread_mutex_t *()
{
	return &_m;
}

scope_mutex::scope_mutex(pthread_mutex_t *m)
	: _m(m)
{
	if(_m)
		pthread_mutex_lock(_m);
}

scope_mutex::~scope_mutex()
{
	if(_m)
		pthread_mutex_unlock(_m);
}
