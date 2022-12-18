#ifndef CODEREVIEWTASK_MYVECTOR_HPP
#define CODEREVIEWTASK_MYVECTOR_HPP

#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>

/*
 * MyVector stores a collection of objects with their names.
 *
 * For each object T, MyVector stores T`s name as std::string.
 * Several objects can have similar name.
 * operator[](const std::string& name) should return the first object
 * with the given name.
 *
 * Your task is to find as many mistakes and drawbacks in this code
 * (according to the presentation) as you can.
 * Annotate these mistakes with comments.
 *
 * Once you have found all the mistakes, rewrite the code
 * so it would not change its original purpose
 * and it would contain no mistakes.
 * Try to make the code more efficient without premature optimization.
 *
 * You can change MyVector interface completely, but there are several rules:
 * 1) you should correctly and fully implement copy-on-write idiom.
 * 2) std::pair<const T&, const std::string&> operator[](int index) const must take constant time at worst.
 * 3) const T& operator[](const std::string& name) const should be present.
 * 4) both operator[] should have non-const version.
 * 5) your implementation should provide all the member types of std::vector.
 * 6) your implementation should provide the following functions:
 *    1) begin(), cbegin(), end(), cend()
 *    2) empty(), size()
 *    3) reserve(), clear()
 */

template <typename T>
class MyVector : public std::vector<T> // Aggregation over inheritance principle. vector<T> should be private variable
{
public:
    MyVector()  // See variables comments (line 112)
    {
        m_ref_ptr = new size_t(1);  // Smart-pointers should be used instead of raw pointers 
        m_names = new std::vector<std::string>();   // std::make_shared<vector<...>>() should be used
    }

    MyVector(const MyVector& other) // See COW comments (line 112)
        : std::vector<T>(other),
        m_ref_ptr(other.m_ref_ptr),
        m_names(other.m_names)
    {
        (*m_ref_ptr)++;
    }

    ~MyVector() // No need of manualy freeing memory with smart pointers
    {
        if (-- * m_ref_ptr == 0)
        {
            delete m_ref_ptr;
            delete m_names;
        }
    }

    void push_back(const T& obj, const std::string& name)
    {
        copy_names();

        std::vector<T>::push_back(obj);
        m_names->push_back(name);
    }

    std::pair<const T&, const std::string&> operator[](int index) const // With pair<const T&, const string&> it's impossible to correctly overload operator[] with correct COW implementation.
    {
        if (index >= std::vector<T>::size())    // Øndex shoud have size_t type (int is signed)
        {
            throw new std::out_of_range("Index is out of range"); // Generally flow control via exceptions is bad practice
        }

        return std::pair<const T&, const std::string&>(std::vector<T>::operator[](index), (*m_names)[index]); // Line length shoud not exceed 80 columns
    }

    const T& operator[](const std::string& name) const
    {
        std::vector<std::string>::const_iterator iter = std::find(m_names->begin(), m_names->end(), name); // Line length shoud not exceed 80 columns
        if (iter == m_names->end())
        {
            throw new std::invalid_argument(name + " is not found in the MyVector"); // See previous comments
        }

        return std::vector<T>::operator[](iter - m_names->begin());
    }

private:
    void copy_names() // See COW comments (line 112)
    {
        if (*m_ref_ptr == 1)
        {
            return;
        }

        size_t* temp_ref_ptr = new size_t(1);
        std::vector<std::string>* temp_names = new std::vector<std::string>(*m_names);

        (*m_ref_ptr)--;
        m_ref_ptr = temp_ref_ptr;
        m_names = temp_names;
    }

private:
    /*
        To correctly implement COW we must control state change of all data.
        With implementation like this (inherited vector + agregated names) it would be impossible.
        Vector of pairs(std::<pair<T, string>>) also could not be used as struct is default public members.
        So we could not controll change of internal state of data.
    */
    std::vector<std::string>* m_names;  // shared_pointer<vector<>> shoud be used. 
    size_t* m_ref_ptr;  // shared_pointer<vector<...>>'s unique() can be used
};


#endif //CODEREVIEWTASK_MYVECTOR_HPP