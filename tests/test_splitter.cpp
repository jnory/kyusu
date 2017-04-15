#include <gtest/gtest.h>

#include "test_env.h"
#include "../src/splitter.h"
#include "../src/config.h"


class TestSplitter: public ::testing::Test {
    virtual void SetUp(){
    }

    virtual void TearDown(){
    }
};

TEST_F(TestSplitter, test_decider) {
    kyusu::Config config(KYUSU_TEST_DATA_DIR + "/config.json");
    kyusu::Decider<decltype(config.kutens())> decider(config.kutens(), config.kakko_hirakus(), config.kakko_tojirus());

    ASSERT_FALSE(decider.is_eof("a", "b"));
    ASSERT_FALSE(decider.is_eof("b", "("));
    ASSERT_FALSE(decider.is_eof("(", "x"));
    ASSERT_FALSE(decider.is_eof("x", "."));
    ASSERT_FALSE(decider.is_eof(".", "y"));
    ASSERT_FALSE(decider.is_eof("y", "z"));
    ASSERT_FALSE(decider.is_eof("z", ")"));
    ASSERT_FALSE(decider.is_eof(")", "c"));
    ASSERT_FALSE(decider.is_eof("c", "."));
    ASSERT_TRUE(decider.is_eof(".", " "));
    ASSERT_FALSE(decider.is_eof(" ", "a"));
    ASSERT_FALSE(decider.is_eof("a", "b"));
    ASSERT_FALSE(decider.is_eof("b", "."));
    ASSERT_FALSE(decider.is_eof(".", "b"));
    ASSERT_FALSE(decider.is_eof("c", "."));
    ASSERT_TRUE(decider.is_eof(".", " "));
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

