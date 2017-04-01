#include <gtest/gtest.h>

#include "test_env.h"
#include "../src/config.h"


class TestConfig: public ::testing::Test {
    virtual void SetUp(){
    }

    virtual void TearDown(){
    }
};

TEST_F(TestConfig, test_spaces) {
    kyusu::Config config(KYUSU_TEST_DATA_DIR + "/config.json");
    GTEST_ASSERT_EQ(config.spaces()[0], "　");
}

//TODO: Write more tests for additional configurations.