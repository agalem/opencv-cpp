#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

#include <iostream>

#define ESC 27

using namespace std;
using namespace cv;



int main() {
    Mat image;

    VideoCapture cap("/Users/agalempaszek/Desktop/opencv-lab/robot_no_loop.avi");

    Mat firstFrame, prevFrame, nextFrame, result, result2;

    if (!cap.isOpened()) {
        cerr << "Błąd w otworzeniu pliku" << endl;
        return -1;
    }

    cap.read(firstFrame);
    cvtColor(firstFrame, firstFrame, COLOR_RGB2GRAY);


    while (true) {

        cap >> prevFrame;
        cap >> nextFrame;

        if(prevFrame.empty() || nextFrame.empty()) {
            break;
        }

        imshow("Orginalny", nextFrame);

        cvtColor(prevFrame, prevFrame, COLOR_RGB2GRAY);
        cvtColor(nextFrame, nextFrame, COLOR_RGB2GRAY);

        absdiff(firstFrame, nextFrame, result);
        absdiff(prevFrame, nextFrame, result2);

        threshold(result, result, 100, 255, THRESH_TRUNC);
        threshold(result2, result2, 100, 255, THRESH_TRUNC);

        erode(result, result, 1);
        erode(result2, result2, 1);

        dilate(result, result, 1);
        dilate(result2, result2, 1);

        imshow("Pierwsza klatka", result);
        imshow("Poprzednia klatka", result2);


        if (waitKey(30) == ESC) break;
    }


    cap.release();
    destroyAllWindows();

    return 0;
}