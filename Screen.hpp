#pragma once
#include "Singleton.hpp"
#include "DxLib.h"
#include "Box.hpp"
#include "Scene.hpp"

class cScreen : public Singleton<cScreen> {
    cScreen() {}
    friend class Singleton<cScreen>;
    cBox mBox, mUpperFrame, mLowerFrame;
    int mColorDepth = 32, mFPS = 60;
    static constexpr int DEFAULT_WIDTH = 900;
    static constexpr int DEFAULT_HEIGHT = 500;
    static constexpr int DEFAULT_FPS = 60;
public:
    // Functions
    void init() { 
        SetGraphMode(DEFAULT_WIDTH, DEFAULT_HEIGHT, 32); mColorDepth = 32; mFPS = 60;
        mBox.setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT); mBox.setUpperLeft(0, 0); 
        mUpperFrame.setSize(DEFAULT_WIDTH, cScene::DEFAULT_ICON_HEIGHT);
        mUpperFrame.setUpperLeft(mBox.upperLeft());
        mLowerFrame.setSize(DEFAULT_WIDTH, cScene::DEFAULT_ICON_HEIGHT);
        mLowerFrame.setLowerLeft(mBox.lowerLeft());
    }
    void setSize(int width, int height, int colorDepth = 32, int FPS = 60) { 
        SetGraphMode(width, height, colorDepth, FPS); mColorDepth = colorDepth; mFPS = FPS;
        mBox.setSize(width, height); mBox.setUpperLeft(0, 0); 
        mUpperFrame.setUpperLeft(mBox.upperLeft());
        mLowerFrame.setLowerLeft(mBox.lowerLeft());
    }
    void setFrameHeight(int height) {
        mUpperFrame.setSize(mBox.width(), height);
        mUpperFrame.setUpperLeft(mBox.upperLeft());
        mLowerFrame.setSize(mBox.width(), height);
        mLowerFrame.setLowerLeft(mBox.lowerLeft());
    }
    cBox box() { return mBox; }
    cBox upperFrame() { return mUpperFrame; }
    cBox lowerFrame() { return mLowerFrame; }
};