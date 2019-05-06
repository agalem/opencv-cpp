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
    int frames;
    int framesCounter = 0;
    Mat image;

    cout << "Podaj liczbę klatek:" << endl;
    cin >> frames;

    VideoCapture cap(0);

    if (!cap.isOpened()) {
        cout << "Błąd w otworzeniu pliku" << endl;
        return -1;
    }

    while (true) {
        cap >> image;

        if (framesCounter < frames && !image.empty()) {
            string name = "frame" + to_string(framesCounter) + ".jpg";
            imwrite("/Users/agalempaszek/Desktop/opencvNEW/lab2/zad2c/frames/" + name, image);
            framesCounter++;
        } else {
            break;
        }

        imshow("window", image);

        if (waitKey(30) == ESC) break;
    }


    cap.release();
    destroyAllWindows();

    return 0;
}
