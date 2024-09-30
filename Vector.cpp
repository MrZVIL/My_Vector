#include "Vector.h"

namespace my_vector{

    static size_t max_for_size(size_t a, size_t b) {
        return a > b? a : b;
    }

    template <typename T>
    void Vector<T>::expand() {
        size_t new_capacity = max_for_size(capacity_ * utils::COEF_FOR_EXTENS, size_t{1});
        transport_data(new_capacity);
    }

    template <typename T>
    void Vector<T>::shrink() {
        size_t new_capacity = max_for_size(capacity_ / utils::COEF_FOR_SHRINK, size_);
        transport_data(new_capacity);
    }

    template <typename T>
    void Vector<T>::transport_data(int new_capacity) {
        T* new_data = reinterpret_cast<T*>(operator new(new_capacity * sizeof(T)));

        for(int i = 0; i < size_; ++i) {
            try {
                new (&new_data[i]) T{data_[i]};
            } catch(...) {
                for(int j = 0; j < i; ++j) {
                    new_data[j].~T();
                }

                operator delete(new_data);
                throw;
            }
        }

        clear_data();
        data_ = new_data;
        capacity_ = new_capacity;
    }

    template <typename T>
    void Vector<T>::clear_data() {  //clears only data_
        for(int i = 0; i < size_; ++i) {
            data_[i].~T();
        }
        operator delete(data_);
    }

    template <typename T>
    Vector<T>::Vector(size_t count, const T& value) {

        static_assert(std::is_copy_constructible_v<T>, "Trying to copy non-copyable object");
        
        data_ = reinterpret_cast<T*>(operator new(count * sizeof(T)));

        for(int i = 0; i < count; ++i) {
            try {
                new (&data_[i]) T{value};
            } catch(...) {
                for(int j = 0; j < i; ++j) {
                    data_[j].~T();
                }

                operator delete(data_);
                throw;
            }
        }

        capacity_ = count;
        size_ = count;
    }

    template<typename T>
    Vector<T>::Vector(size_t count) {
        static_assert(std::is_default_constructible_v<T>, "Element type must be default constructable");

        data_ = reinterpret_cast<T*>(operator new(count * sizeof(T)));

        for(int i = 0; i < count; ++i) {
            try {
                new (&data_[i]) T();
            } catch(...) {
                for(int j = 0; j < i; ++j) {
                    data_[j].~T();
                }

                operator delete(data_);
                throw;
            }
        }

        capacity_ = count;
        size_ = count;
    }

    template <typename T>
    Vector<T>::Vector(const Vector& other) {
        capacity_ = other.capacity_;
        size_ = other.size_;

        data_ = reinterpret_cast<T*>(operator new(capacity_ * sizeof(T)));

        for(int i = 0; i < size_; ++i) {
            try {
                new (&data_[i]) T{other[i]};
            } catch(...) {
                for(int j = 0; j < i; ++j) {
                    data_[j].~T();
                }

                operator delete(data_);
                throw;
            }
        }
    }

    template <typename T>
    Vector<T>::Vector(Vector&& other) noexcept {
        capacity_ = other.capacity_;
        size_ = other.size_;
        data_ = other.data_;

        other.capacity_ = 0;
        other.data_ = nullptr;
        other.size_ = 0;
    }

    template <typename T>
    Vector<T>::Vector(std::initializer_list<T> init) {
        size_ = init.size();
        capacity_ = init.size();

        data_ = reinterpret_cast<T*>(operator new(capacity_ * sizeof(T)));

        for(int i = 0; i < size_; ++i) {
            try {
                new (&data_[i]) T{*(init.begin() + i)};
            } catch(...) {
                for(int j = 0; j < i; ++j) {
                    data_[j].~T();
                }

                operator delete(data_);
                throw;
            }
        }
    }

    // template <typename T>
    // template <class RandomAcIt>
    // Vector<T>::Vector(RandomAcIt first, RandomAcIt last) {
    //     size_ = last - first;
    //     capacity_ = last - first;
        
    //     data_ = reinterpret_cast<T*>(operator new(capacity_ * sizeof(T)));

    //     for(int i = 0; i < size_; ++i) {
    //         try {
    //             new (&data_[i]) T{*(first + i)};
    //         } catch(...) {
    //             for(int j = 0; j < i; ++j) {
    //                 data_[j].~T();
    //             }

    //             operator delete(data_);
    //             throw;
    //         }
    //     }
    // }

