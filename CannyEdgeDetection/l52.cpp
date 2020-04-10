//Ankit Gupta, TJHSST Computer Vision 1, Mr. Jurj
//Lab 5, Canny Edge Detection, Part 2


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
//#include <math.h>
//#include <bits/stdc++.h>

#define PI 3.14159265

using namespace std;

int main(){


    cout << "reachable" << endl;



    std::ifstream file("imagenew.ppm");
    std::string str;
    int count = 0;
    int xlen = 0;
    int ylen = 0;
    vector <int> points;

    while (std::getline(file, str)){ //remove 2nd condition
        if(count == 1){
            int spaceindex = str.find(" ");
            string firstpart = str.substr(0, spaceindex);
            string secondpart = str.substr(spaceindex+1);
            xlen = stoi(firstpart);
            ylen = stoi(secondpart);
        }
        if(count >= 3){
            string word = "";
            for (auto x : str){
                if (x == ' '){
                    points.push_back(stoi(word));
                    word = "";
                }
                else{
                    word = word + x;
                }
            }
        }
        count+=1;
    }

    int ppmimage[ylen][xlen]; //initializing matrix

    int *magmat[ylen][xlen];
    int *valmat[ylen][xlen];

//    cout << points.size() << endl;
    for(int i = 0;i<points.size()-2;i=i+3){
        int avg = (points[i]+points[i+1]+points[i+2])/3;
        int realindex = i/3;
        int aindex = realindex/xlen;
        int bindex = realindex - aindex*xlen;
        ppmimage[aindex][bindex] = avg;
    }

    for(int i = 0;i<ylen;i++){
        for(int j = 0;j<xlen;j++){
            int avgnum = ppmimage[i][j];
        }
    }

    int gxmat[ylen][xlen];
    for(int i = 0;i<ylen;i++){
        for(int j = 0;j<xlen;j++){
            gxmat[i][j] = 0;
        }
    }

    for(int a = 1;a<ylen-1;a++){
        for(int b = 1;b<xlen-1;b++){
            int tleft = -1*ppmimage[a-1][b-1];
            int tmid = 0*ppmimage[a][b-1];
            int tright = 1*ppmimage[a+1][b-1];
            int mleft = -2*ppmimage[a-1][b];
            int mmid = 0*ppmimage[a][b];
            int mright = 2*ppmimage[a+1][b];
            int bleft = -1*ppmimage[a-1][b+1];
            int bmid = 0*ppmimage[a][b+1];
            int bright = 1*ppmimage[a+1][b+1];
            int totalval = tleft+tmid+tright+mleft+mmid+mright+bleft+bmid+bright;
            gxmat[a][b] =  totalval;
        }
    }

    int gymat[ylen][xlen];
    for(int i = 0;i<ylen;i++){
        for(int j = 0;j<xlen;j++){
            gymat[i][j] = 0;
        }
    }

    cout << "reachable" << endl;

    for(int a = 1;a<ylen-1;a++){
        for(int b = 1;b<xlen-1;b++){
            int tleft = -1*ppmimage[a-1][b-1];
            int tmid = -2*ppmimage[a][b-1];
            int tright = -1*ppmimage[a+1][b-1];
            int mleft = 0*ppmimage[a-1][b];
            int mmid = 0*ppmimage[a][b];
            int mright = 0*ppmimage[a+1][b];
            int bleft = 1*ppmimage[a-1][b+1];
            int bmid = 2*ppmimage[a][b+1];
            int bright = 1*ppmimage[a+1][b+1];
            int totalval = tleft+tmid+tright+mleft+mmid+mright+bleft+bmid+bright;
            gymat[a][b] =  totalval;
            cout << "reachable" << endl;

        }
    }
    cout << "yesreachable" << endl;

    //PROBLEM Here

    cout << "reachable" << endl;

    for(int i = 0;i<ylen;i++){
        for(int j = 0;j<xlen;j++){
            magmat[i][j] = &(int(sqrt(gxmat[i][j]*gxmat[i][j] + gymat[i][j]*gymat[i][j])));
            int val = 0;
            if(magmat[i][j] > 70){
                val = 1;
            }
            valmat[i][j] = val;
        }
    }


    ofstream thirdf;
    string thif = "image5.ppm";
    thirdf.open(thif);
    thirdf << "P3" << endl;
    thirdf << std::to_string(xlen) + " " + std::to_string(ylen) << endl;
    thirdf << "1" << endl;

    int maxsuprmat[ylen][xlen];

    for(int i = 0;i<ylen;i++){ //set equal to 0
        for(int j = 0;j<xlen;j++){
            maxsuprmat[i][j] = 0;
        }
    }


    for(int i = 0;i<ylen;i++){
        for(int j = 0;j<xlen;j++){
            if(i>=1 && i<ylen-1 && j>=1 && j<xlen-1) {
//            magmat[i][j] = int(sqrt(gxmat[i][j]*gxmat[i][j] + gymat[i][j]*gymat[i][j]));
                int yval = gymat[i][j];
                int xval = gxmat[i][j];
                double degree = atan2(yval, xval) * 180 / PI;
                //between -180 and 180 --> -180 |-157.5| -135 |-112.5| -90 |-67.5| -45 |-22.5| 0 |22.5| 45 |67.5| 90 |112.5| 135 |157.5| 180,
                int anglepursue = -1000;

                if (degree < -157.5 && degree > -180) {
                    anglepursue = 0;
                }
                if (degree < -135 && degree > -157.5) {
                    anglepursue = 45;
                }
                if (degree < -112.5 && degree > -135) {
                    anglepursue = 45;
                }
                if (degree < -90 && degree > -112.5) {
                    anglepursue = 90;
                }
                if (degree < -67.5 && degree > -90) {
                    anglepursue = 90;
                }
                if (degree < -45 && degree > -67.5) {
                    anglepursue = 135;
                }
                if (degree < -22.5 && degree > -45) {
                    anglepursue = 135;
                }
                if (degree < 0 && degree > -22.5) {
                    anglepursue = 0;
                }
                if (degree > 157.5 && degree < 180) {
                    anglepursue = 180;
                }
                if (degree > 135 && degree < 157.5) {
                    anglepursue = 135;
                }
                if (degree > 112.5 && degree < 135) {
                    anglepursue = 135;
                }
                if (degree > 90 && degree < 112.5) {
                    anglepursue = 90;
                }
                if (degree > 67.5 && degree < 90) {
                    anglepursue = 90;
                }
                if (degree > 45 && degree < 67.5) {
                    anglepursue = 45;
                }
                if (degree > 22.5 && degree < 45) {
                    anglepursue = 45;
                }
                if (degree > 0 && degree < 22.5) {
                    anglepursue = 0;
                }

                int left = -1;
                int mid = magmat[i][j];
                int right = -1;


                if (anglepursue == 0 || anglepursue == 180) {
                    left = magmat[i - 1][j];
                    right = magmat[i + 1][j];
                }

                if (anglepursue == 90) {
                    left = magmat[i][j - 1];
                    right = magmat[i][j + 1];
                    //up down
                }

                if (anglepursue == 45) {
                    left = magmat[i + 1][j - 1];
                    right = magmat[i - 1][j + 1];
                    //up right diagonal
                }

                if (anglepursue == 135) {
                    left = magmat[i + 1][j + 1];
                    right = magmat[i - 1][j - 1];
                    //left right diagonal
                }

                if (mid > left && mid > right) {
                    maxsuprmat[i][j] = 1;
                    thirdf << maxsuprmat[i][j] << " " << maxsuprmat[i][j] << " " << maxsuprmat[i][j] << " ";
                } else {
                    maxsuprmat[i][j] = 0;
                    thirdf << maxsuprmat[i][j] << " " << maxsuprmat[i][j] << " " << maxsuprmat[i][j] << " ";
//                    cout << "yes" << endl;
                }
            }
            else{
//                thirdf << maxsuprmat[i][j] << " " << maxsuprmat[i][j] << " " << maxsuprmat[i][j] << " ";
                thirdf << 0 << " " << 0 << " " << 0 << " ";
            }
        }
    }
    thirdf << endl;
    thirdf.close();

    for(int i = 0;i<ylen;i++) {
        for (int j = 0; j < xlen; j++) {
            int magval = int(sqrt(gxmat[i][j] * gxmat[i][j] + gymat[i][j] * gymat[i][j]));
            if(magval>=90){
                valmat[i][j] = 2;
            }
            else if(magval>=70){
                valmat[i][j] = 1;
            }
            else{
                valmat[i][j] = 0;
            }
        }
    }
    int numchange = 100;
    while(numchange>0) {
        numchange = 0;
        for (int i = 1; i < ylen - 1; i++) {
            for (int j = 1; j < xlen - 1; j++) {
                if (valmat[i][j] == 1) { //if a cell is 2 and there is a one in any of the eight directions, make that a 2
                    if (valmat[i + 1][j] == 2 or valmat[i + 1][j + 1] == 2 or valmat[i + 1][j - 1] == 2 or
                        valmat[i][j + 1] == 2 or valmat[i][j - 1] == 2 or valmat[i - 1][j] == 2 or
                        valmat[i - 1][j - 1] == 2 or valmat[i - 1][j + 1] == 2) {
                        valmat[i][j] = 2;
                        numchange += 1;
                    }
                }
            }
        }
//        cout << numchange << endl;
    }



    ofstream secondf;
    string secf = "image6.ppm";
    secondf.open(secf);
    secondf << "P3" << endl;
    secondf << std::to_string(xlen) + " " + std::to_string(ylen) << endl;
    secondf << "1" << endl;


    ofstream fourthf;
    string fouf = "image7.ppm";
    fourthf.open(fouf);
    fourthf << "P3" << endl;
    fourthf << std::to_string(xlen) + " " + std::to_string(ylen) << endl;
    fourthf << "1" << endl;


    //do weak and strong edge component
    for(int i = 0;i<ylen;i++){ //set equal to 0
        for(int j = 0;j<xlen;j++){
//            fourthf << 0 << " " << 0 << " " << 0 << " ";
            if(valmat[i][j] == 2){ //both hysteresis and non max suppression
                secondf << 1 << " " << 1 << " " << 1 << " ";
            }
            else{
                secondf << 0 << " " << 0 << " " << 0 << " ";
            }

            valmat[i][j] = maxsuprmat[i][j]+valmat[i][j];
            if(valmat[i][j] == 3){ //both hysteresis and non max suppression
                fourthf << 1 << " " << 1 << " " << 1 << " ";
            }
            else{
                fourthf << 0 << " " << 0 << " " << 0 << " ";
            }
//            combinedmat[i][j] = 0;
        }
    }

    secondf << endl;
    secondf.close();

    fourthf << endl;
    fourthf.close();

}