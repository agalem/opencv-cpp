#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <iostream>

#define ESC 27

using namespace std;
using namespace cv;

int main() {

    VideoCapture cap(0);
    //VideoCapture cap("/Users/agalempaszek/Desktop/opencv-lab/bike.avi");
    Mat src, backgroundImg, foregroundImg;
    Mat fgMaskMOG2;
    Ptr<BackgroundSubtractorMOG2> pMOG2 = createBackgroundSubtractorMOG2();

    int history = 100;
    int nMixtures = 10;

    if (!cap.isOpened()) {
        cerr << "File cannot be opened" << endl;
        return -1;
    }

    namedWindow("First Plan", WINDOW_AUTOSIZE);
    namedWindow("Background", WINDOW_AUTOSIZE);
    namedWindow("Sliders", WINDOW_AUTOSIZE);
    namedWindow("Mask", WINDOW_AUTOSIZE);

    createTrackbar("History", "Sliders", &history, 300);
    createTrackbar("nMixtures", "Sliders", &nMixtures, 30);

    while(true) {

        cap >> src;

        if(src.empty()) {
            cap.set(CAP_PROP_POS_FRAMES, 0);
            cap >> src;
        }

        pMOG2->setHistory(history);
        pMOG2->setNMixtures(nMixtures);
        pMOG2->setDetectShadows(true);
        pMOG2->apply(src, fgMaskMOG2);
        pMOG2->getBackgroundImage(backgroundImg);


        vector<vector<Point> > contours;

        findContours( fgMaskMOG2, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);

        for(int i=0; i<contours.size(); i++) {
            double area = contourArea( contours[i],false);
            if(area > 100) {
                drawContours(src, contours, i, Scalar(0, 255, 0), 6);
            }
        }

        imshow("First Plan", src);
        imshow("Mask", fgMaskMOG2);
        imshow("Background", backgroundImg);

        if(waitKey(30) == ESC) break;
    }

    cap.release();
    src.release();
    foregroundImg.release();
    backgroundImg.release();
    destroyAllWindows();

    return 0;
}