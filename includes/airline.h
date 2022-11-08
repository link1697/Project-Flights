/**
 * @file airline.h
 * Implementation of the animation class.
 *
 * @author Haoyu Li
 * @date Fall 2021
 */

#pragma once
#include <string>
#include <iostream>
#include <math.h>
#include <algorithm>
#include "airport.h"

using namespace std;

class airline 
{
    public:
        airline(airport * a, airport * b);

        airport* source;
        airport* destination;
        double Edistance;
        double Hdistance;

        ~airline();

        double toRad(double degree);
        double EuclideanDistance(double latitude1, double longitude1,
                                double latitude2, double longitude2);
        double EuclideanDistance(airport * a, airport * b);
        double EuclideanDistance();

        double HarversineDistance(double latitude1, double longitude1,
                                double latitude2, double longitude2);
        double HaversineDistance(airport * a, airport * b);
        double HaversineDistance();

};