#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/video.hpp>
#include<iostream>

using namespace cv;
using namespace std;

Mat frame_next, frame_prev;
vector<Point2f> corners_prev, corners_next;

vector<uchar> status;
vector<float> err;

bool needToInit = true;

TermCriteria termcrit = TermCriteria(TermCriteria::COUNT | TermCriteria::EPS, 20, 0.03);
Size subPixWinSize(10, 10), winSize(31, 31);

Mat corner_frame;
VideoCapture cap;

// int...
int maxCorners = 150;
int maxCorners_max = 500;

// double...
int qualityLevel = 1;
int qualityLevel_max = 100;

// double...
int minDistance = 250;
int minDistance_max = 250;

int counter = 0;
int timeDelay = 15;

double prepareDoubleValue(int min, int max, int ratio) {
    double val = (double) min / max;
    return val * ratio;
}

double guardRange(double value) {
    if (value == 0.0) value += 0.0001;
    else if (value == 1.0) value -= 0.0001;
    return value;
}

void textWithShadow(string text, double value, Point point, Scalar shadowColor) {
    Point shadow1 = Point(point.x + 1, point.y + 1);
    Point shadow2 = Point(point.x + 1, point.y - 1);
    Point shadow3 = Point(point.x - 1, point.y + 1);
    Point shadow4 = Point(point.x - 1, point.y - 1);
    string textToDisplay = text + format(": %.5g", value);
    putText(corner_frame, textToDisplay, shadow1, FONT_HERSHEY_PLAIN, 1.0, shadowColor);
    putText(corner_frame, textToDisplay, shadow2, FONT_HERSHEY_PLAIN, 1.0, shadowColor);
    putText(corner_frame, textToDisplay, shadow3, FONT_HERSHEY_PLAIN, 1.0, shadowColor);
    putText(corner_frame, textToDisplay, shadow4, FONT_HERSHEY_PLAIN, 1.0, shadowColor);
    putText(corner_frame, textToDisplay, point, FONT_HERSHEY_PLAIN, 1.0, Scalar(255, 255, 255));
}

void putTrackbarRealValuesOnFrame(double newQuality, double newDistance) {
    textWithShadow("Max Corners", maxCorners, Point(5, 15), Scalar(150, 0, 0));
    textWithShadow("Quality Level", newQuality, Point(5, 30), Scalar(0, 100, 0));
    textWithShadow("Min Distance", newDistance, Point(5, 45), Scalar(0, 0, 150));
    textWithShadow("Points", corners_next.size(), Point(5, 60), Scalar(50, 50, 50));
}

void displayCircles(vector<Point2f> corners, Scalar color) {
    for (int i = 0; i < corners.size(); i++) {
        circle(corner_frame, corners[i], 4, color, 2);
        circle(corner_frame, corners[i], 1, color, 1);
    }
}

bool pointInsideImage(Point2f point) {
    if (point.x < 0 || point.x > 640) return false;
    if (point.y < 0 || point.y > 480) return false;
    return true;
}

void ValueChanger() {
    Mat tmp = frame_next.clone();
    cvtColor(tmp, tmp, COLOR_BGR2GRAY);

    double newQuality = guardRange(prepareDoubleValue(qualityLevel, qualityLevel_max, 1));
    double newDistance = prepareDoubleValue(minDistance, minDistance_max, 5);

    if (needToInit) {
        goodFeaturesToTrack(tmp, corners_next, maxCorners, newQuality, newDistance, Mat(), 3, 0, false, 0.04);
        needToInit = false;
    } else if (!corners_prev.empty()) {
        calcOpticalFlowPyrLK(frame_prev, frame_next, corners_prev, corners_next, status, err, winSize, 3, termcrit, 0, 0.001);
        displayCircles(corners_next, Scalar(0, 150, 0));

        for (int i = status.size() - 1; i >= 0; i--) {
            if (status[i] == 1 && !err[i] == 0)
                line(corner_frame, corners_prev[i], corners_next[i], Scalar(0, 255, 0), 1);

            if (!pointInsideImage(corners_next[i])) {
                cout << "Punkt " << i << " uciek³!" << endl;
                corners_next.erase(corners_next.begin() + i);
                if (corners_next.size() < 10) needToInit = true;
            }
        }
        if (counter % timeDelay == 0)
            goodFeaturesToTrack(tmp, corners_next, maxCorners, newQuality, newDistance, Mat(), 3, 0, false, 0.04);
        counter++;
    }

    putTrackbarRealValuesOnFrame(newQuality, newDistance);

    if (counter % timeDelay == 0) {
        frame_prev = frame_next.clone();
        corners_prev = corners_next;
    }

    corners_next.clear();
}

void ValueChange(int, void*) {
    ValueChanger();
}

int main() {
    cap.open(0);
    //cap.open("C:/Users/Mariusz/Desktop/opencv_tmp/lab4/bike.avi");

    namedWindow("window", WINDOW_AUTOSIZE);
    namedWindow("motion", WINDOW_AUTOSIZE);

    createTrackbar("maxCorners", "motion", &maxCorners, maxCorners_max, ValueChange);
    createTrackbar("qualityLevel", "motion", &qualityLevel, qualityLevel_max, ValueChange);
    createTrackbar("minDistance", "motion", &minDistance, minDistance_max, ValueChange);

    cap >> frame_prev;

    while (1) {
        try {
            cap >> frame_next;
            if (frame_next.empty()) {
                cap.set(CAP_PROP_POS_FRAMES, 0);
                cap >> frame_next;
            }
            corner_frame = frame_next.clone();

            ValueChanger();

            imshow("window", frame_next);
            imshow("motion", corner_frame);
        } catch (Exception e) {
            cap.open(1);
        }

        if (waitKey(15) == 27) {
            cap.release();
            destroyAllWindows();
            return 0;
        }
    }
    return 0;
}