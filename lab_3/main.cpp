#include "test_runner.h"
#include <cassert>
#include <deque>
#include <iostream>

using namespace std;

// Вузол двійкового дерева
struct Node {
    Node(int v, Node* p)
        : value(v)
        , parent(p)
    {}
    int value;
    Node* left = nullptr;  // Лівий дочірній вузол
    Node* right = nullptr; // Правий дочірній вузол
    Node* parent;          // Батьківський вузол
};

// Клас для створення дерева
class NodeBuilder {
public:
    Node* CreateRoot(int value) {
        nodes.emplace_back(value, nullptr);
        return &nodes.back();
    }

    Node* CreateLeftSon(Node* me, int value) {
        assert(me->left == nullptr);
        nodes.emplace_back(value, me);
        me->left = &nodes.back();
        return me->left;
    }

    Node* CreateRightSon(Node* me, int value) {
        assert(me->right == nullptr);
        nodes.emplace_back(value, me);
        me->right = &nodes.back();
        return me->right;
    }

private:
    deque<Node> nodes;
};

// Серединний обхід (in-order traversal)
Node* Next(Node* me) {
    if (!me) return nullptr;

    if (me->right) {
        me = me->right;
        while (me->left) {
            me = me->left;
        }
        return me;
    }

    while (me->parent && me == me->parent->right) {
        me = me->parent;
    }

    return me->parent;
}

// Прямий обхід (pre-order traversal)
Node* PreOrder(Node* me) {
    if (me->left) return me->left;
    if (me->right) return me->right;

    while (me->parent) {
        if (me == me->parent->left && me->parent->right) {
            return me->parent->right;
        }
        me = me->parent;
    }
    return nullptr;
}

// Зворотний обхід (post-order traversal)
Node* PostOrder(Node* me) {
    if (!me) return nullptr;

    if (me->parent && me->parent->right == me) {
        return me->parent;
    }

    if (me->parent && me->parent->left == me && me->parent->right) {
        me = me->parent->right;
        while (me->left || me->right) {
            if (me->left) me = me->left;
            else me = me->right;
        }
        return me;
    }

    return me->parent;
}

// Функція для демонстрації обходів дерева
void ShowTraversal(Node* root, Node* (*traversalFunc)(Node*), const string& traversalName) {
    cout << traversalName << " traversal: ";
    Node* current = root;

    while (current->left || current->right) {
        if (traversalName == "Pre-order") break;
        if (current->left) current = current->left;
        else current = current->right;
    }

    while (current) {
        cout << current->value << ' ';
        current = traversalFunc(current);
    }
    cout << endl;
}

void Test1() {
    NodeBuilder nb;
    Node* root = nb.CreateRoot(50);
    ASSERT_EQUAL(root->value, 50);
    Node* l = nb.CreateLeftSon(root, 2);
    Node* min = nb.CreateLeftSon(l, 1);
    Node* r = nb.CreateRightSon(l, 4);
    ASSERT_EQUAL(min->value, 1);
    ASSERT_EQUAL(r->parent->value, 2);
    nb.CreateLeftSon(r, 3);
    nb.CreateRightSon(r, 5);
    r = nb.CreateRightSon(root, 100);
    l = nb.CreateLeftSon(r, 90);
    nb.CreateRightSon(r, 101);
    nb.CreateLeftSon(l, 89);
    r = nb.CreateRightSon(l, 91);

    ASSERT_EQUAL(Next(l)->value, 91);
    ASSERT_EQUAL(Next(root)->value, 89);
    ASSERT_EQUAL(Next(min)->value, 2);
    ASSERT_EQUAL(Next(r)->value, 100);

    // Серединний обхід
    cout << "Seredynnyy obkhid (In-order): ";
    Node* current = min;
    while (current) {
        cout << current->value << ' ';
        current = Next(current);
    }
    cout << endl;

    // Прямий обхід
    cout << "Pryamyy obkhid (Pre-order): ";
    current = root;
    while (current) {
        cout << current->value << ' ';
        current = PreOrder(current);
    }
    cout << endl;

    // Зворотний обхід
    cout << "Zvorotnyy obkhid (Post-order): ";
    current = root;
    while (current->left || current->right) {
        if (current->left) current = current->left;
        else current = current->right;
    }
    while (current) {
        cout << current->value << ' ';
        current = PostOrder(current);
    }
    cout << endl;
}

void TestRootOnly() {
    NodeBuilder nb;
    Node* root = nb.CreateRoot(42);

    // Серединний обхід
    ASSERT(Next(root) == nullptr);

    // Прямий обхід
    ASSERT(PreOrder(root) == nullptr);

    // Зворотний обхід
    ASSERT(PostOrder(root) == nullptr);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, Test1);
    RUN_TEST(tr, TestRootOnly);
    return 0;
}
