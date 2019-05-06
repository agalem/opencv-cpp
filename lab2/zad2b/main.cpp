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
    int delay;
    int frames;
    int framesCounter = 1;
    double timestamp = 0;
    Mat image;

    cout << "Podaj liczbę klatek:" << endl;
    cin >> frames;

    cout << "Podaj opóźnienie (w ms):" << endl;
    cin >> delay;

    VideoCapture cap("/Users/agalempaszek/Desktop/opencv-lab/snake_synth.avi");

    if (!cap.isOpened()) {
        cerr << "Błąd w otworzeniu pliku" << endl;
        return -1;
    }

    while (true) {
        cap >> image;

        timestamp = cap.get(CAP_PROP_POS_MSEC);

        if (timestamp >= delay) {
            if (framesCounter < frames && !image.empty()) {
                string name = "frame" + to_string(framesCounter) + ".jpg";
                imwrite("/Users/agalempaszek/Desktop/opencv-lab/lab2/zad2b/frames/" + name, image);
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