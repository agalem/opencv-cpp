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

int main() {

    VideoCapture cap(0);
    Mat src;
    Mat cannyImg;
    Mat gaussianImg;
    Mat gradX, gradY;
    Mat angles;
    Mat magnitudes;
    Mat anglesColors;

    int scale = 1;
    int delta = 0;

    int gaussian = 5;
    int topThreshold = 150;
    int bottomThreshold  = 50;
    int sobelMask = 15;

    namedWindow("Canny", WINDOW_AUTOSIZE);
    namedWindow("Gaussian", WINDOW_AUTOSIZE);
    namedWindow("Sobel X", WINDOW_AUTOSIZE);
    namedWindow("Sobel Y", WINDOW_AUTOSIZE);
    namedWindow("Sobel Color", WINDOW_AUTOSIZE);
    namedWindow("Slider", WINDOW_AUTOSIZE);

    createTrackbar("Gaussian", "Slider", &gaussian, 100);
    createTrackbar("Top Threshold", "Slider", &topThreshold, 300);
    createTrackbar("Bottom Threshold", "Slider", &bottomThreshold, 200);
    createTrackbar("Sobel Mask", "Slider", &sobelMask, 21);

    while(true) {

        cap >> src;
        cvtColor(src, src, COLOR_RGB2GRAY);

        int gaussianVal;
        int sobelMaskVal;

        if(gaussian % 2 == 0) {
            gaussianVal = gaussian + 1;
        } else {
            gaussianVal = gaussian;
        }

        if(sobelMask % 2 == 0) {
            sobelMaskVal = sobelMask + 1;
        }  else {
            sobelMaskVal = sobelMask;
        }

        if(src.empty()) {
            cap.set(CAP_PROP_POS_FRAMES, 0);
            cap >> src;
        }


        GaussianBlur( src, gaussianImg, Size( gaussianVal,gaussianVal ), 0, 0, BORDER_DEFAULT);
        Canny(gaussianImg, cannyImg, 60, 180);

        Sobel( cannyImg, gradX, CV_32F, 1, 0, sobelMaskVal, scale, delta, BORDER_DEFAULT );
        Sobel( cannyImg, gradY, CV_32F, 0, 1, sobelMaskVal, scale, delta, BORDER_DEFAULT);

        cartToPolar(gradX, gradY, magnitudes, angles, true);

        threshold( gradX, gradX, bottomThreshold, topThreshold, THRESH_BINARY );
        threshold( gradY, gradY, bottomThreshold, topThreshold, THRESH_BINARY );

        //konwersja do ze skali szarości do RGB
        angles.copyTo(anglesColors);
        cvtColor(anglesColors, anglesColors, COLOR_GRAY2BGR);
        anglesColors.convertTo(anglesColors, CV_8UC3, 255);


        for (int i = 0; i < angles.rows; i++) {
            for (int j = 0; j < angles.cols; j++) {

                //pobieranie wartości kąta
                float angleVal = angles.at<float>(i, j);

                //kolorowanie
                if (angleVal > 45 && angleVal <= 135) {
                    anglesColors.at<Vec3b>(i, j) = Vec3b(255, 255, 255);

                } else if (angleVal > 135 && angleVal <= 255) {
                    anglesColors.at<Vec3b>(i, j) = Vec3b(255, 0, 0);

                } else if (angleVal > 255 && angleVal <= 315) {
                    anglesColors.at<Vec3b>(i, j) = Vec3b(0, 255, 0);

                } else if ((angleVal > 315 && angleVal <= 360) || (angleVal > 0 && angleVal <= 45)) {
                    anglesColors.at<Vec3b>(i, j) = Vec3b(0, 0, 255);
                }
            }
        }


        imshow("Angles", angles);
        imshow("Canny", cannyImg);
        imshow("Gaussian", gaussianImg);
        imshow("Sobel X", gradX);
        imshow("Sobel Y", gradY);
        imshow("Sobel Color", anglesColors);

        if(waitKey(30) == ESC) break;
    }

    cap.release();
    src.release();
    angles.release();
    cannyImg.release();
    gaussianImg.release();
    gradX.release();
    gradY.release();
    anglesColors.release();
    destroyAllWindows();

    return 0;
}