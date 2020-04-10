//Ankit Gupta, Computer Vision Exercise
//Lab 6, Coin Detection, Part 2

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
//#include <cmath>
#include <math.h>
#include <bits/stdc++.h>
#include <map>

#define PI 3.14159265

using namespace std;

int main(){

    std::ifstream file("outcoin.ppm");
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

    int magmat[ylen][xlen];
    int valmat[ylen][xlen];


    for(int i = 0;i<ylen;i++){
        for(int j = 0;j<xlen;j++){
            magmat[i][j] = int(sqrt(gxmat[i][j]*gxmat[i][j] + gymat[i][j]*gymat[i][j]));
            int val = 0;
            if(magmat[i][j] > 70){
                val = 1;
            }
            valmat[i][j] = val;
        }
    }


    ofstream thirdf;
    string thif = "image1.ppm";
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
    string secf = "image2.ppm";
    secondf.open(secf);
    secondf << "P3" << endl;
    secondf << std::to_string(xlen) + " " + std::to_string(ylen) << endl;
    secondf << "1" << endl;


    vector <pair<int, int>> valpts;

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
//                fourthf << 1 << " " << 1 << " " << 1 << " ";
                valpts.push_back(make_pair(i,j));
                valmat[i][j] = 6;
            }
            else{
//                fourthf << 0 << " " << 0 << " " << 0 << " ";
            }
        }
    }

    secondf << endl;
    secondf.close();


    //map in c++

    map<pair<int, int>, int> mymap;
    mymap.insert(make_pair(make_pair(1,2), 3));
    mymap[make_pair(1,2)] = 5;
//    cout << "Final: " << mymap[make_pair(1,2)] << endl;

    int iter = 0;
    for(int i = 0;i<ylen;i++) { //set equal to 0
        for (int j = 0; j < xlen; j++) {
            if(valmat[i][j] == 6){ //strong edge
                if(gxmat[i][j] != 0 && (gymat[i][j]*1.0/gxmat[i][j])<5.0){
                    double gradientdir = gymat[i][j]*1.0/gxmat[i][j];
                    for (int z = -300; z < 300; z++) {
                        int newx = i + z * 1;
                        int newy = j + z * gradientdir;
                        if (newx > 0 && newx < ylen && newy > 0 && newy < xlen) {
                            valmat[newx][newy] = 3;
                            if(mymap.find(make_pair(newx,newy)) == mymap.end()){
                                mymap.insert(make_pair(make_pair(newx,newy), 1));
                            }
                            else{
                                int previousval = mymap[make_pair(newx,newy)];
                                mymap[make_pair(newx,newy)] = previousval+1;
                            }
                        }
                    }
                    iter += 1;
//                    cout << gradientdir << endl;
                }
                valmat[i][j] = 6;
            }
        }
    }

    map<pair<int, int>, int>::iterator it = mymap.begin();

    int maxxval = 0;
    int maxyval = 0;
    int maxcounter = 0;

    while(it != mymap.end()) {
        int cter = it->second;
//        cout << cter << endl;
        if(cter>maxcounter){
            maxcounter=cter;
            std::pair<int, int> newpair = it->first;
            maxxval = newpair.first;
            maxyval = newpair.second;
        }
        it++;
    }

