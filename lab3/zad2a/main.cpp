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

    //VideoCapture cap(0);
    VideoCapture cap("/Users/agalempaszek/Desktop/opencv-lab/robot_no_loop.avi");
    Mat watermark = imread("/Users/agalempaszek/Desktop/opencv-lab/logo.bmp", -1);
    Mat logo, frame;
    int logoSize;

    namedWindow("window", WINDOW_AUTOSIZE);

    cap.set(CAP_PROP_FRAME_WIDTH, 640);
    cap.set(CAP_PROP_FRAME_HEIGHT, 480);

    cout << "Podaj rozmiar obrazka" << endl;
    cin >> logoSize;

    resize(watermark,logo, Size(logoSize, logoSize));

    while (true) {

        cap >> frame;

        if(frame.empty()) {
            break;
        }

        try {

            Rect rect(frame.cols - logoSize, frame.rows - logoSize, logoSize, logoSize);
            frame(rect) = logo + frame(rect);

            imshow("window", frame);

        } catch (Exception &e) {

            cerr << e.what() << endl;
            return 1;

        }

        if(waitKey(30) == 27) {
            break;
        }
    }

    cap.release();
    destroyAllWindows();

    return 0;
}