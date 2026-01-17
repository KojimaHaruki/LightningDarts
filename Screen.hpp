#pragma once
#include "Singleton.hpp"
#include "DxLib.h"
#include "Box.hpp"

class cScreen : public Singleton<cScreen> {
    cScreen() {}
    friend class Singleton<cScreen>;
public:
    // Functions
    void init() { 
        SetGraphMode(DEFAULT_WIDTH, DEFAULT_HEIGHT, 32); mColorDepth = 32; mFPS = 60;
        mBox.setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT); mBox.setUpperLeft(0, 0); 
    }
    void setSize(int width, int height, int colorDepth = 32, int FPS = 60) { 
        SetGraphMode(width, height, colorDepth, FPS); mColorDepth = colorDepth; mFPS = FPS;
        mBox.setSize(width, height); mBox.setUpperLeft(0, 0); 
    }
    cBox box() { return mBox; }
private:
    cBox mBox;
    int mColorDepth = 32, mFPS = 60;
    static constexpr int DEFAULT_WIDTH = 900;
    static constexpr int DEFAULT_HEIGHT = 500;
    static constexpr int DEFAULT_FPS = 60;
};