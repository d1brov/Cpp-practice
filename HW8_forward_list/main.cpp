#include <iostream>
#include <iomanip>
#include <functional>

#include "forward_list_tests.h"

int main() 
{
	std::cout << "[ CONSTRUCTOR TESTS ]\n";
	testNprint("ForwardList()", &constructor_test);
	testNprint("ForwardList(const ForwardList& other)", &copy_constructor_test);
	testNprint("ForwardList(ForwardList&& other)", &move_constructor_test);
	testNprint("ForwardList(const size_t count, cref value)", &filling_constructor_test);
	testNprint("ForwardList(const initializer_list& ilist)", &init_list_constructor_test);
	std::cout << "\n";

	std::cout << "[ DESTRUCTOR TESTS ]\n";
	testNprint("~ForwardList()", &destructor_test);
	std::cout << "\n";

	std::cout << "[ OPERATORS TESTS ]\n";
	testNprint("ForwardList& operator=(const ForwardList& other)", &operator_copy_asign_test);
	testNprint("ForwardList& operator=(ForwardList&& other)", &operator_move_asign_test);
	std::cout << "\n";

	std::cout << "[ ELEMENT ACCESS TESTS ]\n";
	testNprint("ref front()", &front_test);
	testNprint("cref front() const", &const_front_test);
	std::cout << "\n";

	std::cout << "[ CAPACITY TESTS ]\n";
	testNprint("size_t size() const", &size_test);
	testNprint("bool empty() const", &empty_test);
	std::cout << "\n";

	std::cout << "[ MODIFIERS TESTS ]\n";
	testNprint("void pushFront(cref value)", &push_front_test);
	testNprint("void pushFront(rvalref value)", &push_front_rval_test);
	testNprint("void popFront()", &pop_front_test);
	std::cout << "\n";

	std::cout << "[ OPERATIONS TESTS ]\n";
	testNprint("size_t remove(cref value)", &remove_test);
	testNprint("ForwardList<T>& splitWhen(split_predicate condition)", &split_when_test);
	std::cout << "\n";

	std::cout << "[ ITERATION TESTS ]\n";
	testNprint("iterator begin()", &begin_test);
	testNprint("iterator end()", &end_test);
	testNprint("iterating cycle for", &interating_for_test);
	std::cout << "\n";
}