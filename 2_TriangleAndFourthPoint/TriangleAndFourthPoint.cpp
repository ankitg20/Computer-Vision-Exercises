//Ankit Gupta, Computer Vision Exercise
//Project 2, Part 1

#include <iostream>
#include <random>
#include <chrono>
#include <fstream>

using namespace std;

float area(double x1, double y1, double x2, double y2, double x3, double y3)
{
    return abs((x1*(y2-y3) + x2*(y3-y1)+ x3*(y1-y2))/2.0);
}

bool isInside(double x1, double y1, double x2, double y2, double x3, double y3, double x, double y)
{
    double A = area (x1, y1, x2, y2, x3, y3);
    double A1 = area (x, y, x2, y2, x3, y3);
    double A2 = area (x1, y1, x, y, x3, y3);
    double A3 = area (x1, y1, x2, y2, x, y);
    return fabs(A-(A1 + A2 + A3))<0.0001;
}

int part1()
{
    std::mt19937_64 rng; //rng defining
    uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed>>32)};
    rng.seed(ss);
    std::uniform_real_distribution<double> unif(0, 1);
    double arr[6] = {};

    for (int i = 0; i < 6; i++){ //Initial points randomly generated
        arr[i] = unif(rng);
    }

    while(fabs((arr[0] * (arr[3] - arr[5]) + arr[2] * (arr[5] - arr[1]) + arr[4] * (arr[1] - arr[3]))) < 0.0001){ //make sure the three points are not colinear
        for(int i = 0; i < 6; i++){ //Regenerating new points if all three points are all collinear
            arr[i] = unif(rng);
        }
    }

    double first = unif(rng);
    double second = unif(rng);

    while(isInside(arr[0],arr[1],arr[2],arr[3],arr[4],arr[5], first, second)){ //make sure the fourth point is outside the three points
        first = unif(rng); //If the point is inside, create a new fourth point
        second = unif(rng);
    }

    //output to points.txt
    string output = "(" + std::to_string(arr[0]) + "," + std::to_string(arr[1]) + ") , " + "(" + std::to_string(arr[2]) + "," + std::to_string(arr[3]) + ") , " + "(" + std::to_string(arr[4]) + "," + std::to_string(arr[5]) + ") , " + "(" + std::to_string(first) + "," + std::to_string(second) + ")";
    std::ofstream file;
    file.open("points.txt");
    file << output;
    return 0;
}

int main(){
    part1();
    return 0;
}