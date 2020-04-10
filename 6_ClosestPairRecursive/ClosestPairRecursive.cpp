//Ankit Gupta, Computer Vision Exercise
//Note: I display the points in an 800 by 800 ppm, each point is a circle with radius 3, the 2 closest points are in red, the rest are in black
//Stores points in points.txt, uses iterators and lists, generates 50 random points and a ppm file displaying the points

#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <list>
#include <iterator>

using namespace std;

class Point
{
public:
    double xcoord;
    double ycoord;

    double getx(){
        return(xcoord);
    }
    double gety(){
        return(ycoord);
    }
};

void pixelchange(int coordx, int coordy, int fullarr[][2400], int redchange)
{
    int setval = 0;
    for(int comp=setval; comp<3; comp++){
        if (coordy<800 && coordy>=0 && comp+3*coordx < 2400 && comp+3*coordx >=0) {
            if(redchange == 1 && comp == 0){
                fullarr[coordy][comp+3*coordx] = 1;
            }
            else{
                fullarr[coordy][comp+3*coordx] = setval;
            }
        }
    }
}

//Draws circle
void percirc(int fullarr[][2400], int radius, int factorx, int factory, int redchange) //rasterized algorithm
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
        if(factorx+x<800 && factorx+x>=0 && factory+y<800 && factory+y>=0){
            pixelchange( factorx+x, factory+y, fullarr, redchange);
        }
        if(factorx+x<800 && factorx+x>=0 && factory-y<800 && factory-y>=0){
            pixelchange( factorx+x, factory-y, fullarr, redchange);;
        }
        if(factorx-x<800 && factorx-x>=0 && factory+y<800 && factory+y>=0){
            pixelchange( factorx-x, factory+y, fullarr, redchange);
        }
        if(factorx-x<800 && factorx-x>=0 && factory-y<800 && factory-y>=0){
            pixelchange( factorx-x, factory-y, fullarr, redchange);
        }
        if(factorx+y<800 && factorx+y>=0 && factory+x<800 && factory+x>=0){
            pixelchange( factorx+y, factory+x, fullarr, redchange);
        }
        if(factorx+y<800 && factorx+y>=0 && factory-x<800 && factory-x>=0){
            pixelchange( factorx+y, factory-x, fullarr, redchange);
        }
        if(factorx-y<800 && factorx-y>=0 && factory+x<800 && factory+x>=0){
            pixelchange( factorx-y, factory+x, fullarr, redchange);
        }
        if(factorx-y<800 && factorx-y>=0 && factory-x<800 && factory-x>=0){
            pixelchange( factorx-y, factory-x, fullarr, redchange);
        }

        y2_new -= (2 * x) - 3;
    }
}

void createPPM(int fullarr[][2400]){
    ofstream outf;
    string fname = "output.ppm";
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

int part1() {

    int ppmimage[800][2400] = {}; //initializing matrix
    for (int width = 0; width < 800; width++) {
        for (int height = 0; height < 2400; height++) {
            ppmimage[width][height] = 1;
        }
    }

    std::mt19937_64 rng;
    uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32)};
    rng.seed(ss);
    std::uniform_real_distribution<double> unif(0, 1);

    std::ofstream newf; //store all points in points.txt, format: xcoord  ycoord (2 spaces in between)
    newf.open("points.txt");

    list<Point> list1;
    list<Point> list2;

    for (int i = 0; i < 50; i++) {
        double firstrand = unif(rng);
        double secondrand = unif(rng);
        string pointsline = to_string(firstrand) + "  " + to_string(secondrand);
        if (i < 49) {
            newf << pointsline << std::endl;
        } else {
            newf << pointsline;
        }
        int adjfirst = int(firstrand * 800);
        int adjsecond = int(secondrand * 800);
        percirc(ppmimage, 3, adjfirst, adjsecond, 0);

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

    int outcount = 0;
    int totalcalcd = 0;
    for (git = list1.begin(); git != list1.end(); ++git) {
        list<Point>::iterator it; //Iterator traverse list
        outcount = outcount + 1;
        int incount = 0;
        for (it = list2.begin(); it != list2.end(); ++it){
            incount = incount + 1;
            double x1 = (*git).getx();
            double y1 = (*git).gety();
            double x2 = (*it).getx();
            double y2 = (*it).gety();
            if(incount<outcount){ //makes it so that (n^2-n)/2 distances are calculated
                double currdist = sqrt(pow(x2 - x1, 2)*1.0 + pow(y2 - y1, 2) * 1.0);
                totalcalcd++;
                if(currdist != 0.0 && currdist<mindist){
                    xcoord1 = x1;
                    ycoord1 = y1;
                    xcoord2 = x2;
                    ycoord2 = y2;
                    mindist = currdist;
                }
            }
        }
    }

    percirc(ppmimage, 3, int(xcoord1*800), int(ycoord1*800), 1);
    percirc(ppmimage, 3, int(xcoord2*800), int(ycoord2*800), 1);

    createPPM(ppmimage);

    newf.close();
    return 0;
}

int main() {
    part1();
    return 0;
}