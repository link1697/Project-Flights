/**
 * @file airline.cpp
 *
 * Implementation of the airline class.
 * 
 * @author Haoyu Li
 * @date Fall 2021
 */

#include "includes/airline.h"

#include <math.h>
#include <algorithm>

#define EARTH_R 6372.79756085;
#define PI 3.14159265358979323846
using namespace std;



airline::airline(airport* a, airport* b) {
    source = a;
    destination = b;
    Edistance = this -> EuclideanDistance();
    Hdistance = this -> HaversineDistance();
}

double airline::EuclideanDistance(airport * a, airport * b) {
    double lati1 = a -> latitude;
    double long1 = a -> longitude;
    double lati2 = b -> latitude;
    double long2 = b -> longitude;

    return sqrt(pow(lati1 - lati2, 2) + pow(long1 - long2, 2));
}

double airline::EuclideanDistance(double lati1, double long1, double lati2, double long2) {
    return sqrt(pow(lati1 - lati2, 2) + pow(long1 - long2, 2));
}

double airline::EuclideanDistance() {
    double lati1 = this->source->latitude;
    double long1 = this->source->longitude;

    double lati2 = this->destination->latitude;
    double long2 = this->destination->longitude;

    return EuclideanDistance(lati1, long1, lati2, long2);
}

double airline::HaversineDistance(airport * a, airport * b) {
    double lati1 = a -> latitude;
    double long1 = a -> longitude;
    double lati2 = b -> latitude;
    double long2 = b -> longitude;

    return HarversineDistance(lati1, long1, lati2, long2);

}

double airline::HarversineDistance(double lati1, double long1, double lati2, double long2) {
    lati1 = toRad(lati1);
    long1 = toRad(long1);

    lati2 = toRad(lati2);
    long2 = toRad(long2);

    double dlati = lati1 - lati2;
    double dlong = long1 - long2;

    double output = pow(sin(dlati / 2), 2) + 
                    cos(lati1) * cos(lati2) *
                    pow(sin(dlong / 2), 2);

    output = 2 * asin(sqrt(output));

    return output * EARTH_R;

}

double airline::HaversineDistance() {
    double lati1 = this->source->latitude;
    double long1 = this->source->longitude;

    double lati2 = this->destination->latitude;
    double long2 = this->destination->longitude;

    return HarversineDistance(lati1, long1, lati2, long2); 
}

double airline::toRad(double degree) {
    return degree/180 * PI;
}