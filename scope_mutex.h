/*
 * scope_mutex.h
 * Scope Mutex class
 *
 * RAII Class implementing the mutex helper on the base of libc mutex
 * which automatically locks mutex during its creation and unlocks while
 * exiting the scope
 */

#ifndef _SCOPE_MUTEX_H_
#define _SCOPE_MUTEX_H_

#include <pthread.h>

/**
 * The wrapper class over libc mutex structure
 */
class mutex_object {
	private:
		pthread_mutex_t _m;
	public:
		mutex_object();
		~mutex_object();
	public:
		operator pthread_mutex_t *();
};


/*-------------------------------------------------------------------------*/

/**
 * Thescope mutex class
 */
class scope_mutex {
	pthread_mutex_t *_m;
	public:
	scope_mutex(pthread_mutex_t *m);
	~scope_mutex();
};

#endif /* _SCOPE_MUTEX_H_ */
