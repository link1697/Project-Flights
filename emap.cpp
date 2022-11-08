/**
 * @file emap.cpp
 *
 * Implementation of the emap class, contains visualization functions.
 * 
 * @author Haoyu Li
 * @date Fall 2021
 */

#include "includes/emap.h"
#define PI 3.14159265358979323846
using namespace cs225;
using namespace std;

//initialization
emap::emap(PNG pg1){
    worldmap=pg1;
    pathmap=pg1;

    //generate 3D map
    height_3d=501;
    width_3d=501;
    worldmapwidth=worldmap.width();
    worldmapheight=worldmap.height();
    PNG* m3 = new PNG(width_3d,height_3d);
    map3d = *m3;
    //mapping the 2D coordinates to 3D coordinates
    for (double x = 0; x< width_3d; x++) {
        for (double y = 0; y < height_3d; y++) {
            // for pixels should be seen
            if (pow(x-250,2) + pow(y-250,2) <= 250*250) {
                //get latitude
                double degree;
                degree = (y-250) / 250;
                if(degree>1) degree=1;    
                if(degree<-1) degree=-1;         
                double temp_latitude = asin(degree) ; 
                double temp = 250*cos(temp_latitude);
                temp_latitude = temp_latitude*180/PI;

                // get longitude   
                double temp_longitude; 
                if (temp < 1) {
                    temp_longitude = 90;
                }
                else {
                    degree = (x-250) / temp;
                    if(degree>1) degree=1;
                    if(degree<-1) degree=1;
                    temp_longitude = asin(degree) ;
                    temp_longitude = temp_longitude*180/PI;
                }

                //change color
                int x2d = worldmapwidth / 2 + temp_longitude / 180 * worldmapwidth / 2;
                int y2d = worldmapheight / 2 + temp_latitude / 90 * worldmapheight / 2;
                if(x2d >= worldmapwidth) x2d = worldmapwidth - 1;
                if(y2d >= worldmapheight) y2d = worldmapheight - 1;  
                map3d.getPixel(x,y) = worldmap.getPixel(x2d,y2d);
            }
        }
    }
    map3d.writeToFile("3dmaporiginal.png");
}

// show original map
void emap::showoriginalmap(){
    worldmap.writeToFile("outputmap.png");
}

// change color for pixel(x,y) in png
void emap::changecolorfor(double x,double y,PNG& png){
    HSLAPixel &pix=png.getPixel(x,y);
    pix.h=180;
	pix.s=0.9;
	pix.l=0.9;
    pix.a=1;
}

//change color in showmap and update the ap
void emap::path_line_color(vector<pair<int, int>> &point_path, vector<pair<int, int>>& path_location, PNG& showmap, int& point, unsigned& i, int& currh, int& lasth, int& floorofh, int& dirofh){
    //go through all points, change the color and add them into path_location
    for(currh = lasth; currh != point_path[i].second+floorofh; currh = currh + dirofh){
        changecolorfor(point,currh, showmap);
        pair<int, int> currpair{point,currh};
        path_location.push_back(currpair);
        if(currh == point_path[i+1].second) break;
    }
    pair<int, int> currpair{point,currh};
    path_location.push_back(currpair);
    changecolorfor(point,currh, showmap);
    //update last height
    lasth =currh;
 
}

