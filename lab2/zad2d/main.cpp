#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <iostream>
#include <thread>
#include <chrono>
#include <unistd.h>

#define ESC 27

using namespace std;
using namespace cv;

int main() {
    int delayTime;
    int frames;
    int framesCounter = 1;
    double timestamp = 0;
    Mat image;

    cout << "Podaj liczbę klatek:" << endl;
    cin >> frames;

    cout << "Podaj opóźnienie (w ms):" << endl;
    cin >> delayTime;

    VideoCapture cap(0);

    if (!cap.isOpened()) {
        cerr << "Błąd w otworzeniu pliku" << endl;
        return -1;
    }

    double fps = cap.get(CAP_PROP_FPS);
    int frame_count = 0;

    while (true) {
        cap >> image;

        frame_count++;
        timestamp = (frame_count/fps) * 1000;


        if (timestamp >= delayTime) {
            if (framesCounter < frames && !image.empty()) {
                string name = "frame" + to_string(framesCounter) + ".jpg";
                imwrite("/Users/agalempaszek/Desktop/opencv-lab/lab2/zad2d/frames/" + name, image);
                framesCounter++;
            } else {
                break;
            }
        }

        imshow("window", image);

        if (waitKey(30) == ESC) break;
    }


    cap.release();
    destroyAllWindows();

    return 0;
}