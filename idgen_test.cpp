/*
 * idgen_test.cpp
 * Test of ID generator
 */

#include <iostream>
#include "idgen.h"
using namespace std;

int main(void)
{
	simple_idgen g;
	const int id = g.next();
	cout << id << endl;
	return 0;
}
