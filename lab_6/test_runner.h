#pragma once

#include <sstream>
#include <stdexcept>
#include <iostream>
#include <map>
#include <unordered_map>
#include <set>
#include <string>
#include <vector>

namespace TestRunnerPrivate {

// Допоміжна функція для друку мап
template <typename K, typename V, template <typename, typename> class Map>
std::ostream& PrintMap(std::ostream& os, const Map<K, V>& m) {
    os << "{";
    bool first = true;
    for (const auto& kv : m) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << kv.first << ": " << kv.second;
    }
    return os << "}";
}

} // namespace TestRunnerPrivate

// Оператор друку для std::vector
template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& s) {
    os << "{";
    bool first = true;
    for (const auto& x : s) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << x;
    }
    return os << "}";
}

// Оператор друку для std::set
template <class T>
std::ostream& operator<<(std::ostream& os, const std::set<T>& s) {
    os << "{";
    bool first = true;
    for (const auto& x : s) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << x;
    }
    return os << "}";
}

// Оператор друку для std::map
template <class K, class V>
std::ostream& operator<<(std::ostream& os, const std::map<K, V>& m) {
    return TestRunnerPrivate::PrintMap(os, m);
}

// Оператор друку для std::unordered_map
template <class K, class V>
std::ostream& operator<<(std::ostream& os, const std::unordered_map<K, V>& m) {
    return TestRunnerPrivate::PrintMap(os, m);
}

// Функція перевірки рівності
template<class T, class U>
void AssertEqual(const T& t, const U& u, const std::string& hint = {}) {
    if (!(t == u)) {
        std::ostringstream os;
        os << "Assertion failed: " << t << " != " << u;
        if (!hint.empty()) {
            os << " hint: " << hint;
        }
        throw std::runtime_error(os.str());
    }
}

// Функція перевірки булевого значення
inline void Assert(bool b, const std::string& hint) {
    AssertEqual(b, true, hint);
}

// Клас для тестування
class TestRunner {
public:
    template <class TestFunc>
    void RunTest(TestFunc func, const std::string& test_name) {
        try {
            func();
            std::cerr << test_name << " OK" << std::endl;
        } catch (std::exception& e) {
            ++fail_count;
            std::cerr << test_name << " fail: " << e.what() << std::endl;
        } catch (...) {
            ++fail_count;
            std::cerr << "Unknown exception caught" << std::endl;
        }
    }

    ~TestRunner() {
        if (fail_count > 0) {
            std::cerr << fail_count << " unit tests failed. Terminate" << std::endl;
            exit(1);
        }
    }

private:
    int fail_count = 0;
};

// Макрос для зручного написання перевірок
#define ASSERT_EQUAL(x, y) { \
    std::ostringstream __assert_equal_private_os; \
    __assert_equal_private_os << #x << " != " << #y << ", " \
                              << __FILE__ << ":" << __LINE__; \
    AssertEqual(x, y, __assert_equal_private_os.str()); \
}

#define ASSERT(x) { \
    std::ostringstream __assert_private_os; \
    __assert_private_os << #x << " is false, " \
                        << __FILE__ << ":" << __LINE__; \
    Assert(x, __assert_private_os.str()); \
}

#define RUN_TEST(tr, func) \
    tr.RunTest(func, #func)
