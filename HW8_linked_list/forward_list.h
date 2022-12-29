#ifndef  FORWARD_LIST_H
#define  FORWARD_LIST_H

/*
	+- a) Основні операції зі списком: конструктор, вставка елементів на початку
	списку, ітерація по списку, розмір списку, видалення елементів.

	+- b) Move-операції (move-конструктор, move-присвоєння)

	+ c) Метод для вставки елементів на початку повинен мати перевантаження для
	посилання rvalue і правильно обробляти move.

	+ d) Створення списку за допомогою конструктора, який приймає
	std::ilist.

	+ e) Список повинен бути шаблонним і працювати не тільки з примітивними
	значеннями, а й з об'єктами (і правильно звільняти пам'ять)

	+ f) В ідеалі ви повинні прагнути до інтерфейсу класу на кшталт std::forward_list,
	але вам не потрібно реалізовувати абсолютно всі методи - кастомний
	конструктор і методи emplace_* не обов'язкові (але це не буде мінусом).
	Сортування, merge, та unique робити не потрібно.

	- g) БОНУСНІ БАЛИ: реалізуйте метод split_when, який дозволяє вам розділити
	список на два, що визначає позицію, з якої елемент розділити, на основі
	логічного значення, що повертається переданою функцією предикату
*/

///////////////////////////////////////////////////////////////////////////////
// Simplified implementation of std::forward_list :							 //
//					https://en.cppreference.com/w/cpp/container/forward_list //
///////////////////////////////////////////////////////////////////////////////
#include <utility>
#include <initializer_list>
#include <iterator>

template <typename T>
class ForwardList {
public:
	using ref = T&;
	using cref = const T&;
	using rvalref = T&&;
	using initializer_list = std::initializer_list<T>;

	// Constructors
	ForwardList();
	//ForwardList(const ForwardList& other);
	ForwardList(ForwardList&& other);
	ForwardList(const size_t count, cref value);
	ForwardList(const initializer_list& ilist);

	// Destructor
	~ForwardList();

	// Operators
	//ForwardList& operator=(const ForwardList& other);
	//ForwardList& operator=(ForwardList&& other);
	//ForwardList& operator=(ilist ilist);

	// Element access
	ref front();
	cref front() const;
	/*
	//iterator beforeBegin();
	//citerator beforeBegin() const;
	//citerator cbeforeBegin() const;
	//iterator begin();
	//citerator begin() const;
	//citerator cbegin() const;
	//iterator end();
	//citerator end() const;
	//citerator cend() const;
	*/

	// Capacity
	size_t size() const;
	//size_t maxSize() const;
	bool empty() const;

	// Modifiers
	void clear();
	void pushFront(cref value);
	void pushFront(rvalref value);
	void popFront();
	//void resize(const size_t count);
	//void resize(const size_t count, cref value);
	void swap(ForwardList& other);
	/*
	//iterator insertAfter(citerator pos, cref value);
	//iterator insertAfter(citerator pos, rvalref value);
	//iterator insertAfter(citerator pos, size_t count, const T& value);
	//iterator insertAfter(citerator pos, ilist ilist);
	//iterator eraseAfter(citerator pos);
	//iterator eraseAfter(citerator first, citerator last);
	*/

	// Operations
	size_t remove(cref value);
	//void reverse();
	//void spliceAfter(citerator pos, ForwardList& other);
	//void spliceAfter(citerator pos, ForwardList&& other);
	//size_t unique();

private:
	struct Node {
		T data_;
		Node* next_;

		Node() = default;

		Node(cref data) :
			data_{ data }
		{}

		Node(rvalref data) :
			data_(std::move(data))
		{}

		/*Node(const Node& other) :
			data_(std::move(other.data_))
		{}*/

		//Node(Node&& other) : 
		//	data_(std::move(other.data_)),
		//	next_(std::move(other.next_)) 
		//{}

		//template<class... Args>
		//const Node(Args&&... args) : data{ std::forward<Args>(args)... } {}
	};

	Node* head_;
	Node* tail_;
	size_t size_{};
};
///////////////////////////////////////////////////////////////////////////////

// Constructors
template<typename T>
ForwardList<T>::ForwardList() : 
	head_{ nullptr },
	tail_{ nullptr }, 
	size_{ 0 } 
{
}

//template<typename T>
//ForwardList<T>::ForwardList(const ForwardList& other)
//{
//	if (!other.empty()) {
//		size_ = other.size_;
//		auto current_node = new Node(other.head_->data);
//		auto current_other_node = other.head_;
//		head_ = current_node;
//		while (current_other_node->next != nullptr) {
//			current_node->next = new Node(current_other_node->next->data);
//			current_node = current_node->next;
//			current_other_node = current_other_node->next;
//		}
//		tail_ = current_node;
//		tail_->next = nullptr;
//	}
//	else {
//		ForwardList();
//	}
//}

template<typename T>
ForwardList<T>::ForwardList(ForwardList&& other) :
	head_{other.head_},
	tail_{other.tail_ },
	size_{other.size_}
{
	other.head_ = nullptr;
	other.tail_ = nullptr;
	other.size_ = 0;
}

