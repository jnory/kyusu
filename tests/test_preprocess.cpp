#include <gtest/gtest.h>

#include "test_env.h"
#include "../src/config.h"
#include "../src/preprocess.h"


class TestPreprocess: public ::testing::Test {
    virtual void SetUp(){
    }

    virtual void TearDown(){
    }
};

TEST_F(TestPreprocess, test_length_filter) {
    kyusu::LengthFilter filter(5, 10);
    ASSERT_FALSE(filter.filter("123"));
    ASSERT_FALSE(filter.filter("1234"));
    ASSERT_TRUE(filter.filter("12345"));
    ASSERT_TRUE(filter.filter("123456"));
    ASSERT_TRUE(filter.filter("1234567"));
    ASSERT_TRUE(filter.filter("12345678"));
    ASSERT_TRUE(filter.filter("123456789"));
    ASSERT_TRUE(filter.filter("123456789A"));
    ASSERT_FALSE(filter.filter("123456789AB"));
}

TEST_F(TestPreprocess, test_space_replacer) {
    kyusu::Config config(KYUSU_TEST_DATA_DIR + "/config.json");
    kyusu::SpaceReplacer replacer(config);
    ASSERT_EQ(replacer.replace("123"), "123");
    ASSERT_EQ(replacer.replace("123 45"), "123 45");
    ASSERT_EQ(replacer.replace("123　45"), "123 45");
}

TEST_F(TestPreprocess, test_strip_replacer) {
    kyusu::StripReplacer replacer;
    ASSERT_EQ(replacer.replace("123"), "123");
    ASSERT_EQ(replacer.replace(" 123"), "123");
    ASSERT_EQ(replacer.replace("123 "), "123");
    ASSERT_EQ(replacer.replace(" 123 "), "123");
}
