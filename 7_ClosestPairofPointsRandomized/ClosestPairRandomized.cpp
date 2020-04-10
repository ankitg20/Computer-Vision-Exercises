//Ankit Gupta, Computer Vision Exercise
#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <list>
#include <iterator>
#include <string.h>
#include <unordered_map>
//#include <bits/stdc++.h>

using namespace std;

class Point{
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

struct myclass {
    bool operator() (Point pt1, Point pt2) { return (pt1.xcoord < pt2.xcoord);}
} myobject;

struct newclass {
    bool operator() (Point pt1, Point pt2) { return (pt1.ycoord < pt2.ycoord);}
} newobject;


double part3closestrip(vector<Point> P, int totalnumsize, float origmin) //O(n) since inner loop runs max of 6 times
{
    float min = origmin;
    sort(P.begin(), P.end(), newobject); //sorted by x
    for (int outervar = 0; outervar < totalnumsize; ++outervar){
        for (int innervar = outervar+1; innervar < totalnumsize && (P[innervar].ycoord - P[outervar].ycoord) < min; ++innervar){
            Point outsidep = P[outervar];
            Point insidep = P[innervar];
            if (calculatedistance(outsidep, insidep) < min) {
                min = calculatedistance(outsidep, insidep);
            }
        }
    }
    return min;
}


double part3closest(vector<Point> p2, int size)
{
    if (size == 2 or size == 3){
        return listbruteForce(p2, size); //just returning min dist
    }
    Point midPoint = p2[size/2];
    float leftpart = part3closest(p2, size/2);
    vector<Point> p2vector; //creating the vector
    int numtimesrun = 0;
    std::vector<Point>::iterator pos;
    for (pos=p2.begin(); pos!=p2.end(); ++pos) { //iterator to traverse elements through a vector
        numtimesrun = numtimesrun + 1;
        if(numtimesrun > size/2){
            p2vector.push_back((*pos));
        }
    }
    float rightpart = part3closest(p2vector, size - (size/2));
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
    return min(rightdist, float(part3closestrip(strip, incrvar, rightdist)));
}

double part3(vector<Point> vectorp2){ //preliminary recursive algorithm, has vector to store points
    sort(vectorp2.begin(), vectorp2.end(), myobject); //sorted by x
    cout.precision(25);
    std::ofstream lastf;
    lastf.open("randomized.txt");
    double result = part3closest(vectorp2, vectorp2.size());
    cout << "Full Recursive Distance is: " << result << "\n";
    lastf << "Full Recursive Distance is: " << result << "\n";
    lastf.close();
    return 0;
}

struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const
    {
        auto hash1 = hash<T1>{}(p.first);
        auto hash2 = hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};

double randapproach(vector<Point> vectornew){
    double finanswer = 0.0;
    int size = vectornew.size();
    srand(time(nullptr));
    for (int i = size - 1; i > 0; i--){ //Knuth Algorithm
        int j = rand() % (i + 1);
        Point temp = vectornew[i]; //swap
        vectornew[i].xcoord = vectornew[j].xcoord;
        vectornew[i].ycoord = vectornew[j].ycoord;
        vectornew[j].xcoord = temp.xcoord;
        vectornew[j].ycoord = temp.ycoord;
    }
    double lastanswer = 0.0;
    double initmindist = calculatedistance(vectornew[0], vectornew[1]); //Minimum distance found so far
    vector<Point> torun;
    for(int y = 0;y<vectornew.size();y++){torun.push_back(vectornew[y]);}
    sort(torun.begin(), torun.end(), myobject); //sorted by x
    lastanswer = part3closest(torun, torun.size());
    double othervar = 2.0/initmindist;

    cout<< "--------------------------" << "\n";
    unordered_map<pair<unsigned long, unsigned long>, Point, hash_pair> points;
    for(int l = 0;l<size;l++){ //change to i<size once done understanding, size = n
        Point toconsider = vectornew[l];
        int xvar = int((toconsider.xcoord)*othervar);
        int yvar = int((toconsider.ycoord)*othervar);
        int tleftx = xvar-2;
        int tlefty = yvar-2;
        double mindistfound = initmindist;
        for(int i = 0;i<=4;i++) {
            for (int j = 0; j <= 4; j++) {
                unsigned long thisx = tleftx + i;
                unsigned long thisy = tlefty + j;
                if (points.find(pair<unsigned long, unsigned long>(thisx, thisy)) != points.end()) { //lookup
                    Point contained = points[pair<unsigned long, unsigned long>(thisx, thisy)]; //point inside subsquare
                    double computedist = calculatedistance(toconsider, contained);
                    if (computedist < mindistfound) {
                        mindistfound = computedist; //finds the minimum distance amongst all
                    }
                }
            }
        }
        finanswer=mindistfound;
        if(mindistfound<initmindist){ //new distance from points in the vicinity is less than the old distance
            auto it = points.begin();
            points.erase(it, points.end()); //deletes current dictionary
            unordered_map<pair<unsigned long, unsigned long>, Point, hash_pair> newp; //new dictionary
            for(int newone = 0; newone<l;newone++){ //going from 1 to i
                Point newusing = vectornew[newone]; //sidelength mindistfound/2
                int xv = int((newusing.xcoord)*2.0/mindistfound);
                int yv = int((newusing.ycoord)*2.0/mindistfound);
                newp[pair<unsigned long, unsigned long>(xv, yv)] = newusing;
            }
        }
        else{
            points[pair<unsigned long, unsigned long>(xvar, yvar)] = toconsider;
        }
    }

    cout << "Randomized Approach Distance is: " << lastanswer << "\n";
    return 0;
}

int main() {
    std::ofstream lastf; //store all points in points.txt, format: xcoord  ycoord (2 spaces in between)
    lastf.open("randomized.txt");

    std::ifstream file("points.txt"); //change to points.txt
    std::string str;
    vector<Point> firstvec;
    vector<Point> secondvec;
    list<Point> list;

    while (std::getline(file, str))
    {
        Point newp;
        int div = str.find("  ");

        double first = stod(str.substr(0,div));
        double second = stod(str.substr(div+2, str.length()));

        newp.xcoord = first;
        newp.ycoord = second;
        list.push_back(newp);
        firstvec.push_back(newp);
        secondvec.push_back(newp); //stored into two vectors
    }

    const clock_t thirdtime = clock(); //begun AFTER creating vector
    part3(firstvec);
    float thitime = float(clock()-thirdtime)/CLOCKS_PER_SEC;
    cout << "Full Recursive Run Time: " << thitime << "\n"; //full recursive method
    lastf << "Full Recursive Run Time: " << thitime << "\n"; //output to file

    const clock_t randtime = clock(); //begun AFTER creating vector
    randapproach(secondvec);
    float lastrandtime = float(clock()-randtime)/CLOCKS_PER_SEC;
    cout << "Randomized Approach Run Time: " << lastrandtime << "\n"; //full recursive method
    lastf << "Randomized Approach Run Time: " << lastrandtime << "\n";

    lastf.close();
    return 0;
}

//Code to Generate Points:
//    int numpoints = 50; //vary the number of points to run the program for (initially 50)
//    std::mt19937_64 rng;
//    uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
//    std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32)};
//    rng.seed(ss);
//    std::uniform_real_distribution<double> unif(0, 1);
//    std::ofstream newf; //store all points in points.txt, format: xcoord  ycoord (2 spaces in between)
//    newf.open("points.txt");
//    for (int i = 0; i < numpoints; i++) {
//        double firstrand = unif(rng);
//        double secondrand = unif(rng);
//        string pointsline = to_string(firstrand) + "  " + to_string(secondrand);
//        if (i < numpoints - 1) {
//            newf << pointsline << std::endl;
//        } else {
//            newf << pointsline;
//        }
//    }
//    newf.close();
