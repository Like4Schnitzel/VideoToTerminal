#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <stdexcept>
#include <iostream>
#include "binaryUtils.hpp"

class VideoTranscoder 
{
    private:
        cv::Mat frame;
        cv::VideoCapture vidCap;
        std::string vidPath;
        uint16_t vidHeight;
        uint16_t vidWidth;
        uint32_t vidFrames;
        float vidFPS;
    public:
        VideoTranscoder(std::string);
        ~VideoTranscoder();
        cv::Mat getFrame();
        void transCodeFile();
};
