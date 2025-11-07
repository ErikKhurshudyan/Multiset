#include "headers/Multiset.hpp"
#include <gtest/gtest.h>
#include <string>
#include <vector>

///==================== COUNT ====================
TEST(MultisetTest, CountDuplicates) {
    Multiset<int> ms;
    ms.insert(5);
    ms.insert(5);
    ms.insert(3);
    EXPECT_EQ(ms.count(5), 2);
    EXPECT_EQ(ms.count(3), 1);
    EXPECT_EQ(ms.count(9), 0);
}

///==================== LOWER_BOUND ====================
TEST(MultisetTest, LowerBoundWorksCorrectly) {
    Multiset<int> ms;
    ms.insert(2);
    ms.insert(4);
    ms.insert(4);
    ms.insert(6);
    Multiset<int>::iterator it = ms.lower_bound(4);
    EXPECT_EQ(it == ms.end(), false);
    EXPECT_EQ(*it >= 4, true);
}

TEST(MultisetTest, LowerBoundNoSuchElement) {
    Multiset<int> ms;
    ms.insert(1);
    ms.insert(2);
    Multiset<int>::iterator it = ms.lower_bound(5);
    EXPECT_EQ(it == ms.end(), true);
}

///==================== UPPER_BOUND ====================
TEST(MultisetTest, UpperBoundSkipsEqualKeys) {
    Multiset<int> ms;
    ms.insert(2);
    ms.insert(4);
    ms.insert(4);
    ms.insert(6);
    Multiset<int>::iterator it = ms.upper_bound(4);
    if (it != ms.end()) {
        EXPECT_EQ(*it > 4, true);
    } else {
        EXPECT_EQ(it == ms.end(), true);
    }
}

///==================== EQUAL_RANGE ====================
TEST(MultisetTest, EqualRangeReturnsCorrectPair) {
    Multiset<int> ms;
    ms.insert(3);
    ms.insert(3);
    ms.insert(4);
    std::pair<Multiset<int>::iterator, Multiset<int>::iterator> range = ms.equal_range(3);
    int count = 0;
    for (Multiset<int>::iterator it = range.first; it != range.second; ++it) {
        ++count;
    }
    EXPECT_EQ(count, 2);
}

///==================== ERASE ====================
TEST(MultisetTest, EraseByKeyRemovesAllMatches) {
    Multiset<int> ms;
    ms.insert(5);
    ms.insert(5);
    ms.insert(7);
    size_t removed = ms.erase(5);
    EXPECT_EQ(removed, 2);
    EXPECT_EQ(ms.count(5), 0);
}

TEST(MultisetTest, EraseByIteratorRemovesSingleElement) {
    Multiset<int> ms;
    ms.insert(7);
    ms.insert(8);
    Multiset<int>::iterator it = ms.find(7);
    ms.erase(it);
    EXPECT_EQ(ms.count(7), 0);
}

///==================== SIZE AND EMPTY ====================
TEST(MultisetTest, SizeAndEmptyWorkCorrectly) {
    Multiset<int> ms;
    ms.insert(1);
    ms.insert(2);
    EXPECT_EQ(ms.empty(), false);
    EXPECT_EQ(ms.size(), 2u);
    ms.clear();
    EXPECT_EQ(ms.empty(), true);
    EXPECT_EQ(ms.size(), 0u);
}

///==================== COPY CONSTRUCTOR ====================
TEST(MultisetTest, CopyConstructorCreatesEqualSet) {
    Multiset<int> ms;
    ms.insert(1);
    ms.insert(2);
    Multiset<int> copy(ms);
    EXPECT_EQ(copy == ms, true);
    copy.insert(3);
    EXPECT_EQ(copy == ms, false);
}

///==================== ASSIGNMENT OPERATOR ====================
TEST(MultisetTest, AssignmentOperatorCopiesCorrectly) {
    Multiset<int> a;
    a.insert(1);
    a.insert(2);
    Multiset<int> b;
    b = a;
    EXPECT_EQ(b == a, true);
    b.insert(5);
    EXPECT_EQ(b == a, false);
}

///==================== COMPARISON OPERATORS ====================
TEST(MultisetTest, ComparisonOperatorsWorkCorrectly) {
    Multiset<int> a;
    a.insert(1);
    a.insert(2);
    Multiset<int> b;
    b.insert(1);
    b.insert(3);
    EXPECT_EQ(a < b, true);
    EXPECT_EQ(b > a, true);
    EXPECT_EQ(a != b, true);
}

///==================== ITERATOR TRAVERSAL ====================
TEST(MultisetTest, IteratorTraversalInOrder) {
    Multiset<int> ms;
    ms.insert(3);
    ms.insert(1);
    ms.insert(2);
    Multiset<int>::iterator it = ms.begin();
    int prev = *it;
    ++it;
    while (it != ms.end()) {
        EXPECT_EQ(*it >= prev, true);
        prev = *it;
        ++it;
    }
}

///==================== CLEAR ====================
TEST(MultisetTest, ClearRemovesAll) {
    Multiset<int> ms;
    ms.insert(1);
    ms.insert(2);
    ms.clear();
    EXPECT_EQ(ms.empty(), true);
    EXPECT_EQ(ms.size(), 0u);
}

///==================== SWAP ====================
TEST(MultisetTest, SwapExchangesContents) {
    Multiset<int> a;
    a.insert(1);
    a.insert(2);
    Multiset<int> b;
    b.insert(100);
    a.swap(b);
    EXPECT_EQ(a.count(100), 1);
    EXPECT_EQ(b.count(1), 1);
}

int
main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


