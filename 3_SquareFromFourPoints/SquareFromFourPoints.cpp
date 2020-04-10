//Ankit Gupta, Computer Vision Exercise

#include <iostream>
#include <random>
#include <chrono>
#include <fstream>
using namespace std;

#include <string>

double minarea = 1.0;
int minval = -1;

using namespace std;

//Changes value of a pixel
void pixelchange(int coordx, int coordy, int fullarr[][2400])
{
    int setval = 0;
    for(int comp=setval; comp<3; comp++){
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
        else{
            pixelchange(loopthru, gothrough, fullarr);
        }
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
        if(factorx+x<800 && factorx+x>=0 && factory+y<800 && factory+y>=0){
            pixelchange( factorx+x, factory+y, fullarr);
        }
        if(factorx+x<800 && factorx+x>=0 && factory-y<800 && factory-y>=0){
            pixelchange( factorx+x, factory-y, fullarr);;
        }
        if(factorx-x<800 && factorx-x>=0 && factory+y<800 && factory+y>=0){
            pixelchange( factorx-x, factory+y, fullarr);
        }
        if(factorx-x<800 && factorx-x>=0 && factory-y<800 && factory-y>=0){
            pixelchange( factorx-x, factory-y, fullarr);
        }
        if(factorx+y<800 && factorx+y>=0 && factory+x<800 && factory+x>=0){
            pixelchange( factorx+y, factory+x, fullarr);
        }
        if(factorx+y<800 && factorx+y>=0 && factory-x<800 && factory-x>=0){
            pixelchange( factorx+y, factory-x, fullarr);
        }
        if(factorx-y<800 && factorx-y>=0 && factory+x<800 && factory+x>=0){
            pixelchange( factorx-y, factory+x, fullarr);
        }
        if(factorx-y<800 && factorx-y>=0 && factory-x<800 && factory-x>=0){
            pixelchange( factorx-y, factory-x, fullarr);
        }

        y2_new -= (2 * x) - 3;
    }
}

//Constructs the ppm file
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
    std::mt19937_64 rng;
    uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed>>32)};
    rng.seed(ss);
    std::uniform_real_distribution<double> unif(0, 1);
    double arr[6] = {};

    for (int i = 0; i < 6; i++){
        arr[i] = unif(rng);
    }

    while(fabs((arr[0] * (arr[3] - arr[5]) + arr[2] * (arr[5] - arr[1]) + arr[4] * (arr[1] - arr[3]))) < 0.0001){ //make sure the three points are not colinear
        for(int i = 0; i < 6; i++){
            arr[i] = unif(rng);
        }
    }

    double first = unif(rng);
    double second = unif(rng);

    while(isInside(arr[0],arr[1],arr[2],arr[3],arr[4],arr[5], first, second)){ //make sure the fourth point is outside the three points
        first = unif(rng);
        second = unif(rng);
    }

    string output = "(" + std::to_string(arr[0]) + "," + std::to_string(arr[1]) + ") , " + "(" + std::to_string(arr[2]) + "," + std::to_string(arr[3]) + ") , " + "(" + std::to_string(arr[4]) + "," + std::to_string(arr[5]) + ") , " + "(" + std::to_string(first) + "," + std::to_string(second) + ")";
    std::ofstream file;
    file.open("points.txt");
    file << output;
    return 0;
}


string persquareform(int ppmimage[][2400], double ax, double ay, double bx, double by, double cx, double cy, double dx, double dy, int direction, int runnum, int enable){
    double distab = sqrt(pow(bx - ax, 2)*1.0 + pow(by - ay, 2) * 1.0);
    double slope = (1.0*(by-ay))/(bx-ax);
    double perpslope = -1.0/slope;

    double newx = -1.0;
    double newy = -1.0;
    for (int i = 0; i < 500; i++) {
        newx = dx+1.0*direction*i;
        newy = dy+1.0*direction*i*perpslope;
        double newdist = sqrt(pow(dx - newx, 2)*1.0 + pow(dy - newy, 2) * 1.0);
        if(newdist == distab){
//            intbresenhamsalgo(ppmimage,dx,dy,newx,newy);
            break;
        }
    }
//    intbresenhamsalgo(ppmimage,cx,cy,newx,newy);
    double oneslope = 1.0*(newy-cy)/(newx-cx);
    //MAIN for one:
    double perpslopeone = -1.0/oneslope;
    if(enable == 1){
        intbresenhamsalgo(ppmimage,int(cx-1000.0),int(cy-1000.0*oneslope),int(cx+1000),int(cy+1000.0*oneslope));
        intbresenhamsalgo(ppmimage,int(ax-1000.0),int(ay-1000.0*perpslopeone),int(ax+1000),int(ay+1000.0*perpslopeone));
        intbresenhamsalgo(ppmimage,int(bx-1000.0),int(by-1000.0*perpslopeone),int(bx+1000),int(by+1000.0*perpslopeone));
        intbresenhamsalgo(ppmimage,int(dx-1000.0),int(dy-1000.0*oneslope),int(dx+1000),int(dy+1000.0*oneslope));
    }


    double m1 = oneslope;
    double c1 = cy-cx*oneslope;
    double m2 = perpslopeone;
    double c2 = ay-ax*perpslopeone;
    double m3 = oneslope;
    double c3 = dy-dx*oneslope;
    double m4 = perpslopeone;
    double c4 = by-bx*perpslopeone;

    double intersection12x = (c2 - c1) / (m1 - m2);
    double intersection12y = m1 * intersection12x + c1;
    double intersection23x = (c3 - c2) / (m2 - m3);
    double intersection23y = m2 * intersection23x + c2;
    double intersection34x = (c4 - c3) / (m3 - m4);
    double intersection34y = m3 * intersection34x + c3;
    double intersection41x = (c1 - c4) / (m4 - m1);
    double intersection41y = m4 * intersection41x + c4;

    if(enable == 1){
        percirc(ppmimage, 5, int(intersection12x), int(intersection12y)); //for vertices
        percirc(ppmimage, 5, int(intersection23x), int(intersection23y));
        percirc(ppmimage, 5, int(intersection34x), int(intersection34y));
        percirc(ppmimage, 5, int(intersection41x), int(intersection41y));
    }


    double inter12x = intersection12x/800.0;
    double inter12y = intersection12y/800.0;
    double inter23x = intersection23x/800.0;
    double inter23y = intersection23y/800.0;
    double inter34x = intersection34x/800.0;
    double inter34y = intersection34y/800.0;
    double inter41x = intersection34x/800.0;
    double inter41y = intersection34y/800.0;

    double sidea = sqrt(pow(inter23x - inter12x, 2)*1.0 + pow(inter23y - inter12y, 2)*1.0);
    double finarea = pow(sidea,2)*1.0;

    if(finarea-minarea<0){
        minarea = finarea;
        minval = runnum;
    }
    return("(" + std::to_string(inter12x) + "," + std::to_string(inter12y) + ") , " + "(" + std::to_string(inter23x) + "," + std::to_string(inter23y) + ") , " + "(" + std::to_string(inter34x) + "," + std::to_string(inter34y) + ") , " + "(" + std::to_string(inter41x) + "," + std::to_string(inter41y) + ")" + " Area = " + std::to_string(finarea));

}

