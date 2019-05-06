#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <iostream>
#include <vector>

#define ESC 27

using namespace std;
using namespace cv;

int counter = 0;
int timeDelay = 10;

//stays as int
int topMaxCorners = 500;
int maxCorners = 500;

//converted to double
int topQualityLevel = 100;
int qualityLevel = 1;
double dQualityLevel;

//converted to double
int topMinDistance = 700;
int minDistance = 500;
double dMinDistance;

double convertToDouble(int value) {
    return double(value) / 100.00;
}

int main() {

    //VideoCapture cap("/Users/agalempaszek/Desktop/opencv-lab/bike.avi");
    VideoCapture cap(0);
    Mat mask, prevFrame, frame, prevFrameGray, frameGray, frameMotion;
    vector<uchar> status;
    vector<float> err;
    vector<Point2f> currCorners, prevCorners;
    TermCriteria criteria = TermCriteria(TermCriteria::COUNT | TermCriteria::EPS, 20, 0.03);
    bool init = true;


    namedWindow("Image", WINDOW_AUTOSIZE);
    namedWindow("Motion", WINDOW_AUTOSIZE);
    namedWindow("Slider", WINDOW_AUTOSIZE);

    createTrackbar("maxCorners", "Slider", &maxCorners, topMaxCorners);
    createTrackbar("qualityLevel", "Slider", &qualityLevel, topQualityLevel);
    createTrackbar("minDistance", "Slider", &minDistance, topMinDistance);



    cap >> prevFrame;
    cvtColor(prevFrame, prevFrameGray, COLOR_RGB2GRAY);

    while(true) {

        cap >> frame;

        if (frame.empty()) {
            cap.set(CAP_PROP_POS_FRAMES, 0);
            cap >> frame;
        }

        frame.copyTo(frameMotion);

        cvtColor(frame, frameGray, COLOR_RGB2GRAY);

        if(qualityLevel == 0) {
            qualityLevel += 1;
        }
        dQualityLevel = convertToDouble(qualityLevel);
        dMinDistance = convertToDouble(minDistance);

        if (init) {
            goodFeaturesToTrack(prevFrameGray, currCorners, maxCorners, dQualityLevel, dMinDistance, mask, 3, 3, false, 0.04);
            init = false;
        } else if (!prevCorners.empty()) {
            calcOpticalFlowPyrLK(prevFrameGray, frameGray, prevCorners, currCorners, status, err, Size(31, 31), 2,
                                 criteria);

            for (int i = 1; i < currCorners.size(); i++) {
                if (norm(prevCorners[i] - currCorners[i]) > 1) {
                    line(frameMotion, prevCorners[i], currCorners[i], Scalar(255, 255, 255), 2);
                    circle(frameMotion, currCorners[i], 3, Scalar(0, 0, 0), 2);
                } else {
                    prevCorners.erase(prevCorners.begin() + i);
                    currCorners.erase(currCorners.begin() + i);

                    if (currCorners.size() < 10) init = true;
                }
            }

            if (counter % timeDelay == 0) init = true;

            counter++;
        }

        prevCorners = currCorners;
        prevFrameGray = frameGray.clone();

        currCorners.clear();

        imshow("Image", frame);
        imshow("Motion", frameMotion);

        if(waitKey(30) == ESC) break;

    }

    cap.release();
    frame.release();
    frameMotion.release();
    destroyAllWindows();

    return 0;
}