#include "ini.h"
#include "test_runner.h"
#include <sstream>

using namespace Ini;

void TestLoadIni() {
    std::istringstream input(
        "[section1]\n"
        "key1=value1\n"
        "key2=value2\n"
        "\n"
        "[section2]\n"
        "key3=value3\n"
        "key4=value4\n"
    );

    Document doc = Load(input);

    ASSERT_EQUAL(doc.SectionCount(), 2u);
    ASSERT_EQUAL(doc.GetSection("section1").at("key1"), "value1");
    ASSERT_EQUAL(doc.GetSection("section1").at("key2"), "value2");
    ASSERT_EQUAL(doc.GetSection("section2").at("key3"), "value3");
    ASSERT_EQUAL(doc.GetSection("section2").at("key4"), "value4");
}

void TestDocument() {
    Document doc;
    doc.AddSection("test_section")["test_key"] = "test_value";

    ASSERT_EQUAL(doc.SectionCount(), 1u);
    ASSERT_EQUAL(doc.GetSection("test_section").at("test_key"), "test_value");
}

void TestUnknownSection() {
    Document doc;
    try {
        doc.GetSection("nonexistent");
        Assert(false, "Очікувалась виключна ситуація для неіснуючої секції");
    } catch (const std::out_of_range&) {
    } catch (...) {
        Assert(false, "Неочікувана виключна ситуація для неіснуючої секції");
    }
}

void TestDuplicateSections() {
    std::istringstream input(
        "[section1]\n"
        "key1=value1\n"
        "[section1]\n"
        "key2=value2\n"
    );

    Document doc = Load(input);
    ASSERT_EQUAL(doc.SectionCount(), 1u);
    ASSERT_EQUAL(doc.GetSection("section1").at("key1"), "value1");
    ASSERT_EQUAL(doc.GetSection("section1").at("key2"), "value2");
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestLoadIni);
    RUN_TEST(tr, TestDocument);
    RUN_TEST(tr, TestUnknownSection);
    RUN_TEST(tr, TestDuplicateSections);
    return 0;
}