int part2(){ //reads from points.txt
    ifstream infile;
    string STRING;
    infile.open ("points.txt");

    int ppmimage[800][2400] = {}; //initializing matrix
    for(int width = 0; width < 800; width++){
        for (int height = 0; height < 2400; height++){
            ppmimage[width][height] = 1;
        }
    }
    srand(time(0)); //starting seed 0
    int xcoords[4];
    int ycoords[4];

    string firststr = "";
    while(!infile.eof()){
        getline(infile,STRING); // Saves the line in STRING.
//        printer(STRING);
        firststr = STRING;
        std::string s = STRING;
        std::string delim = " , ";
        s = s + " , ";
        int start = 0;
        int end = s.find(delim);
        int counter = 0;
        while (end != std::string::npos)
        {
            string value = s.substr(start, end - start);
            std::size_t pos = value.find(",");
            std::string str1 = value.substr (1,pos-1);
            std::string str2 = value.substr (pos+1,str1.length());
            double xcoord = stod(str1);
            double ycoord = stod(str2);

            double newx = double(800*xcoord); //convert to double to be more accurate
            double newy = double(800*ycoord);
            xcoords[counter] = newx;
            ycoords[counter] = newy;
            percirc(ppmimage, 2, int(newx), int(newy));

            counter+=1;

            start = end + delim.length();
            end = s.find(delim, start);
        }
    }
    double ax = xcoords[0];
    double ay = ycoords[0];
    double bx = xcoords[1];
    double by = ycoords[1];
    double cx = xcoords[2];
    double cy = ycoords[2];
    double dx = xcoords[3];
    double dy = ycoords[3];

    infile.close();
    std::ofstream newf;
    newf.open("output.txt");
    newf << firststr <<std::endl;
    string firstvertex = persquareform(ppmimage,ax,ay,bx,by,cx,cy,dx,dy,1, 1, 0);
    newf << firstvertex << endl;

    string secondvertex = persquareform(ppmimage,ax,ay,bx,by,cx,cy,dx,dy,-1, 2, 0);
    newf << secondvertex << endl;
    string thirdvertex = persquareform(ppmimage,ax,ay,dx,dy,bx,by,cx,cy,1, 3, 0);
    newf << thirdvertex << endl;
    string fourthvertex = persquareform(ppmimage,ax,ay,dx,dy,bx,by,cx,cy,-1, 4, 0);
    newf << fourthvertex << endl;
    string fifthvertex = persquareform(ppmimage,ax,ay,cx,cy,bx,by,dx,dy,1, 5, 0);
    newf << fifthvertex << endl;
    string sixthvertex = persquareform(ppmimage,ax,ay,cx,cy,bx,by,dx,dy,-1, 6, 0);
    newf << sixthvertex;
//    printer(minval);
//    printer(minarea);
    if(minval == 1){
        persquareform(ppmimage,ax,ay,bx,by,cx,cy,dx,dy,1, 1, 1);
    }
    if(minval == 2){
        persquareform(ppmimage,ax,ay,bx,by,cx,cy,dx,dy,-1, 2, 1);
    }
    if(minval == 3){
        persquareform(ppmimage,ax,ay,dx,dy,bx,by,cx,cy,1, 3, 1);
    }
    if(minval == 4){
        persquareform(ppmimage,ax,ay,dx,dy,bx,by,cx,cy,-1, 4, 1);
    }
    if(minval == 5){
        persquareform(ppmimage,ax,ay,cx,cy,bx,by,dx,dy,1, 5, 1);
    }
    if(minval == 6){
        persquareform(ppmimage,ax,ay,cx,cy,bx,by,dx,dy,-1, 6, 1);
    }
    createPPM(ppmimage); //creating the image

}

int main(){
    part1();
    part2();
    return 0;
}
