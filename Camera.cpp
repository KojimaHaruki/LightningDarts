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
        cam.latestBack = cam.back.clone();
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
        y = cScreen::inst()->upperFrame().B() + 0.5 * cams.front().scale * cams.front().frame.rows;
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

bool cCamera::projectTipToBoard(const cv::Point2f& A, const cv::Point2f& B,
    const cv::Point2f& P, cv::Point2f& H, float& ratio) {
    cv::Point2f AB = B - A;
    float len2 = AB.dot(AB);
    if (len2 < 1e-6f) return false;

    float t = (P - A).dot(AB) / len2;

    H = A + t * AB;

    ratio = t - 0.5f;
    
    return true;
}

bool cCamera::update() {
    if (cams.empty()) return false;
    resetTimeCount++;
    if (resetTimeCount >= RESET_TIME) {
        resetTimeCount = 0;
        for (sCamera& cam : cams) {
            if (!cam.latestBack.empty()) {
                cam.back = cam.latestBack;
            }
            cam.cap.read(cam.latestBack);
        }
    }
    for (sCamera& cam : cams) {
        if (!cam.cap.read(cam.frame) || cam.frame.empty()) {
            return false;
        }
        cam.drawFrame = cam.frame.clone();

        cv::Point2f boardMinXPt, boardMaxXPt, arrowMinYPt, arrowMaxYPt;
        
        // detect board
        cv::Mat board, boardMask;
        cv::Size ksize = cv::Size(5, 5);
        cv::GaussianBlur(cam.frame, board, ksize, 0);
        cv::cvtColor(board, board, cv::COLOR_BGR2HSV); // RGB = > HSV
        cv::inRange(board, cv::Scalar(70, 30, 40), cv::Scalar(130, 255, 255), board); // HSVの値の範囲によるマスク生成
        cv::erode(board, board, cv::Mat(), cv::Point(-1, -1), 2);
        cv::dilate(board, board, cv::Mat(), cv::Point(-1, -1), 3);
        std::vector<std::vector<cv::Point>> boardConts;
        cv::findContours(board, boardConts, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE); // 輪郭を求める
        int idx = -1, bestIdx = 0;
        double maxArea = 0.0, area = 0.0;
        if (!boardConts.empty()) {
            for (std::vector<cv::Point>& contour : boardConts) {
                idx++;
                area = contourArea(contour); // 輪郭線から領域を求める
                if (area > maxArea) {
                    maxArea = area;
                    bestIdx = idx;
                }
            }
            boardMinXPt = boardConts.at(bestIdx).front();
            boardMaxXPt = boardConts.at(bestIdx).front();
            for (const cv::Point& p : boardConts.at(bestIdx)) {
                if (p.x < boardMinXPt.x) {
                    boardMinXPt = p;
                }
                if (p.x > boardMaxXPt.x) {
                    boardMaxXPt = p;
                }
            }
        }
        boardMask = cv::Mat::zeros(board.rows, board.cols, CV_8UC1);
        drawContours(board, boardConts, bestIdx, cv::Scalar(255), -1);
        board = cv::Mat::zeros(board.rows, board.cols, CV_8UC3);
        for (int idx = 0; idx < boardConts.size(); idx++) {
            drawContours(board, boardConts, idx, cv::Scalar(255, 255, 255), -1);
        }
        drawContours(board, boardConts, bestIdx, cv::Scalar(0, 255, 255), -1);
        cv::line(board, boardMinXPt, boardMaxXPt, cv::Scalar(0, 0, 255), 2);
        cv::circle(board, 0.5f * (boardMinXPt + boardMaxXPt), 5, cv::Scalar(0, 0, 255), -1);
        cv::line(cam.drawFrame, boardMinXPt, boardMaxXPt, cv::Scalar(255, 0, 0), 2);
        cv::circle(cam.drawFrame, 0.5f * (boardMinXPt + boardMaxXPt), 5, cv::Scalar(255, 0, 0), -1);
        
        cam.base.GraphData = board.data;
        ReCreateGraphFromBaseImage(&cam.base, cam.boardHandle);

        // calculate background motion by board mask
        cv::Mat grayFrame, grayBack;
        cv::cvtColor(cam.frame, grayFrame, cv::COLOR_BGR2GRAY);
        cv::cvtColor(cam.back, grayBack, cv::COLOR_BGR2GRAY);

        cv::Ptr<cv::ORB> orb = cv::ORB::create(500);

        std::vector<cv::KeyPoint> kp1, kp2;
        cv::Mat desc1, desc2;

        orb->detectAndCompute(grayBack, boardMask, kp1, desc1);
        orb->detectAndCompute(grayFrame, boardMask, kp2, desc2);

        cv::Mat backAligned;
        if (desc1.empty() || desc2.empty() || kp1.size() < 10 || kp2.size() < 10) {
            backAligned = cam.back.clone();
        }
        else {
            cv::BFMatcher matcher(cv::NORM_HAMMING);
            std::vector<cv::DMatch> matches;
            matcher.match(desc1, desc2, matches);

            if (matches.size() < 8) {
                backAligned = cam.back.clone();
            }
            else {
                std::vector<cv::Point2f> pts1, pts2;
                for (cv::DMatch& m : matches) {
                    pts1.push_back(kp1[m.queryIdx].pt);
                    pts2.push_back(kp2[m.trainIdx].pt);
                }

                cv::Mat M = cv::estimateAffinePartial2D(pts1, pts2);

                if (M.empty()) {
                    backAligned = cam.back.clone();
                }
                else {
                    cv::warpAffine(cam.back, backAligned, M, cam.frame.size());
                }
            }
        }

        // detect arrow
        cv::Mat arrow;
        cv::absdiff(cam.frame, backAligned, arrow);
        cv::cvtColor(arrow, arrow, cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(arrow, arrow, ksize, 0);
        cv::threshold(arrow, arrow, 20, 255, cv::THRESH_BINARY);
        cv::bitwise_and(arrow, ~boardMask, arrow);
        cv::dilate(arrow, arrow, cv::Mat(), cv::Point(-1, -1), 2);
        std::vector<std::vector<cv::Point>> arrowConts;
        cv::findContours(arrow, arrowConts, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        idx = -1; bestIdx = -1;
        double score = 0.0, bestScore = 0.0;
        for (std::vector<cv::Point>& contour : arrowConts) {
            idx++;
            area = cv::contourArea(contour);
            // delete noise by area
            if (area < 600 || area > 5000) { 
                continue; 
            } 

            cv::Rect rect = cv::boundingRect(contour);
            double width = rect.width;
            double height = rect.height;

            if (width <= 0 || height <= 0) continue;

            double aspect = height / width;
            //if (aspect < 2.5) continue; // 細長さチェック

            //double solidity = area / (width * height);
            //if (solidity < 0.2) continue; // スカスカな形を除外

            // スコア（縦に長くて面積があるほど高評価）
            score = area * aspect;

            if (score > bestScore) {
                bestScore = score;
                bestIdx = idx;
            }
        }
        arrow = cv::Mat::zeros(arrow.rows, arrow.cols, CV_8UC3);
        for (int idx = 0; idx < arrowConts.size(); idx++) {
            drawContours(arrow, arrowConts, idx, cv::Scalar(255, 255, 255), -1);
        }

        cv::Point2f tip;
        
        if (bestIdx >= 0) {
            drawContours(arrow, arrowConts, bestIdx, cv::Scalar(0, 255, 255), -1);

            // 矢の上下端
            arrowMinYPt = arrowConts[bestIdx].front();
            arrowMaxYPt = arrowConts[bestIdx].front();
            for (const cv::Point& p : arrowConts[bestIdx]) {
                if (p.y < arrowMinYPt.y) arrowMinYPt = p;
                if (p.y > arrowMaxYPt.y) arrowMaxYPt = p;
            }
            cv::line(arrow, arrowMinYPt, arrowMaxYPt, cv::Scalar(0, 0, 255), 2);
            cv::line(cam.drawFrame, arrowMinYPt, arrowMaxYPt, cv::Scalar(0, 0, 255), 2);

            if (projectTipToBoard(boardMinXPt, boardMaxXPt, arrowMaxYPt, tip, cam.ratio)) {
                cv::circle(arrow, tip, 5, cv::Scalar(0, 0, 255), -1);
                cv::circle(cam.drawFrame, tip, 5, cv::Scalar(0, 0, 255), -1);
            }
        }
        cam.base.GraphData = arrow.data;
        ReCreateGraphFromBaseImage(&cam.base, cam.arrowHandle);

        cv::putText(cam.drawFrame, std::to_string(cam.ratio), cv::Point2f(0.0f, 30.0f),
            cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 0, 0), 2);
        cam.base.GraphData = cam.drawFrame.data;
        ReCreateGraphFromBaseImage(&cam.base, cam.handle);
    }
    return true;
}

bool cCamera::fin() {
    if (cams.empty()) return false;
    for (sCamera& cam : cams) cam.cap.release();
    return true;
}