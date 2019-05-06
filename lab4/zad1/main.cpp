#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <iostream>

using namespace std;
using namespace cv;


int main() {

    //VideoCapture cap("/Users/agalempaszek/Desktop/opencv-lab/bike.avi");
    //VideoCapture cap("/Users/agalempaszek/Desktop/opencv-lab/robot_no_loop.avi");
    VideoCapture cap(0);

    Mat firstFrame, secondFrame, nextFrame, src, dst;
    int alphaSlider = 1;
    int thresholdSlider = 1;


    if(!cap.isOpened()) {
        cerr << "Image source cannot be opened" << endl;
    }

    cap >> firstFrame;
    secondFrame = firstFrame;
    cvtColor(firstFrame, firstFrame, COLOR_RGB2GRAY);
    cvtColor(secondFrame, secondFrame, COLOR_RGB2GRAY);

    dst = firstFrame;

    namedWindow("Image", WINDOW_AUTOSIZE);
    namedWindow("Slider", WINDOW_AUTOSIZE);

    createTrackbar("Alpha", "Slider", &alphaSlider, 300);
    createTrackbar("Threshold", "Slider", &thresholdSlider, 300);

    while(true) {
        cap >> secondFrame;
        cap >> nextFrame;
        double alphaVal = (double) alphaSlider/100;

        if(secondFrame.empty() || nextFrame.empty()) {
            cap.set(CAP_PROP_POS_FRAMES, 0);
            cap >> secondFrame;
            cap >> nextFrame;
        }

        cvtColor(secondFrame, secondFrame, COLOR_RGB2GRAY);
        cvtColor(nextFrame, nextFrame, COLOR_RGB2GRAY);

        absdiff(secondFrame, nextFrame, src);
        threshold(src, src, thresholdSlider, 255, THRESH_BINARY);

        dst = (1 - alphaVal) * dst + alphaVal * src;

        try {
            imshow("Image", dst);
        } catch (Exception &e) {
            cout << e.what() << endl;
            return  1;
        }

        if(waitKey(30) == 27) break;
    }

    cap.release();
    firstFrame.release();
    secondFrame.release();
    nextFrame.release();
    src.release();
    dst.release();
    destroyAllWindows();

    return 0;
}