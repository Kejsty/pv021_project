//
// Created by kejsty on 15.11.16.
//

#include "../algorithmsTest.h"
#include <gtest/gtest.h>
#include <unistd.h>

using matrix = std::vector<std::vector<double>>;
TEST(matrixMultiplication, simple) {
    matrix fst({{1,2},{4,5}});
    matrix snd({{1,1},{1,1}});
    auto result = matrixMultiplication(fst,snd);
    EXPECT_EQ(result[0][0], 5);
    EXPECT_EQ(result[0][1], 4);
    EXPECT_EQ(result[1][0], 11);
    EXPECT_EQ(result[1][1], 10);
}
