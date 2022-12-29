#include <iostream>

template <typename T>
class Control_block 
{
public:
	Control_block() = delete;

	explicit Control_block(T* ptr) :
		ptr_(ptr),
		ref_count_{ 0 },
		weak_ref_count_{ 0 }
	{
		if (ptr_) {
			ref_count_++;
		}
	}

	~Control_block() 
	{
		delete ptr_;
	}

	void kill() 
	{
		if (ref_count_ == 0 && weak_ref_count_ == 0)
		{
			delete this;
		}
	}

	void increment_ref_count() 
	{
		ref_count_++;
	}

	void increment_weak_ref_count()
	{
		weak_ref_count_++;
	}

	void decrement_ref_count() 
	{
		if (ref_count_-- == 1)
		{
			kill();
		}
	}

	void decrement_weak_ref_count()
	{
		if (weak_ref_count_ > 0)
		{
			weak_ref_count_--;
		}
	}

	unsigned int ref_count() const 
	{
		return ref_count_;
	}

	unsigned int weak_ref_count() const
	{
		return weak_ref_count_;
	}

private:
	T* ptr_;
	unsigned int ref_count_;
	unsigned int weak_ref_count_;
};

template <typename T> class Weak_ptr;

template <typename T>
class Shared_ptr 
{
	friend class Weak_ptr<T>;
	friend void weak_ptr_demo(); // for demo

public:
	Shared_ptr() 
	{
	}

	explicit Shared_ptr(T* ptr) :
		control_block_(new Control_block<T>(ptr)),
		ptr_(ptr) 
	{
	}

	Shared_ptr(Shared_ptr& other) : 
		control_block_(other.control_block_), 
		ptr_(other.ptr_)
	{
		control_block_->increment_ref_count();
	}

	~Shared_ptr() {
		control_block_->decrement_ref_count();
	}

	unsigned int use_count() const 
	{
		if (control_block_ != nullptr)
			return control_block_->ref_count();
		else
			return 0;
	}

	T* get() const 
	{
		return ptr_;
	}

	T& operator*() 
	{
		return *ptr_;
	}

	T* operator->() 
	{
		return ptr_;
	}

private:
	T* ptr_ = nullptr;
	Control_block<T>* control_block_ = nullptr;
};

template <typename T>
class Weak_ptr
{
public:
	Weak_ptr()
	{
	}

	explicit Weak_ptr(const Shared_ptr<T>& shared_ptr) :
		control_block_(shared_ptr.control_block_),
		ptr_(shared_ptr.ptr_)
	{
		control_block_->increment_weak_ref_count();
	}

	~Weak_ptr()
	{
		control_block_->decrement_weak_ref_count();
	}

	unsigned int use_count() const 
	{
		if (control_block_ != nullptr)
		{
			return control_block_->ref_count();
		}
		else
		{
			return 0;
		}
	}

	bool expired() const 
	{
		return ptr_ ? false : true;
	}

	Shared_ptr<T>& lock() 
	{
		if (expired()) 
		{
			return Shared_ptr<T>(); // will fail as not implemented valid constructor
		}
		else
		{
			Shared_ptr<T> tmp;
			tmp.ptr_ = ptr_;
			tmp.control_block_ = control_block_;
			control_block_->increment_ref_count();
			return tmp;
		}
	}

	void operator = (Shared_ptr<T> shared)
	{
		ptr_ = shared.ptr_;
		control_block_ = shared.control_block_;
		control_block_->increment_weak_ref_count();
	}

private:
	T* ptr_ = nullptr;
	Control_block<T>* control_block_ = nullptr;
};


///////////////////////////////////////////////////////////////////////////////
// Shared_ptr demo															 //
///////////////////////////////////////////////////////////////////////////////
void shared_ptr_demo()
{
	std::cout << "SHARED_PTR DEMO:\n";

	Shared_ptr<int> sp1 = Shared_ptr<int>(new int(42));
	std::cout << "sp1 value: " << *sp1 << ", usages: " << sp1.use_count() << '\n';
	{
		auto sp2(sp1);
		std::cout << "SCOPE BEGIN\n";
		std::cout << "\tsp2 value: " << *sp2 << ", usages: " << sp2.use_count() << '\n';
		std::cout << "\tset new sp2 value 24\n";
		*sp2 = 24;
		std::cout << "SCOPE END\n";
	}
	std::cout << "sp1 value: " << *sp1 << ", usages: " << sp1.use_count() << '\n';
}

///////////////////////////////////////////////////////////////////////////////
// Weak_ptr demo															 //
// example from https://en.cppreference.com/w/cpp/memory/weak_ptr/use_count  //
///////////////////////////////////////////////////////////////////////////////
void weak_ptr_demo()
{
	std::cout << "WEAK_PTR DEMO:\n";
	auto shared_ptr = Shared_ptr<int>(new int(42));
	std::cout << "shared_ptr value: " << *shared_ptr 
		<< ", usages: " << shared_ptr.use_count() 
		<< ", weak usages("<< shared_ptr.control_block_->weak_ref_count() <<")\n";
	{
		std::cout << "SCOPE BEGIN\n";

		Weak_ptr<int> wp(shared_ptr);
		std::cout << "\tCreated Weak_ptr \n";
		std::cout << "\tsp value: " << *shared_ptr 
			<< ", usages: " << shared_ptr.use_count() 
			<< ", weak usages(" << shared_ptr.control_block_->weak_ref_count() << ")\n";

		auto shared_ptr_2(wp.lock());
		std::cout << "\tCreated Shared_ptr from Weak_ptr\n";
		std::cout << "\tsp2 value: " << *shared_ptr_2 
			<< ", usages: " << shared_ptr_2.use_count() 
			<< ", weak usages(" << shared_ptr_2.control_block_->weak_ref_count() << ")\n";

		std::cout << "SCOPE END\n";
	}
	std::cout << "shared_ptr value: " << *shared_ptr << ", usages: " << shared_ptr.use_count() << ", weak usages(" << shared_ptr.control_block_->weak_ref_count() << ")\n";
}

int main() 
{
	shared_ptr_demo();

	std::cout << '\n';

	weak_ptr_demo();
}