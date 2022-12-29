#include <iostream>
#include <iomanip>
#include <functional>
#include <forward_list>

#include "forward_list.h"

///////////////////////////////////////////////////////////////////////////////
static int created = 0;
static int deleted = 0;
static int copied = 0;
static int moved = 0;
void zeroStats() 
{
	created = 0;
	deleted = 0;
	copied = 0;
	moved = 0;
}

class TestingEntity 
{
public:
	TestingEntity() = delete;
	TestingEntity(const std::string& key, int value) : key_(key), value_(value) 
	{ 
		//std::cout << "creating testing entity: " << key_ << ':' << value_ << '\n';
		created++;
	}

	TestingEntity(const TestingEntity& other) :
		key_(other.key_),
		value_(other.value_)
	{
		copied++;
	}

	TestingEntity(TestingEntity&& other) :
		key_(std::move(other.key_)),
		value_(std::move(other.value_))
	{
		moved++;
	}

	~TestingEntity() 
	{
		//std::cout << "deleting testing entity: " << key_ << ':' << value_ << '\n';
		deleted++;
	}

	const std::string& key() const { return key_; }

	int value() const { return value_; }

	int compare(const TestingEntity& other) const
	{
		if (key_ == other.key_ && value_ == other.value_)
			return 0;
		else
			return 1;
	}

private:
	std::string key_;
	int value_;
};

void testNprint(const std::string& name, const std::function<bool()>& test)
{
	std::cout << name << std::setw(54 - name.size()) <<
		(test() ? "" : "FAILED") << '\n';
}

///////////////////////////////////////////////////////////////////////////////
// CONSTRUCTORS TESTS
// ForwardList();
bool constructor_test() 
{
	ForwardList<TestingEntity> list;

	return list.empty();
}

// ForwardList(const ForwardList& other);
bool copy_constructor_test()
{
	return false;
	//const TestingEntity testing_entity("key", 3);
	//int n = 3;
	//ForwardList<TestingEntity> other_list(n, testing_entity);

	//ForwardList<TestingEntity> list(other_list);
	//
	//return list.size() == other_list.size() &&
	//	   list.front().compare(other_list.front()) == 0 &&
	//	   &list.front() != &other_list.front();
}

// ForwardList(ForwardList&& other);
bool move_constructor_test()
{
	size_t n{ 3 };
	ForwardList<TestingEntity> list_a(n, TestingEntity("one", 1));
	zeroStats();

	auto list_b(std::move(list_a));

	return copied == 0 &&
		   list_b.size() == n;
}

// ForwardList(const size_t count, cref value);
bool filling_constructor_test()
{
	// zero amount test
	if (!ForwardList<int>(0, 1).empty())
		return false;

	// default test
	zeroStats();
	TestingEntity entity("key", 3);
	int n = 3;

	auto list = ForwardList<TestingEntity>(n, entity);

	return list.size() == n &&
		   list.front().compare(entity) == 0 &&
		   copied == n;
}

// ForwardList(const initializer_list& ilist)
bool init_list_constructor_test()
{
	ForwardList<int> list = {1, 2, 3};
	
	return list.size() == 3 &&
		   list.front() == 1;
}

///////////////////////////////////////////////////////////////////////////////
// DESTRUCTOR TESTS
bool destructor_test() 
{ 
	zeroStats();

	ForwardList<TestingEntity>(3, TestingEntity("key", 42));

	return deleted == created + copied; 
}

///////////////////////////////////////////////////////////////////////////////
// OPERATORS TESTS
//ForwardList& operator=(const ForwardList& other);
bool operator_copy_asign_test()
{
	return false;
	//const TestingEntity other_entity("key", 3);
	//int other_n = 3;
	//ForwardList<TestingEntity> other_list(other_n, other_entity);

	//ForwardList<TestingEntity> list(1, TestingEntity("hello", 331));

	//list = other_list;

	//return list.size() == other_list.size() &&
	//	   list.front().compare(other_entity) == 0;
}

//ForwardList& operator=(ForwardList&& other);
bool operator_move_asign_test()
{
	return false;
	//const TestingEntity testing_entity("key", 3);
	//int n = 3;

	//auto list = ForwardList<TestingEntity>(n, testing_entity);

	//return list.size() == n &&
	//	   list.front().compare(testing_entity) == 0;
}

//ForwardList& operator=(ilist ilist);
bool operator_asign_init_list_test()
{
	return false;
}


///////////////////////////////////////////////////////////////////////////////
// ELEMENT ACCESS TESTS
//ref front();
bool front_test() 
{	
	auto list = ForwardList<TestingEntity>();
	auto entity = TestingEntity("key", 42);
	list.pushFront(entity);

	return list.front().compare(entity) == 0; 
}

//cref front() const;
bool const_front_test() 
{ 
	size_t n = 2;
	auto entity = TestingEntity("key", 42);
	const ForwardList<TestingEntity> list(n, TestingEntity("key", 42));

	return list.front().compare(entity) == 0;
}

///////////////////////////////////////////////////////////////////////////////
// CAPACITY TESTS
//
//size_t size() const;
bool size_test() 
{
	if (ForwardList<int>().size() != 0) 
		return false;

	auto list = ForwardList<int>();
	list.pushFront(1);
	list.pushFront(1);

	return list.size() == 2;
}

//size_t maxSize() const;
bool max_size_test() { return false; }

//bool empty() const;
bool empty_test()
{
	if (!ForwardList<int>().empty())
		return false;

	auto list = ForwardList<int>(1, 1);
	list.popFront();

	return list.empty();
}

