/**
 * @file flymap.h
 *
 * Defintion for class flymap, this class is for storing information of flights.
 *
 * @author Haoyu Li
 * @date Fall 2021
 */

#pragma once
#include "flyheap.h"
#include"airport.h"
#include"airline.h"

#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <queue>
#include<map>
#include<fstream>
#include <math.h>

using namespace std;

class Flymap{
    public:
        Flymap(const string& filename1,const string& filename2);//1:airport 2:airl
        
        //DFS traversal from the startindex, print the path.
        void DFS(int air);

        //DFS traversal from the startindex, return the path.
        vector<int> dfspath(int air);

        //BFS traversal from the startindex, print the path.
        void BFS(int air);

        //BFS traversal from the startindex, return the path.
        vector<int> bfspath(int starti);

        // using dijkstra algorithm, return the shortest distance between two airports
        // return the distance, print the path information
        double spdijkstra(int air1, int air2);

        // using dijkstra algorithm, return the shortest distance between two airports
        // return the distance, print the path information
        vector<int> spdijkstrapath(int itair1, int itair2);


        // using astar algorithm, return the shortest distance between two airports
        // return the distance, print the path information
        double astar(int air1, int air2);

        // using astar algorithm, return the shortest distance between two airports
        // return the path (return the order of airports in id)
        vector<int> astarpath(int itair1, int itair2);


        void addAirport (airport * x);
        void addAirline (airline * x);
        
        //print id and name
        void getairport (int id);

        // return airport id, return -1 if doesnt exist
        int getairportid(string name);

        // For dijkstra pahtes, return the path can be used in emap->showpath function.
        vector<pair<double,double>> get_path_location_airports(vector<int> &path);
        
        // For traversal pahtes, return the path can be used in emap->showpath function.
        vector<pair<double,double>> get_path_location_airlines(vector<int> &path);
        

    private:
        vector<airport*> airp;
        vector<airline*> airl;
        
        //helper funcition for spdijkstra
        int findmin(vector<double> & dis, vector<bool> & visited);
        //helper funcition for spdijkstra
        int astarfindmin(vector<double> & dis, vector<bool> & open) ;
       


};

