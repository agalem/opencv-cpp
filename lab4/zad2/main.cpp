#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace cv;
using namespace std;

int main() {
    VideoCapture cap;
    Mat frame;
    vector<Mat> frames;
    vector<cv::String> fileSystem;


    std::string srcFiles = "/Users/agalempaszek/Desktop/opencv-lab/lab4/zad2/files/*.jpg";


    glob(srcFiles, fileSystem, false);

    //tu error na windowsie
    for(int i=0; i<fileSystem.size(); i++) {
        frames.push_back(imread(fileSystem[i]));
    }

    Size frameSize(frames[0].cols, frames[0].rows);

    VideoWriter video("VidFromFiles.avi", -1, 30, frameSize, false);

    for(int i=0; i<frames.size(); i++) {
        video << frames[i];
        waitKey(10);
    }

    video.release();


    return 0;
}