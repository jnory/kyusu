#include <gtest/gtest.h>

#include "test_env.h"
#include "../src/splitter.h"
#include "../src/config.h"
#include "../src/utf8.h"


class TestSplitter: public ::testing::Test {
    virtual void SetUp(){
    }

    virtual void TearDown(){
    }
};

TEST_F(TestSplitter, test_decider) {
    kyusu::Config config(KYUSU_TEST_DATA_DIR + "/config.json");
    kyusu::Decider<decltype(config.kutens())> decider(config.kutens(), config.kakko_hirakus(), config.kakko_tojirus());

    auto func = [&](char const *c1, char const *c2){
        auto h1 = kyusu::UTF8::to_u8char(c1, kyusu::UTF8::get_variable_length(c1));
        auto h2 = kyusu::UTF8::to_u8char(c2, kyusu::UTF8::get_variable_length(c2));
        return decider.is_eof(h1, h2);
    };

    ASSERT_FALSE(func("a", "b"));
    ASSERT_FALSE(func("b", "("));
    ASSERT_FALSE(func("(", "x"));
    ASSERT_FALSE(func("x", "."));
    ASSERT_FALSE(func(".", "y"));
    ASSERT_FALSE(func("y", "z"));
    ASSERT_FALSE(func("z", ")"));
    ASSERT_FALSE(func(")", "c"));
    ASSERT_FALSE(func("c", "."));
    ASSERT_TRUE(func(".", " "));
    ASSERT_FALSE(func(" ", "a"));
    ASSERT_FALSE(func("a", "b"));
    ASSERT_FALSE(func("b", "."));
    ASSERT_FALSE(func(".", "b"));
    ASSERT_FALSE(func("c", "."));
    ASSERT_TRUE(func(".", " "));
}

TEST_F(TestSplitter, test_splitter) {
    kyusu::Config config(KYUSU_TEST_DATA_DIR + "/config.json");
    kyusu::Decider<std::vector<std::string>> decider(config.kutens(), config.kakko_hirakus(), config.kakko_tojirus());
    std::vector<std::string> lines;
    auto callback = [&](std::string const &line){
        lines.push_back(line);
    };
    kyusu::Splitter<decltype(decider), decltype(callback)> splitter(decider, callback);
    splitter.split("ab(x.yz)c. ab.c.");
    ASSERT_EQ(lines.size(), 2);
    ASSERT_EQ(lines[0], "ab(x.yz)c.");
    ASSERT_EQ(lines[1], " ab.c.");
}

