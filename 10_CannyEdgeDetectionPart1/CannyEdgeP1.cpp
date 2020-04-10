//Ankit Gupta, Computer Vision Exercise
//Lab 5, Canny Edge Detection

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <bits/stdc++.h>


using namespace std;

int main(){

    std::ifstream file("image.ppm");
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

    ofstream outf;
    string fname = "imageg.ppm";
    outf.open(fname);
    outf << "P3" << endl;
    outf << std::to_string(xlen) + " " + std::to_string(ylen) << endl;
    outf << "255" << endl;

    int ppmimage[ylen][xlen]; //initializing matrix

    cout << points.size() << endl;
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
            outf << avgnum <<  " " << avgnum << " " <<  avgnum << " ";
        }
    }

    outf << endl;
    outf.close();

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
        }
    }

    ofstream secondf;
    string secf = "imagem.ppm";
    secondf.open(secf);
    secondf << "P3" << endl;
    secondf << std::to_string(xlen) + " " + std::to_string(ylen) << endl;
    secondf << "1" << endl;

    int magmat[ylen][xlen];
    for(int i = 0;i<ylen;i++){
        for(int j = 0;j<xlen;j++){
            magmat[i][j] = int(sqrt(gxmat[i][j]*gxmat[i][j] + gymat[i][j]*gymat[i][j]));
            int val = 0;
            if(magmat[i][j] > 70){
                val = 1;
            }
            secondf << val <<  " " << val << " " <<  val << " ";
        }
    }
    secondf << endl;
    secondf.close();
}