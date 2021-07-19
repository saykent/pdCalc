//
// Created by Ken You on 17/7/21.
//

#include "gtest/gtest.h"

namespace {

    class PdCalc: public testing::Test {};

    TEST_F(PdCalc, CanReallyTest) {
        EXPECT_EQ(0, 0);
    }

}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}