//    cout << "Count: " << maxcounter << endl;
//    cout << "X: " << maxxval << ", Y: " << maxyval << endl;

    ofstream fourthf;
    string fouf = "imagef.ppm";
    fourthf.open(fouf);
    fourthf << "P3" << endl;
    fourthf << std::to_string(xlen) + " " + std::to_string(ylen) << endl;
    fourthf << 255 << endl;


    vector <pair<int, int>> vect;

    for(int i = 0;i<ylen;i++) { //set equal to 0
        for (int j = 0; j < xlen; j++) {
            if(mymap.find(make_pair(i,j)) == mymap.end()){
            }
            else{
                if(mymap[make_pair(i,j)]>12){
                    vect.push_back(make_pair(i, j));
                }
            }
        }
    }

    sort(vect.begin(), vect.end());

    vector <pair<int, int>> avgvect;

    int totalx = 0;
    int totaly = 0;
    int totcount = 0;
    double avgx = 0;
    double avgy = 0;

    for (int y=0; y<vect.size(); y++)
    {
        int firstval = vect[y].first;
        int secondval = vect[y].second;

        if(y!=0){
            if(firstval-avgx > 20){
                avgvect.push_back(make_pair(int(avgx), int(avgy)));
               totalx = 0;
               totaly = 0;
               totcount = 0;
               avgx = 0.0;
               avgy = 0.0;
            }
        }

        totalx = totalx + firstval;
        totaly = totaly + secondval;
        totcount = totcount+1;
        avgx = totalx*1.0/totcount;
        avgy = totaly*1.0/totcount;
    }
    avgvect.push_back(make_pair(int(avgx), int(avgy)));

    vector <int> allradii;

    for(int h = 0;h<avgvect.size();h++){
        pair<int, int> thispair = avgvect[h];
        int firstpart = thispair.first;
        int secondpart = thispair.second;


        int maxcount = 0;
        int bestrad = -1;

        for(int rad = 10; rad<70; rad++){
            int fincount = 0;
            for(int t = 0;t<valpts.size();t++){
                if(int(sqrt(1.0*(pow(valpts[t].second-avgvect[h].second, 2)+ pow(valpts[t].first-avgvect[h].first, 2)))) == rad){
                    fincount+=1;
                }
            }
            if(fincount > maxcount){
                maxcount = fincount;
                bestrad = rad;
            }
        }
        allradii.push_back(bestrad);
    }

    vector<pair<int, int>> circpoints;

    for(int e = 0;e<allradii.size();e++){
        int thisrad = allradii[e];
        for(int tryrad = thisrad-1; tryrad<thisrad+2; tryrad++) {
//            cout << tryrad << endl;
            for (int theta = 0; theta < 360; theta += 1) {
                int newx = avgvect[e].first + int(tryrad * cos(theta * PI / 180));
                int newy = avgvect[e].second + int(tryrad * sin(theta * PI / 180));
                if (find(circpoints.begin(), circpoints.end(), make_pair(newx, newy)) == circpoints.end()) {
                    circpoints.push_back(make_pair(newx, newy));
                }
            }
        }
    }

    for(int i = 0;i<ylen;i++) { //set equal to 0
        for (int j = 0; j < xlen; j++) {
//            if (find(circpoints.begin(), circpoints.end(), make_pair(i, j)) != circpoints.end()) {
//                fourthf << 18 << " " << 18 << " " << 18 << " ";
//            }
              if(valmat[i][j] == 6){
                  fourthf << 255 << " " << 255 << " " << 255 << " ";
              }
              else{
                  fourthf << 0 << " " << 0 << " " << 0 << " ";
              }
//            if (mymap.find(make_pair(i, j)) == mymap.end()) {
//                fourthf << 0 << " " << 0 << " " << 0 << " ";
//            } else {
//                if (mymap[make_pair(i, j)] > 12) {
//                    fourthf << 18 << " " << 18 << " " << 18 << " ";
//                } else {
//                    fourthf << 0 << " " << 0 << " " << 0 << " ";
//                }
//            }
        }
    }

    fourthf << endl;
    fourthf.close();

    ofstream fifthf;
    string fiftf = "imagec.ppm";
    fifthf.open(fiftf);
    fifthf << "P3" << endl;
    fifthf << std::to_string(xlen) + " " + std::to_string(ylen) << endl;
    fifthf << "255" << endl;

    for(int e = 0;e<points.size()-2;e=e+3){
        int realindex = e/3;
        int aindex = realindex/xlen;
        int bindex = realindex - aindex*xlen;
        if(find(circpoints.begin(), circpoints.end(), make_pair(aindex, bindex)) != circpoints.end()){
            fifthf << 255 << " " << 0 << " " << 0 << " ";
        }
        else {
            fifthf << points[e] << " " << points[e + 1] << " " << points[e + 2] << " ";
        }
    }

    fifthf << endl;
    fifthf.close();

    int pennies = 0;
    int nickels = 0;
    int quarters = 0;
    int dimes = 0;


    for(int r = 0;r<allradii.size();r++) {
        if(allradii[r] > 40 && allradii[r] < 50){
            pennies+=1;
        }
        if(allradii[r] > 50 && allradii[r] < 60){
            nickels+=1;
        }
        if(allradii[r] > 30 && allradii[r] < 40){
            dimes+=1;
        }
        if(allradii[r] > 80 && allradii[r] < 100){
            quarters+=1;
        }

    }

    ofstream outputresults;
    string outr = "results.txt";
    outputresults.open(outr);

    int totalamount = int(pennies)*1 +  int(nickels)*5 + int(quarters)*25 + int(dimes)*10;
    double numdol = totalamount/100.0;
    cout << quarters << " quarters, " << dimes << " dimes, " << nickels << " nickels, " << pennies << " pennies | " << "Total Sum: $" << numdol;
    outputresults << quarters << " quarters, " << dimes << " dimes, " << nickels << " nickels, " << pennies << " pennies | " << "Total Sum: $" << numdol;
    outputresults.close();

}