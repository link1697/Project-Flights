/**
 * @file flymap.cpp
 * Implementation of the Flymap class.
 * Using Flymap class to load the path and location information from dataset.
 * 
 * @author Haoyu Li
 * @date Fall 2021
 */

#include"includes/flymap.h"

using namespace std;

#define EARTH_R 6372.79756085;
#define PI 3.14159265358979323846


Flymap::Flymap(const string& filename1,const string& filename2) {
    string line;
    //import the airports file.
    ifstream infile(filename1);
    if (infile.is_open()) {
        while (getline(infile,line)) {
            string theline = line;
            vector<string> split;
            split.push_back("");
            unsigned cnt = 0;
            /* split the input files into lines and put them in arrays.
            each line represents one airport.*/
            bool flag = true; // to avoid "," in the ""
            for (unsigned i = 0; i < theline.length(); i++) {
                if (theline[i] == '"') {
                    flag = !flag;
            
                }
                if (flag) {
                    // use "," to split one line into one array.
                    if (theline[i] == ',') {          
                        split.push_back("");
                        cnt++;
                        continue;
                    }
                }
                split[cnt] += theline[i]; 
            }
            // take the information from the array.
            unsigned id_ = atoi(split[0].c_str());
            string name_ = split[1].substr(1, split[1].size()-2);
            double latitude_ = stod(split[6].c_str());
            double longitude_ = stod(split[7].c_str());
            airport * currAirport = new airport(airp.size(), id_, name_, latitude_, longitude_);  
            airp.push_back(currAirport);
        }
    }
    //import the airlines file.
    ifstream infile2(filename2);
    if (infile2.is_open()) {
        while (getline(infile2,line)) {
            string theline = line;
            vector<string> split;
            split.push_back("");
            unsigned cnt = 0;
            /* split the input files into lines and put them in arrays.
            each line represents one airline.*/
            bool flag = true; // to avoid "," in the ""
            for (unsigned i = 0; i < theline.length(); i++) {
                if (theline[i] == '"') {
                    flag = !flag;
                }
                if (flag) {
                    // use "," to split one line into one array.
                    if (theline[i] == ',') {                 
                        split.push_back("");
                        cnt++;
                        continue;
                    }
                }
                split[cnt] += theline[i];
            }
            // take the information from the array.
            size_t source_airport_id = atoi(split[3].c_str());
            size_t destination_airport_id = atoi(split[5].c_str());
            airport * source_airport;
            airport * destination_airport;
            for (unsigned i = 0; i < airp.size(); i++) {
                if (airp[i]->dataid == source_airport_id) {
                    source_airport = airp[i];
                }
                if (airp[i]->dataid == destination_airport_id) {
                    destination_airport = airp[i];
                }
            }
            airline * currAirline = new airline(source_airport, destination_airport);
            airl.push_back(currAirline);
            source_airport -> lines.push_back(airl.size()-1);
        }
    }
}


int Flymap::findmin(vector<double> & dis, vector<bool> & visited) {
    int index = -1;
    bool checkinitial = false;
    for (unsigned i = 0; i < dis.size(); i++) {
        if (!checkinitial) {
            if (dis[i] >= 0 && visited[i] == false) {
                index = i;
                checkinitial = true;
            }
        }
        if (dis[i] >= 0 && dis[i] < dis[index] && visited[i] == false) {
            index = i;
        }
    }
    return index;
}

//return haversinedistance of two airports
double HaversineDistance(airport * a, airport * b) {
    double lati1 = a -> latitude;
    double long1 = a -> longitude;
    double lati2 = b -> latitude;
    double long2 = b -> longitude;

    lati1 = lati1/180 * PI;
    long1 = long1/180 * PI;

    lati2 = lati2/180 * PI;
    long2 = long2/180 * PI;

    double dlati = lati1 - lati2; 
    double dlong = long1 - long2;

    double output = pow(sin(dlati / 2), 2) + 
                    cos(lati1) * cos(lati2) *
                    pow(sin(dlong / 2), 2);

    output = 2 * asin(sqrt(output));

    return output * EARTH_R;

}

