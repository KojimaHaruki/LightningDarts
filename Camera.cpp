#include "Camera.hpp"
#include "DxLib.h"
#include "Scene.hpp"
#include "Screen.hpp"

bool cCamera::init() {
    for (int index = 0; index < MAX_CAM_NUM; index++) {
        cv::VideoCapture capture(index);
        if (!capture.isOpened()) {
            continue;
        }
        cams.push_back(sCamera());
        cams.back().cap = capture;
        cams.back().idx = index;
    }
    if (cams.empty()) return false;
    for (sCamera& cam : cams) {
        cam.cap.read(cam.back);
        cam.base.GraphData = cam.back.data;
        cam.base.Width = cam.back.cols;
        cam.base.Height = cam.back.rows;
        cam.base.Pitch = cam.back.step;
        cam.base.MipMapCount = 0;
        CreateFullColorData(&cam.base.ColorData);
        cam.handle = CreateGraphFromBaseImage(&cam.base);
        cam.boardHandle = CreateGraphFromBaseImage(&cam.base);
        cam.arrowHandle = CreateGraphFromBaseImage(&cam.base);
        cam.scale = 0.4;
    }
    return true;
}

bool cCamera::reset() {
    if (cams.empty()) return false;
    for (sCamera& cam : cams) {
        if (!cam.cap.read(cam.back)) {
            return false;
        }
    }
    return true;
}

bool cCamera::draw() {
    if (cams.empty()) return false;
    int x = cams.front().scale * 0.5 * cams.front().frame.cols,
        y = cScreen::instance()->upperFrame().bottom() + 0.5 * cams.front().scale * cams.front().frame.rows;
    for (sCamera& cam : cams) {
        if (cam.frame.empty()) {
            return false;
        }
        DrawRotaGraph(x, y, cam.scale, 0.0, cam.handle, FALSE);
        x += cam.scale * cam.frame.cols;
        DrawRotaGraph(x, y, cam.scale, 0.0, cam.boardHandle, FALSE);
        x += cam.scale * cam.frame.cols;
        DrawRotaGraph(x, y, cam.scale, 0.0, cam.arrowHandle, FALSE);
        x = cam.scale * 0.5 * cam.frame.cols;
        y += cam.scale * cam.frame.rows;
    }
    return true;
}

