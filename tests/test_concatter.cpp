#include <gtest/gtest.h>

#include "test_env.h"
#include "../src/concatter.h"
#include "../src/config.h"


class TestConcatter: public ::testing::Test {
    virtual void SetUp(){
    }

    virtual void TearDown(){
    }
};

TEST_F(TestConcatter, test_concatter_need_concat) {
    kyusu::Config config(KYUSU_TEST_DATA_DIR + "/config.json");
    kyusu::Concatter<decltype(config.hyphens())> concatter(config.hyphens());

    auto func = [&](char const *c1, char const *c2){
        auto h1 = kyusu::UTF8::to_u8char(c1, kyusu::UTF8::get_variable_length(c1));
        auto h2 = kyusu::UTF8::to_u8char(c2, kyusu::UTF8::get_variable_length(c2));
        return concatter.need_concat(h1, h2);
    };

    ASSERT_TRUE(func(u8"a", u8"b"));
    ASSERT_FALSE(func(u8"あ", u8"b"));
    ASSERT_FALSE(func(u8"a", u8"あ"));
    ASSERT_FALSE(func(u8"あ", u8"い"));
    ASSERT_FALSE(func(u8"ア", u8"い"));
    ASSERT_FALSE(func(u8"あ", u8"イ"));
    ASSERT_TRUE(func(u8"ア", u8"-"));
    ASSERT_TRUE(func(u8"-", u8"イ"));
}

TEST_F(TestConcatter, test_concatter_concat) {
    kyusu::Config config(KYUSU_TEST_DATA_DIR + "/config.json");
    kyusu::Concatter<decltype(config.hyphens())> concatter(config.hyphens());

    ASSERT_EQ(
            concatter.concat(u8"素材 A B C と X Y Z を 組み合わせ た もの を 、 アイウ - エオ と 呼ぶ 。"),
            u8"素材 ABC と XYZ を 組み合わせ た もの を 、 アイウ-エオ と 呼ぶ 。"
    );
}