//get the point_path as the input of the showanimation function and change the color inside the path
vector<pair<int, int>> emap::path_find_color(vector<pair<int, int>> &point_path, PNG& showmap){
    vector<pair<int, int>> loc;
    if(point_path.size()==0) return loc;
    for(unsigned i=0;i<point_path.size()-1;i++){
        //from airport point_path[i] to airport point_path[i+1]

        int point = point_path[i].first;  //using width information to track point
        int lasth = point_path[i].second; //last height
        int dis = 0;
        int floorofh;  //floor of height
        int disofw;  //moveing distance of width
        int currh;  //current height

        //moving direction of height
        int dirofh;
        if (point_path[i+1].second - point_path[i].second > 0) 
            dirofh = 1;
        else if (point_path[i+1].second - point_path[i].second < 0) 
            dirofh = -1;
        else 
            dirofh = 0;

        //four conditions with different edge case setting and different moving direction.
        if (point_path[i].first < worldmapwidth/2 ) {
            if (point_path[i+1].first < point_path[i].first + worldmapwidth/2 && point_path[i+1].first > point_path[i].first) {
                //case 1: move right (point++) and wouldn't cross the map

                while (point != point_path[i+1].first) {
                    //case: move from one edge to another
                    if(point == worldmapwidth) point=0;
                    //move
                    point++;
                    dis++;
                    //calculate the floor of height
                    if(point_path[i+1].first == point_path[i].first) 
                        floorofh = worldmapheight;  
                    else 
                        floorofh = (point_path[i+1].second-point_path[i].second)*dis/(point_path[i+1].first-point_path[i].first);
                    //update the path and change the color
                    path_line_color(point_path, loc, showmap, point, i, currh, lasth, floorofh, dirofh);
                }
            }
            else {
                //case 1: move left (point--) and might cross the map

                while (point != point_path[i+1].first){
                    //case: move from one edge to another
                    if(point==0) point=worldmapwidth;
                    //move
                    point--;
                    dis++;     
                    //calculate distance of width first               
                    if (point_path[i+1].first < point_path[i].first) 
                        disofw = point_path[i].first-point_path[i+1].first;
                    else if (point_path[i+1].first==point_path[i].first) 
                        disofw=1;
                    else 
                        disofw = point_path[i].first-point_path[i+1].first+worldmapwidth;
                    //then calculate the floor of height, 
                    floorofh = (point_path[i+1].second-point_path[i].second)*dis/disofw;
                    //update the path and change the color
                    path_line_color(point_path, loc, showmap, point, i, currh, lasth, floorofh, dirofh);
                }
            }

        } else if (point_path[i].first >= worldmapwidth/2 ) {
            if(point_path[i+1].first >= point_path[i].first - worldmapwidth/2 && point_path[i+1].first < point_path[i].first ){
                //case 3: move left (point--) and wouldn't cross the map

                while( point != point_path[i+1].first ){
                    //case: move from one edge to another
                    if(point==0) point=worldmapwidth;
                    //move
                    point--;
                    dis++;                             
                    //calculate distance of width first  
                    if(point_path[i+1].first==point_path[i].first) 
                        disofw =1;    
                    else 
                        disofw = point_path[i].first-point_path[i+1].first;
                    //then calculate the floor of height, 
                    floorofh = (point_path[i+1].second-point_path[i].second)*dis/disofw;
                    //update the path and change the color
                    path_line_color(point_path, loc, showmap, point, i, currh, lasth, floorofh, dirofh);
                }
            } else{
                //case 4: move right (point++) and might cross the map

                while( point != point_path[i+1].first ){
                    //move
                    point++;
                    dis++;
                    //case: move from one edge to another
                    if (point == worldmapwidth) point=0;
                    //calculate distance of width first  
                    if (point_path[i+1].first > point_path[i].first) 
                        disofw = -point_path[i].first+point_path[i+1].first;
                    else if (point_path[i+1].first==point_path[i].first) 
                        disofw=1;
                    else 
                        disofw = -point_path[i].first+point_path[i+1].first+worldmapwidth; 
                    //then calculate the floor of height     
                    floorofh = (point_path[i+1].second-point_path[i].second)*dis/disofw;
                    //update the path and change the color
                    path_line_color(point_path, loc, showmap, point, i, currh, lasth, floorofh, dirofh);
                }
            }
        }
    }
    return  loc;
} 


void emap::showpath(vector<pair<double,double>> path, string outputfilename, int countnum){
    if( path.size() <= 1 ) return;
    PNG showpath_map;
    showpath_map = worldmap;
    
    //airports vector
    vector<pair<int, int>> map_path;

    //for all airports, store in map_path and change the color
    for (auto airp : path) {
        int x=worldmapwidth/2+airp.first/180*worldmapwidth/2;
        int y=worldmapheight/2-airp.second/90*worldmapheight/2;
        if( x==worldmapwidth ) x=0;
        if( y==worldmapheight ) y=worldmapheight-1;
        pair<int, int> p{x,y};
        map_path.push_back(p);
        changecolorfor(x,y,showpath_map);
    }

    //transform path and change its color
    vector<pair<int, int>> thepath;
    thepath = path_find_color(map_path, showpath_map); 
    
    showpath_map.writeToFile(outputfilename+"_showpath.png");

    //visualization
    showanimation(thepath,countnum,outputfilename);
 
}

//create 2-D animation
void emap::showanimation(vector<pair<int,int>>& path, int count, string outputfilename)   {
    Animation animation1;
    PNG showmap = worldmap;
    animation1.addFrame(showmap);
    //create frame per count points
    for(unsigned i=0;i<path.size();i++){
        int temp_x=path[i].first;
        int temp_y=path[i].second;
        changecolorfor(temp_x,temp_y,showmap);
        if(i%count==0) 
        animation1.addFrame(showmap);
    }   
    animation1.addFrame(showmap);
    animation1.write(outputfilename+"_2D_path.gif");
    
    //create 3-D animation
    create3danimate(0, outputfilename,showmap);

    //create another 3-D animation, not suitable for traversal pathes, use count as a fliter.
    if( count<=100 )
        create3danimatev2(path, outputfilename);
}


