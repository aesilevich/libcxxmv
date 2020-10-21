
/// \file vector.hpp
/// Contains definitin of the vector class.

#pragma once

#include "concepts.hpp"
#include <vector>


namespace mv {


/// Vector model
template <typename T>
class vector {
public:
    /// Type of const iterator over vector elements
    using const_iterator = std::vector<T>::const_iterator;

    /// Type of iterator over vector elements
    using iterator = std::vector<T>::iterator;

    /// Constructs empty vector
    vector() = default;

    /// Constructs vector from initializer list
    vector(const std::initializer_list<T> & vals):
        storage_{vals} {}

    /// Returns const iterator pointing to the first element
    auto begin() const { return storage_.begin(); }

    /// Returns const iterator pointing to one past the last element
    auto end() const { return storage_.end(); }

    /// Returns size of vector
    size_t size() const { return storage_.size(); }

    /// Inserts elements at specified position
    template <typename It>
    void insert(const const_iterator & pos, It first, It last) {
        auto sz = std::distance(first, last);
        before_inserted(pos, sz);
        storage_.insert(pos, first, last);
        after_inserted(pos, sz);
    }

    /// Inserts element at specified position
    void insert(const const_iterator & pos, const T & val) {
        before_inserted(pos, 1);
        storage_.insert(pos, val);
        after_inserted(pos, 1);
    }

    /// Inserts element at the end of vector
    void push_back(const T & val) {
        insert(end(), val);
    }

    /// Erases elements
    void erase(const_iterator first, const_iterator last) {
        auto idx = std::distance(begin(), first);
        auto sz = std::distance(first, last);
        before_erased(begin() + idx, sz);
        erase(first, last);
        after_erased(begin() + idx, sz);
    }

    // /// Inserts item at specified index
    // void insert(size_t idx, const T & val) {
    //     assert(idx < size() && "invalid item index");
    //     this->before_added(idx, idx);
    //     items_.insert(idx, val);
    //     this->after_added(idx, idx);
    // }

    // /// Removes item at specified index
    // void erase(size_t idx) {
    //     assert(idx < size() && "invalid item index");
    //     before_removed(idx, idx);
    //     items_.erase(idx);
    //     after_removed(idx, idx);
    // }

    // /// Removes all items from model
    // void clear() {
    //     if (empty()) {
    //         return;
    //     }

    //     before_removed(0, size() - 1);
    //     items_.clear();
    //     after_removed(0, size() - 1);
    // }

    /// The signal is emitted before items added
    mutable signal<void (const_iterator, size_t)> before_inserted;

    /// The signal is emitted after items added
    mutable signal<void (const_iterator, size_t)> after_inserted;

    /// The signal is emitted before items removed
    mutable signal<void (const_iterator, size_t)> before_erased;

    /// The signal is emitted after items removed
    mutable signal<void (const_iterator, size_t)> after_erased;

    // /// The signal is emitted after item is changed
    // mutable signal<void (size_t)> before_changed;

    // /// The signal is after after item is changed
    // mutable signal<void (size_t)> after_changed;

private:
    std::vector<T> storage_;
};


}
