
/// \file basic_model.hpp
/// Contains definition of the basic_model class.

#pragma once

#include "model.hpp"


namespace mv {


/// Basic implementation of model concept
template <typename T>
requires std::equality_comparable<T>
class basic_model {
public:
    basic_model(const basic_model &) = delete;
    basic_model(basic_model &&) = delete;

    /// Constructs model with default value
    explicit basic_model() requires std::default_initializable<T>:
        val_{T()} {}

    /// Constructs model with specified value
    explicit basic_model(const T & val) requires std::copy_constructible<T>:
        val_{val} {}

    /// Constructs model with specified rvalue reference to value
    explicit basic_model(T && val) requires std::move_constructible<T>:
        val_{std::move(val)} {}

    /// Reads model value
    const T & get() const { return val_; }

    /// Reads model value
    operator T const() requires std::copy_constructible<T> { return val_; }

    /// Writes model value. Emits changed signals before and after write
    void set(const T & val) requires std::is_copy_assignable_v<T> {
        if (val_ == val) {
            return;
        }

        before_changed();
        val_ = val;
        after_changed();
    }

    /// Writes model value. Emits changed signals before and after write
    void set(T && val) requires std::is_move_assignable_v<T> {
        if (val_ == val) {
            return;
        }

        before_changed();
        val_ = std::move(val);
        after_changed();
    }

    /// Writes model value. Emits changed signals before and after write
    basic_model & operator=(const T & val) requires std::is_copy_assignable_v<T> { set(val); }

    /// Writes model value. Emits changed signals before and after write
    basic_model & operator=(T && val) requires std::is_move_assignable_v<T> { set(std::move(val)); return *this; }

    /// The signal is emitted before value changes
    mutable signal<void ()> before_changed;

    /// The signal is emitted after value changes
    mutable signal<void ()> after_changed;

private:
    T val_;             ///< Model value
};


static_assert(model<basic_model<int>, int>);


}