///////////////////////////////////////////////////////////////////////////////
//MODIFIERS TESTS
//void clear();
bool clear_test() { return false; }

//void pushFront(cref value);
bool push_front_test() 
{
	zeroStats();
	ForwardList<TestingEntity> list;
	TestingEntity entity("test1", 1);

	list.pushFront(entity);
	list.pushFront(entity);

	return list.size() == 2 &&
		   copied == 2 &&
		   list.front().compare(entity) == 0; 
}

//void pushFront(rvalref value);
bool push_front_rval_test() 
{
	zeroStats();
	ForwardList<TestingEntity> list;
	TestingEntity entity1("test1", 1);
	
	list.pushFront(std::move(entity1));
	list.pushFront(TestingEntity("test2", 2));

	return list.size() == 2 &&
		   moved == 2 &&
		   list.front().key() == "test2";
}

//void popFront();
bool pop_front_test() { 
	zeroStats();
	auto entity = TestingEntity("key", 42);
	ForwardList<TestingEntity> list(2, entity);
	list.popFront();
	list.popFront();
	list.popFront(); // extra pop to enshure correctness

	return list.empty() &&
		   deleted == 2;
}

//void resize(const size_t count);
bool resize_test() { return false; }

//void resize(const size_t count, cref value);
bool resize_fill_test() { return false; }

//void swap(ForwardList& other);
bool swap_test()
{
	return false;

	//int amount_a = 30;
	//const TestingEntity testing_entity_a("a", 42);
	//ForwardList<TestingEntity> list_a(amount_a, testing_entity_a);
	//int amount_b = 1;
	//const TestingEntity testing_entity_b("b", 24);
	//ForwardList<TestingEntity> list_b(amount_b, testing_entity_b);

	//list_a.swap(list_b);

	//if (list_a.front().compare(testing_entity_b) != 0 &&
	//	list_a.size() != amount_b)
	//	return false;

	//if (list_b.front().compare(testing_entity_a) != 0 &&
	//	list_b.size() != amount_a)
	//	return false;

	//return true;
}

///////////////////////////////////////////////////////////////////////////////
// OPERATIONS TESTS
//size_t remove(cref value);
bool remove_test() 
{ 
	zeroStats();
	auto list = ForwardList<int>();
	list.pushFront(1);
	list.pushFront(1);
	list.pushFront(2);
	list.pushFront(2);
	list.pushFront(1);
	list.pushFront(3);
	list.pushFront(4);
	list.pushFront(1);

	// todo iterate confirm
	return false;

	size_t removed = list.remove(1);
	return removed == 4;
}

//void reverse();
bool reverse_test() { return false; }

//void spliceAfter(citerator pos, ForwardList& other);
bool splice_after_test() { return false; }

//void spliceAfter(citerator pos, ForwardList&& other);
bool splice_after_rval_test() { return false; }

//size_t unique();
bool unique_test() { return false; }

int main() 
{
	std::cout << "[ CONSTRUCTOR TESTS ]\n";
	testNprint("ForwardList()", &constructor_test);
	//testNprint("ForwardList(const ForwardList & other)", &copy_constructor_test);
	testNprint("ForwardList(ForwardList && other)", &move_constructor_test);
	testNprint("ForwardList(const size_t count, cref value)", &filling_constructor_test);
	testNprint("ForwardList(const initializer_list& ilist)", &init_list_constructor_test);
	std::cout << "\n";

	std::cout << "[ DESTRUCTOR TESTS ]\n";
	testNprint("~ForwardList()", &destructor_test);
	std::cout << "\n";

	//std::cout << "[ OPERATORS TESTS ]\n";
	////testNprint("ForwardList& operator=(const ForwardList& other)", &operator_copy_asign_test);
	////testNprint("ForwardList& operator=(ForwardList&& other)", &operator_move_asign_test);
	////testNprint("ForwardList& operator=(ilist ilist)", &operator_asign_init_list_test);
	//std::cout << "\n";

	std::cout << "[ ELEMENT ACCESS TESTS ]\n";
	testNprint("ref front()", &front_test);
	testNprint("cref front() const", &const_front_test);
	std::cout << "\n";

	std::cout << "[ CAPACITY TESTS ]\n";
	testNprint("size_t size() const", &size_test);
	////testNprint("size_t maxSize() const", &max_size_test);
	testNprint("bool empty() const", &empty_test);
	std::cout << "\n";

	std::cout << "[ MODIFIERS TESTS ]\n";
	//testNprint("void clear()", &clear_test);
	testNprint("void pushFront(cref value)", &push_front_test);
	testNprint("void pushFront(rvalref value)", &push_front_rval_test);
	testNprint("void popFront()", &pop_front_test);
	////testNprint("void resize(const size_t count)", &resize_test);
	////testNprint("void resize(rvalref value)", &resize_fill_test);
	////testNprint("void swap(ForwardList& other)", &swap_test);
	std::cout << "\n";

	std::cout << "[ OPERATIONS TESTS ]\n";
	testNprint("size_t remove(cref value)", &remove_test);
	////testNprint("void reverse()", &reverse_test);
	////testNprint("void spliceAfter(citerator pos, ForwardList& other)", &splice_after_test);
	////testNprint("void spliceAfter(citerator pos, ForwardList&& other)", &splice_after_rval_test);
	////testNprint("size_t unique()", &unique_test);
	std::cout << "\n";
}