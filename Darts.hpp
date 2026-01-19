#pragma once
#include "Singleton.hpp"
#include <string>
#include <complex>
#include "DxLib.h"
#include "Box.hpp"

class cDarts : public Singleton<cDarts> {
    cDarts() {}
    friend class Singleton<cDarts>;
public:
    struct sPoint {
        static constexpr int MISS = 0;
        static constexpr int MIN_CRICKET_POINT = 15;
        static constexpr int MAX_CRICKET_POINT = 20;
        static constexpr int OUTER_BULL = 21;
        static constexpr int INNER_BULL = 22;
        static constexpr int NUM = 23;
    };
    struct sPower {
        static constexpr int MISS = 0;
        static constexpr int SINGLE = 1;
        static constexpr int DOUBLE = 2;
        static constexpr int TRIPLE = 3;
        static constexpr int NUM = 4;
    };

    // Functions
    void loadScreen();
    void loadColor();
    void loadFont();
    void loadImage();
    void draw();
    bool updateByKeyboard();
    void updateByMouse();
    void update();
    void setCenter(float x, float y) { std::complex<float>center(x, y); mCenter = center; }
    void setCenter(std::complex<float> center) { mCenter = center; }
    bool setPointValidation(int point, bool isValid);
    bool isValidPoint(int point);
    int arrowImage() { return mArrowImage; }
    int point() { return mPoint; }
    std::string pointName();
    std::string pointName(int point);
    std::string pointName(int point, int power);
    int power() { return mPower; }
    int radialPos() { return mRadialPos; }
    int totalPoint() { return mTotalPoint; }
    bool isTouched() { return mIsTouched; }
    bool isThrowed() { return mIsThrowed; }
private:
    // Screen
    cBox screen, upperFrame, lowerFrame;

    // Color
    unsigned int white = 0U, black = 0U, gray = 0U,
        red = 0U, green = 0U, blue = 0U, magenta = 0U, cyan = 0U, yellow = 0U, tableColor = 0U,
        touchColor = 0U, pressColor = 0U, executeColor = 0U;

    // Font
    int Sfont = 0, SfontSize = 0, Mfont = 0, MfontSize = 0, XLfont = 0, XLfontSize = 0;

    struct sRadialPos {
        static constexpr int INNER_BULL = 0;
        static constexpr int OUTER_BULL = 1;
        static constexpr int INNER_SINGLE = 2;
        static constexpr int TRIPLE = 3;
        static constexpr int OUTER_SINGLE = 4;
        static constexpr int DOUBLE = 5;
        static constexpr int OUTSIDE = 6;
        static constexpr int NUM = 7;
        static constexpr float RADIUS[NUM] = { 8.0f, 22.0f, 108.77f, 126.77f, 178.85f, 196.85f, 226.0f };
        static constexpr int POWER[NUM] = { 2, 1, 1, 3, 1, 2, 0 };
    };
    bool mIsValidPoint[sPoint::NUM] = {};
    std::complex<float> mCenter;
    int mBoardImage[4][4] = {}, mArrowImage = 0;
    int mPoint = 0, mPower = 0, mRadialPos = 0, mTotalPoint = 0;
    bool mIsTouched = false, mIsThrowed = false;
    const std::string radialPosName[sRadialPos::NUM] = {
        "Inner Bull", "Outer Bull", "Single", "Triple", "Single", "Double", "Miss"
    };
    static constexpr int BOARD_POINT[21] = {
        11, 8, 16, 7, 19, 3, 17, 2, 15, 10, 6, 13, 4, 18, 1, 20, 5, 12, 9, 14, 11
    };
    static constexpr int POINT_KEY[sPoint::NUM] = { KEY_INPUT_SPACE,
        KEY_INPUT_1, KEY_INPUT_2, KEY_INPUT_3, KEY_INPUT_4, KEY_INPUT_5,
        KEY_INPUT_6, KEY_INPUT_7, KEY_INPUT_8, KEY_INPUT_9, KEY_INPUT_0,
        KEY_INPUT_F1, KEY_INPUT_F2, KEY_INPUT_F3, KEY_INPUT_F4, KEY_INPUT_F5,
        KEY_INPUT_F6, KEY_INPUT_F7, KEY_INPUT_F8, KEY_INPUT_F9, KEY_INPUT_F10,
        KEY_INPUT_F11, KEY_INPUT_F12 
    };
    const std::string POINT_NAME[sPoint::NUM] = { "Miss",
        " 1", " 2", " 3", " 4", " 5", " 6", " 7", " 8", " 9", "10",
        "11", "12", "13", "14", "15", "16", "17", "18", "19", "20",
        "Outer Bull", "Inner Bull"
    };
    const std::string POWER_NAME[sPower::NUM] = { "Miss", "Single", "Double", "Triple" };

    int SkillEffectMode = TRUE;
};