//DFS traversal from the startindex, print the path.
void Flymap::DFS(int starti){
    vector<bool> visited; // for vertex
    for (unsigned u = 0; u < airp.size(); u++) {
        visited.push_back(false);
    }
    
    stack<int> st; // for DFS traversal
    stack<int> prev; // for each vertex in the traversal stack, store the previous vertex.
    st.push(starti);
    prev.push(-1);

    while (!st.empty()){
        // take the top vertex in the stack.
        int currVertex = st.top();
        st.pop();
        int prevVertex = prev.top();
        prev.pop();
        // Check if the vertex is visited.
        if (!visited[currVertex]) {
            // if not, output the edge between the previous vertex and current vertex as a discover edge.
            if (prevVertex != -1)
            std::cout << "discover " << airp[prevVertex]->name << "->" << airp[currVertex]->name << std::endl;
            visited[currVertex] = true;
            // put the next vertices of the current Vertex in the stack.
            if (airp[currVertex]->lines.size() != 0)
            for (int nextlines: airp[currVertex]->lines) { 
                int nextv = airl[nextlines]->destination->id; 
                st.push(nextv);
                prev.push(currVertex);
            } 
        } else {
            // if it is, output the edge between the previous vertex and current vertex as a back edge.
            if (prevVertex != -1) {
                std::cout << "back " << airp[prevVertex]->name << "->" << airp[currVertex]->name << std::endl;
            }   
        }
    }
}

//DFS traversal from the startindex, return the path.
vector<int> Flymap::dfspath(int starti){
    vector<bool> visited; // for vertex
    vector<int> path;

    for(unsigned u = 0; u < airp.size(); u++){
        visited.push_back(false);
    }
    
    stack<int> st; // for DFS traversal
    st.push(starti);

    while (!st.empty()) {
        // take the top vertex in the stack.
        int currVertex = st.top(); 
        st.pop();
        if (!visited[currVertex]) {
            // if it is not visited, we put all the next vertices into the stack.
            visited[currVertex] = true;
            path.push_back(currVertex);
            if (airp[currVertex]->lines.size() != 0)
            for (int nextlines: airp[currVertex]->lines) { 
                int nextv = airl[nextlines]->destination->id;
                if (!visited[nextv])
                st.push(nextv);
            }
        }
    }
    return path;
}

//BFS traversal from the startindex, print the path.
void Flymap::BFS(int starti) {
    int v = airp.size();
    int e = airl.size();
    vector<bool> visited;
    vector<bool> evisited;

    for(int u = 0; u < e; u++) {
        evisited.push_back(false);
    }

    for (int u = 0; u < v; u++) {
        visited.push_back(false);
    }
    
    v=starti;
    queue<int> qu;
    visited[v] = true;
    qu.push(v);

    while (!qu.empty()){

        v = qu.front();
     
        std::cout<<endl;
        std::cout<<"airport ";
        std::cout << airp[v]->name<< endl;

        qu.pop();
    
        if(airp[v] -> lines.size()!=0)
        for(int nextlines: airp[v] -> lines) {
            int nextv=airl[nextlines] -> destination -> id;
            if (visited[nextv] == false) {
                evisited[nextlines] = true; 

                std::cout<<"discover ";

                std::cout << airl[nextlines]->source->name << "->" << airl[nextlines]->destination->name << std::endl;
    
                qu.push(nextv);
                visited[nextv] = true;
            } else {
                if (evisited[nextlines] == false) {
                    evisited[nextlines] = true; 

                    std::cout<<"cross ";
                    std::cout << airl[nextlines]->source->name << "->" << airl[nextlines]->destination->name << std::endl;
                }    
            }
        }
    }
}

