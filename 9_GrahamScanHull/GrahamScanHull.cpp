//Ankit Gupta, Computer Vision Exercise
//Project 4, Part 2: Graham Scan Hull

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <chrono>
#include <random>
#include <iostream>
#include <stack>
#include <set>
#include <bits/stdc++.h>

using namespace std;


class Point{
public:
    double xcoord;
    double ycoord;
};

Point veryfirstpoint;

int checkangle(Point first, Point second, Point third){
    int ycoorddiffqp = second.ycoord - first.ycoord;
    int xcoorddiffrq = third.xcoord - second.xcoord;
    int xcoorddiffqp = second.xcoord - first.xcoord;
    int ycoorddiffrq = third.ycoord - second.ycoord;
    double output = ycoorddiffqp*xcoorddiffrq - xcoorddiffqp*ycoorddiffrq;
    if(output == 0){ //collinear
        return 0;
    }
    else if(output > 0){
        return 1;
    }
    else{
        return 2;
    }
}

double pttoptdistancecalc(Point firstpt, Point secpt){
    double xpart = firstpt.xcoord - secpt.xcoord;
    double ypart = firstpt.ycoord - secpt.ycoord;
    double sqoutput = xpart*xpart+ypart*ypart;
    return sqoutput;
}

//Changes value of a pixel
void pixelchange(int coordx, int coordy, int fullarr[][2400])
{
    int setval = 0;
    for (int comp = setval; comp < 3; comp++) {
        if (coordy<800 && coordy>=0 && comp+3*coordx < 2400 && comp+3*coordx >=0) {
            fullarr[coordy][comp+3*coordx] = setval;
        }
    }
}

//Bresenham's Line Drawing Algorithm
void intbresenhamsalgo(int fullarr[][2400], int firstx, int firsty, int secondx, int secondy)
{
    bool biggery = (abs(secondy-firsty) > abs(secondx-firstx));
    if(biggery){swap(firstx, firsty);swap(secondx, secondy);}
    bool firstbiggerthansec = firstx > secondx;
    if(firstbiggerthansec){swap(firstx, secondx);swap(firsty, secondy);}
    int minval = 0;
    int xdelta = secondx - firstx;
    int deltay = abs(secondy - firsty);
    double lasterr = deltay - xdelta;
    int increment = (firsty < secondy) ? 1 : -1;
    int gothrough  = firsty;
    for(int loopthru=firstx; loopthru<secondx; loopthru++){
        if(biggery)pixelchange(gothrough, loopthru, fullarr);
        else{pixelchange(loopthru, gothrough, fullarr);}
        if(lasterr >= minval){gothrough = increment+gothrough;lasterr = lasterr-xdelta;}
        lasterr = deltay+lasterr;
    }
}

//Draws circle
void percirc(int fullarr[][2400], int radius, int factorx, int factory) //rasterized algorithm
{
    int x, y, xmax, y2, y2_new, ty; //provided algorithm
    xmax = (int) (radius * 0.70710678); // maximum x at radius/sqrt(2)
    y = radius;
    y2 = y * y;
    ty = (2 * y) - 1;
    y2_new = y2;
    for (x = 0; x <= xmax+2; x++) {
        if ((y2 - y2_new) >= ty) {
            y2 -= ty;
            y -= 1;
            ty -= 2;
        }
        if(factorx+x<800 and factorx+x>=0){
            pixelchange( factorx+x, factory+y, fullarr);
        }
        if(factorx+x<800 and factorx+x>=0){
            pixelchange( factorx+x, factory-y, fullarr);;
        }
        if(factorx-x<800 and factorx-x>=0){
            pixelchange( factorx-x, factory+y, fullarr);
        }
        if(factorx-x<800 and factorx-x>=0){
            pixelchange( factorx-x, factory-y, fullarr);
        }
        if(factorx+y<800 and factorx+y>=0){
            pixelchange( factorx+y, factory+x, fullarr);
        }
        if(factorx+y<800 and factorx+y>=0){
            pixelchange( factorx+y, factory-x, fullarr);
        }
        if(factorx-y<800 and factorx-y>=0){
            pixelchange( factorx-y, factory+x, fullarr);
        }
        if(factorx-y<800 and factorx-y>=0){
            pixelchange( factorx-y, factory-x, fullarr);
        }

        y2_new -= (2 * x) - 3;
    }
}

int arrangebypolarangle(const void *fpoint, const void *spoint){ //change this
    Point* newfirstpt = (Point*)fpoint;
    Point* newsecpt = (Point*)spoint;

    int signalvalue = checkangle(veryfirstpoint, *newfirstpt, *newsecpt);

    if (signalvalue == 0) {
        int firstdist = pttoptdistancecalc(veryfirstpoint, *newsecpt);
        int secdist = pttoptdistancecalc(veryfirstpoint, *newfirstpt);
        if(firstdist >= secdist){
            return -1;
        }
        else{
            return 1;
        }
    }
    if(signalvalue==2){
        return -1;
    }
    else{
        return 1;
    }
}

