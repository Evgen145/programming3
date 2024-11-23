#include "priority_collection.h"
#include "test_runner.h"
#include <iostream>

void TestNoCopy() {
    PriorityCollection<std::string> strings;
    const auto white_id = strings.Add("white");
    const auto yellow_id = strings.Add("yellow");
    const auto red_id = strings.Add("red");

    strings.Promote(yellow_id);
    strings.Promote(red_id);
    strings.Promote(red_id);

    auto max_item = strings.PopMax();
    ASSERT_EQUAL(max_item.first, "red");
    ASSERT_EQUAL(max_item.second, 2);

    max_item = strings.PopMax();
    ASSERT_EQUAL(max_item.first, "yellow");
    ASSERT_EQUAL(max_item.second, 1);

    max_item = strings.PopMax();
    ASSERT_EQUAL(max_item.first, "white");
    ASSERT_EQUAL(max_item.second, 0);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestNoCopy);
    return 0;
}