//BFS traversal from the startindex, return the path.
vector<int> Flymap::bfspath(int starti) {
    int v = airp.size();
    int e = airl.size();
    vector<bool> visited;
    vector<bool> evisited;
    vector<int> path;

    for(int u=0; u < e; u++) {
        evisited.push_back(false);
    }

    for (int u = 0; u < v; u++) {
        visited.push_back(false);
    }
    
    v = starti;
    queue<int> qu;
    visited[v] = true;
    qu.push(v);

    while (!qu.empty()){
        v = qu.front();
        qu.pop();
        if(airp[v] -> lines.size()!=0)
        for (int nextlines: airp[v] -> lines) {
            int nextv=airl[nextlines] -> destination -> id;
            if (visited[nextv] == false) {
                evisited[nextlines] = true;
                path.push_back(nextlines);
                qu.push(nextv);
                visited[nextv] = true;
            } else {
                if (evisited[nextlines] == false) {
                    evisited[nextlines] = true; 
                    continue;
                }    
            }
        }
    }
    return path;
}

//Add an airport
void Flymap::addAirport (airport * x) {
    airp.push_back(x);
}

//Add an airline
void Flymap::addAirline (airline * x) {
    airl.push_back(x);
}

// using dijkstra algorithm, return the shortest distance between two airports
// return the distance, print the path information
double Flymap::spdijkstra(int indxair1, int indxair2){
    std::cout << endl;
    std::cout << "the shortest path using dijkstra:" << endl;
    std::cout << endl;
    if (indxair1 == indxair2) {
        return 0;
    }
    int v=airp.size();
    int e=airl.size();
    vector<bool> visited;
    vector<bool> evisited;
    vector<double> distance;
    vector<int> pred;
    vector<int> airportvisited;
    vector<int> notvisited;
    // vector< vector<int> > adj; 

    e = indxair2;
    
    for (int u = 0; u < v; u++) {
        visited.push_back(false);
        notvisited.push_back(u);
        distance.push_back(-1);
        pred.push_back(-1);
    }
   
    v = indxair1;

    visited[v] = true;
    distance[v] = 0;
    airportvisited.push_back(v);
    while (visited[e] != true) {
        if (airp[v] -> lines.size() != 0)
        for (int nextlines: airp[v] -> lines) {
            int nextv=airl[nextlines] -> destination -> id; 
            if (!visited[nextv]) {
                double newdistance = airl[nextlines] -> Hdistance;
                if (distance[nextv] < 0 || distance[nextv] > distance[v] + newdistance) {
                    distance[nextv] = distance[v] + newdistance;
                    pred[nextv] = v;
                }
            }
        }
        v = findmin(distance, visited);   
        if(v == -1) {
            std::cout<<"no way"<<endl;
            return -1;
        }
        visited[v] = true;
    }

    double shortestdis = distance[e];

    v=indxair1;
    vector<int> path;
    while(e != v){
        path.push_back(e);
        e=pred[e];
    }   
    vector<int> shortestpath;
    shortestpath.push_back(v);
    for(unsigned i = path.size()-1; i > 0; i--){
        shortestpath.push_back(path[i]);
    }  
    shortestpath.push_back(path[0]);
    for(unsigned i=0; i<shortestpath.size(); i++){
        std::cout << airp[shortestpath[i]]->name << endl;
    }   
    std::cout<<endl;
    return shortestdis;
}

// using dijkstra algorithm, return the shortest distance between two airports
// return the distance, print the path information
vector<int> Flymap::spdijkstrapath(int indxair1, int indxair2){    
    if (indxair1 == indxair2) {
        vector<int> dijpath;
        return dijpath;
    }
    int v=airp.size();
    int e=airl.size();
    vector<bool> visited;
    vector<bool> evisited;
    vector<double> distance;
    vector<int> pred;
    vector<int> airportvisited;
    vector<int> notvisited;
    // vector< vector<int> > adj; 

    e = indxair2;
    
    for (int u = 0; u < v; u++) {
        visited.push_back(false);
        notvisited.push_back(u);
        distance.push_back(-1);
        pred.push_back(-1);
    }
   
    v = indxair1;

    visited[v] = true;
    distance[v] = 0;
    airportvisited.push_back(v);
    while (visited[e] != true) {
        if (airp[v] -> lines.size() != 0)
        for (int nextlines: airp[v] -> lines) {
            int nextv=airl[nextlines] -> destination -> id; 
            if (!visited[nextv]) {
                double newdistance = airl[nextlines] -> Hdistance;
                if (distance[nextv] < 0 || distance[nextv] > distance[v] + newdistance) {
                    distance[nextv] = distance[v] + newdistance;
                    pred[nextv] = v;
                }
            }
        }
        v = findmin(distance, visited);   
        if(v == -1) {
            std::cout<<"no way"<<endl;
            vector<int> dijpath;
            return dijpath;
        }
        visited[v] = true;
    }

    double shortestdis = distance[e];

    v=indxair1;
    vector<int> path;
    while(e != v){
        path.push_back(e);
        e=pred[e];
    }   
    vector<int> shortestpath;
    shortestpath.push_back(v);
    for(unsigned i = path.size()-1; i > 0; i--){
        shortestpath.push_back(path[i]);
    }  
    shortestpath.push_back(path[0]);
    return shortestpath;
}

