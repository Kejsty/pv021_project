//
// Created by Kejsty, Katarina Kejstova on 15.11.16.
//

#include "../algorithms.h"
#include <gtest/gtest.h>
#include <unistd.h>

using matrix = std::vector<std::vector<double>>;
TEST(matrixMultiplication, simple) {
    matrix fst({{1,2},{3,4}});
    matrix snd({{1,2},{2,1}});
    auto result = algorithms::matrixMultiplication(fst,snd);
    EXPECT_EQ(result[0][0], 5);
    EXPECT_EQ(result[0][1], 4);
    EXPECT_EQ(result[1][0], 11);
    EXPECT_EQ(result[1][1], 10);
}

TEST(matrixMultiplication, basic) {
    matrix fst({{1,1},{1,2},{2,1},{2,2}});
    matrix snd({{3},{4}});
    auto result = algorithms::matrixMultiplication(fst,snd);
    EXPECT_EQ(result.size(), 4);
    EXPECT_EQ(result[0].size(), 1);
    EXPECT_EQ(result[0][0], 7);
    EXPECT_EQ(result[1][0], 11);
    EXPECT_EQ(result[2][0], 10);
    EXPECT_EQ(result[3][0], 14);

}

TEST(matrixSum, basic) {
    matrix fst({{1,1},{1,2}});
    matrix snd({{3,3},{4,4}});
    auto result = algorithms::matrixSum(fst,snd);
    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(result[0].size(), 2);
    EXPECT_EQ(result[0][0], 4);
    EXPECT_EQ(result[0][1], 4);
    EXPECT_EQ(result[1][0], 5);
    EXPECT_EQ(result[1][1], 6);

}