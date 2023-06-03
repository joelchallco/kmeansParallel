// OpenMP header
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <fstream>

using namespace std;

#define INF 999999;

struct point {
    double _x, _y;
    int _group;
    point(double x, double y, int group=-1) {
        _x = x;
        _y = y;
        _group = group;
        //minDist = INF;
    }
    double distancia(point p) {
        return sqrt((p._x - _x) * (p._x - _x) + (p._y - _y) * (p._y - _y));
    }
    
};

class kmeans {
public:
    int _k;
    string _fName;
    vector<point> points;
    vector<point> centroides;

    kmeans(int k, string fName) {
        _k = k;
        _fName = fName;
        setPoints(fName);
        initCentroides();
        for (int i = 0; i < 100; i++) {
            updatePointCluster();

            updateCentroideCluster();
        }

        write_csv("points_clustered.csv", points);
        write_csv("centroids.csv", centroides);
    }


    bool setPoints(string fName) {
        ifstream file;
        string line;
        file.open(fName);

        if ( file.is_open() ) {
            //getline(file, line);
            while ( getline(file, line)) {
                double x,y;
                stringstream iss;
                iss << line;

                iss >> x >> y;
                //point tmp(stof(x), stof(y));
                //point tmp(x, y);
                points.push_back(point(x, y));

            }
        }
        return 1;
    }
    bool getPoints() {
        cout << points.size() << endl;
        return 1;
    }

    bool initCentroides() {
        srand(time(0));
        

        for (int i = 0; i < _k; i++) {
            point ctrd(rand() % 10, rand() % 10);
            centroides.push_back( ctrd);
        }
        //cout << centroides[1]._x << endl;
        //cout << centroides[1]._y << endl;
        //cout << centroides.size() << endl;
        return 1;
    }


    bool updatePointCluster() {
        for (int i = 0; i < points.size(); i++) {
            vector<double> dis;
            for (int j=0; j < centroides.size(); j++) {
                dis.push_back(points[i].distancia(centroides[j]));
                //cout << points[i].distancia(centroides[j]) << endl;
            }
            //cout << dis.size() << endl;
            double min = dis[0];
            points[i]._group = 0;
            for (int j = 1; j < dis.size(); j++) {
                if (min > dis[j]) {
                    min = dis[j];
                    points[i]._group = j;
                    //cout << "sdfsf" << endl;
                }
                    //cout << points[i]._group << endl;
                    //system("pause");
            }
            //cout <<"cluster: " << points[i]._group << endl;
            
        }
        //cout << "primer punto: " << points[7000]._group << endl;
        //system("pause");
        return 1;
    }

    bool updateCentroideCluster() {
        //cout << "divPoint" << endl;
        for (int i = 0; i < centroides.size(); i++) {
            double tmpx = 0.0, tmpy = 0.0;
            int divPoint = 0;
            for (int j = 0; j < points.size(); j++) {
                if (i == points[j]._group) {
                    tmpx += points[j]._x;
                    tmpy += points[j]._y;
                    divPoint++;
                    //cout << tmpx << endl;
                }
            }
            if (divPoint == 0) {
                centroides[i]._x = 0;
                centroides[i]._y = 0;
            }
            else {
                centroides[i]._x = tmpx / divPoint;
                centroides[i]._y = tmpy / divPoint;
            }
            //cout << centroides[i]._x << endl;
            //cout << centroides[i]._y << endl;
        }
        return 1;
    }




    void write_csv(string filename, vector<point>& dataset) {
        // Make a CSV file with one or more columns of integer values
        // Each column of data is represented by the pair <column name, column data>
        //   as std::pair<std::string, std::vector<int>>
        // The dataset is represented as a vector of these columns
        // Note that all columns should be the same size

        // Create an output filestream object
        std::ofstream myFile(filename);

        // Send column names to the stream

        myFile << "x,y,cluster_id";
        myFile << "\n";

        // Send data to the stream
        for (int j = 0; j < dataset.size(); ++j)
        {
            myFile << dataset[j]._x << "," << dataset[j]._y << "," << dataset[j]._group;
            myFile << "\n";
        }
        // Close the file
        myFile.close();
    }


};



class kmeansParallel {
public:
    int _k;
    string _fName;
    vector<point> points;
    vector<point> centroides;

    kmeansParallel(int k, string fName) {
        _k = k;
        _fName = fName;
        setPoints(fName);
        initCentroides();
        for (int i = 0; i < 100; i++) {
            updatePointCluster();

            updateCentroideCluster();
        }

        write_csv("points_clustered.csv", points);
        write_csv("centroids.csv", centroides);
    }


