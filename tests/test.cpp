/**
 * @file test.cpp
 * 
 * @author Haoyu Li
 * @date Fall 2021
 */

#include "../cs225/catch/catch.hpp"

#include "../includes/airport.h"
#include "../includes//airline.h"
#include "../includes//flymap.h"

#include <math.h>
#include <algorithm>
#include <vector>
#include <stack>
#include <string>
#include <queue>
#include<map>

using namespace std;

TEST_CASE ("basic_test")
{
    string test = "Hello, World!";

    REQUIRE (test == "Hello, World!");
}

TEST_CASE("test DFS")
{
    //the dfspath return the path based on the id of airline
    Flymap * graph1 = new Flymap("src/test_airp.dat", "src/test_airl.dat");
    vector<int> output = graph1 -> dfspath(0);
    REQUIRE(output[0] == 0);
    REQUIRE(output[1] == 3);
    REQUIRE(output[2] == 2);
    REQUIRE(output[3] == 7);
    REQUIRE(output[4] == 8);
    REQUIRE(output[5] == 4);
    REQUIRE(output[6] == 1);
    REQUIRE(output[7] == 6);
    REQUIRE(output[8] == 5);
    REQUIRE(output[9] == 9);

    vector<int> output2 = graph1 -> dfspath(2);
    REQUIRE(output2[0] == 2);
    REQUIRE(output2[1] == 7);
    REQUIRE(output2[2] == 8);
    REQUIRE(output2[3] == 4);
    REQUIRE(output2[4] == 1);
    REQUIRE(output2[5] == 6);
    REQUIRE(output2[6] == 5);
    REQUIRE(output2[7] == 9);
}

TEST_CASE("test BFS")
{
    //the bfspath return the path based on the id of airline
    Flymap * graph1 = new Flymap("src/test_airp.dat", "src/test_airl.dat");
    vector<int> output = graph1 -> bfspath(0);
    REQUIRE(output[0] == 0);
    REQUIRE(output[1] == 1);
    REQUIRE(output[2] == 2);
    REQUIRE(output[3] == 3);
    REQUIRE(output[4] == 5);
    REQUIRE(output[5] == 6);
    REQUIRE(output[6] == 7);
    REQUIRE(output[7] == 9);
    REQUIRE(output[8] == 8);

    vector<int> output2 = graph1 -> bfspath(2);
    REQUIRE(output2[0] == 5);
    REQUIRE(output2[1] == 6);
    REQUIRE(output2[2] == 7);
    REQUIRE(output2[3] == 9);
    REQUIRE(output2[4] == 8);
    REQUIRE(output2[5] == 10);
    REQUIRE(output2[6] == 4);

    vector<int> output3 = graph1 -> bfspath(4);
    REQUIRE(output3[0] == 4);
}

TEST_CASE("test A*")
{
    //the astarpath return the order based on if of airports
    Flymap * graph1 = new Flymap("src/test_airp.dat", "src/test_airl.dat");
    vector<int> output = graph1 -> astarpath(2,4);
    REQUIRE(output[0] == 2);
    REQUIRE(output[1] == 7);
    REQUIRE(output[2] == 8);
    REQUIRE(output[3] == 4);
}
