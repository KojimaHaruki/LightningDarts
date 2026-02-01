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
        cv::Mat frame, drawFrame, back, latestBack;
        int handle = -1, boardHandle = -1, arrowHandle = -1;
        BASEIMAGE base = {};
        double scale = 1.0;
        float ratio = 0.0;
    };
    std::vector<sCamera> cams;
    static constexpr int RESET_TIME = 120;
    int resetTimeCount = 0;

public:
    // Functions
    bool init();
    bool reset();
    bool draw();
    bool projectTipToBoard(const cv::Point2f& A, const cv::Point2f& B,
        const cv::Point2f& P, cv::Point2f& H, float& ratio);
    bool update();
    bool fin();
};