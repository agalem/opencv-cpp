#include "/usr/local/Cellar/opencv/4.0.1/include/opencv4/opencv2/highgui.hpp"

#include <iostream>

#define ESC 27

using namespace std;
using namespace cv;

int main() {
    Mat image;
    VideoCapture cap;

    cap.open(0);

    if(!cap.isOpened()) {
        cerr << "Cannot open a camera" << endl;
        return -1;
    }

    namedWindow("window", WINDOW_AUTOSIZE);

    double dWidth = cap.set(CAP_PROP_FRAME_WIDTH, 640);
    double dHeight = cap.set(CAP_PROP_FRAME_HEIGHT, 480);


    while (true) {
        cap >> image;

        try {
            flip(image, image, +1);

            imshow("window", image);
        } catch (Exception &e) {
            cout << e.what() << endl;

            return 1;
        }

        if (waitKey(30) == ESC) break;
    }


    return 0;
}
