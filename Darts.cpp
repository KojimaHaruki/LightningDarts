#define _USE_MATH_DEFINES
#include "Darts.hpp"
#include <numbers>
#include "resource.h"
#include "Mouse.hpp"
#include "Keyboard.hpp"
#include "Color.hpp"
#include "Font.hpp"

void cDarts::load() {
    for (int pos = 0; pos < 4; pos++) {
        for (int color = 0; color < 4; color++) {
            mBoardImage[pos][color] = LoadGraphToResource(
                MAKEINTRESOURCE(IDB_PNG94 + 4 * pos + color), "PNG");
        }
    }
    mArrowImage = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG110), "PNG");
}

void cDarts::draw() {
    unsigned int white = cColor::instance()->white(), black = cColor::instance()->black(),
        gray = cColor::instance()->gray(), red = cColor::instance()->red(),
        touchColor = cColor::instance()->touchColor(), pressColor = cColor::instance()->pressColor();
    int MFont = cFont::instance()->MFont(), MFontSize = cFont::instance()->MfontSize();
    
    // draw base board
    DrawCircleAA(real(mCenter), imag(mCenter), sRadialPos::Radius[sRadialPos::OUTSIDE], 100, black);
    DrawCircleAA(real(mCenter), imag(mCenter), sRadialPos::Radius[sRadialPos::DOUBLE], 100, gray);

    // draw point part
	float phi = -M_PI, r = sRadialPos::Radius[sRadialPos::OUTSIDE] - MFontSize + 2.0f,
        x = -11.0f, y = -10.0f;
    for (int i = 0; i < 20; i++, phi += 0.1 * M_PI) {
        if (mIsValidPoint[cDarts::BOARD_POINT[i]]) {
            DrawStringToHandle(real(mCenter) + r * cos(phi) + x, imag(mCenter) - r * sin(phi) + y,
                pointName[cDarts::BOARD_POINT[i]].c_str(), white, MFont);
            for (int posNo = sRadialPos::DOUBLE, state = 0; posNo > sRadialPos::OUTER_BULL; posNo--) {
                if (mPoint == cDarts::BOARD_POINT[i] && mRadialPos == posNo) {
                    if (mIsTouched) {
                        state = 2;
                    }
                    else if (mIsThrowed) {
						state = 3;
                    }
                    DrawStringToHandle(0, 475 - MFontSize - 5,
                        (std::to_string(mPoint) + radialPosName[mRadialPos]).c_str(), white, MFont);
                }
                else {
                    state = i % 2;
                }
                DrawCircleGauge(real(mCenter), imag(mCenter),
                    77.5 - 5.0 * i, mBoardImage[sRadialPos::DOUBLE - posNo][state], 72.5 - 5.0 * i);
            }
            continue;
        }
        DrawStringToHandle(
            real(mCenter) + r * cos(phi) + x, imag(mCenter) - r * sin(phi) + y,
            pointName[cDarts::BOARD_POINT[i]].c_str(), gray, MFont);
    }

	// draw outer bull
    unsigned int color = red;
	if (!mIsValidPoint[sPoint::OUTER_BULL]) color = gray;
    else if (mRadialPos == sRadialPos::OUTER_BULL) {
        if (mIsThrowed) {
			color = pressColor;
        }
        else if (mIsTouched) {
			color = touchColor;
        }
        DrawStringToHandle(0, 475 - MFontSize - 5, pointName[sPoint::OUTER_BULL].c_str(), white, MFont);
    }
    DrawCircleAA(real(mCenter), imag(mCenter), sRadialPos::Radius[sRadialPos::OUTER_BULL], 100, color);

	// draw inner bull
	color = black;
	if (!mIsValidPoint[sPoint::INNER_BULL]) color = gray;
    else if (mRadialPos == sRadialPos::INNER_BULL) {
        if (mIsThrowed) {
            color = pressColor;
        }
        else if (mIsTouched) {
            color = touchColor;
        }
        DrawStringToHandle(0, 475 - MFontSize - 5, pointName[sPoint::INNER_BULL].c_str(), white, MFont);
    }
    DrawCircleAA(real(mCenter), imag(mCenter), sRadialPos::Radius[sRadialPos::INNER_BULL], 100, color);

	// draw circumferencial frame
    for (int posNo = 0; posNo < sRadialPos::NUM; posNo++) 
        DrawCircleAA(real(mCenter), imag(mCenter), sRadialPos::Radius[posNo], 100, black, FALSE, 2);
    
    // draw radial frame
    phi = 0.05 * M_PI;
	float r1 = sRadialPos::Radius[sRadialPos::OUTER_BULL], r2 = sRadialPos::Radius[sRadialPos::DOUBLE];
    for (int i = 0; i < 20; i++, phi += 0.1 * M_PI) 
        DrawLineAA(real(mCenter) + r1 * cos(phi), imag(mCenter) + r1 * sin(phi),
            real(mCenter) + r2 * cos(phi), imag(mCenter) + r2 * sin(phi), black, 2);
}

