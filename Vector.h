#ifndef MYVECTOR_H
#define MYVECTOR_H

#include <cstddef>
#include <type_traits>
#include <initializer_list>
#include <stdexcept>
#include <algorithm>
//#include <limits>

#include "Utils.h"

namespace my_vector {

template <typename T>
class Iterator : public std::iterator<std::input_iterator_tag, T> {
public:
    Iterator(T* p) : ptr(p) {}

    Iterator(const Iterator& it) : ptr(it.ptr) {}

    bool operator!=(Iterator const& other) const;
    bool operator==(Iterator const& other) const;
    T& operator*() const;
    Iterator& operator++();

private:
    T* ptr;

};

template <typename T>
class Vector
{
private:
    T* data_ = nullptr;
    size_t size_ = 0;
    size_t capacity_ = 0;

    void expand();
    void shrink();
    void clear_data();
    void transport_data(int new_capacity);
public:

    typedef Iterator<T> iterator;
    typedef Iterator<const T> const_iterator;

    Vector() noexcept = default;
    Vector(size_t count, const T& value);
    explicit Vector(size_t count);
    Vector(const Vector& other);
    Vector(Vector&& other) noexcept;
    Vector(std::initializer_list<T> init);

    // template< class RandomAcIt >
    // Vector( RandomAcIt first, RandomAcIt last);



    Vector& operator=(const Vector& other);
    Vector& operator=(Vector&& other);
    Vector& operator=(std::initializer_list<T> ilist);

    void assign( size_t count, const T& value );
    // template< class RandomAcIt >
    // void assign( RandomAcIt first, RandomAcIt last );
    void assign( std::initializer_list<T> ilist );

    ~Vector();

    void reserve(size_t new_capacity);
    void shrink_to_fit();
    void clear();
    void resize(size_t new_size);
    void resize(size_t new_size, const T& new_val);
    void swap(Vector& other);


    void push_back(const T& new_value);
    template <typename ...Args>
    T& emplace_back(const Args& ...args);
    void pop_back();

    const T& operator[](size_t index) const;
    T& operator[](size_t index);
    const T& at(size_t index) const;
    T& at(size_t index);
    const T& front(size_t index) const;
    T& front(size_t index);
    const T& back(size_t index) const;
    T& back(size_t index);

    T* data() noexcept;
    const T* data() const noexcept;

    size_t max_size() const noexcept;


    size_t size();
    size_t capacity();

    iterator begin();
    const_iterator cbegin();

    iterator end();
    const_iterator cend();


    bool empty() const noexcept;
    
    friend bool operator==<>(const Vector& lhs, const Vector& rhs);
};



template <typename T>
class ConstIterator : public Iterator<const T> {};

} // namespace my_vector


#endif // MYVECTOR_H