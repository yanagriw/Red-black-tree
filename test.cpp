// Check for memory leaks
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <memory>
#include <set>
#include <algorithm>
#include <iostream>
#include "RedBlackTree.h"

template <typename T>
bool red_black_properties(RedBlackNode<T>* node, size_t blackheight, size_t blackheight_prev) {
    bool test = true;
    if (node != NULL) {

        //check the properties of search tree
        if ((node->left != NULL) && (node->left->value > node->value)) {
            std::cout << "The left subtree of a node must contain only nodes with values lesser than the nodes key!" << std::endl;
            test = false;
        }
        else if ((node->right != NULL) && (node->right->value < node->value)) {
            std::cout << "The right subtree of a node must contain only nodes with values greater than the nodes key!" << std::endl;
            test = false;
        }

        //check the properties of red-black tree
        else if (node->color == RED) { //If a node is red, then both its children are black
            if (((node->left != NULL) && (node->left->color == RED))
                || ((node->right != NULL) && (node->right->color == RED))) {
                std::cout << "If a node is red, then both its children must be black!" << std::endl;
                test = false;
            }
        }
        //every path from a node to a leaf contains the same number of black nodes
        else if (node->color == BLACK) {
            blackheight++;
            if (node->left == NULL && node->right == NULL) {
                if (blackheight_prev == 0) {
                    blackheight_prev = blackheight;
                }
                else if (blackheight != blackheight_prev) {
                    std::cout << "Every path from a node to a leaf must contain the same number of black nodes!" << std::endl;
                    test = false;
                }
            }
        }

        if (test == false) {
            return false;
        }
        else {
            test = (red_black_properties(node->left, blackheight, blackheight_prev) && red_black_properties(node->right, blackheight, blackheight_prev));
        }
    }
    return test;
}

template <typename T>
void check_properties(RedBlackTree<T> tree) {
    if (red_black_properties(tree.root, 0, 0)) {
        std::cout << "TREE PASSED ALL TESTS OF RED-BLACK TREE PROPERTIES" << std::endl;
    }
    else {
        std::cout << "ERROR: TREE DOES NOT SATISFY THE PROPERTIES OF RED-BLACK TREES!" << std::endl;
        tree.print();
    }
}

bool test_multi_insert(size_t element_count)
{
    std::multiset<int> reference_multiset;
    RedBlackTree<int> tree;

    // Insert the same pseudo random values both to RedBlackTree and std::set
    srand(42);
    for (size_t i = 0; i < element_count; i++)
    {
        int value = rand();

        reference_multiset.insert(value);
        tree.insert(value);
    }

    check_properties(tree);

    // Check that they both return the same values
    return std::equal(reference_multiset.begin(), reference_multiset.end(), tree.begin());
}

bool test_multi_erase(size_t element_count, size_t delete_count)
{
    std::multiset<int> reference_multiset;
    RedBlackTree<int> tree;

    srand(42);
    for (size_t i = 0; i < element_count; i++)
    {
        int value = rand();

        reference_multiset.insert(value);
        tree.insert(value);
    }

    for (size_t i = 0; i < element_count; i++)
    {
        int value = rand();

        reference_multiset.erase(value);
        tree.erase(value);
    }

    check_properties(tree);

    // Check that they both return the same values
    return std::equal(reference_multiset.begin(), reference_multiset.end(), tree.begin());
}

bool test_multi_merge(size_t element_count1, size_t element_count2)
{
    std::multiset<int> reference_multiset;
    RedBlackTree<int> tree1;
    RedBlackTree<int> tree2;

    srand(100);
   
    for (size_t i = 0; i < element_count1; i++)
    {
        int value = rand() % 100;

        reference_multiset.insert(value);
        tree1.insert(value);
    }

    for (size_t i = 0; i < element_count2; i++)
    {
        int value = rand();

        reference_multiset.insert(value);
        tree2.insert(value);
    }

    tree1.merge(tree2);
    check_properties(tree1);

    // Check that they both return the same values
    return std::equal(reference_multiset.begin(), reference_multiset.end(), tree1.begin());
}

bool test_multi_find(size_t element_count)
{
    std::multiset<int> reference_multiset;
    RedBlackTree<int> tree;

    srand(42);

    for (size_t i = 0; i < element_count; i++)
    {
        int value = rand();

        reference_multiset.insert(value);
        tree.insert(value);
    }

    for (size_t i = 0; i < 100; i++)
    {
        int value = rand();
        bool found_tree = true;
        bool found_set = true;
        if (tree.find(value) == NULL) {
            found_tree = false;
        }
        if ((reference_multiset.find(value) != reference_multiset.end()) && !found_tree) {
            return false;
        }
        else if (found_tree && (tree.find(value)->value != value)) {
            return false;
        }
    }
    return true;
}

