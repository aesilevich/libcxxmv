
/// \file transform.hpp
/// Contains definition of the transform_view range adaptor and related functions.

#include "adaptor.hpp"
#include "observable.hpp"
#include "ref.hpp"


namespace mv {


struct empty_set_fn {};


/// Transformed observable and model view
template <viewable_observable Observable, typename GetFn, typename SetFn>
class transform_view: public view_base {
public:
    transform_view(Observable b, GetFn gf, SetFn sf):
    base_{b}, get_fn_{std::move(gf)}, set_fn_{std::move(sf)},
    before_changed{b.before_changed},
    after_changed{b.after_changed} {}

    decltype(auto) get() const {
        return get_fn_(base_.get());
    }

    template <typename T>
    void set(T && val) requires (!std::same_as<std::decay_t<SetFn>, empty_set_fn>) {
        auto transformed_val = base_.get();
        set_fn_(transformed_val, val);
        base_.set(transformed_val);
    }

    decltype(std::declval<Observable>().before_changed) before_changed;
    decltype(std::declval<Observable>().after_changed) after_changed;

private:
    Observable base_;
    GetFn get_fn_;
    SetFn set_fn_;
};


template <viewable_observable Observable, typename GetFn, typename SetFn>
transform_view(Observable && obj, GetFn &&, SetFn &&) -> transform_view<views::all_t<Observable>, GetFn, SetFn>;

template <viewable_observable Observable, typename GetFn>
transform_view(Observable && obj, GetFn &&) -> transform_view<views::all_t<Observable>, GetFn, empty_set_fn>;


template <typename SetFn, typename GetFn>
class transform_adaptor_closure {
public:
    transform_adaptor_closure(const GetFn & gf, const SetFn & sf):
    get_fn_{gf}, set_fn_{sf} {}

    template <observable Observable>
    auto operator()(Observable && obj) const {
        return transform_view{obj, get_fn_, set_fn_};
    }

private:
    GetFn get_fn_;
    SetFn set_fn_;
};


template <observable Observable, typename SetFn, typename GetFn>
auto operator|(Observable && obj, const transform_adaptor_closure<SetFn, GetFn> & c) {
    return c(std::forward<Observable>(obj));
}


class transform_adaptor {
public:
    constexpr transform_adaptor() = default;

    template <observable Observable, typename GetFn, typename SetFn>
    auto operator()(Observable && obj, GetFn && gf, SetFn && sf) const {
        return transform_view{obj, gf, sf};
    }

    template <typename GetFn, typename SetFn>
    auto operator()(GetFn && gf, SetFn && sf) const {
        return transform_adaptor_closure{std::forward<GetFn>(gf), std::forward<SetFn>(sf)};
    }

    template <observable Observable, typename GetFn>
    auto operator()(Observable && obj, GetFn && gf) const {
        return transform_view{obj, gf};
    }

    template <typename GetFn>
    auto operator()(GetFn & gf) const {
        return transform_adaptor_closure{std::forward<GetFn>(gf), empty_set_fn{}};
    }
};


namespace views {
    inline constexpr auto transform = transform_adaptor{};
}


}
