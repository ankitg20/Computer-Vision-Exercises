//Ankit Gupta, Computer Vision Exercise


#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>

using namespace cv;

int main()
{
    Mat src, src_gray;

    src = imread("image.jpg", 1 ); //change to image.jpg

    if( !src.data )
    { return -1; }

    cvtColor( src, src_gray, COLOR_BGR2GRAY );

    GaussianBlur( src_gray, src_gray, Size(9, 9), 2, 2 );

    std::vector<Vec3f> circles;

    //easy (works perfectly)
//    HoughCircles( src_gray, circles, HOUGH_GRADIENT, 1, src_gray.rows/16, 100, 30, 15, 150 );

    //medium (works perfectly)
//    HoughCircles( src_gray, circles, HOUGH_GRADIENT, 1, src_gray.rows/40, 100, 49, 30, 150);

//universal approximation
    HoughCircles( src_gray, circles, HOUGH_GRADIENT, 1, src_gray.rows/40, 100, 45, 30, 150);

    int quarters = 0;
    int dimes = 0;
    int nickels = 0;
    int pennies = 0;
    int onedollar = 0;

    for( size_t i = 0; i < circles.size(); i++ )
    {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        circle( src, center, radius-1, Scalar(0,0,255), 3, 8, 0 );
        circle( src, center, radius, Scalar(0,0,255), 3, 8, 0 );
        circle( src, center, radius+1, Scalar(0,0,255), 3, 8, 0 );
        if(radius < 80){
            dimes+=1;
        }
        else if(radius <95){
            pennies+=1;
//            circle( src, center, 15, Scalar(0,255,0), -1, 8, 0 );
        }
        else if(radius < 100){
            nickels+=1;
//            circle( src, center, 15, Scalar(0,0,255), -1, 8, 0 );
        }
        else if(radius < 125){
            quarters+=1;
//            circle( src, center, 15, Scalar(255,0,0), -1, 8, 0 );
        }
        else{
            onedollar+=1;
        }
    }

    std::ofstream myfile;
    myfile.open("results.txt");
    double totalcount = (.25 * quarters) + (.1 * dimes) + (.05 * nickels) + (.01 * pennies) + onedollar;
    myfile << quarters << " quarters, " << dimes << " dimes, " << nickels << " nickels, " << pennies << " pennies, " << onedollar << " one-dollar coins" << std::endl;
    myfile << "Total Sum: $" << totalcount;
    myfile.close();

    std::cout << quarters << " quarters, " << dimes << " dimes, " << nickels << " nickels, " << pennies << " pennies, " << onedollar << " one-dollar coins" << std::endl;
    std::cout << "Total Sum: $" << totalcount;

    imwrite( "imagec.jpg", src );
    return 0;
}