bool cDarts::updateByKeyboard() {
    // check inner bull
    if (Keyboard::instance()->getPressState(cDarts::POINT_KEY[sPoint::INNER_BULL]) == Key::RELEASEDtoPRESSED) {
		mRadialPos = sRadialPos::INNER_BULL; mPoint = 25; mPower = 2; mTotalPoint = 50; mIsThrowed = true;
        return true;
    }

    // check outer bull
    if (Keyboard::instance()->getPressState(cDarts::POINT_KEY[sPoint::OUTER_BULL]) == Key::RELEASEDtoPRESSED) {
        mRadialPos = sRadialPos::OUTER_BULL; mPoint = 25; mPower = 1; mIsThrowed = true;
        switch (mGameNo) {
        case sGame::STANDARD_CRICKET: case sGame::RANDAM_CRICKET: case sGame::HIDDEN_CRICKET:
        case sGame::SELECT_A_CRICKET: case sGame::ALL_NUMBER_CRICKET:
            mTotalPoint = 25;
            return true;
        default:
            mTotalPoint = 50;
            return true;
        }
    }

    // check miss
    if (Keyboard::instance()->getPressState(cDarts::POINT_KEY[sPoint::MISS]) == Key::RELEASEDtoPRESSED) {
        mIsThrowed = true; return true;
    }
    
    // check points 1 to 20
    for (int point = 1; point <= 20; point++) {
        if (Keyboard::instance()->getPressState(cDarts::POINT_KEY[point]) == Key::RELEASEDtoPRESSED) {
            if (Keyboard::instance()->getPressState(KEY_INPUT_D) == Key::PRESSED) {
                mRadialPos = sRadialPos::DOUBLE; mPower = 2;
            }
            else if (Keyboard::instance()->getPressState(KEY_INPUT_T) == Key::PRESSED) {
                mRadialPos = sRadialPos::TRIPLE; mPower = 3;
            }
            else if (Keyboard::instance()->getPressState(KEY_INPUT_O) == Key::PRESSED) {
                mRadialPos = sRadialPos::OUTER_SINGLE; mPower = 1;
            }
            else {
                mRadialPos = sRadialPos::INNER_SINGLE; mPower = 1;
            }
            mPoint = point; mTotalPoint = mPower * mPoint; mIsThrowed = true; return true;
        }
    }
	return false;
}

void cDarts::updateByMouse() {
    std::complex<float> cursor(
        Mouse::instance()->x() - real(mCenter), imag(mCenter) - Mouse::instance()->y());
    float r = abs(cursor), theta = arg(cursor);

	// check radial position
    for (int radialPosNo = sRadialPos::INNER_BULL; radialPosNo < sRadialPos::OUTSIDE; radialPosNo++) {
        if (r < sRadialPos::Radius[radialPosNo]) {
            mRadialPos = radialPosNo; mPower = sRadialPos::Power[radialPosNo];
            if (Mouse::instance()->getClickState() == Key::PRESSEDtoRELEASED) {
                mIsThrowed = true;
                break;
            }
            mIsTouched = true;
            break;
        }
    }

    // check point
    float phi = -M_PI + 0.05 * M_PI;
    switch (mRadialPos) {
    case sRadialPos::INNER_BULL:
        mPoint = 25; mTotalPoint = 50;
        return;
    case sRadialPos::OUTER_BULL:
        mPoint = 25;
        switch (mGameNo) {
        case sGame::STANDARD_CRICKET: case sGame::RANDAM_CRICKET: case sGame::HIDDEN_CRICKET:
        case sGame::SELECT_A_CRICKET: case sGame::ALL_NUMBER_CRICKET:
            mTotalPoint = 25;
            break;
        default:
            mTotalPoint = 50;
            break;
        }
        return;
    default:
        for (int i = 0; i < 21; i++, phi += 0.1 * M_PI) {
            if (theta < phi) {
                mPoint = cDarts::BOARD_POINT[i];
                mTotalPoint = mPower * mPoint;
                break;
            }
        }
        return;
    }
}

void cDarts::update() {
    mTimer.update();

    // initialize darts
    mRadialPos = sRadialPos::OUTSIDE; mPoint = 0; mPower = 0; mTotalPoint = 0;
    mIsTouched = false; mIsThrowed = false;
	if (updateByKeyboard()) return;
	updateByMouse();
}

bool cDarts::setGameNo(int game) {
    if (game < sGame::DEFAULT || game > sGame::NUM) return false;
    mGameNo = game;
    return true;
}

bool cDarts::setPointValidation(int point, bool isValid) {
    if (point == 25) {
        mIsValidPoint[sPoint::OUTER_BULL] = isValid; mIsValidPoint[sPoint::INNER_BULL] = isValid;
        return true;
    }
    if (point < 0 || point >= sPoint::NUM) return false;
    mIsValidPoint[point] = isValid; return true;
}

bool cDarts::isValidPoint(int point) {
    if (point == 25) return mIsValidPoint[sPoint::OUTER_BULL] && mIsValidPoint[sPoint::INNER_BULL];
    if (point < 0 || point >= sPoint::NUM) return false;
    return mIsValidPoint[point];
}