void emap::create3danimate(double degree, string outputfilename, PNG& pathmap)   {
    Animation animation2;
    PNG c3d = map3d;

    // 90 frames, degree change 4 per frame
    double d = degree;
    for(int count = 0; count < 90; count++){
        // change degree for edge case
        while(d >= 180){
            d = d - 360;
        }
        while(d < -180){
            d = d + 360;
        }
        // mapping coordinates information from pathmap, use the assigned degree(d) at the time
        for(double x = 0; x<width_3d; x++) {
            for(double y = 0; y < height_3d; y++){
                if( pow(x-250,2)+pow(y-250,2) <= 250*250 ){
                    //latitude
                    double deg; 
                    deg = (y-250) / 250;
                    if(deg>1) deg=1;    
                    if(deg<-1) deg=-1;
                    double temp_latitude = asin(deg) ; 
                    double temp = 250*cos(temp_latitude );
                    temp_latitude  = temp_latitude *180/PI;

                    // longitude 
                    deg = (x-250) / temp;
                    if(deg>1) deg=1;    
                    if(deg<-1) deg=-1;   
                    double temp_longitude = asin(deg) ;       
                    temp_longitude = temp_longitude*180/PI+d;
                    if(temp_longitude>180) temp_longitude=temp_longitude-360;
                    if(temp_longitude<-180) temp_longitude=temp_longitude+360;

                    int x2d=worldmapwidth/2+temp_longitude/180*worldmapwidth/2;
                    int y2d=worldmapheight/2+temp_latitude /90*worldmapheight/2;
                    if(x2d>=worldmapwidth) x2d=worldmapwidth-1;
                    if(y2d>=worldmapheight) y2d=worldmapheight-1;

                    //get color
                    c3d.getPixel(x,y) =  pathmap.getPixel(x2d,y2d);
                }
            }
        }
        d=d+4;
        animation2.addFrame(c3d);
    } 
    animation2.write(outputfilename+"_3D_animation_v1.gif");
}

void emap::create3danimatev2(vector<pair<int,int>>& path, string outputfilename)   {
    Animation animation3;
    PNG mapan3=map3d;
    PNG mapan2d=worldmap;
    for(unsigned i=0;i<path.size();i++){
        //change color for 2-D map
        int tempx=path[i].first;
        int tempy=path[i].second;
        changecolorfor(tempx,tempy,mapan2d); 
        // add a frame per 10 counts    
        if (i%10 == 0){
            // change color for pixels in 3-D map that should be shown
            for(double x=0;x<width_3d;x++) {
                for(double y=0;y<height_3d;y++){
                    if( pow(x-250,2)+pow(y-250,2) <= 250*250 ){
                        //latitude
                        double deg;
                        deg = (y-250) / 250;
                        if(deg>1) deg=1;    
                        if(deg<-1) deg=-1;
                        double temp_latitude = asin(deg) ; 
                        double temp = 250*cos(temp_latitude);
                        temp_latitude = temp_latitude*180/PI;

                        // longitude   
                        deg = (x-250) / temp;
                        if(deg>1) deg=1;    
                        if(deg<-1) deg=-1;
                        double temp_longitude = asin(deg) ;         
                        
                        //use the location of the point to get the longitude 
                        temp_longitude = temp_longitude*180/PI+(tempx-worldmapwidth/2)*360/worldmapwidth;
                        if(temp_longitude>180) temp_longitude=temp_longitude-360;
                        if(temp_longitude<-180) temp_longitude=temp_longitude+360;

                        //get location
                        int x2d=worldmapwidth/2+temp_longitude/180*worldmapwidth/2;
                        int y2d=worldmapheight/2+temp_latitude/90*worldmapheight/2;
                        if(x2d>=worldmapwidth) x2d=worldmapwidth-1;
                        if(y2d>=worldmapheight) y2d=worldmapheight-1;

                        //change color for 3-D map
                        mapan3.getPixel(x,y) = mapan2d.getPixel(x2d,y2d);
                    }
                }
            }
            //add frame
            animation3.addFrame(mapan3);
        }
    }  
    animation3.write(outputfilename+"_3D_animation_v2.gif");
}
