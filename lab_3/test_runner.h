#pragma once

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
using namespace std;

// Макрос для перевірки рівності
#define ASSERT_EQUAL(x, y) { \
    ostringstream os; \
    os << #x << " != " << #y << ", " \
       << __FILE__ << ":" << __LINE__; \
    AssertEqual(x, y, os.str()); \
}

// Макрос для перевірки істинності виразу
#define ASSERT(x) { \
    ostringstream os; \
    os << #x << " is false, " \
       << __FILE__ << ":" << __LINE__; \
    Assert(x, os.str()); \
}

// Макрос для запуску тесту
#define RUN_TEST(tr, func) \
    tr.RunTest(func, #func)

// Функція для перевірки рівності
template <class T, class U>
void AssertEqual(const T& t, const U& u, const string& hint = {}) {
    if (!(t == u)) {
        ostringstream os;
        os << "Assertion failed: " << t << " != " << u;
        if (!hint.empty()) {
            os << " Hint: " << hint;
        }
        throw runtime_error(os.str());
    }
}

// Функція для перевірки істинності
inline void Assert(bool b, const string& hint) {
    AssertEqual(b, true, hint);
}

// Клас для запуску тестів
class TestRunner {
public:
    template <class TestFunc>
    void RunTest(TestFunc func, const string& test_name) {
        try {
            func();
            cerr << test_name << " OK" << endl;
        } catch (exception& e) {
            ++fail_count;
            cerr << test_name << " fail: " << e.what() << endl;
        } catch (...) {
            ++fail_count;
            cerr << test_name << " fail: unknown exception caught" << endl;
        }
    }

    ~TestRunner() {
        if (fail_count > 0) {
            cerr << fail_count << " unit test(s) failed. Terminate." << endl;
            exit(1);
        }
    }

private:
    int fail_count = 0;
};