    template <typename T>
    Vector<T>& Vector<T>::operator=(const Vector<T>& other) {

        T* new_data = reinterpret_cast<T*>(operator new(other.capacity_ * sizeof(T)));

        for(int i = 0; i < size_; ++i) {
            try {
                new (&new_data[i]) T{other[i]};
            } catch(...) {
                for(int j = 0; j < i; ++j) {
                    new_data[j].~T();
                }

                operator delete(new_data);
                throw;
            }
        }

        this->clear_data();
        data_ = new_data;

        capacity_ = other.capacity_;
        size_ = other.size_;

        return *this;
    }

    template <typename T>
    Vector<T>& Vector<T>::operator=(Vector<T>&& other) {
        std::swap(capacity_, other.capacity_);
        std::swap(size_, other.size_);
        std::swap(data_, other.data_);

        return *this;
    }

    template <typename T>
    Vector<T>& Vector<T>::operator=(std::initializer_list<T> ilist) {

        T* new_data = reinterpret_cast<T*>(operator new(capacity_ * sizeof(T)));

        for(int i = 0; i < size_; ++i) {
            try {
                new (&new_data[i]) T{*(ilist.begin() + i)};
            } catch(...) {
                for(int j = 0; j < i; ++j) {
                    new_data[j].~T();
                }

                operator delete(new_data);
                throw;
            }
        }

        this->clear_data();

        data_ = new_data;
        size_ = ilist.size();
        capacity_ = ilist.size();

        return *this;
    }

    template <typename T>
    void Vector<T>::assign( size_t count, const T& value ) {        

        static_assert(std::is_copy_constructible_v<T>, "Trying to copy non-copyable object");
        
        T* new_data = reinterpret_cast<T*>(operator new(count * sizeof(T)));

        for(int i = 0; i < count; ++i) {
            try {
                new (&new_data[i]) T{value};
            } catch(...) {
                for(int j = 0; j < i; ++j) {
                    new_data[j].~T();
                }

                operator delete(new_data);
                throw;
            }
        }

        this->clear_data();
        data_ = new_data;
        capacity_ = count;
        size_ = count;
    }

    // template <typename T>
    // template< class RandomAcIt >
    // void Vector<T>::assign( RandomAcIt first, RandomAcIt last ) {
    //     this->clear_data();

    //     size_ = last - first;
    //     capacity_ = last - first;
        
    //     data_ = reinterpret_cast<T*>(operator new(capacity_ * sizeof(T)));

    //     for(int i = 0; i < size_; ++i) {
    //         try {
    //             new (&data_[i]) T{*(first + i)};
    //         } catch(...) {
    //             for(int j = 0; j < i; ++j) {
    //                 data_[j].~T();
    //             }

    //             operator delete(data_);
    //             throw;
    //         }
    //     }
    // }

    template <typename T>
    void Vector<T>::assign( std::initializer_list<T> ilist ) {

        T* new_data = reinterpret_cast<T*>(operator new(capacity_ * sizeof(T)));

        for(int i = 0; i < size_; ++i) {
            try {
                new (&data_[i]) T{*(ilist.begin() + i)};
            } catch(...) {
                for(int j = 0; j < i; ++j) {
                    data_[j].~T();
                }

                operator delete(data_);
                throw;
            }
        }

        this->clear_data();
        data_ = new_data;
        size_ = ilist.size();
        capacity_ = ilist.size();
    }

    template <typename T>
    Vector<T>::~Vector() {
        clear_data();
    }

    template <typename T>
    void Vector<T>::reserve(size_t new_capacity) {
        if(new_capacity > capacity_) transport_data(new_capacity);
    }

    template <typename T>
    void Vector<T>::shrink_to_fit() {
        if (size_ < capacity_) transport_data(size_);
    }

    template <typename T>
    void Vector<T>::clear() {
        clear_data();
        capacity_ = 0;
        size_ = 0;
    }

    template <typename T>
    void Vector<T>::resize(size_t new_size) {
        if(new_size > size_) {
            if(!std::is_default_constructible_v<T>)
                throw "Elements type must be default constructible";
            T new_val = T();
            for(int i = 0; i < size_ - new_size; ++ i) {
                push_back(new_val);
            }
        } else if(new_size < size_) {
            for(int i = new_size; i < size_; ++i) {
                data_[i].~T();
            }
        }

        size_ = new_size;
    }

    template <typename T>
    void Vector<T>::swap(Vector& other){
        std::swap(capacity_, other.capacity_);
        std::swap(size_, other.size_);
        std::swap(data_, other.data_);
    }

