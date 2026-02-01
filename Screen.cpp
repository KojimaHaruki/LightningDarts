#include "Screen.hpp"
#include "DxLib.h"
#include "resource.h"

void cScreen::setSize(int w, int h, int frameH, int colorDepth, int FPS) {
    SetGraphMode(w, h, colorDepth, FPS); mColorDepth = colorDepth; mFPS = FPS;
    mBox.setS(w, h);
    mUpperFrame = mBox; mUpperFrame.setH(frameH);
    mLowerFrame = mUpperFrame; mLowerFrame.setB(h);
}

void cScreen::setFrameHeight(int height) {
    mUpperFrame.setH(height);
    mLowerFrame = mUpperFrame;
    mLowerFrame.setB(mBox.B());
}

void cScreen::init() {
	initSize();
    loadImage();
}

void cScreen::initSize() {
    setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    mDarts.box().setS(900, 600);
    mThunder.box().setUL(-50, -200);
    mThunder.box().setS(900, 945);
}

void cScreen::loadImage() {
    mDarts.load(IDB_JPG1, "JPG");
    mThunder.load(IDB_PNG91, "PNG");
}

void cScreen::reloadImage() {
    mDarts.reload();
    mThunder.reload();
}

void cScreen::draw() {
    mThunder.draw();
    mDarts.draw();
}