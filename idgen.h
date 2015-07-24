/*
 * idgen.h
 * id generator class
 *
 * Basic requirements:
 * 0. Template type of id (unsigned char, int, long)
 * 1. Sync/async
 * 2. Different algs (move to next id every second,
 * or even provide an unpredicted service of unique ID generation)
 * 3. Get min/max/cur
 * 4. Round robin or single iteration of ids (id = ++id % N)
 * 5. Distributed ID generation system: De-couple generator, algorithm, id pool
   * Register an ID generator in the remote Manager
 * 6. Release and reuse ids (it may be interesting for entity list management);
 * Get used/released ids (leverage a hash table for this)
 * 7. Add names for generators
 */


#ifndef __IDGEN_H__
#define __IDGEN_H__

#include <climits>
#include <algorithm>
#include <stdexcept>
#include "scope_mutex.h"


/**
 * Algorithm of generating of IDs
 */
template <class T>
class idgen_basic_algorithm {
	public:
		idgen_basic_algorithm() {}
		virtual ~idgen_basic_algorithm() {}
	public:
		virtual T get_next_id(const T min_id,
				const T max_id,
				const T cur_id) const;
		virtual bool finished(const T min_id,
				const T max_id,
				const T cur_id) const;
};

template <class T>
T idgen_basic_algorithm<T>::get_next_id(const T min_id,
		const T max_id,
		const T cur_id) const
{
	return (cur_id + 1);
}

template <class T>
bool idgen_basic_algorithm<T>::finished(const T min_id,
		const T max_id,
		const T cur_id) const
{
	return (cur_id >= max_id);
}


/**
 * A very basic and minimalistic ID generator,
 * providing service we all can imaging immediately
 */
template <class T>
class basic_idgen {
	protected:
		T _cur;
	public:
		basic_idgen(T cur_id = 0) : _cur(cur_id) {}
		virtual ~basic_idgen() {}
		/* Add copy constructor, operato= */
	public:
		/**
		 * Get the currently available generated ID
		 */
		virtual T get_id() const { return _cur; }

		/**
		 * Set the current ID and continue generation from this value
		 */
		virtual void set_id(const T cur_id = 0) { _cur = cur_id; }

		/**
		 * Return the incremented current value
		 */
		virtual T next_id() { return ++_cur; }
};

/**
 * An asynchronous version of basic ID generator
 */
template <class T>
class basic_idgen_async : public basic_idgen<T> {
	protected:
		mutable mutex_object _m;
	public:
		basic_idgen_async(T cur_id = 0) : basic_idgen<T>(cur_id) {}
		virtual ~basic_idgen_async() {}
	public:
		virtual T get_id() const;
		virtual void set_id(const T cur_id = 0);
		virtual T next_id();
};

template <class T>
T basic_idgen_async<T>::get_id() const {
	scope_mutex sm(_m);
	return basic_idgen<T>::get_id();
}

template <class T>
void basic_idgen_async<T>::set_id(const T cur_id) {
	scope_mutex sm(_m);
	basic_idgen<T>::set_id(cur_id);
}

template <class T>
T basic_idgen_async<T>::next_id() {
	scope_mutex sm(_m);
	return basic_idgen<T>::next_id();
}


/**
 * A customizable ID generator.
 * This class is expected to be a "wheelhorse" and be sed mainly in tasks,
 * requiring ID generation
 */
template <class T = unsigned int,
	 class ALG = idgen_basic_algorithm<T>,
	 class SYNC_MUTEX = no_mutex_object>
class idgen : public basic_idgen<T> {
	protected:
		T _min;
		T _max;
		bool _round;
		ALG _alg;
		mutable SYNC_MUTEX _sync_mutex;
	public:
		idgen(T min_id = 0, T max_id = UINT_MAX, T cur_id = 0);
		virtual ~idgen() {}
	public:
		virtual T get_id() const;
		/**
		 * Set the current ID and continue generation from this value
		 */
		virtual void set_id(const T cur_id = 0);
		virtual T next_id();
	public:
		T get_min() const;
		void set_min(T min_id = 0);
		T get_max() const;
		void set_max(T max_id = UINT_MAX);
		void set_range(T min_id = 0, T max_id = UINT_MAX, T cur_id = 0);
		bool is_round() const;
		void set_round(const bool round);
};

