#pragma once

#include <sstream>
#include <exception>
#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>

using namespace std;

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

inline void Assert(bool b, const string& hint) {
    AssertEqual(b, true, hint);
}

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

template <class T>
ostream& operator<<(ostream& os, const vector<T>& vec) {
    os << "[";
    bool first = true;
    for (const auto& elem : vec) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << elem;
    }
    return os << "]";
}

template <class T>
ostream& operator<<(ostream& os, const set<T>& s) {
    os << "{";
    bool first = true;
    for (const auto& elem : s) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << elem;
    }
    return os << "}";
}

template <class K, class V>
ostream& operator<<(ostream& os, const map<K, V>& m) {
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

#define ASSERT_EQUAL(x, y) {            \
  ostringstream __assert_equal_private_os;  \
  __assert_equal_private_os << #x << " != " << #y << ", " \
    << __FILE__ << ":" << __LINE__;     \
  AssertEqual(x, y, __assert_equal_private_os.str()); \
}

#define ASSERT(x) {                     \
  ostringstream __assert_private_os;    \
  __assert_private_os << #x << " is false, " \
    << __FILE__ << ":" << __LINE__;     \
  Assert(x, __assert_private_os.str()); \
}

#define RUN_TEST(tr, func) \
  tr.RunTest(func, #func)
