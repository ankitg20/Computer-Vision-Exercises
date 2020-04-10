//Ankit Gupta, Computer Vision Exercise
//Project 1 Lab Submission, Final

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <chrono>
#include <random>

using namespace std;

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

//Draws euler line, circum, in, and nine point circles
void eulerlineincircumninepointCircles(int fullarr[][2400], double ycoords[], double xcoords[]) //also draws euler line
{
    int powtoput = 2;


    for (int iter=0; iter<3; iter++)
    {
        xcoords[iter] = xcoords[iter]*800; //still a double
        ycoords[iter] = ycoords[iter]*800; //still a double
    }

    double athird = pow(ycoords[1]-ycoords[0], powtoput);
    double bthird = pow(xcoords[1]-xcoords[0], powtoput);
    double afirst = pow(ycoords[2]-ycoords[1], powtoput);
    double bfirst = pow(xcoords[2]-xcoords[1], powtoput);
    double asecond = pow(ycoords[0]-ycoords[2], powtoput);
    double bsecond = pow(xcoords[0]-xcoords[2], powtoput);
    double thirdside = sqrt(athird+bthird);
    double firstside = sqrt(afirst+bfirst);
    double secondside = sqrt(asecond+bsecond);
    double sumallsides = secondside + thirdside + firstside; //semi = .5*sumallsides

    double firstfactor = ((.5*sumallsides)-secondside);
    double secondfactor = ((.5*sumallsides)-thirdside);
    double thirdfactor = ((.5*sumallsides) - firstside)/(.5*sumallsides);
    double radinner = sqrt(firstfactor*secondfactor*thirdfactor);
    double helperforout = 2 * radinner * sumallsides;
    double radouter = (secondside * thirdside * firstside)/helperforout;

    double diff1 = ycoords[1]-ycoords[2];
    double diff2 = ycoords[2]-ycoords[0];
    double diff3 = ycoords[0] - ycoords[1];
    double totalmult = diff1*xcoords[0] + diff2*xcoords[1] + diff3*xcoords[2];
    double doubledmult = totalmult*2;

    double xdiff1 = xcoords[2]-xcoords[1];
    double xdiff2 = xcoords[0]-xcoords[2];
    double xdiff3 = xcoords[1]-xcoords[0];

    double recipmultiply = 1/doubledmult;
    double xcirclearound = recipmultiply*((pow(xcoords[0], 2) + pow(ycoords[0], 2))*(diff1)+(pow(xcoords[1], 2) + pow(ycoords[1], 2))*(diff2)+(pow(xcoords[2], 2) + pow(ycoords[2], 2))*(diff3));
    double ycirclearound = recipmultiply*((pow(xcoords[0], 2) + pow(ycoords[0], 2))* xdiff1+(pow(xcoords[1], 2) + pow(ycoords[1], 2))*(xdiff2)+((pow(xcoords[2], 2) + pow(ycoords[2], 2))*(xdiff3)));

    double centroidX = (xcoords[0] + xcoords[1] + xcoords[2])/3.0;
    double centroidY = (ycoords[0] + ycoords[1] + ycoords[2])/3.0;
    double slope = (centroidY-ycirclearound)/(centroidX-xcirclearound);
    intbresenhamsalgo(fullarr, int(centroidX)-int(centroidX), int(centroidY)-int(int(centroidX)*slope), int(centroidX)+(800-int(centroidX)), int(centroidY)+int((800-int(centroidX))*slope)); //extended euler line
//    intbresenhamsalgo(fullarr, int(xcirclearound), int(ycirclearound), int(centroidX), int(centroidY));     //part of euler line

    double numinx = firstside * xcoords[0] + secondside * xcoords[1] + thirdside * xcoords[2];
    double samedenom = firstside + secondside + thirdside;
    double numiny = firstside * ycoords[0] + secondside * ycoords[1] + thirdside * ycoords[2];
    percirc(fullarr, int(radinner), int(numinx/samedenom), int(numiny/samedenom));
    percirc(fullarr, int(radouter), int(xcirclearound), int(ycirclearound));

    double x1 = xcoords[0]; //can be replaced to condense
    double y1 = ycoords[0];
    double x2 = xcoords[1];
    double y2 = ycoords[1];
    double x3 = xcoords[2];
    double y3 = ycoords[2];
    double aslope = (double(y2-y1))/(double(x2-x1));
    double bslope = (double(y3-y2))/(double(x3-x2));
    double recipaslope = -1.0/aslope; //x3, y3
    double recipbslope = -1.0/bslope; //x1, y1
    double recipaintercept = y3 - recipaslope*x3;
    double recipbintercept = y1 - recipbslope*x1;
    double num = recipaintercept-recipbintercept;
    double var = recipbslope - recipaslope;
    double xval = num/var;
    double yval = xval*recipaslope + recipaintercept;
    double ninepointx = (xval + xcirclearound)/2.0; //midpoint of orthocenter and circumcenter
    double ninepointy = (yval + ycirclearound)/2.0;
    double ninepointr = radouter/2;
    percirc(fullarr, int(ninepointr), int(ninepointx), int(ninepointy)); //drawing nine point circle
}

//Constructs the ppm file
void createPPM(int fullarr[][2400]){
    ofstream outf;
    printf("yo");
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


//main method
int main(){
    std::mt19937_64 rng;
    uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed>>32)};
    rng.seed(ss);
    std::uniform_real_distribution<double> unif(0, 1);

    int ppmimage[800][2400] = {}; //initializing matrix
    for(int width = 0; width < 800; width++){
        for (int height = 0; height < 2400; height++){
            ppmimage[width][height] = 1;
        }
    }

    srand(time(0)); //starting seed 0

//    int ycoords[3]; //instantiating
//    int xcoords[3];

    double ycoords[3]; //instantiating
    double xcoords[3];
    for (int iter=0; iter<3; iter++)
    {
        xcoords[iter] = unif(rng); //randomly generated on [0,1]
        ycoords[iter] = unif(rng); //randomly generated on [0,1]
        pixelchange(int(xcoords[iter]*800), int(ycoords[iter]*800), ppmimage);
    }

    int firstx = int(xcoords[0]*800);
    int firsty = int(ycoords[0]*800);
    int secondx = int(xcoords[1]*800);
    int secondy = int(ycoords[1]*800);
    int thirdx = int(xcoords[2]*800);
    int thirdy = int(ycoords[2]*800);
//
    intbresenhamsalgo(ppmimage, firstx, firsty, secondx, secondy);
    intbresenhamsalgo(ppmimage, secondx, secondy, thirdx, thirdy);
    intbresenhamsalgo(ppmimage, thirdx, thirdy, firstx, firsty);

    eulerlineincircumninepointCircles(ppmimage, ycoords, xcoords);
    createPPM(ppmimage); //creating the image
}