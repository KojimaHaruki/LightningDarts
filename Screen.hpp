#pragma once
#include "Singleton.hpp"
#include "Box.hpp"
#include "Control.hpp"

class cScreen : public Singleton<cScreen> {
    cScreen() {}
    friend class Singleton<cScreen>;
    cBox mBox, mUpperFrame, mLowerFrame;
    cImage mDarts, mThunder;
    int mColorDepth = 32, mFPS = 60;
    static constexpr int DEFAULT_WIDTH = 900;
    static constexpr int DEFAULT_HEIGHT = 500;
    static constexpr int DEFAULT_FPS = 60;
public:
    // Setters
    void setSize(int w, int h, int frameH = cControl::DEFAULT_ICON_HEIGHT, 
        int colorDepth = 32, int FPS = 60);
    void setFrameHeight(int h);

    // Getters
    cBox box() { return mBox; }
    cBox upperFrame() { return mUpperFrame; }
    cBox lowerFrame() { return mLowerFrame; }

    // Functions
    void init();
    void initSize();
    void loadImage();
	void reloadImage();
    void draw();
};