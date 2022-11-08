/**
 * @file airport.h
 *
 * Defintion for class airport
 *
 * @author Haoyu Li
 * @date Fall 2021
 */

#pragma once

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class airport 
{
    public:
        airport(unsigned id, unsigned dataid,string name, double latitude, double longitude);
        unsigned id;
        unsigned dataid;
        string name;
        double latitude;
        double longitude;
        std::vector<int> lines; //airlines from the airport
};
