#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "parser/parser.hpp"
#include "../mocks/MockParser.hpp"

using namespace std;
using ::testing::_;

class ParserTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

TEST_F(ParserTest, ToSizeTConvertsStringToSizeT) {
    EXPECT_EQ(parser::toSizeT("42"), 42);
    EXPECT_EQ(parser::toSizeT("0"), 0);
    EXPECT_EQ(parser::toSizeT("9999"), 9999);
}

TEST_F(ParserTest, ToStringConvertsSizeTToString) {
    EXPECT_EQ(parser::toString(42), "42");
    EXPECT_EQ(parser::toString(0), "0");
    EXPECT_EQ(parser::toString(9999), "9999");
}

TEST_F(ParserTest, ToLowerConvertsStringToLowercase) {
    EXPECT_EQ(parser::toLower("HELLO"), "hello");
    EXPECT_EQ(parser::toLower("Hello World"), "hello world");
    EXPECT_EQ(parser::toLower("123ABC"), "123abc");
    EXPECT_EQ(parser::toLower("already lowercase"), "already lowercase");
}

TEST_F(ParserTest, ReplaceChangesCharacters) {
    string test = "hello world";
    parser::replace(test, 'l', 'x');
    EXPECT_EQ(test, "hexxo worxd");
    
    string test2 = "replace me";
    parser::replace(test2, ' ', '_');
    EXPECT_EQ(test2, "replace_me");
}

TEST_F(ParserTest, EraseRemovesSubstrings) {
    string test = "hello world hello";
    parser::erase(test, "hello", 5);
    EXPECT_EQ(test, " world ");
    
    string test2 = "test123test123";
    parser::erase(test2, "test", 4);
    EXPECT_EQ(test2, "123123");
}

TEST_F(ParserTest, ReraseFunctionWorks) {
    string test = "hello123 world456";
    parser::rerase(test, "hello", 3);
    EXPECT_EQ(test, "he123 world456");
    
    string test2 = "abc123abc123";
    parser::rerase(test2, "abc", 1);
    EXPECT_EQ(test2, "ab123ab123");
}

TEST_F(ParserTest, TrimRemovesCharactersFromBothEnds) {
    string test = "  hello world  ";
    parser::trim(test, " ");
    EXPECT_EQ(test, "hello world");
    
    string test2 = "---test---";
    parser::trim(test2, "-");
    EXPECT_EQ(test2, "test");
    
    string test3 = "     ";
    parser::trim(test3, " ");
    EXPECT_EQ(test3, "");
}

TEST_F(ParserTest, CompareChecksStartOfString) {
    string test = "hello world";
    EXPECT_TRUE(parser::compare("hello", test));
    EXPECT_FALSE(parser::compare("world", test));
    EXPECT_FALSE(parser::compare("hello world!", test));
    
    string empty = "";
    EXPECT_FALSE(parser::compare("test", empty));
}

TEST_F(ParserTest, FindExtractsSubstring) {
    string test = "key value;other stuff";
    EXPECT_EQ(parser::find("key ", test, ";"), "value");
    EXPECT_EQ(test, "other stuff");
    
    string test2 = "prefix something;";
    EXPECT_EQ(parser::find("non-existent ", test2, ";"), "");
    EXPECT_EQ(test2, "prefix something;");
    
    string test3 = "start without delimiter";
    EXPECT_EQ(parser::find("start ", test3, ";"), "");
    EXPECT_EQ(test3, "without delimiter");
}

TEST_F(ParserTest, SplitBreaksStringIntoList) {
    string test = "one,two,three,four";
    list<string> result = parser::split(test, ',');
    ASSERT_EQ(result.size(), 4);
    EXPECT_EQ(result.front(), "one");
    result.pop_front();
    EXPECT_EQ(result.front(), "two");
    result.pop_front();
    EXPECT_EQ(result.front(), "three");
    result.pop_front();
    EXPECT_EQ(result.front(), "four");
    
    string test2 = ",,multiple,empty,,parts,,";
    list<string> result2 = parser::split(test2, ',');
    ASSERT_EQ(result2.size(), 3);
    EXPECT_EQ(result2.front(), "multiple");
    result2.pop_front();
    EXPECT_EQ(result2.front(), "empty");
    result2.pop_front();
    EXPECT_EQ(result2.front(), "parts");
}

TEST_F(ParserTest, BasenameExtractsFileExtension) {
    EXPECT_EQ(parser::basename("file.txt"), ".txt");
    EXPECT_EQ(parser::basename("image.jpeg"), ".jpeg");
    EXPECT_EQ(parser::basename("no_extension"), "");
    EXPECT_EQ(parser::basename("multiple.dots.in.name"), ".name");
    EXPECT_EQ(parser::basename(""), "");
} 