bool test_min_max(size_t element_count)
{
    std::multiset<int> reference_multiset;
    RedBlackTree<int> tree;

    srand(42);

    for (size_t i = 0; i < element_count; i++)
    {
        int value = rand();

        reference_multiset.insert(value);
        tree.insert(value);
    }

    if (*tree.min() != *std::min_element(reference_multiset.begin(), reference_multiset.end())) {
        return false;
    }

    if (*tree.max() != *std::max_element(reference_multiset.begin(), reference_multiset.end())) {
        return false;
    }
    return true;
}

bool test_iterators(size_t element_count)
{
    std::multiset<int> reference_multiset;
    RedBlackTree<int> tree;

    srand(42);

    for (size_t i = 0; i < element_count; i++)
    {
        int value = rand();

        reference_multiset.insert(value);
        tree.insert(value);
    }

    auto it_set = reference_multiset.begin();
    for (auto it = tree.begin(); it != tree.end(); it++) {
        if (*it != *it_set) {
            return false;
        }
        it_set++;
    }

    it_set = reference_multiset.begin();
    for (auto it = tree.begin(); it != tree.end(); ++it) {
        if (*it != *it_set) {
            return false;
        }
        ++it_set;
    }

    it_set = std::max_element(reference_multiset.begin(), reference_multiset.end());
    for (auto it = tree.max(); it != tree.end(); it--) {
        if (*it != *it_set) {
            return false;
        }
        it_set--;
    }

    it_set = std::max_element(reference_multiset.begin(), reference_multiset.end());
    for (auto it = tree.max(); it != tree.end(); --it) {
        if (*it != *it_set) {
            return false;
        }
        --it_set;
    }

    return true;
}

bool run_tests()
{

    bool currentTestOk;
    bool allTestsOk = true;

    std::cout << "Small multiple insert test:\t";
    currentTestOk = test_multi_insert(100);
    allTestsOk = allTestsOk || !currentTestOk;
    std::cout << (currentTestOk ? "OK" : "ERROR");
    std::cout << std::endl;

    std::cout << "Large multiple insert test:\t";
    currentTestOk = test_multi_insert(10000);
    allTestsOk = allTestsOk || !currentTestOk;
    std::cout << (currentTestOk ? "OK" : "ERROR");
    std::cout << std::endl;

    std::cout << "Erase test:\t";
    currentTestOk = test_multi_erase(100, 30);
    allTestsOk = allTestsOk || !currentTestOk;
    std::cout << (currentTestOk ? "OK" : "ERROR");
    std::cout << std::endl;

    std::cout << "Small merge test:\t";
    currentTestOk = test_multi_merge(50, 100);
    allTestsOk = allTestsOk || !currentTestOk;
    std::cout << (currentTestOk ? "OK" : "ERROR");
    std::cout << std::endl;

    std::cout << "Large merge test:\t";
    currentTestOk = test_multi_merge(400, 500);
    allTestsOk = allTestsOk || !currentTestOk;
    std::cout << (currentTestOk ? "OK" : "ERROR");
    std::cout << std::endl;

    std::cout << "Find test:\t";
    currentTestOk = test_multi_find(300);
    allTestsOk = allTestsOk || !currentTestOk;
    std::cout << (currentTestOk ? "OK" : "ERROR");
    std::cout << std::endl;

    std::cout << "Min/max test:\t";
    currentTestOk = test_multi_find(300);
    allTestsOk = allTestsOk || !currentTestOk;
    std::cout << (currentTestOk ? "OK" : "ERROR");
    std::cout << std::endl;

    std::cout << "Iterators test:\t";
    currentTestOk = test_iterators(300);
    allTestsOk = allTestsOk || !currentTestOk;
    std::cout << (currentTestOk ? "OK" : "ERROR");
    std::cout << std::endl;

    std::cout << std::endl;
    if (allTestsOk) {
        std::cout << "All tests completed successfully" << std::endl;
    }
    else {
        std::cout << "There were failed tests" << std::endl;
    }

    return allTestsOk;
}

int main() {
    bool allOk = run_tests();


    // Check for memory leaks (display them in standard output)
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);
    _CrtDumpMemoryLeaks();

    // Propagate the information about failed tests outside (e.g. for CI/CD scenarios)
    return allOk ? 0 : -1;
}
