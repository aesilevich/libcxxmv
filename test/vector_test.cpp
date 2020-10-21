
/// \file vector_test.cpp
/// Contains unit tests for the vector class.

#include <boost/test/unit_test.hpp>
#include <cxxmv/vector.hpp>


namespace mv {

BOOST_AUTO_TEST_SUITE(vector_test)


/// Tests vector default construction
BOOST_AUTO_TEST_CASE(ctor) {
    vector<int> v;
    BOOST_CHECK(std::ranges::begin(v) == std::ranges::end(v));
}


/// Tests vector constructor
BOOST_AUTO_TEST_CASE(ctor_init_list) {
    vector<int> v = {10, 20, 30};
    BOOST_CHECK_EQUAL(v.size(), 3);
}


/// Tests pushing element at the end of vector
BOOST_AUTO_TEST_CASE(push_back) {
    vector<int> v = {10, 20, 30};

    int n_before_inserted_called = 0;
    v.before_inserted.connect([&n_before_inserted_called, &v](auto it, auto sz) {
        ++n_before_inserted_called;
        BOOST_CHECK(it == std::ranges::begin(v) + 3);
        BOOST_CHECK_EQUAL(sz, 1);
        BOOST_CHECK_EQUAL(v.size(), 3);
    });

    int n_after_inserted_called = 0;
    v.after_inserted.connect([&n_after_inserted_called, &v](auto begin, auto sz) {
        ++n_after_inserted_called;
        BOOST_CHECK(begin == std::ranges::begin(v) + 3);
        BOOST_CHECK_EQUAL(sz, 1);
        BOOST_CHECK_EQUAL(v.size(), 4);
    });

    v.push_back(40);

    BOOST_CHECK_EQUAL(n_before_inserted_called, 1);
    BOOST_CHECK_EQUAL(n_after_inserted_called, 1);
    BOOST_CHECK_EQUAL(v.size(), 4);
}


BOOST_AUTO_TEST_SUITE_END()

}