//Constructs the ppm file
void createPPM(int fullarr[][2400]){
    ofstream outf;
    string fname = "cv.ppm";
    int size = 800;
    string firstline = "P3 " + std::to_string(size) + " " + std::to_string(size) + " 1";
    outf.open(fname);
    outf << firstline << endl;
    for (int counterr=0; counterr<800; counterr++) //looping through each line
    {
        for(int x=0; x<2399; x++){
            int thispix = fullarr[counterr][x];
            outf <<  thispix <<  " ";
        }
        int finalpix = fullarr[counterr][2399];
        outf << finalpix << endl; //endline
    }
    outf.close();
}


Point secondtopFind(stack<Point> stkin){ //getting the second point from the top
    Point topmostpt = stkin.top();
    stkin.pop();
    Point secondtoppt = stkin.top(); //getting the second element
    stkin.push(topmostpt);
    return secondtoppt;
}

vector<Point> calcGraham(Point allpts[], int usingsize){
    Point firstpt = allpts[0];
    int ymin = firstpt.ycoord;

    int valueminimum = 0;

    for (int loopincrement= 1; loopincrement < usingsize; loopincrement++){
        int y = allpts[loopincrement].ycoord;
        bool checkfirst = y < ymin;

        int firstx = allpts[loopincrement].xcoord;
        int secondx =  allpts[valueminimum].xcoord;
        bool checksecond = ymin == y &&  firstx < secondx;

        if (checkfirst || checksecond){
            valueminimum = loopincrement;
            ymin = allpts[loopincrement].ycoord;
        }
    }

    Point valorary = allpts[0];
    allpts[0] = allpts[valueminimum];
    allpts[valueminimum] = valorary;

    veryfirstpoint = allpts[0];
    int psize = sizeof(Point);
    int minusonepart = usingsize-1;
    Point* nextpttouse = &allpts[1];

    qsort(nextpttouse, minusonepart, psize, arrangebypolarangle);

    int modsize = 1;
    int zeroequiv = 0;
    for (int indexused=1; indexused<usingsize; indexused++){
        while (indexused < minusonepart && checkangle(veryfirstpoint, allpts[indexused],allpts[indexused+1]) == zeroequiv){
            indexused = indexused + 1;
        }
        allpts[modsize] = allpts[indexused];
        modsize++;
    }

    if (modsize <= 2){
        vector<Point> lastret;
        return lastret;
    }

    stack<Point> pointstack;
    Point firpt = allpts[0];
    Point secpt = allpts[1];
    Point thirpt = allpts[2];

    pointstack.push(firpt); //adding the first three points
    pointstack.push(secpt);
    pointstack.push(thirpt);

    int initval = 3;
    int checknonleft = 2;
    for (int nonleftlooper = initval; nonleftlooper < modsize; nonleftlooper++){
        while (checkangle(secondtopFind(pointstack), pointstack.top(), allpts[nonleftlooper]) != checknonleft){ //non-left turn
            pointstack.pop();
        }
        pointstack.push(allpts[nonleftlooper]);
    }

    vector<Point> returningStack;
    while (!pointstack.empty()){
        returningStack.push_back(pointstack.top());
        pointstack.pop(); //remove
    }

    return returningStack;
}


int main(){
    int ppmimage[800][2400] = {}; //initializing matrix
    for(int width = 0; width < 800; width++){
        for (int height = 0; height < 2400; height++){
            ppmimage[width][height] = 1;
        }
    }

    srand(time(0)); //starting seed 0
    std::mt19937_64 rng;
    uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32)};
    rng.seed(ss);
    std::uniform_real_distribution<double> unif(0, 1);
    Point points[50];
    for(int i= 0;i<50;i++){
        points[i].xcoord = int(800*unif(rng));
        points[i].ycoord = int(800*unif(rng));
        percirc(ppmimage, 2, points[i].xcoord, points[i].ycoord);
    }
    int sizetouse = sizeof(points)/sizeof(points[0]);
    vector<Point> result = calcGraham(points, sizetouse);

    for(int z= 0;z<result.size()-1;z++){
        Point p1 = result[z];
        Point p2 = result[z+1];
        intbresenhamsalgo(ppmimage, p1.xcoord, p1.ycoord, p2.xcoord, p2.ycoord);
    }
    intbresenhamsalgo(ppmimage, result[0].xcoord, result[0].ycoord, result[result.size()-1].xcoord, result[result.size()-1].ycoord);

    createPPM(ppmimage); //creating the image
};