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
    std::stringstream strg;
    strg << number;
    return strg.str();
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

    //wartości w tablicach: żółty, zielony, czerwony, niebieski
    int min_H_tab[4] = {0, 34, 115, 0};
    int max_H_tab[4] = {266, 82, 180, 21};
    int min_S_tab[4] = {115, 65, 164, 63};
    int max_S_tab[4] = {223, 253, 279, 300};
    int min_V_tab[4] = {152, 75, 99, 34};
    int max_V_tab[4] = {300, 154, 300, 191};

    namedWindow("HSV", WINDOW_AUTOSIZE);
    namedWindow("Contour", WINDOW_AUTOSIZE);

    int i = 0;
    int temp = 0;
    long timeVal;

    while(true) {
        timeVal = time(0);

        if (timeVal % 5 == 0) {

            if (temp == 0) {

                min_H = min_H_tab[i];
                max_H = max_H_tab[i];
                min_S = min_S_tab[i];
                max_S = max_S_tab[i];
                min_V = min_V_tab[i];
                max_V = max_V_tab[i];

                if(i == 0) {
                    cout << "Żółty" << endl;
                } else if (i == 1) {
                    cout << "Zielony" << endl;
                } else if (i == 2) {
                    cout << "Czerwony" << endl;
                } else if (i == 3) {
                    cout << "Niebieski" << endl;
                }

                i += 1;
                temp += 1;

                if (i > 3) {
                    i = 0;
                }
            }
        } else {
            temp = 0;
        }

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