#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/objdetect.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <iostream>
#include <vector>

#define ESC 27

using namespace std;
using namespace cv;

int main() {
    VideoCapture cap(0);
    Mat frame, frame1, frame2, motion;
    Mat diff, blurDiff, thresh;
    vector<Point> largestContour;
    vector<Moments> center(1);
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    bool isPaused = false;
    bool isDetection = true;

    if(!cap.isOpened()) {
        cout << "Camera cannot be opened" << endl;
        return -1;
    }

    namedWindow("Original", WINDOW_AUTOSIZE);
    namedWindow("Diff", WINDOW_AUTOSIZE);
    namedWindow("Blur diff", WINDOW_AUTOSIZE);
    namedWindow("Motion detection", WINDOW_AUTOSIZE);
    namedWindow("Threshold", WINDOW_AUTOSIZE);

    while(true) {
        cap >> frame;

        if(!isPaused) {
            frame.copyTo(frame1);
            frame.copyTo(motion);

            cvtColor(frame1, frame1, COLOR_RGB2GRAY);
            cap >> frame2;
            cvtColor(frame2, frame2, COLOR_RGB2GRAY);

            absdiff(frame1, frame2, diff);
            GaussianBlur(diff, blurDiff, Size(0, 0), 3);
            threshold(blurDiff, thresh, 20, 255, THRESH_BINARY);
            findContours(thresh, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

            largestContour.clear();
            double maxArea = 0.0;
            for (int i = 0; i < contours.size(); i++) {
                double area = contourArea(contours[i], false);
                if (area > maxArea) {
                    maxArea = area;
                    largestContour = contours[i];
                }
            }

            if (!largestContour.empty() && isDetection) {
                center[0] = moments(largestContour, false);
                int x = center[0].m10 / center[0].m00;
                int y = center[0].m01 / center[0].m00;
                Point p(x, y);

                Rect boundingRectangle = boundingRect(largestContour);
                vector<Point> hull;
                convexHull(largestContour, hull, true, true);

                rectangle(motion, boundingRectangle, Scalar(0, 255, 0), 2);
                polylines(motion, hull, true, Scalar(255, 0, 0), 2);

                string text = "(" + to_string((int) p.x) + ", " + to_string((int) p.y) + ")";
                putText(motion, text, p, FONT_HERSHEY_PLAIN, 1.25, Scalar(0, 0, 200), 2);
            }
        }

        if(isDetection) {
            imshow("Blur diff", blurDiff);
            imshow("Threshold", thresh);
            imshow("Diff", diff);
        }
        if(!isDetection) {
            destroyWindow("Threshold");
            destroyWindow("Diff");
            destroyWindow("Blur diff");
        }
        imshow("Original", frame);
        imshow("Motion detection", motion);

        int key = waitKeyEx(1);

        if(key == 49) {
            isPaused = !isPaused;
            cout << "pauza: " << isPaused << endl;
        } else if (key == 50) {
            isDetection = !isDetection;
            cout << "detection" << isDetection << endl;
        }else if (key == 30) {
            break;
        }

    }

    cap.release();
    frame.release();
    destroyAllWindows();

    return 0;
}