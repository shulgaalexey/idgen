/*
 * idgen.h
 * ID generator class
 *
 * Basic requirements:
 * 0. Template type of id (unsigned char, int, long)
 * 1. Sync/async
 * 2. Different algs (move to next id every second,
 * or even provide an unpredicted service of unique ID generation)
 * 3. Get min/max/cur
 * 4. Round robin or single iteration of ids (id = ++id % N)
 * 5. Release and reuse ids (it may be interesting for entity list management);
 * Get used/released ids (leverage a hash table for this)
 * 6. Add names for generators
 * 7. De-couple generator, algorithm, id pool
 */

/*
 * Необходимо реализовать класс(ы) для генерирования идентификаторов.
 * Идентификаторы должны быть последовательными целыми числами (например, 3, 4, 5
 * и т.п.). Другие свойства генератора вам нужно придумать самому на основе того,
 * что по вашему мнению может понадобится пользователям вашего генератора. Код
 * должен быть написан на C++ используя любой официальный стандарт. Из библиотек
 * можно использовать libc, stl и boost.
 * */

#ifndef _MAPS_SERVICE_PLUGIN_MODULE_H_
#define _MAPS_SERVICE_PLUGIN_MODULE_H_

#include <climits>
#include <algorithm>

/-------------------------------------------------------------------------/
/**
 * Algorithm of generating of IDs
 */
template <class T>
class idgen_basic_algorithm {
	public:
		idgen_basic_algorithm() {}
		virtual ~idgen_basic_algorithm() {}
	public:
		virtual T next(const T min, const T max, const T cur);
};

	template <class T>
T idgen_basic_algorithm<T>::next(const T min, const T max, const T cur)
{
	T next = cur ++;
	next = max(next, min);
	next = min(next, max);
	return next;
}

/-------------------------------------------------------------------------/

/**
 * A very basic and minimalistic ID generator,
 * providing service we all can imaging immediately
 */
template <class T>
class basic_idgen {
	protected:
		T _cur;
	public:
		basic_idgen(T cur = 0) : _cur(cur) {}
		virtual basic_idgen() {}
	public:
		virtual T next() { return ++_cur; }
};

/-------------------------------------------------------------------------/

/**
 * A customizable ID generator.
 * This class is expected to be a "wheelhorse" and be sed mainly in tasks,
 * requiring ID generation
 */
template <class T, class ALG = idgen_basic_algorithm<T> >
class idgen : public basic_idgen {
	protected:
		T _min;
		T _max;
		bool _round;
		ALG _alg;
	public:
		idgen(int min = 0, int max = UINT_MAX, int cur = 0);
		virtual ~idgen();
	public:
		virtual T next();
	public:
		bool is_round() const { return _round; }
		void set_round(const bool round) { _round = round; }
};

/-------------------------------------------------------------------------/
	template <class T, class ALG>
	idgen<T, ALG>::idgen(int min = 0, int max = UINT_MAX, int cur = 0)
	: _min(min)
	, _max(max)
	  , _round(true)
{
	/* Check if _max is bigger than _min */
	_cur = max(cur, _min);
}

	template <class T, class ALG>
idgen<T, ALG>::~idgen()
{
}

	template <class T, class ALG>
T idgen<T, ALG>::next()
{
	if(is_round())
		_cur = alg.next(_min, _max, _cur) % _max;
	else
		if(!alg.finished(_min, _max, _cur))
			_cur = alg.next();
		else {
			/* exception */
		}

	return _cur;
}

/-------------------------------------------------------------------------/
/**
 * An asynchronous version of customizable ID generator
 */
template <class T, class ALG>
class idgen_async : public idgen<T, ALG> {
	public:
		idgen_async() {}
		virtual ~idgen_async() {}
	public:
		virtual T next();
};


	template <class T, class ALG>
T idgen_async<T, ALG>::next()
{
	/scope_mutex sm(mutex);/
	return idgen::next();
}

/-------------------------------------------------------------------------/
/**
 * The simplest and one of the most useful configuration of ID generators.
 * It provide service of ID generation in range of 0 to the value,
 * corresponding to UINT_MAX on this machine; when the ID exceeds this maximum
 * value, the generator starts from zero
 */
typedef basic_idgen<unsigned int> simple_idgen;

#endif /*__MAPS_SERVICE_PLUGIN_MODULE_H__*/



