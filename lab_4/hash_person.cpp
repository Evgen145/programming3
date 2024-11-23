#include "test_runner.h"
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include <functional>

using namespace std;

struct Address {
    string city, street;
    int building;

    bool operator==(const Address& other) const {
        return city == other.city &&
               street == other.street &&
               building == other.building;
    }
};

struct Person {
    string name;
    int height;
    double weight;
    Address address;

    bool operator==(const Person& other) const {
        return name == other.name &&
               height == other.height &&
               weight == other.weight &&
               address == other.address;
    }
};

struct AddressHasher {
    size_t operator()(const Address& address) const {
        const size_t coef = 514'229;
        const hash<string> string_hasher;
        const hash<int> int_hasher;

        return coef * coef * string_hasher(address.city) +
               coef * string_hasher(address.street) +
               int_hasher(address.building);
    }
};

struct PersonHasher {
    size_t operator()(const Person& person) const {
        const size_t coef = 39'916'801;
        const hash<string> string_hasher;
        const hash<int> int_hasher;
        const hash<double> double_hasher;
        const AddressHasher address_hasher;

        return coef * coef * coef * string_hasher(person.name) +
               coef * coef * int_hasher(person.height) +
               coef * double_hasher(person.weight) +
               address_hasher(person.address);
    }
};

const vector<string> WORDS = {
    "Kieran", "Jong", "Jisheng", "Vickie", "Adam", "Simon", "Lance",
    "Everett", "Bryan", "Timothy", "Daren", "Emmett", "Edwin", "List",
    "Sharon", "Trying", "Dan", "Saad", "Kamiya", "Nikolai", "Del",
    "Casper", "Arthur", "Mac", "Rajesh", "Belinda", "Robin", "Lenora",
    "Carisa", "Penny", "Sabrina", "Ofer", "Suzanne", "Pria", "Magnus",
    "Ralph", "Cathrin", "Phill", "Alex", "Reinhard", "Marsh", "Tandy",
    "Mongo", "Matthieu", "Sundaresan", "Piotr", "Ramneek", "Lynne", "Erwin",
    "Edgar", "Srikanth", "Kimberly", "Jingbai", "Lui", "Jussi", "Wilmer",
    "Stuart", "Grant", "Hotta", "Stan", "Samir", "Ramadoss", "Narendra",
    "Gill", "Jeff", "Raul", "Ken", "Rahul", "Max", "Agatha",
    "Elizabeth", "Tai", "Ellen", "Matt", "Ian", "Toerless", "Naomi",
    "Rodent", "Terrance", "Ethan", "Florian", "Rik", "Stanislaw", "Mott",
    "Charlie", "Marguerite", "Hitoshi", "Panacea", "Dieter", "Randell", "Earle",
    "Rajiv", "Ted", "Mann", "Bobbie", "Pat", "Olivier", "Harmon",
    "Raman", "Justin"
};

void TestSmoke() {
    unordered_set<Person, PersonHasher> people;
    people.insert({"John Doe", 180, 75.5, {"New York", "5th Avenue", 10}});
    people.insert({"Jane Smith", 165, 60.0, {"Los Angeles", "Main Street", 20}});
    AssertEqual(people.size(), 2u, "Two unique persons added");
}

void TestPurity() {
    Person person = {"Alice", 170, 65.0, {"Paris", "Rue de Rivoli", 5}};
    PersonHasher hasher;

    size_t hash1 = hasher(person);
    size_t hash2 = hasher(person);

    AssertEqual(hash1, hash2, "Hash function is pure");
}

void TestDistribution() {
    vector<Person> people;
    for (int i = 0; i < 1000; ++i) {
        people.push_back({"Name" + to_string(i), i % 200, i % 100 * 1.5, {"City" + to_string(i % 50), "Street" + to_string(i % 20), i % 10}});
    }

    unordered_set<size_t> hashes;
    PersonHasher hasher;

    for (const auto& person : people) {
        hashes.insert(hasher(person));
    }

    Assert(hashes.size() > 900, "Hash distribution is uniform");
}

int main() {
    TestRunner tr;
    tr.RunTest(TestSmoke, "TestSmoke");
    tr.RunTest(TestPurity, "TestPurity");
    tr.RunTest(TestDistribution, "TestDistribution");
    return 0;
}