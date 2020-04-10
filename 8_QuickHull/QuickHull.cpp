//Ankit Gupta, Computer Vision Exercise
//Project 4, Part 1: QuickHull
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <chrono>
#include <random>
#include <stack>
#include <bits/stdc++.h>


using namespace std;


//START Change
class Point{
public:
    double xcoord;
    double ycoord;
};

bool checkless(Point fir, Point sec){
    return(fir.xcoord<sec.xcoord);
}

double checksides(Point fir, Point sec, Point thi) {
    double firstx = sec.xcoord - fir.xcoord;
    double secondy = thi.ycoord - fir.ycoord;
    double firsty = sec.ycoord - fir.ycoord;
    double secondx = thi.xcoord - fir.xcoord;
    return firstx*secondy-firsty*secondx;
}

double calculatelinetopointdist(Point fir, Point sec, Point thi) {
    double newfir = sec.ycoord - fir.ycoord;
    double newsec = sec.xcoord - fir.xcoord;
    double divsqrt = sqrt(newfir*newfir + newsec*newsec);
    double abspart = newfir*thi.xcoord - newsec*thi.ycoord + sec.xcoord*fir.ycoord - sec.ycoord*fir.xcoord;
    return(abs(abspart/divsqrt));
}

vector<Point> usingvector;

void hullsearch(vector<Point> inputpts, Point first, Point second) {
    bool empty = inputpts.empty();
    if(empty){ //if it is empty, return
        return;
    }

    int indexoflargestdist = 0;
    Point firstpoint = inputpts[0];
    double initialdistancetouse = calculatelinetopointdist(first, second, firstpoint);

    for (int zip=1; zip<inputpts.size(); zip++) {
        Point thispointiter = inputpts[zip];
        if (calculatelinetopointdist(first, second, thispointiter) > initialdistancetouse) {
            initialdistancetouse = calculatelinetopointdist(first, second, inputpts[zip]);
            indexoflargestdist = zip;
        }
    }

    Point toadd = inputpts[indexoflargestdist];
    usingvector.push_back(toadd);
    vector<Point> rightpart;
    vector<Point> leftpart;

    for (int iterate=0; iterate<inputpts.size(); iterate++) {
        if (iterate != indexoflargestdist) {
            Point largestpt = inputpts[indexoflargestdist];
            Point currentiteration = inputpts[iterate];
            int crossproduct = checksides(first, largestpt, currentiteration);

            if (crossproduct >= 0){
                rightpart.push_back(currentiteration);
            }

            else {
                int newcross = checksides(inputpts[indexoflargestdist], second, inputpts[iterate]);
                if (newcross >= 0){
                    leftpart.push_back(currentiteration);
                }
            }
        }
    }

    Point newerlargedistinput = inputpts[indexoflargestdist];

    hullsearch(rightpart, first, newerlargedistinput); //recurring for the right side

    hullsearch(leftpart, newerlargedistinput, second); //recurring for the left side
    return;
}

vector<Point> quickhullrun(vector<Point> inputpts){
    int vectorsize = inputpts.size();

    if (vectorsize < 2) {
        return inputpts;
    }

    sort(inputpts.begin(), inputpts.end(), checkless); //sorting to get the leftmost and highest
    Point leftmost = inputpts[0];
    Point rightmost = inputpts.back();

    usingvector.push_back(leftmost);
    usingvector.push_back(rightmost);

    vector<Point> lpart; //initializing vectors for points that lie on the left, on the line, and on the right
    vector<Point> onpart;
    vector<Point> rpart;

    int maxout = inputpts.size()-1;

    for (int looper=1; looper<maxout; looper++) {
        int crossprodout = checksides(inputpts[0], inputpts.back(), inputpts[looper]);
        Point thisloop = inputpts[looper];
        if (crossprodout < 0){ //must lie to the right
            rpart.push_back(thisloop);
        }
        if (crossprodout == 0){ //must lie on the line
            onpart.push_back(thisloop);
        }
        if(crossprodout > 0){//must lie to the left
            lpart.push_back(thisloop);
        }
    }

    bool leftout = lpart.empty();
    bool rightout = rpart.empty();
    bool leftorright = leftout || rightout;

    if (leftorright) {
        //push all of the points onto usingvector
        int highestsizetorun = onpart.size();
        for (int lastloop = 0; lastloop < highestsizetorun; lastloop++) {
            Point onpartindex = onpart[lastloop];
            usingvector.push_back(onpartindex);
        }
    }

    Point firstpt = inputpts[0];
    Point lastpt = inputpts.back();

    hullsearch(lpart, firstpt, lastpt); //running hull search on left component

    hullsearch(rpart, lastpt, firstpt); //running hull search on right component

    return usingvector;
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

struct myclass {
    bool operator() (Point pt1, Point pt2) { return (pt1.xcoord < pt2.xcoord);}
} myobject;

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
    vector<Point> input;
    for(int i= 0;i<50;i++){
        Point toinclude;
        toinclude.xcoord = int(800*unif(rng));
        toinclude.ycoord = int(800*unif(rng));
        input.push_back(toinclude);
        percirc(ppmimage, 3, toinclude.xcoord, toinclude.ycoord);
    }
    int n = input.size();
    vector<Point> result = quickhullrun(input);
    vector<Point> up;
    vector<Point> down;

    sort(result.begin(), result.end(), myobject);
    Point minx = result[0];
    Point maxx = result[result.size()-1];
    double slope = (maxx.ycoord-minx.ycoord)*1.0/(maxx.xcoord-minx.xcoord);
    double ycoord = minx.ycoord-(slope*minx.xcoord);

    for(int z= 0;z<result.size()-1;z++){
        Point thisone = result[z];
        if(thisone.ycoord>ycoord+slope*thisone.xcoord){
            up.push_back(thisone);
        }
        else{
            down.push_back(thisone);
        }
    }

    for(int z= 0;z<up.size()-1;z++){
        Point p1 = up[z];
        Point p2 = up[z+1];
        intbresenhamsalgo(ppmimage, p1.xcoord, p1.ycoord, p2.xcoord, p2.ycoord);
    }
    intbresenhamsalgo(ppmimage, maxx.xcoord, maxx.ycoord, up[up.size()-1].xcoord, up[up.size()-1].ycoord);
    for(int z= 0;z<down.size()-1;z++){
        Point p1 = down[z];
        Point p2 = down[z+1];
        intbresenhamsalgo(ppmimage, p1.xcoord, p1.ycoord, p2.xcoord, p2.ycoord);
    }

    intbresenhamsalgo(ppmimage, minx.xcoord, minx.ycoord, up[0].xcoord, up[0].ycoord);
    intbresenhamsalgo(ppmimage, maxx.xcoord, maxx.ycoord, down[down.size()-1].xcoord, down[down.size()-1].ycoord);

    createPPM(ppmimage); //creating the image
};