//helper funciton for astar, heuristic: HaversineDistance
int Flymap::astarfindmin(vector<double> & dis, vector<bool> & open) {
    int index = 0;
    bool checkinitial = false;
    for (unsigned i = 0; i < dis.size(); i++) {
        if (!checkinitial) {
            if (dis[i] >= 0 && open[i]) {
                index = i;
                checkinitial = true;
            }
        }
        if (dis[i] >= 0 && dis[i] < dis[index] && open[i]) {
            index = i;
        }
    }
    return index;
}


// using astar algorithm, return the shortest distance between two airports
// return the distance, print the path information
double Flymap::astar(int itair1, int itair2){
    int v = airp.size();
    int e = airl.size();
    vector<bool> inclose;  
    vector<bool> inopen; 
    vector<bool> evisited;
    vector<double> distance;
    vector<double> heuristic;
    vector<int> pred;
    vector<double> f; 
    // vector< vector<int> > adj; 

    e = itair2;
    for (int u = 0; u < v; u++) {
        inclose.push_back(false);
        inopen.push_back(false);
        heuristic.push_back(HaversineDistance(airp[u],airp[e]));
        distance.push_back(0);
        f.push_back(heuristic[u]+0);
        pred.push_back(-1);
    }

    vector<double>* fh = &f;
    flyheap fheap(fh);
 
    v = itair1;
    inopen[v] = true;
    fheap.push(v);
    int num = 1;// num of open list
    while (!inopen[e]) {
        if (num == 0){
            return 0;
        }
        v = fheap.pop();
        inclose[v] = true;
        inopen[v] = false;
        num--;
        if (airp[v]->lines.size() != 0)
        for (int nextlines : airp[v]->lines){
            int nextv = airl[nextlines]->destination->id;
            if (inclose[nextv]) continue;
            double newdistance = airl[nextlines]->Hdistance;
            if (!inopen[nextv]) {
                inopen[nextv] = true;
                num++;
                fheap.push(nextv);
                if (pred[nextv] < 0 || distance[nextv] > distance[v] + newdistance) {
                    distance[nextv] = distance[v]+newdistance;
                    pred[nextv] = v;
                }
                f[nextv] = heuristic[nextv] + distance[nextv];
                fheap.updateElem(nextv);
            } else{
                if(distance[nextv] > distance[v] + newdistance){
                    distance[nextv] = distance[v] + newdistance;
                    pred[nextv] = v;
                    f[nextv] = heuristic[nextv] + distance[nextv];
                    fheap.updateElem(nextv);
                }
            }
        }
    }

    double shortestdis = distance[e];
    v = itair1;
    vector<int> path;
   
    while(e != v){
        path.push_back(e);
        e = pred[e];
    }   

    vector<int> shortestpath;
    shortestpath.push_back(v);
    for(unsigned i = path.size()-1; i > 0; i--){
        shortestpath.push_back(path[i]);
    }  
    shortestpath.push_back(path[0]);

    std::cout << "astar find way:" << endl;
    for(unsigned i = 0; i < shortestpath.size(); i++){
        std::cout << airp[shortestpath[i]]->name << endl;
     
    }   
    std::cout << endl;
    return shortestdis;
}
    
