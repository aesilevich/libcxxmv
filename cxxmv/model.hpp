
/// \file model.hpp
/// Contains definition of the model concept.

#pragma once

#include "observable.hpp"


namespace mv {


/// Model concept
template <typename T, typename Val> concept model = observable_as<T, Val> && requires(T mdl) {
    { mdl.set(std::declval<Val>()) };
};


}
