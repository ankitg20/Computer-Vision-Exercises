//Ankit Gupta, Computer Vision Exercise
#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <list>
#include <iterator>
#include <bits/stdc++.h>

using namespace std;

class Point
{
public:
    double xcoord;
    double ycoord;
};

double calculatedistance(Point p1, Point p2)
{
    double xp1 = p1.xcoord;
    double yp1 = p1.ycoord;
    double xp2 = p2.xcoord;
    double yp2 = p2.ycoord;
    return sqrt((xp1 - xp2)*(xp1 - xp2) + (yp1 - yp2)*(yp1 - yp2));
}

double listbruteForce(vector<Point> P, int size)
{
    double min = 1000000000;
    for (int firstvar = 0; firstvar < size; ++firstvar){
        for (int secondvar = firstvar+1; secondvar < size; ++secondvar){
            if (calculatedistance(P[firstvar], P[secondvar]) < min){
                min = calculatedistance(P[firstvar], P[secondvar]);
            }
        }
    }
    return min;
}

float part2closestrip(vector<Point> P, int totalnumsize, float origmin) //part 3
{
    float min = origmin;
    for (int outervar = 0; outervar < totalnumsize; ++outervar){
        for (int innervar = outervar+1; innervar < totalnumsize; ++innervar){
            Point outsidep = P[outervar];
            Point insidep = P[innervar];
            if (calculatedistance(outsidep, insidep) < min) {
                min = calculatedistance(outsidep, insidep);
            }
        }
    }
    return min;
}


float part2closest(vector<Point> p2, int size)
{
    if (size == 2 or size == 3){
        return listbruteForce(p2, size); //just returning min dist
    }
    Point midPoint = p2[size/2];
    float leftpart = part2closest(p2, size/2);
    vector<Point> p2vector; //creating the vector
    int numtimesrun = 0;
    std::vector<Point>::iterator pos;
    for (pos=p2.begin(); pos!=p2.end(); ++pos) { //iterator to traverse elements through a vector
        numtimesrun = numtimesrun + 1;
        if(numtimesrun > size/2){
            p2vector.push_back((*pos));
        }
    }
    float rightpart = part2closest(p2vector, size - (size/2));
    float rightdist = min(leftpart, rightpart);
    vector<Point> strip; //creating the vector
    int incrvar = 0;
    for (int lastforvar = 0; lastforvar < size; lastforvar++){
        double absdist = abs(p2[lastforvar].xcoord - midPoint.xcoord);
        if (absdist < rightdist) {
            strip.push_back(p2[lastforvar]);
            incrvar++;
        }
    }
    return min(rightdist, part2closestrip(strip, incrvar, rightdist));
}

struct myclass {
    bool operator() (Point pt1, Point pt2) { return (pt1.xcoord < pt2.xcoord);}
} myobject;

int part2(vector<Point> vectorp2){ //preliminary recursive algorithm, has vector to store points
    sort(vectorp2.begin(), vectorp2.end(), myobject); //sorted by x
    cout << "Recursive Distance is:   " << part2closest(vectorp2, vectorp2.size()) << "\n";
    cout << "--------------" << "\n";
    return 0;
}

list<Point> part1() {

    int numpoints = 1000; //vary the number of points to run the program for (initially 50)
    cout << "Number of Points: " << numpoints << endl;
    cout << "--------------" << "\n";

    std::mt19937_64 rng;
    uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32)};
    rng.seed(ss);
    std::uniform_real_distribution<double> unif(0, 1);

    std::ofstream newf; //store all points in points.txt, format: xcoord  ycoord (2 spaces in between)
    newf.open("points.txt");

    list<Point> list1;
    list<Point> list2;

    for (int i = 0; i < numpoints; i++) {
        double firstrand = unif(rng);
        double secondrand = unif(rng);
        string pointsline = to_string(firstrand) + "  " + to_string(secondrand);
        if (i < numpoints-1) {
            newf << pointsline << std::endl;
        } else {
            newf << pointsline;
        }


        Point firstp;
        firstp.xcoord = firstrand;
        firstp.ycoord = secondrand;
        Point secondp;
        secondp.xcoord = firstrand;
        secondp.ycoord = secondrand;

        list1.push_back(firstp);
        list2.push_back(secondp);
    }

    double xcoord1 = 0.0;
    double ycoord1 = 0.0;
    double xcoord2 = 0.0;
    double ycoord2 = 0.0;
    double mindist = 100000;

    list<Point>::iterator git; //Iterator traverse list

    const clock_t begin_time = clock();
    int outcount = 0;
    int totalcalcd = 0;
    for (git = list1.begin(); git != list1.end(); ++git) {
        list<Point>::iterator it; //Iterator traverse list
        outcount = outcount + 1;
        int incount = 0;
        for (it = list2.begin(); it != list2.end(); ++it){
            incount = incount + 1;
            double x1 = (*git).xcoord;
            double y1 = (*git).ycoord;
            double x2 = (*it).xcoord;
            double y2 = (*it).ycoord;
//            if(incount<outcount){ //makes it so that (n^2-n)/2 distances are calculated
            double currdist = sqrt(pow(x2 - x1, 2)*1.0 + pow(y2 - y1, 2) * 1.0);
            totalcalcd++;
            if(currdist != 0.0 && currdist<mindist){
                xcoord1 = x1;
                ycoord1 = y1;
                xcoord2 = x2;
                ycoord2 = y2;
                mindist = currdist;
//                }
            }
        }
    }

    double currdist = sqrt(pow(xcoord2 - xcoord1, 2)*1.0 + pow(ycoord2 - ycoord1, 2) * 1.0);
    cout << "Brute Force Distance is: " << currdist << "\n";

    newf.close();
    return list1;
}

int main() {
    const clock_t begin_time = clock();

    list<Point> p1list = part1();

    float bftime = float(clock()-begin_time )/CLOCKS_PER_SEC;

    vector<Point> p2vector; //creating the vector
    std::list<Point>::iterator it;
    for (it = p1list.begin(); it != p1list.end(); ++it){
        Point newp;
        newp.xcoord = (*it).xcoord;
        newp.ycoord = (*it).ycoord;
        p2vector.push_back(newp);
    }

    const clock_t newbegin_time = clock(); //begun AFTER creating vector
    part2(p2vector); //finding 2 closest points, part 2 call
    float rectime = float(clock()-newbegin_time)/CLOCKS_PER_SEC;

    cout << "Brute Force Time: " << bftime << "\n";
    cout << "Recursive Time:   " << rectime << "\n";

    return 0;
}