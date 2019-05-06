#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <iostream>

#define ESC 27

using namespace cv;
using namespace std;

string convertToString(int number){
    std::stringstream stringStream;
    stringStream << number;
    return stringStream.str();
}

int main() {

    VideoCapture cap(0);
    Mat src;
    Mat hsvImg;
    Mat values;
    Mat contoursHSV;
    Mat contourTracking;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    int largestArea=0;
    int largestContourIndex=0;
    vector<Point> largestContour;
    vector<Moments> center(1);

    int min_H = 0;
    int min_S = 0;
    int min_V = 0;
    int max_H = 180;
    int max_S = 255;
    int max_V = 255;
    int min_Thresh = 50;
    int max_Thresh = 300;

    namedWindow("HSV", WINDOW_AUTOSIZE);
    namedWindow("Contour", WINDOW_AUTOSIZE);
    namedWindow("Slider", WINDOW_AUTOSIZE);
    namedWindow("Values", WINDOW_AUTOSIZE);

    createTrackbar("MIN_H", "Slider", &min_H, 360);
    createTrackbar("MAX_H", "Slider", &max_H, 360);

    createTrackbar("MIN_S", "Slider", &min_S, 360);
    createTrackbar("MAX_S", "Slider", &max_S, 360);

    createTrackbar("MIN_V", "Slider", &min_V, 360);
    createTrackbar("MAX_V", "Slider", &max_V, 360);

    int oldMin_H = min_H;
    int oldMin_S = min_S;
    int oldMin_V = min_V;
    int oldMax_H = max_H;
    int oldMax_S = max_S;
    int oldMax_V = max_V;

    while(true) {

        cap >> src;

        src.copyTo(contourTracking);

        cvtColor(src, hsvImg, COLOR_RGB2HSV);
        inRange(hsvImg, Scalar(min_H, min_S, min_V), Scalar(max_H, max_S, max_V), hsvImg);

        erode(hsvImg, hsvImg, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
        erode(hsvImg, hsvImg, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
        dilate(hsvImg, hsvImg, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
        dilate(hsvImg, hsvImg, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));

        hsvImg.copyTo(contoursHSV);

        findContours( contoursHSV, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );

        for( int i = 0; i < contours.size(); i++ )
        {
            double area = contourArea( contours[i],false);
            if(area > largestArea && area > 400){
                largestContourIndex=i;
            }
        }
        
        largestContour = contours[largestContourIndex];
        center[0] = moments( largestContour, false );
        int x = center[0].m10/center[0].m00;
        int y = center[0].m01/center[0].m00;
        Point p(x, y);

        circle(contourTracking, p, 10, Scalar(255,0,0), 2);
        putText(contourTracking,convertToString(x)+","+convertToString(y),Point(x,y+30),1,1,Scalar(0,255,0),2);


        //wypisywanie wartości z trackbaru -> macOS defaultowo nie wyświetla ich obok suwakow
        if(oldMin_H != min_H || oldMin_S != min_S || oldMin_V != min_V ||
            oldMax_H != max_H || oldMax_S != max_S || oldMax_V != max_V) {

            oldMin_H = min_H;
            oldMin_S = min_S;
            oldMin_V = min_V;
            oldMax_H = max_H;
            oldMax_S = max_S;
            oldMax_V = max_V;
            cout << "MIN_H: " << min_H << endl;
            cout << "MAX_H: " << max_H << endl;
            cout << "MIN_S: " << min_S << endl;
            cout << "MAX_S: " << max_S << endl;
            cout << "MIN_V: " << min_V << endl;
            cout << "MAX_V: " << max_V << "\n\n\n";

       }

        imshow("Contour Tracking", contourTracking);
        imshow("Contour", contoursHSV);
        imshow("HSV", hsvImg);

        if(waitKey(30) == ESC) break;
    }

    src.release();
    hsvImg.release();
    contourTracking.release();
    contoursHSV.release();
    values.release();
    destroyAllWindows();

    return 0;
}