    template <typename T>
    void Vector<T>::resize(size_t new_size, const T& new_val) {
        if(new_size > size_) {
            for(int i = 0; i < size_ - new_size; ++ i) {
                push_back(new_val);
            }
        } else if(new_size < size_) {
            for(int i = new_size; i < size_; ++i) {
                data_[i].~T();
            }
        }

        size_ = new_size;
    }

    template <typename T> 
    void Vector<T>::push_back(const T& new_value) {
        if(size_ == capacity_) {
            expand();
        }
        new (&data_[size_]) T{new_value};
        ++size_;
    }

    template <typename T>
    template <typename ...Args>
    T& Vector<T>::emplace_back(const Args& ...args) {
        if(size_ == capacity_) {
            expand();
        }
        new (&data_[size_]) T{args...};
        ++size_;
        return data_[size_ - 1];
    }

    template <typename T> 
    void Vector<T>::pop_back() {
        if(size_ == 0) {
            throw std::out_of_range("Trying to remove element from empty vector");
        }
        --size_;
        data_[size_].~T();

        if(size_ * utils::DIFF_FOR_SHRINK <= capacity_) {
            shrink();
        }
    }


    template <typename T>
    const T& Vector<T>::operator[](size_t index) const {
        return data_[index];
    }

    template <typename T> 
    T& Vector<T>::operator[](size_t index) {
        return data_[index];
    }

    template <typename T>
    const T& Vector<T>::at(size_t index) const {
        if(index >= size_) throw std::out_of_range("Not enough elements in vector");

        return data_[index];
    }

    template <typename T> 
    T& Vector<T>::at(size_t index) {
        if(index >= size_) throw std::out_of_range("Not enough elements in vector");
        
        return data_[index];
    }

    template <typename T>
    const T& Vector<T>::front(size_t index) const {
        return data_[0];
    }

    template <typename T> 
    T& Vector<T>::front(size_t index) {
        return data_[0];
    }

    template <typename T>
    const T& Vector<T>::back(size_t index) const {
        return data_[size_ - 1];
    }

    template <typename T> 
    T& Vector<T>::back(size_t index) {
        return data_[size_ - 1];
    }

    template <typename T>
    T* Vector<T>::data() noexcept {
        return data_;
    }

    template <typename T>
    const T* Vector<T>::data() const noexcept {
        return data_;
    }

    template <typename T>
    size_t Vector<T>::max_size() const noexcept {
        return __gnu_cxx::__numeric_traits<ptrdiff_t>::__max / sizeof(T);
    }

    template <typename T> 
    size_t Vector<T>::size() {
        return size_;
    }

    template <typename T> 
    size_t Vector<T>::capacity() {
        return capacity_;
    }

    template <typename T> 
    Vector<T>::iterator Vector<T>::begin() {
        return iterator(data_);
    }

    template <typename T> 
    Vector<T>::const_iterator Vector<T>::cbegin() {
        return const_iterator(data_);
    }

    template <typename T> 
    Vector<T>::iterator Vector<T>::end() {
        return iterator(data_ + size_);
    }

    template <typename T> 
    Vector<T>::const_iterator Vector<T>::cend() {
        return const_iterator(data_ + size_);
    }

    template <typename T>
    bool Vector<T>::empty() const noexcept {
        return size_ == 0;
    }
    
    template <typename T>
    bool Iterator<T>::operator==(Iterator<T> const& other) const{
        return ptr == other.ptr;
    }

    template <typename T>
    bool Iterator<T>::operator!=(Iterator<T> const& other) const{
        return ptr != other.ptr;
    }

    template <typename T>
    T& Iterator<T>::operator*() const{
        return *ptr;
    }

    template <typename T>
    Iterator<T>& Iterator<T>::operator++() {
        return ++ptr;
    }

} // namespace my_vector

template <typename T>
bool operator==(const my_vector::Vector<T>& lhs, const my_vector::Vector<T>& rhs) {
    if(lhs.size_ != rhs.size_) return false;
    bool res = true;
    for(int i = 0; i < lhs.size_; ++i) {
        if(lhs.data_[i] != rhs.data_[i]) {
            res = false;
            break;
        }
    }
    return res;
}

namespace std {
    template <typename T>
    void swap(my_vector::Vector<T>& lhs, my_vector::Vector<T>& rhs) {
        lhs.swap(rhs);
    }
} // namespace std