    bool setPoints(string fName) {
        ifstream file;
        string line;
        file.open(fName);

        if (file.is_open()) {
            //getline(file, line);
            while (getline(file, line)) {
                double x, y;
                stringstream iss;
                iss << line;

                iss >> x >> y;
                //point tmp(stof(x), stof(y));
                //point tmp(x, y);
                points.push_back(point(x, y));

            }
        }
        return 1;
    }

    bool initCentroides() {
        srand(time(0));


        for (int i = 0; i < _k; i++) {
            point ctrd(rand() % 10, rand() % 10);
            centroides.push_back(ctrd);
        }
        //cout << centroides[1]._x << endl;
        //cout << centroides[1]._y << endl;
        //cout << centroides.size() << endl;
        return 1;
    }

    bool updateCentroideCluster() {
        //cout << "divPoint" << endl;
        for (int i = 0; i < centroides.size(); i++) {
            double tmpx = 0.0, tmpy = 0.0;
            int divPoint = 0;
            for (int j = 0; j < points.size(); j++) {
                if (i == points[j]._group) {
                    tmpx += points[j]._x;
                    tmpy += points[j]._y;
                    divPoint++;
                    //cout << tmpx << endl;
                }
            }
            if (divPoint == 0) {
                centroides[i]._x = 0;
                centroides[i]._y = 0;
            }
            else {
                centroides[i]._x = tmpx / divPoint;
                centroides[i]._y = tmpy / divPoint;
            }
            //cout << centroides[i]._x << endl;
            //cout << centroides[i]._y << endl;
        }
        return 1;
    }

    bool updatePointCluster(vector<point> secPoint) {
        for (int i = 0; i < points.size(); i++) {
            vector<double> dis;
            for (int j = 0; j < centroides.size(); j++) {
                dis.push_back(points[i].distancia(centroides[j]));
                //cout << points[i].distancia(centroides[j]) << endl;
            }
            //cout << dis.size() << endl;
            double min = dis[0];
            points[i]._group = 0;
            for (int j = 1; j < dis.size(); j++) {
                if (min > dis[j]) {
                    min = dis[j];
                    points[i]._group = j;
                    //cout << "sdfsf" << endl;
                }
                //cout << points[i]._group << endl;
                //system("pause");
            }
            //cout <<"cluster: " << points[i]._group << endl;

        }
        //cout << "primer punto: " << points[7000]._group << endl;
        //system("pause");
        return 1;
    }








    void write_csv(string filename, vector<point>& dataset) {
        // Make a CSV file with one or more columns of integer values
        // Each column of data is represented by the pair <column name, column data>
        //   as std::pair<std::string, std::vector<int>>
        // The dataset is represented as a vector of these columns
        // Note that all columns should be the same size

        // Create an output filestream object
        std::ofstream myFile(filename);

        // Send column names to the stream

        myFile << "x,y,cluster_id";
        myFile << "\n";

        // Send data to the stream
        for (int j = 0; j < dataset.size(); ++j)
        {
            myFile << dataset[j]._x << "," << dataset[j]._y << "," << dataset[j]._group;
            myFile << "\n";
        }
        // Close the file
        myFile.close();
    }
};












  
int main(int argc, char* argv[])
{
    int nthreads, tid;

    kmeans k(5, "../puntos_5_bloques.txt");
    //k.setPoints("../puntos_2_bloques.txt");
    //k.initCentroides();
    //cout << k.centroides[0]._x << endl;
    //cout << k.centroides[0]._y << endl;
    //k.updatePointCluster();
    //k.updateCentroideCluster();

    //cout << "---------------" << endl;
    //k.updatePointCluster();
    //k.updateCentroideCluster();

    //cout << "---------------" << endl;
    //k.updatePointCluster();
    //k.updateCentroideCluster();
    //cout << k.centroides[0]._x << endl;
    //cout << k.centroides[0]._y << endl;
    //k.getPoints();

    // Begin of parallel region
    #pragma omp parallel private(nthreads, tid)
    {
        // Getting thread number
        tid = omp_get_thread_num();
        printf("Welcome to GFG from thread = %d\n",
               tid);
  
        if (tid == 0) {
  
            // Only master thread does this
            nthreads = omp_get_num_threads();
            printf("Number of threads = %d\n",
                   nthreads);
        }
    }
}
