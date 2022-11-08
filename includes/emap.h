/**
 * @file emap.h
 *
 * Defintion for class emap, this class is for visualization
 *
 * @author Haoyu Li
 * @date Fall 2021
 */

#pragma once

#include "../cs225/PNG.h"
#include "../cs225/Animation.h"
#include "flymap.h"
#include <list>
#include <cmath>
 #include <math.h>
#include <iostream>
using namespace cs225;

class emap{
    public:
        emap(PNG png);

        void showoriginalmap();

        //visualization for path
        void showpath(vector<pair<double,double>> path, string outputfilename, int countnum);
        
        //visualization for traversal
        void showlines(vector<pair<double,double>>& path, string outputfilename);
        

    private:
        PNG worldmap;
        PNG pathmap;
        // PNG amap;
        PNG map3d;
        int worldmapwidth;
        int worldmapheight;
        int width_3d;
        int height_3d;
        
        // void changecolor(double x,double y);
        void changecolorfor(double x,double y,PNG& png);
        

        // generate 2-D animation, add a frame per countnum counts. 
        // Because of running time, set countnum 10 for path and 100000 for traversal.
        // call functions for 3-D animation
        void showanimation(vector<pair<int,int>>& path,int countnum, string outputfilename) ;
        
        // create 3-D animation
        void create3danimate(double degree, string outputfilename, PNG& pathmap);
        
        //create 3-D animation v2
        void create3danimatev2(vector<pair<int,int>>& path, string outputfilename);

        //showpath function use this to get the path as the input of the showanimation function and change the color inside the path
        vector<pair<int, int>> path_find_color(vector<pair<int, int>> &poin_path, PNG& map);
        
        //helper funciton in path_find_color, change color in showmap and update the ap
        void path_line_color(vector<pair<int, int>> &poin_path, vector<pair<int, int>>& loc, PNG& showmap, int& point, unsigned& i, int& currh, int& lasth, int& floorofh, int& dirofh);


};