template <class T, class ALG, class SYNC_MUTEX>
idgen<T, ALG, SYNC_MUTEX>::idgen(T min_id,
		T max_id,
		T cur_id)
	: basic_idgen<T>(cur_id)
	, _min(min_id)
	, _max(max_id)
	  , _round(true)
{
	/* Check if _max is bigger than _min */
	this->_cur = std::max(cur_id, _min);
}

template <class T, class ALG, class SYNC_MUTEX>
T idgen<T, ALG, SYNC_MUTEX>::get_id() const
{
	scope_mutex sm(_sync_mutex);
	return basic_idgen<T>::get_id();
}

template <class T, class ALG, class SYNC_MUTEX>
T idgen<T, ALG, SYNC_MUTEX>::next_id()
{
	scope_mutex sm(_sync_mutex);
	const T current_id = this->get_id();

	if(!is_round() && _alg.finished(_min, _max, current_id))
		/*
		 * Throw out-of-range exception when
		 * "no-round-robin" algorithm can no longer
		 * generate new IDs
		 */
		throw std::out_of_range("idgen::next_id out of range");

	set_id(_alg.get_next_id(_min, _max, current_id));

	return this->get_id();
}

template <class T, class ALG, class SYNC_MUTEX>
void idgen<T, ALG, SYNC_MUTEX>::set_id(const T cur)
{
	scope_mutex sm(_sync_mutex);
	if(is_round())
		basic_idgen<T>::set_id(cur % _max);
	else
		if((cur >= _min) && (cur <= _max))
			basic_idgen<T>::set_id(cur);
		else {
			/*
			 * Throw out-of-range exception when
			 * "no-round-robin" algorithm attempts to set
			 * the IDs, exceeding allowed ranges
			 */
			throw std::out_of_range("idgen::set_id out of range");
		}
}

template <class T, class ALG, class SYNC_MUTEX>
T idgen<T, ALG, SYNC_MUTEX>::get_min() const
{
	scope_mutex sm(_sync_mutex);
	return _min;
}

template <class T, class ALG, class SYNC_MUTEX>
void idgen<T, ALG, SYNC_MUTEX>::set_min(T min_id)
{
	scope_mutex sm(_sync_mutex);
	_min = min_id;
}

template <class T, class ALG, class SYNC_MUTEX>
T idgen<T, ALG, SYNC_MUTEX>::get_max() const
{
	scope_mutex sm(_sync_mutex);
	return _max;
}

template <class T, class ALG, class SYNC_MUTEX>
void idgen<T, ALG, SYNC_MUTEX>::set_max(T max_id)
{
	scope_mutex sm(_sync_mutex);
	_max = max_id;
}

template <class T, class ALG, class SYNC_MUTEX>
void idgen<T, ALG, SYNC_MUTEX>::set_range(T min_id, T max_id, T cur_id)
{
	scope_mutex sm(_sync_mutex);
	set_min(min_id);
	set_max(max_id);
	set_id(cur_id);
}

template <class T, class ALG, class SYNC_MUTEX>
bool idgen<T, ALG, SYNC_MUTEX>::is_round() const
{
	scope_mutex sm(_sync_mutex);
	return _round;
}

template <class T, class ALG, class SYNC_MUTEX>
void idgen<T, ALG, SYNC_MUTEX>::set_round(const bool round)
{
	scope_mutex sm(_sync_mutex);
	_round = round;
}

/*-----------------------------------------------------------------------*/
/**
 * Distributed ID generation: there is a group of generators who connect to
 * ID generation manager
 */
/*
   class idgen_manager {
   private:
   list_of_generators;
   public:
   register_generator(distributed_idgen *gen);
   unregister_generator(distributed_idgen *gen);
   unsigned long long get_id() const;
   unsigned long long next_id();
   };
   */

/*-----------------------------------------------------------------------*/
/**
 * The simplest and one of the most useful configuration of ID generators.
 * It provide service of ID generation in range of 0 to the value,
 * corresponding to UINT_MAX on this machine; when the ID exceeds this maximum
 * value, the generator starts from zero
 */
typedef basic_idgen<unsigned int> simple_idgen;

/**
 * The asynchronous thread-safe variant of the simplest ID generator
 */
typedef basic_idgen_async<unsigned int> simple_idgen_async;

#endif /*__IDGEN_H__*/
