
/// \file ref_view.hpp
/// Contains definition of the ref_view view and ref adaptor.

#pragma once

#include "adaptor.hpp"
#include "view.hpp"


namespace mv {


template <typename Base, typename ... Args>
requires Signal<Base, Args...>
class signal_ref {
public:
    signal_ref(Base & b): base_{b} {}

    template <typename F>
    signal_connection connect(const F & f) const {
        return base_.connect(f);
    }

private:
    Base & base_;
};


template <typename T>
requires observable<T>
class ref_view: public view_base {
    static auto & get_sig_type(T && b) { return b.before_changed; }
public:
    /// Constructs view with reference to another range
    ref_view(T & b):
    base_{b},
    before_changed{b.before_changed},
    after_changed{b.after_changed} {
    }

    /// Reads value from observable
    decltype(auto) get() const {
        return base_.get();
    }

    /// Writes value to model
    template <typename T2>
    void set(T2 && val) requires model<T, std::decay_t<T2>> {
        base_.set(std::forward<T2>(val));
    }

    signal_ref<decltype(get_sig_type(std::declval<T>()))> before_changed;
    signal_ref<decltype(get_sig_type(std::declval<T>()))> after_changed;

private:
    T & base_;          ///< Reference to base observable
};


namespace views {
    inline constexpr auto all = adaptor_closure {
        []<viewable_observable Observable>(Observable && obj) {
            if constexpr (observable_view<std::decay_t<Observable>>) {
                return std::forward<Observable>(obj);
            } else {
                return ref_view{std::forward<Observable>(obj)};
            }
        }
    };

    template <viewable_observable Observable>
    using all_t = decltype(all(std::declval<Observable>()));
}


}
