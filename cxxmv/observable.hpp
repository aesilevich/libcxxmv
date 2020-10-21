
/// \file observable.hpp
/// Contains definition of the observable concept.

#pragma once

#include "signals.hpp"
#include <concepts>


namespace mv {


/// Observable concept
template <typename T>
concept observable = requires(const T obj) {
    { obj.before_changed } -> Signal<>;
    { obj.after_changed } -> Signal<>;
    { obj.get() };
};


/// Concept for observable object value of which can be obtained and used to construct another type
template <typename T, typename Val>
concept observable_as = observable<T> && requires(T obj) {
    { Val{obj.get()} };
};


template<typename T>
inline constexpr bool enable_borrowed_observable = false;

/// Borrowed observable is an observable that can be safely taken/stored by value
template <typename T>
concept borrowed_observable = observable<T> &&
(std::is_lvalue_reference_v<T> || enable_borrowed_observable<std::remove_cvref_t<T>>);


template <observable T>
using observable_value_t = decltype(std::declval<T>().get());


}
