#include <iostream>

#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <memory>

#include "CowVector.h"

void demoDetach() {
	CowVector<int> v1;			// v1 {}
	v1.push_back(1, "first");	// v1 {1}
	v1.push_back(2, "second");	// v1 {1, 2}

	CowVector<int> v2(v1);		// v1 {1, 2} == v2 {1, 2}    no detach
	v2.push_back(3, "third");	// v1 {1, 2} != v2 {1, 2, 3} detach (deep copying happened)
}

void demoDetachOnSet() {
	CowVector<int> v1;			// v1 {}
	v1.push_back(1, "first");	// v1 {1:first}
	v1.push_back(2, "second");	// v1 {1:first, 2:second}

	CowVector<int> v2(v1);		// v1 {1:first, 1:second} == v2 {1:first, 1:second}    no detach
	v2[0].setName("ZERO");		// v1 {1:first, second} != v2 {1:ZERO, 2:second}	   detach
	v2["ZERO"].setObject(0);	// v1 {1:first, second} != v2 {0:ZERO, 2:second}	   no detach
}

int main() 
{
	demoDetach();

	demoDetachOnSet();

	CowVector<int> v1;
	v1.push_back(1, "first");
	v1.push_back(2, "second");
	v1.push_back(3, "third");
	v1.push_back(4, "fourth");
	
	for (const auto& i : v1)
		std::cout << i.getObject() << ':' << i.getName() << '\n';
}