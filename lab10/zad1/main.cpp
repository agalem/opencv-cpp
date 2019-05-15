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

CascadeClassifier face_cascade("/Users/agalempaszek/Desktop/opencv-lab/lab10/zad1/haarcascade_frontalface_default.xml");
CascadeClassifier eyes_cascade("/Users/agalempaszek/Desktop/opencv-lab/lab10/zad1/haarcascade_eye.xml");

void showContours(Mat& outputImg, Mat frameGray, vector<Rect> faces, double doubleScaleFactor, int minNeigh, int minSiz );
void blurFaceArea(Mat& outputImg, vector<Rect> faces);
void hideEyes(Mat& outputImg,Mat frameGray, vector<Rect> faces, double doubleScaleFactor, int minNeigh, int minSiz );
void replaceFace(Mat& outputImg, vector<Rect> faces);
double convertToDouble(int input);

String choosingOptions = "Options available: \n1. Face contours (default)\n2. Face blur\n3. Eyes hiding\n4. Face replacement\n";

int chosenOption = 1;

int scaleFactor = 110;
int maxScaleFactor = 300;

int minNeighbours = 3;
int maxMinNeighbours = 6;

int minSize = 50;
int maxMinSize = 200;

int main() {

    VideoCapture cap(0);
    Mat frame, frameGray, faceRecognition;

    namedWindow("Original", WINDOW_AUTOSIZE);
    namedWindow("Face recognition", WINDOW_AUTOSIZE);
    namedWindow("Sliders", WINDOW_AUTOSIZE);

    createTrackbar("scaleFactor", "Sliders", &scaleFactor, maxScaleFactor);
    createTrackbar("minNeighbours", "Sliders", &minNeighbours, maxMinNeighbours);
    createTrackbar("minSize", "Sliders", &minSize, maxMinSize);

    if(!cap.isOpened()) {
        cout << "Cannot open a web cam";
        return -1;
    }

    while(true) {

        cap >> frame;
        frame.copyTo(faceRecognition);

        if(scaleFactor <= 100) {
            scaleFactor = 101;
        }
        double doubleScaleFactor = convertToDouble(scaleFactor);

        cvtColor(frame, frameGray, COLOR_BGR2GRAY);
        equalizeHist(frameGray, frameGray);

        vector<Rect> faces;
        face_cascade.detectMultiScale(frameGray, faces, doubleScaleFactor, minNeighbours, minSize);

        if(chosenOption == 1) {
            showContours(faceRecognition, frameGray, faces, doubleScaleFactor, minNeighbours, minSize);
        } else if (chosenOption == 2) {
            blurFaceArea(faceRecognition, faces);
        } else if(chosenOption == 3) {
            hideEyes(faceRecognition, frameGray, faces, doubleScaleFactor, minNeighbours, minSize);
        } else if (chosenOption == 4) {
            replaceFace(faceRecognition, faces);
        }

        imshow("Original", frame);
        imshow("Face recognition", faceRecognition);

        int key = waitKeyEx(1);

        if(key == 49) {
            chosenOption = 1;
            cout << "Face & eyes detection" << endl;
        } else if (key == 50) {
            chosenOption = 2;
            cout << "Blur face area" << endl;
        } else if(key == 51) {
            chosenOption = 3;
            cout << "Hide eyes" << endl;
        } else if(key == 52) {
            chosenOption = 4;
            cout << "Face replacement" << endl;
        } else if(key != -1 && key != ESC) {
            cout << "Invalid key pressed" << endl;
            cout << choosingOptions;
        } else if (key == ESC) {
            break;
        }
    }

    cap.release();
    frame.release();
    faceRecognition.release();
    destroyAllWindows();

    return 0;
}

void showContours(Mat& outputImg, Mat frameGray, vector<Rect> faces, double doubleScaleFactor, int minNeigh, int minSiz) {

    for(size_t i = 0; i < faces.size(); i++) {
        Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
        ellipse( outputImg, center, Size( faces[i].width/2, faces[i].height/2 ), 0, 0, 360, Scalar( 255, 0, 0 ), 4 );

        Mat faceROI = frameGray( faces[i] );
        vector<Rect> eyes;
        eyes_cascade.detectMultiScale( faceROI, eyes, doubleScaleFactor, minNeigh, minSiz );

        for(size_t j=-0; j<eyes.size(); j++) {
            Point eye_center( faces[i].x + eyes[j].x + eyes[j].width/2, faces[i].y + eyes[j].y + eyes[j].height/2 );
            int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
            circle( outputImg, eye_center, radius, Scalar( 0, 0, 255 ), 4 );
        }
    }

}

void blurFaceArea(Mat& outputImg, vector<Rect> faces) {

    for(size_t i = 0; i < faces.size(); i++) {
        GaussianBlur(outputImg(faces[i]), outputImg(faces[i]), Size(0,0), 10);
    }

}

void hideEyes(Mat& outputImg, Mat frameGray, vector<Rect> faces, double doubleScaleFactor, int minNeigh, int minSiz) {

    for(size_t i = 0; i < faces.size(); i++) {

        Mat faceROI = frameGray( faces[i] );
        vector<Rect> eyes;
        eyes_cascade.detectMultiScale( faceROI, eyes, doubleScaleFactor, minNeigh, minSiz );
        int posY;
        if(eyes.size() >= 2) {
            posY = faces[i].y + max(eyes[0].y, eyes[1].y) - 20;
            int rectHeight = max(eyes[0].height, eyes[1].height) + 50;
            Rect blackRect = Rect(faces[i].x, posY, faces[i].width, rectHeight);
            rectangle(outputImg, blackRect, Scalar(0,0,0), FILLED);
            cout << "Min 2 oczy" << endl;
        } else {
            posY = (int)((faces[i].height / 3)*2) - 15;
            int rectHeight = (int)(posY/2);
            Rect blackRect = Rect(faces[i].x, posY, faces[i].width, rectHeight);
            rectangle(outputImg, blackRect, Scalar(0,0,0), FILLED);
            cout << "Brak oczu" << endl;
        }
    }

}

void replaceFace(Mat& outputImg, vector<Rect> faces) {

    for(size_t i = 0; i < faces.size(); i++) {
            Rect chosenFace;
            if(!faces.empty()) {
                //select first found face area
                chosenFace = faces[0];
            }
            if(!chosenFace.empty() && faces.size() > 1) {

                Mat faceToReplace = outputImg(chosenFace);

                if(i >= 1 && !faceToReplace.empty()) {
                    resize(faceToReplace, faceToReplace, Size(faces[i].width, faces[i].height));
                    faceToReplace.copyTo(outputImg(faces[i]));
                }
            }
    }

}

double convertToDouble(int input) {
    return double(input)/100.00;
}