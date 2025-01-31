#ifndef TEST_RUNNER_H
#define TEST_RUNNER_H

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

using namespace std;

template <class T, class U>
void AssertEqual(const T& t, const U& u, const string& hint = {}) {
    if (!(t == u)) {
        ostringstream os;
        os << "Assertion failed: " << t << " != " << u;
        if (!hint.empty()) {
            os << " (" << hint << ")";
        }
        throw runtime_error(os.str());
    }
}

void Assert(bool b, const string& hint = {}) {
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
            cerr << test_name << " fail: unknown exception" << endl;
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

#endif // TEST_RUNNER_H
