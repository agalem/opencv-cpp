#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <iostream>

#define ESC 27

using namespace std;
using namespace cv;

int main() {

    VideoCapture cap(0);
    Mat src;
    Mat cannyImg, linesImg;

    if (!cap.isOpened()) {
        cerr << "Camera cannot be opened" << endl;
        return -1;
    }

    namedWindow("Canny", WINDOW_AUTOSIZE);
    namedWindow("Lines", WINDOW_AUTOSIZE);

    while(true) {

        cap >> src;

        Canny(src, cannyImg, 50, 200, 3);

        src.copyTo(linesImg);

        vector<Vec4i> lines;

        //threshold, line lenght, line gap
        HoughLinesP(cannyImg, lines, 1, CV_PI/180, 50, 50, 10 );
        for( size_t i = 0; i < lines.size(); i++ )
        {
            Vec4i l = lines[i];
            line( linesImg, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, LINE_AA);
        }


        imshow("Canny", cannyImg);
        imshow("Lines", linesImg);

        if(waitKey(30) == ESC) break;
    }

    src.release();
    linesImg.release();
    cannyImg.release();
    destroyAllWindows();

    return 0;
}