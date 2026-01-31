#pragma once
#include "Singleton.hpp"
#include <opencv2/opencv.hpp>
#include <vector>
#include "DxLib.h"

class cCamera : public Singleton<cCamera> {
    cCamera() {}
    friend class Singleton<cCamera>;
    static constexpr int MAX_CAM_NUM = 8;
    struct sCamera {
        cv::VideoCapture cap;
        int idx = 0;
        cv::Mat frame, back;
        int handle = -1, boardHandle = -1, arrowHandle = -1;
        BASEIMAGE base;
        double scale = 1.0;
    };
    std::vector<sCamera> cams;

public:
    // Functions
    bool init();
    bool reset();
    bool draw();
    bool update();
    bool fin();
};