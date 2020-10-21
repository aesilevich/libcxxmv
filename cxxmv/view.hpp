
/// \file view.hpp
/// Contains definition of view concepts for observable and models.

#pragma once

#include "observable.hpp"
#include "model.hpp"
#include <concepts>


namespace mv {


/// Base class for new. All views should be derived from this to indicate
/// them confirm to view concept.
struct view_base {};


template <typename T>
inline constexpr bool enable_view = std::derived_from<T, view_base>;


/// Observable view concept
template <typename T>
concept observable_view = observable<T> && enable_view<T>;


/// Observable as view concept
template <typename T, typename Val>
concept observable_view_as = observable_as<T, Val> && observable_view<T>;


template <typename T>
concept viewable_observable = observable<T> && (borrowed_observable<T> || observable_view<T>);

template <typename T, typename Val>
concept viewable_observable_as = viewable_observable<T> && observable_as<T, Val>;



template <typename T, typename Val>
concept model_view = observable_view<T> && model<T, Val>;

template <typename T, typename Val>
concept viewable_model = viewable_observable<T> && model<T, Val>;


}