template<typename T>
ForwardList<T>::ForwardList(const size_t count, cref value) : 
	head_{ nullptr },
	tail_{ nullptr },
	size_{ 0 }
{
	if (count > 0)
	{
		for (size_t i{ 0 }; i < count; i++)
			pushFront(value);
	}
}

template<typename T>
ForwardList<T>::ForwardList(const initializer_list& ilist)
{
	for (auto it = std::rbegin(ilist); it != std::rend(ilist); it++)
	{
		pushFront(*it);
	}
}

///////////////////////////////////////////////////////////////////////////////
// Destructor
template<typename T>
ForwardList<T>::~ForwardList()
{
	while (head_)
	{
		popFront();
	}
}

///////////////////////////////////////////////////////////////////////////////
// Operators
//template<typename T>
//ForwardList<T>& ForwardList<T>::operator=(const ForwardList& other)
//{
//	// todo replace swap with copy constructor?
//
//	ForwardList temp_list(other);
//	temp_list.swap(*this);
//	return *this;
//}
//
//template<typename T>
//ForwardList<T>& ForwardList<T>::operator=(ForwardList&& other)
//{
//	// todo check, todo replace swap with copy constructor?
//
//	/*
//		Move assignment operator. Replaces the contents with those of other 
//		using move semantics (i.e. the data in other is moved from other into 
//		this container). other is in a valid but unspecified state afterwards.
//		https://en.cppreference.com/w/cpp/container/forward_list/operator%3D
//	*/
//
//	//other.swap(*this);
//	//
//	//// free other
//	//if (!other.head_) {
//	//	return; 
//	//}
//	//Node* current = other.head_;
//	//while (current != nullptr)
//	//{
//	//	Node* next = current->next;
//	//	delete current;
//	//	current = next;
//	//}
//	//other.head_ = nullptr;
//	//other.tail_ = nullptr;
//	//other.size_ = 0;
//
//
//	return *this;
//}

///////////////////////////////////////////////////////////////////////////////
// Element access
template<typename T>
inline typename::ForwardList<T>::ref ForwardList<T>::front()
{
	return head_->data_;
}

template<typename T>
inline typename::ForwardList<T>::cref ForwardList<T>::front() const
{
	return head_->data_;
}

///////////////////////////////////////////////////////////////////////////////
// Capacity
template<typename T>
inline size_t ForwardList<T>::size() const
{
	return size_;
}

template<typename T>
inline bool ForwardList<T>::empty() const
{
	return size_ ? false : true;
}

///////////////////////////////////////////////////////////////////////////////
// Modifiers
template<typename T>
void ForwardList<T>::clear()
{
	if (head_ == nullptr)
	{
		return;
	}

	for (auto current{ head_ }; current != nullptr; ) 
	{
		auto tmp{ current };
		current = current->next_;
		delete tmp;
	}

	head_ = nullptr;
	tail_ = nullptr;
	size_ = 0;
}

template<typename T>
void ForwardList<T>::pushFront(cref value)
{
	// todo DRY?
	auto new_node = new Node(value);
	new_node->next_ = head_;
	head_ = new_node;
	size_++;
}

template<typename T>
void ForwardList<T>::pushFront(rvalref value)
{
	// todo DRY?
	auto new_node = new Node(std::move(value));
	new_node->next_ = head_;
	head_ = new_node;
	size_++;
}
 
template<typename T>
void ForwardList<T>::popFront()
{
	if (head_)
	{
		Node* tmp = head_;
		head_ = head_->next_;
		delete tmp;
		size_--;
	}
}

//void resize(size_t count);
//void resize(size_t count, const T& value);

template<typename T>
void ForwardList<T>::swap(ForwardList& other)
{
	std::swap(head_, other.head_);
	std::swap(tail_, other.tail_);
	std::swap(size_, other.size_);
	//Node* temp_node;
	//temp_node = head_;
	//head_ = other.head_;
	//other.head_ = temp_node;
	//temp_node = nullptr;

	//Node* temp_tail;
	//temp_tail = tail_;
	//tail_ = other.tail_;
	//other.tail_ = temp_tail;
	//temp_tail = nullptr;

	//std::swap(size_, other.size_);
}

///////////////////////////////////////////////////////////////////////////////
// Operations
template<typename T>
size_t ForwardList<T>::remove(cref value)
{
	size_t removed_counter {0};

	while (head_->data_ == value) {
			popFront();
			removed_counter++;
	}

	Node* current = head_;
	while (current->next_)
	{
		if (current->next_->data_ == value)
		{
			Node* tmp = current->next_;
			current->next_ = current->next_->next_;
			delete tmp;
			removed_counter++;
		}
		else
		{
			current = current->next_;
		}
	}

	return removed_counter;
}

//void reverse();
//void spliceAfter(citerator pos, ForwardList& other);
//void spliceAfter(citerator pos, ForwardList&& other);
//size_t unique();
#endif // ! FORWARD_LIST_H
