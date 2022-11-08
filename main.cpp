/**
 * @file main.cpp
 * 
 * @author Haoyu Li
 * @date Fall 2021
 */

#include <iostream>
#include "includes/flymap.h"
#include "includes/flyheap.h"
#include "includes/emap.h"
 
#include <stdlib.h>
#include <vector>


int main(int argc, const char** argv){
  //load airports and airlines information
  Flymap * graph1 = new Flymap("src/airports.dat","src/airlines.dat");

  //load orininal 2D map
  PNG png;
  png.readFromFile("src/earthmapsmall.png");

  //define emap
  emap worldmap=emap(png);

  // show map:
  worldmap.showoriginalmap();

  //get airport id, if airport doesn't exist, end
  int airp_1 = graph1 ->getairportid("Los Angeles International Airport");
  int airp_2 = graph1 ->getairportid("Brigadier Mayor D Cesar Raul Ojeda Airport");
  if (airp_1 < 0 or airp_2 < 0){
    return 0;
  }

  //Run sections
  bool Do_Traversal = false;
  bool Visualize_Traversal = true;
  bool Print_dis_dijkstra = false;
  bool Print_dis_astar = false;
  bool Visualize_dijkstra_path = true;
  bool Visualize_astar_path = true;

  // Different kinds of traversal
  if (Do_Traversal) {
    graph1->DFS(airp_1);
    graph1->BFS(airp_1);
  }

  //visualization for traversal
  if (Visualize_Traversal) {
    vector<int> path_traversal;
    vector<int> path_dfs = graph1->dfspath(airp_1);
    vector<std::pair<double, double>>  lines_location_dfs = graph1->get_path_location_airlines(path_dfs);
    worldmap.showpath(lines_location_dfs, "DFS", 100000); 
  }

  // Select a path using dijkstra, only return distance
  if (Print_dis_dijkstra) {
    double dis= graph1->spdijkstra(airp_1,airp_2);
    cout << dis <<endl;
  }

  // Select a path using dijkstra and visualize
  if (Visualize_dijkstra_path){
    vector<int> path_dij  = graph1->spdijkstrapath(airp_1,airp_2);
    vector<pair<double,double>> path_location_dij;
    path_location_dij = graph1->get_path_location_airports(path_dij);
    worldmap.showpath(path_location_dij,"Visualization_dij",10); 
  }

  // Select a path using astar, only return distance
  if (Print_dis_astar) {
    double astardis= graph1->astar(airp_1,airp_2);
  }

  // Select a path using astar and visualize
  if (Visualize_astar_path){
    vector<int> path_astar = graph1->astarpath(airp_1,airp_2);
    vector<pair<double,double>> path_location_astar;
    path_location_astar=graph1->get_path_location_airports(path_astar);
    worldmap.showpath(path_location_astar,"Visualization_astar",10);  
  }

  return 0;
}
