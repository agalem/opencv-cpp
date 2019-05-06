#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

#include <iostream>

#define ESC 27

using namespace std;
using namespace cv;

int main() {
    Mat image;
    VideoCapture cap;

    cap.open(0);

    namedWindow("window", WINDOW_AUTOSIZE);

    double dWidth = cap.set(CAP_PROP_FRAME_WIDTH, 640);
    double dHeight = cap.set(CAP_PROP_FRAME_HEIGHT, 480);

    while (true) {
        cap >> image;

        try {
            flip(image, image, +1);

            cvtColor(image, image, COLOR_RGB2GRAY);

            for (int i = 0; i < image.rows; i++) {
                for (int j = 0; j < image.cols; j++) {
                    image.at<char>(i, j) = 255 - image.at<char>(i, j);
                }
            }

            imshow("window", image);
        } catch (Exception &e) {
            cout << e.what() << endl;

            return 1;
        }

        if (waitKey(30) == ESC) break;
    }

    return 0;
}