bool cCamera::update() {
    for (sCamera& cam : cams) {
        if (!cam.cap.read(cam.frame) || cam.frame.empty()) {
            return false;
        }
        cam.base.GraphData = cam.frame.data;
        ReCreateGraphFromBaseImage(&cam.base, cam.handle);

        // detect board
        cv::Mat board;
        cv::Size ksize = cv::Size(5, 5);
        cv::GaussianBlur(cam.frame, board, ksize, 0);
        cv::cvtColor(board, board, cv::COLOR_BGR2HSV); // RGB = > HSV
        cv::inRange(board, cv::Scalar(70, 30, 40), cv::Scalar(130, 255, 255), board); // HSVÇÃílÇÃîÕàÕÇ…ÇÊÇÈÉ}ÉXÉNê∂ê¨
        cv::erode(board, board, cv::Mat(), cv::Point(-1, -1), 2);
        cv::dilate(board, board, cv::Mat(), cv::Point(-1, -1), 3);
        std::vector<std::vector<cv::Point>> boardConts;
        cv::findContours(board, boardConts, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE); // ó÷äsÇãÅÇﬂÇÈ
        int idx = 0, biggestIdx = 0;
        double maxArea = 0.0, area = 0.0;
        cv::Point minXPt, maxXPt;
        if (!boardConts.empty()) {
            for (std::vector<cv::Point>& contour : boardConts) {
                area = contourArea(contour); // ó÷äsê¸Ç©ÇÁóÃàÊÇãÅÇﬂÇÈ
                if (area > maxArea) {
                    maxArea = area;
                    biggestIdx = idx;
                }
                idx++;
            }
            minXPt = boardConts.at(biggestIdx).front();
            maxXPt = boardConts.at(biggestIdx).front();
            for (const cv::Point& p : boardConts.at(biggestIdx)) {
                if (p.x < minXPt.x) {
                    minXPt = p;
                }
                if (p.x > maxXPt.x) {
                    maxXPt = p;
                }
            }
        }
        board = cv::Mat::zeros(board.rows, board.cols, CV_8UC3);
        drawContours(board, boardConts, 0, cv::Scalar(255, 255, 255), -1);
        drawContours(board, boardConts, biggestIdx, cv::Scalar(0, 255, 255), -1);
        cv::line(board, minXPt, maxXPt, cv::Scalar(0, 0, 255), 2);
        cam.base.GraphData = board.data;
        ReCreateGraphFromBaseImage(&cam.base, cam.boardHandle);

        // detect arrow
        cv::Mat arrow;
        cv::absdiff(cam.frame, cam.back, arrow);
        cv::cvtColor(arrow, arrow, cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(arrow, arrow, ksize, 0);
        cv::threshold(arrow, arrow, 20, 255, cv::THRESH_BINARY);
        cv::dilate(arrow, arrow, cv::Mat(), cv::Point(-1, -1), 2);
        std::vector<std::vector<cv::Point>> arrowConts;
        cv::findContours(arrow, arrowConts, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        idx = 0; biggestIdx = 0;
        cv::Point minYPt, maxYPt;
        maxArea = 0.0;
        if (!arrowConts.empty()) {
            for (std::vector<cv::Point>& contour : arrowConts) {
                area = contourArea(contour); // ó÷äsê¸Ç©ÇÁóÃàÊÇãÅÇﬂÇÈ
                if (area > maxArea) {
                    maxArea = area;
                    biggestIdx = idx;
                }
                idx++;
            }
            minYPt = arrowConts.at(biggestIdx).front();
            maxYPt = arrowConts.at(biggestIdx).front();
            for (const cv::Point& p : arrowConts.at(biggestIdx)) {
                if (p.y < minYPt.y) {
                    minYPt = p;
                }
                if (p.y > maxYPt.y) {
                    maxYPt = p;
                }
            }
        }
        /*cv::Canny(arrow, arrow, 1, 5);
        cv::Point tip;
        cv::minMaxLoc(arrow, nullptr, nullptr, nullptr, &tip);*/
        cv::cvtColor(arrow, arrow, cv::COLOR_GRAY2BGR);
        //arrow = cv::Mat::zeros(arrow.rows, arrow.cols, CV_8UC3);
        /*cv::circle(arrow, tip, 3, cv::Scalar(0, 0, 255), 2);*/
        arrow = cv::Mat::zeros(arrow.rows, arrow.cols, CV_8UC3);
        drawContours(arrow, arrowConts, 0, cv::Scalar(255, 255, 255), -1);
        drawContours(arrow, arrowConts, biggestIdx, cv::Scalar(0, 255, 255), -1);
        cv::line(arrow, minYPt, maxYPt, cv::Scalar(0, 0, 255), 2);

        /*std::vector<cv::Vec4i> lines;
        cv::absdiff(cam.frame, cam.back, arrow);
        cv::cvtColor(arrow, arrow, cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(arrow, arrow, ksize, 0);
        cv::threshold(arrow, arrow, 10, 255, cv::THRESH_BINARY);
        cv::Canny(arrow, arrow, 1, 5);
        cv::HoughLinesP(arrow, lines, 1, CV_PI / 180.0, 80, 30, 10);
        cv::cvtColor(arrow, arrow, cv::COLOR_GRAY2BGR);
        idx = 0; int verticalIdx = 0;
        double maxD = 0.0, d = 0.0, dx = 0.0, dy = 0.0;
        for (cv::Vec4i& line : lines) {
            cv::line(arrow, 
                cv::Point(line[0], line[1]), cv::Point(line[2], line[3]), cv::Scalar(0, 0, 255), 2);
            dx = abs(line[2] - line[0]);
            if (!dx) {
                verticalIdx = idx;
                break;
            }
            dy = abs(line[3] - line[1]);
            d = dy / dx;
            if (d > maxD) {
                maxD = d;
                verticalIdx = idx;
            }
            idx++;
        }
        if (maxD > 1.0) {
            cv::line(arrow, cv::Point(lines.at(verticalIdx)[0], lines.at(verticalIdx)[1]),
                cv::Point(lines.at(verticalIdx)[2], lines.at(verticalIdx)[3]), cv::Scalar(255, 0, 0), 4);
        }*/
        cam.base.GraphData = arrow.data;
        ReCreateGraphFromBaseImage(&cam.base, cam.arrowHandle);
    }
    return true;
}

bool cCamera::fin() {
    if (cams.empty()) return false;
    for (sCamera& cam : cams) cam.cap.release();
    return true;
}