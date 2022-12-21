#pragma once

/*
 * MyVector stores a collection of objects with their names.
 *
 * For each object T, MyVector stores T`s name as std::string.
 * Several objects can have similar name.
 * operator[](const std::string& name) should return the first object with the given name.
 * Try to make the code more efficient without premature optimization.
 *
 * You can change MyVector interface completely, but there are several rules:
 * 1) you should correctly and fully implement copy-on-write idiom.
 * 2) std::pair<const T&, const std::string&> operator[](int index) const must take constant time at worst.
 * 3) const T& operator[](const std::string& name) const should be present.
 * 4) both operator[] should have non-const version.
 *
 * 5) your implementation should provide all the member types of std::vector.
 * 6) your implementation should provide the following functions:
 *    1) begin(), cbegin(), end(), cend()
 *    2) empty(), size()
 *    3) reserve(), clear()
 */

#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <memory>

#include "NamedObject.h"

template <typename T>
class CowVector{

    using named_objects_vector = std::vector<NamedObject<T>>;

    template <typename T>
    friend class NamedObject;

private:
    
    std::shared_ptr<named_objects_vector> m_vector_ptr;

    /* COW's detach */
    void detach();

public:
    /* --- Constructors --- */
    CowVector();

    CowVector(const CowVector& other);



    /* --- Iterator methods --- */
    typename named_objects_vector::iterator begin();

    typename named_objects_vector::const_iterator ñbegin();

    typename named_objects_vector::iterator end();

    typename named_objects_vector::const_iterator ñend();



    /*--- Vector methods ---*/
    //push_back*/
    void push_back(const T& obj, const std::string& name);

    // empty() of internal vector
    bool empty() const;

    // size() of internal vector
    size_t size() const;

    // reserve() of internal vector. Triggers detach.
    void reserve(size_t new_size);
    
    // clear() of internal vector. Triggers detach.
    void clear();



    /* --- Operator overloads --- */
    // Non-const operator[] by index. Triggers detach.
    NamedObject<T>& operator[](size_t index);

    //Const& operator[] by index
    const NamedObject<T>& operator[](size_t index) const;

    // Non-const operator[] by object name. Triggers detach.
    NamedObject<T>& operator[](const std::string& name);

    // Const operator[] by object name
    const NamedObject<T>& operator[](const std::string& name) const;
};


///////////////////////////////////////////
// IMPLEMENTATION SHOULD BE IN .cpp FILE //
///////////////////////////////////////////

/* --- COW's detach --- */
template<typename T>
void CowVector<T>::detach() {
    named_objects_vector* tmp = m_vector_ptr.get();
    if (!m_vector_ptr.unique())
    {
        m_vector_ptr = std::make_shared<named_objects_vector>(*tmp);
        return;
    }
}



/* --- Constructors --- */
template<typename T>
CowVector<T>::CowVector()
{
    m_vector_ptr = std::make_shared<named_objects_vector>();
}

template<typename T>
CowVector<T>::CowVector(const CowVector& other) : 
    m_vector_ptr(other.m_vector_ptr)
{
}



/* --- Iterator methods --- */
template<typename T>
typename CowVector<T>::named_objects_vector::iterator CowVector<T>::begin() {
    return m_vector_ptr->begin();
}

template<typename T>
typename CowVector<T>::named_objects_vector::iterator CowVector<T>::end() {
    return m_vector_ptr->end();
}

template<typename T>
typename CowVector<T>::named_objects_vector::const_iterator CowVector<T>::ñbegin() {
    return m_vector_ptr->cbegin();
}

template<typename T>
typename CowVector<T>::named_objects_vector::const_iterator CowVector<T>::ñend() {
    return m_vector_ptr->cend();
}



/* --- Vector methods --- */
template<typename T>
void CowVector<T>::push_back(const T& obj, const std::string& name) {
    detach();
    m_vector_ptr->push_back(NamedObject<T>(obj, name));
}

template<typename T>
bool CowVector<T>::empty() const {
    return size() == 0;
}

template<typename T>
size_t CowVector<T>::size() const {
    return m_vector_ptr->size();
}

template<typename T>
void CowVector<T>::reserve(size_t new_size) {
    detach();
    m_vector_ptr->reserve(new_size);
}

template<typename T>
void CowVector<T>::clear() {
    detach();
    m_vector_ptr->clear();
}



/* --- Operator overloads --- */
template<typename T>
NamedObject<T>& CowVector<T>::operator[](size_t index) {
    if (index >= m_vector_ptr->size()) {
        throw new std::out_of_range("Index is out of range");
    }

    /* 
        Detach happens on return non-const reference.
        Optimisation possible by detaching in NamedObject's setters
        or implementing Observer pattern.
    */
    detach();

    return m_vector_ptr->operator[](index);
}

template<typename T>
const NamedObject<T>& CowVector<T>::operator[](size_t index) const {
    if (index >= m_vector_ptr->size()) {
        throw new std::out_of_range("Index is out of range");
    }

    return m_vector_ptr->operator[](index);
}

template<typename T>
NamedObject<T>& CowVector<T>::operator[](const std::string& name) {
    typename named_objects_vector::iterator it = std::find_if(
        m_vector_ptr->begin(),
        m_vector_ptr->end(),
        [name](NamedObject<T>& obj) {
            return obj.getName() == name;
        });

    if (it == m_vector_ptr->end()) {
        throw std::invalid_argument("Object with name \""+name+"\" cannot be found");   // It's bad practice to use exceptions as program logic
    }

    /* 
        Detach happens on return non-const reference.
        Optimisation possible by detaching in NamedObject's setters
        or implementing Observer pattern.
    */
    std::size_t found_index = it - m_vector_ptr->begin();
    detach();

    return m_vector_ptr->operator[](found_index);
}

template<typename T>
const NamedObject<T>& CowVector<T>::operator[](const std::string& name) const {
    typename named_objects_vector::const_iterator it = std::find_if(
        m_vector_ptr->begin(),
        m_vector_ptr->end(),
        [name](const NamedObject<T>& obj) {
            return obj.get_name() == name;
        });

    if (it == m_vector_ptr->end()) {
        throw std::invalid_argument("Object with name \"" + name + "\" cannot be found");   // It's bad practice to use exceptions as program logic
    }

    return it->get_object();
}