// using astar algorithm, return the shortest distance between two airports
// return the path (return the order of airports in id)
vector<int> Flymap::astarpath(int itair1, int itair2){ 
    int v = airp.size();
    int e = airl.size();
    vector<bool> inclose;  
    vector<bool> inopen; 
    vector<bool> evisited;
    vector<double> distance;
    vector<double> heuristic;
    vector<int> pred;
    vector<double> f; 
    // vector< vector<int> > adj; 
   
    e=itair2;
    for (int u = 0; u < v; u++) {
        inclose.push_back(false);
        inopen.push_back(false);
        heuristic.push_back(HaversineDistance(airp[u],airp[e]) );
        distance.push_back(0);
        f.push_back(heuristic[u]+0);
        pred.push_back(-1);
    }

    vector<double>* fh = &f;
    flyheap fheap(fh);
 
    v=itair1;
    inopen[v] = true;
    fheap.push(v);
    int num=1;// num of open list
    while (!inopen[e]) {
        if (num == 0){
            vector<int> r;
            return r;
        }
        v = fheap.pop();
        inclose[v] = true;
        inopen[v] = false;
        num--;

        if(airp[v] -> lines.size() != 0)
        for(int nextlines: airp[v] -> lines){
            int nextv=airl[nextlines] -> destination -> id;
            if(inclose[nextv]==true) continue;
            double newdistance = airl[nextlines] -> Hdistance;
            if(inopen[nextv] == false){
                inopen[nextv]=true;
                        num++;
                        fheap.push(nextv);
                if (pred[nextv] < 0 || distance[nextv] > distance[v] + newdistance) {
                    distance[nextv] = distance[v]+newdistance;
                    pred[nextv]=v;
                }
                f[nextv] = heuristic[nextv] + distance[nextv];
                fheap.updateElem(nextv);
            } else{
                if(distance[nextv] > distance[v] + newdistance){
                    distance[nextv] = distance[v] + newdistance;
                    pred[nextv] = v;
                    f[nextv] = heuristic[nextv] + distance[nextv];
                    fheap.updateElem(nextv);
                }
            }
        }
    }

    double shortestdis = distance[e];
    v = itair1;
    vector<int> path;
   
    while(e != v){
        path.push_back(e);
        e = pred[e];
    }   

    vector<int> shortestpath;
    shortestpath.push_back(v);
    for(unsigned i = path.size()-1; i > 0; i--){
        shortestpath.push_back(path[i]);
    }  
    shortestpath.push_back(itair2);
    std::cout << endl;
    std::cout << "path astar finds:" << endl;
    for(unsigned i = 0; i < shortestpath.size(); i++){
        std::cout << airp[shortestpath[i]]->name << endl;
    }
    return shortestpath;
}

// For dijkstra pahtes, return the path can be used in emap->showpath function.
vector<pair<double,double>> Flymap::get_path_location_airports(vector<int> &path){
    vector<pair<double,double>> res;
    for(int aid : path){
        pair<double,double> p{airp[aid]->longitude,airp[aid]->latitude};
        res.push_back(p);
    }
    return res;
}

// For traversal pahtes, return the path can be used in emap->showpath function.
vector<pair<double,double>> Flymap::get_path_location_airlines(vector<int> &path){
    vector<pair<double,double>> res;
    for(int aid : path){
        pair<double,double> s{airl[aid]->source->longitude,airl[aid]->source->latitude};
        pair<double,double> d{airl[aid]->destination->longitude,airl[aid]->destination->latitude};
        res.push_back(s);
        res.push_back(d);
    }
    return res;
}

//print id and name
void Flymap::getairport(int idair1){
    std::cout << airp[idair1]->id << endl;
    std::cout << airp[idair1]->name << endl;

}

// return airport id, return -1 if doesnt exist
int Flymap::getairportid(string ap_name){
    for(auto ap : airp){
        if (ap->name == ap_name) {
            return ap->id;
        }   
    }
    return -1;
}
