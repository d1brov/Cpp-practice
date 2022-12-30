#ifndef  FORWARD_LIST_TESTS_H
#define  FORWARD_LIST_TESTS_H

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
	TestingEntity(const std::string& key, int value) :
		key_(key),
		value_(value)
	{
		created++;
	}

	TestingEntity(const TestingEntity& other) :
		key_{ other.key_ },
		value_{ other.value_ }
	{
		copied++;
	}

	TestingEntity(TestingEntity&& other) :
		key_(std::move(other.key_)),
		value_{ other.value_ }
	{
		moved++;
	}

	~TestingEntity()
	{
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

bool isEqual(ForwardList<TestingEntity>& a, ForwardList<TestingEntity>& b)
{
	if (a.size() != b.size())
		return false;

	auto a_it = a.begin();
	auto b_it = b.begin();

	while (a_it != nullptr)
	{
		if ((*a_it).compare(*b_it) != 0)
			return false;

		a_it++;
		b_it++;
	}

	return true;
}

void testNprint(const std::string& name, const std::function<bool()>& test)
{
	std::cout << name << std::setw(60 - name.size()) <<
		(test() ? "PASSED" : "FAILED") << '\n';
}

///////////////////////////////////////////////////////////////////////////////
// CONSTRUCTORS TESTS

// TEST ForwardList()
bool constructor_test()
{
	ForwardList<TestingEntity> list;

	return list.empty();
}

// TEST ForwardList(const ForwardList<T>& other)
bool copy_constructor_test()
{
	ForwardList<TestingEntity> list = { {"one", 1}, {"two", 2}, {"three", 3} };

	zeroStats();
	auto copied_list(list);

	return copied == 3 && isEqual(list, copied_list);
}

// TEST ForwardList(ForwardList&& other)
bool move_constructor_test()
{
	size_t n{ 3 };
	ForwardList<TestingEntity> list_a(n, TestingEntity("one", 1));
	zeroStats();

	auto list_b(std::move(list_a));

	return copied == 0 &&
		list_b.size() == n;
}

// TEST ForwardList(const size_t count, cref value)
bool filling_constructor_test()
{
	if (!ForwardList<int>(0, 1).empty())
		return false;

	zeroStats();
	TestingEntity entity("key", 3);
	int n = 3;

	auto list = ForwardList<TestingEntity>(n, entity);

	return list.size() == n &&
		list.front().compare(entity) == 0 &&
		copied == n;
}

// TEST ForwardList(const initializer_list& ilist).
bool init_list_constructor_test()
{
	ForwardList<int> list = { 1, 2, 3 };

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

// TEST ForwardList& operator=(const ForwardList& other)
bool operator_copy_asign_test()
{
	ForwardList<TestingEntity> list = { {"one", 1}, {"two", 2}, {"three", 3} };
	ForwardList<TestingEntity> copy_asigned_list;

	zeroStats();
	copy_asigned_list = list;

	return copied == 3 && copy_asigned_list.size() == 3;
}

// TEST ForwardList& operator=(ForwardList&& other)
bool operator_move_asign_test()
{
	ForwardList<TestingEntity> move_asigned_list;

	zeroStats();
	move_asigned_list = ForwardList<TestingEntity>(3, { "one", 1 });

	// IDK how to check that move operator= was called
	// so did manual check
	return move_asigned_list.size() == 3;
}


///////////////////////////////////////////////////////////////////////////////
// ELEMENT ACCESS TESTS
// TEST ref front()
bool front_test()
{
	auto list = ForwardList<TestingEntity>();
	auto entity = TestingEntity("key", 42);
	list.pushFront(entity);

	return list.front().compare(entity) == 0;
}

// TEST cref front() const.
bool const_front_test()
{
	size_t n = 2;
	auto entity = TestingEntity("key", 42);
	const ForwardList<TestingEntity> list(n, TestingEntity("key", 42));

	return list.front().compare(entity) == 0;
}

///////////////////////////////////////////////////////////////////////////////
// CAPACITY TESTS

// TEST size_t size() const.
bool size_test()
{
	if (ForwardList<int>().size() != 0)
		return false;

	auto list = ForwardList<int>();
	list.pushFront(1);
	list.pushFront(1);

	return list.size() == 2;
}

// TEST bool empty() const.
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

// TEST void pushFront(cref value)
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

// TEST void pushFront(rvalref value)
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

// TEST void popFront().
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

///////////////////////////////////////////////////////////////////////////////
// OPERATIONS TESTS

// TEST size_t remove(cref value)
bool remove_test()
{
	zeroStats();
	ForwardList<int> list = { 1,1,2,2,1,3,4,1 };

	size_t removed = list.remove(1);

	for (auto it : list)
		if (it == 1)
			return false;

	return removed == 4;
}

// TEST ForwardList<T>& splitWhen(split_predicate condition).
bool split_when_test()
{
	TestingEntity split_entity("three", 3);

	// also tested with border indexes
	ForwardList<TestingEntity> list = {
		{"one", 1}, {"two", 2}, split_entity, {"four", 4}, {"five", 5} };

	zeroStats();

	auto tail = list.splitWhen(
		[&split_entity](const TestingEntity& entity) {
			return entity.compare(split_entity) == 0;
		});

	return list.size() == 2 &&
		tail.size() == 3 &&
		(*tail.begin()).compare(split_entity) == 0 &&
		copied == 0;
}

///////////////////////////////////////////////////////////////////////////////
// ITERATIONS TESTS

// TEST iterator begin()
bool begin_test()
{
	ForwardList<int> list = { 1, 2, 3,4 };

	auto it = list.begin();
	auto value = *it;
	return value == 1;
}

// TEST iterator end()
bool end_test()
{
	ForwardList<int> list = { 1 };
	auto first = list.begin();

	return ++first == list.end();
}

// TEST iterating cycle for
bool interating_for_test()
{
	auto init_list = { 1, 2, 3, 4, 5 };
	auto ilist_it = init_list.begin();

	ForwardList<int> list = init_list;

	for (auto it : list) {
		if (it != *ilist_it++)
			return false;
	}

	return true;
}

#endif // FORWARD_LIST_TESTS_H