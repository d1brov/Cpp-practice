#ifndef  FORWARD_LIST_H
#define  FORWARD_LIST_H

///////////////////////////////////////////////////////////////////////////////
// Simplified implementation of std::forward_list :							 //
//					https://en.cppreference.com/w/cpp/container/forward_list //
///////////////////////////////////////////////////////////////////////////////
#include <utility>
#include <initializer_list>
#include <functional>

template <typename T>
class ForwardList {

	using ref = T&;
	using cref = const T&;
	using ptr = T*;
	using rvalref = T&&;
	using initializer_list = std::initializer_list<T>;
	using split_predicate = std::function<bool(cref value)>;
	
	struct Node {
		T data_;
		Node* next_;

		Node() = default;

		explicit Node(cref data) :
			data_(data),
			next_{ nullptr }
		{}

		explicit Node(rvalref data) :
			data_(std::move(data)),
			next_{ nullptr }
		{}

		//template<class... Args>
		//const Node(Args&&... args) : data{ std::forward<Args>(args)... } {}
	};

public:
	template <typename T>
	class Iterator {
	public:
		Iterator(Node* node) :
			node_(node)
		{}

		T& operator*() { return node_->data_; }

		T* operator->() { return &(node_->data_); }

		Iterator<T>& operator++()
		{
			node_ = node_->next_;
			return *this;
		}

		Iterator<T> operator++(int)
		{
			Iterator<T> it = *this;
			++*this;
			return it;
		}

		bool operator==(Iterator<T> other) const { return node_ == other.node_; }

		bool operator!=(Iterator<T> other) const { return node_ != other.node_; }

	private:
		Node* node_;
	};
	using iterator = Iterator<T>;
	using citerator = Iterator<T const>;

public:
	// Constructors
	ForwardList();
	ForwardList(const ForwardList<T>& other);
	ForwardList(ForwardList<T>&& other);
	ForwardList(const size_t count, cref value);
	ForwardList(const initializer_list& ilist);

	// Destructor
	~ForwardList();

	// Operators
	ForwardList& operator=(const ForwardList& other);
	ForwardList& operator=(ForwardList&& other);

	// Element access
	ref front();
	cref front() const;

	// Capacity
	size_t size() const;
	bool empty() const;

	// Modifiers
	void clear();
	void pushFront(cref value);
	void pushFront(rvalref value);
	void popFront();

	// Operations
	size_t remove(cref value);
	ForwardList<T> splitWhen(split_predicate condition);

	// Iteration operations
	iterator begin();
	iterator end();

private:
	Node* head_;
	size_t size_{};
};

//////////////////
// Constructors //
//////////////////
template<typename T>
ForwardList<T>::ForwardList() : 
	head_{ nullptr },
	size_{ 0 } 
{
}

template<typename T>
ForwardList<T>::ForwardList(const ForwardList<T>& other)
{
	ForwardList();

	if (other.size_ > 0)
	{
		head_ = new Node(other.head_->data_);
		Node* it = head_;
		size_++;

		
		Node* other_it = other.head_->next_;
		while (other_it)
		{
			it->next_ = new Node(other_it->data_);
			size_++;

			it = it->next_;
			other_it = other_it->next_;
		}
	}
}

template<typename T>
ForwardList<T>::ForwardList(ForwardList<T>&& other) :
	head_{other.head_},
	size_{other.size_}
{
	other.head_ = nullptr;
	other.size_ = 0;
}

template<typename T>
ForwardList<T>::ForwardList(const size_t count, cref value) : 
	head_{ nullptr },
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

////////////////
// Destructor //
////////////////
template<typename T>
ForwardList<T>::~ForwardList()
{
	clear();
}

///////////////
// Operators //
///////////////

template<typename T>
ForwardList<T>& ForwardList<T>::operator=(const ForwardList<T>& other)
{
	ForwardList<T> tmp(other);
	std::swap(tmp.head_, head_);
	size_ = other.size_;
	return *this;
}

template<typename T>
ForwardList<T>& ForwardList<T>::operator=(ForwardList<T>&& other)
{
	/*
		Move assignment operator. Replaces the contents with those of other 
		using move semantics (i.e. the data in other is moved from other into 
		this container). other is in a valid but unspecified state afterwards.
		https://en.cppreference.com/w/cpp/container/forward_list/operator%3D
	*/

	std::swap(head_, other.head_);
	std::swap(size_, other.size_);
	return *this;
}

////////////////////
// Element access //
////////////////////
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

//////////////
// Capacity //
//////////////
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

///////////////
// Modifiers //
///////////////
template<typename T>
void ForwardList<T>::clear()
{
	while (head_)
	{
		popFront();
	}
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

////////////////
// Operations //
////////////////
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

template<typename T>
ForwardList<T> ForwardList<T>::splitWhen(split_predicate condition)
{
	ForwardList<T> tmp;
	tmp.head_ = head_;
	tmp.size_ = size_;

	Node* previous = nullptr, *current = head_;
	size_t head_len{ 0 };
	
	while (current)
	{
		if (condition(current->data_))
		{
			if(previous)
				previous->next_ = nullptr;

			
			tmp.head_ = current;
			tmp.size_ = size_ - head_len;
			size_ = head_len;
			break;
		}
		head_len++;
		previous = current;
		current = current->next_;
	}

	return tmp;
}

//////////////////////////
// Iteration operations //
//////////////////////////

template<typename T>
typename::ForwardList<T>::iterator ForwardList<T>::begin()
{
	return Iterator<T>(head_);
}

template<typename T>
typename::ForwardList<T>::iterator ForwardList<T>::end()
{
	return Iterator<T>(nullptr);
}

#endif // FORWARD_LIST_H