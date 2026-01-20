#define _USE_MATH_DEFINES
#include "Darts.hpp"
#include <numbers>
#include "resource.h"
#include "Screen.hpp"
#include "Color.hpp"
#include "Font.hpp"
#include "Mouse.hpp"
#include "Keyboard.hpp"
#include "Game.hpp"
#include "Control.hpp"
#include "Timer.hpp"

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

std::string cDarts::pointName(int point) {
    if (point >= 0 && point < sPoint::NUM) return POINT_NAME[point];
    if (point == BULL_POINT) return "Bull";
    return "None";
}

std::string cDarts::pointName() {
    return pointName(mPoint, mPower);
}

std::string cDarts::pointName(int point, int power) {
    if (point > 0 && point < sPoint::NUM && power > 0 && power < sPower::NUM)
        return POINT_NAME[point] + POWER_NAME[power];
    if (!point || !power) return "Miss";
    if (point == BULL_POINT) {
        if (power == 1) {
            return POINT_NAME[sPoint::OUTER_BULL];
        }
        return POINT_NAME[sPoint::INNER_BULL];
    }
	return "None";
}

void cDarts::loadScreen() {
    screen = cScreen::instance()->box();
    upperFrame = cScreen::instance()->upperFrame();
    lowerFrame = cScreen::instance()->lowerFrame();
}

void cDarts::loadColor() {
    white = cColor::instance()->white(); black = cColor::instance()->black();
    gray = cColor::instance()->gray(); red = cColor::instance()->red();
    green = cColor::instance()->green(); blue = cColor::instance()->blue();
    magenta = cColor::instance()->magenta(); cyan = cColor::instance()->cyan();
    yellow = cColor::instance()->yellow(); tableColor = cColor::instance()->tableColor();
    touchColor = cColor::instance()->touchColor(); pressColor = cColor::instance()->pressColor();
    executeColor = cColor::instance()->executeColor();
}

void cDarts::loadFont() {
    Sfont = cFont::instance()->Sfont(); SfontSize = cFont::instance()->SfontSize();
    Mfont = cFont::instance()->MFont(); MfontSize = cFont::instance()->MfontSize();
    XLfont = cFont::instance()->XLfont(); XLfontSize = cFont::instance()->XLfontSize();
}

void cDarts::loadImage() {
    for (int pos = 0; pos < 4; pos++) {
        for (int color = 0; color < 4; color++) {
            mBoardImage[pos][color] = LoadGraphToResource(
                MAKEINTRESOURCE(IDB_PNG94 + 4 * pos + color), "PNG");
        }
    }
    mArrowImage = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG110), "PNG");
}

void cDarts::draw() {
    // draw icon
    if (cTimer::instance()->isPaused()) cControl::instance()->icon(cControl::PAUSE).draw();
    else cControl::instance()->icon(cControl::RESUME).draw();
    cControl::instance()->icon(cControl::GAME_SELECT).draw();
    cControl::instance()->icon(cControl::PLAYER_SELECT).draw();
    cControl::instance()->icon(cControl::SKILL).draw();
    cControl::instance()->icon(cControl::SKIP).draw();

    // draw time
    cTimer::instance()->drawLapseTime(cScreen::instance()->box().left(),
        upperFrame.bottom() + 10, white, Sfont, cTimer::Mode::HMSmS);

    // draw game name
    DrawStringToHandle(cControl::instance()->icon(cControl::MUTE).box().right() + 5,
        upperFrame.center().y() - MfontSize / 2,
        cGame::instance()->modeName().c_str(), white, Mfont);
    
    // draw base board
    DrawCircleAA(real(mCenter), imag(mCenter), sRadialPos::RADIUS[sRadialPos::OUTSIDE], 100, black);
    DrawCircleAA(real(mCenter), imag(mCenter), sRadialPos::RADIUS[sRadialPos::DOUBLE], 100, gray);

    DrawStringToHandle(0, cScreen::instance()->lowerFrame().top() - MfontSize - 5, 
        pointName().c_str(), white, Mfont);
    // draw point part
	float phi = -M_PI, r = sRadialPos::RADIUS[sRadialPos::OUTSIDE] - MfontSize + 2.0f,
        x = -11.0f, y = -10.0f;
    for (int i = 0; i < 20; i++, phi += 0.1 * M_PI) {
        if (mIsValidPoint[cDarts::BOARD_POINT[i]]) {
            DrawStringToHandle(real(mCenter) + r * cos(phi) + x, imag(mCenter) - r * sin(phi) + y,
                POINT_NAME[cDarts::BOARD_POINT[i]].c_str(), white, Mfont);
            for (int posNo = sRadialPos::DOUBLE, state = 0; posNo > sRadialPos::OUTER_BULL; posNo--) {
                if (mPoint == cDarts::BOARD_POINT[i] && mRadialPos == posNo) {
                    if (mIsTouched) {
                        state = 2;
                    }
                    else if (mIsThrowed) {
						state = 3;
                    }
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
            POINT_NAME[cDarts::BOARD_POINT[i]].c_str(), gray, Mfont);
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
    }
    DrawCircleAA(real(mCenter), imag(mCenter), sRadialPos::RADIUS[sRadialPos::OUTER_BULL], 100, color);

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
    }
    DrawCircleAA(real(mCenter), imag(mCenter), sRadialPos::RADIUS[sRadialPos::INNER_BULL], 100, color);

	// draw circumferencial frame
    for (int posNo = 0; posNo < sRadialPos::NUM; posNo++) 
        DrawCircleAA(real(mCenter), imag(mCenter), sRadialPos::RADIUS[posNo], 100, black, FALSE, 2);
    
    // draw radial frame
    phi = 0.05 * M_PI;
	float r1 = sRadialPos::RADIUS[sRadialPos::OUTER_BULL], r2 = sRadialPos::RADIUS[sRadialPos::DOUBLE];
    for (int i = 0; i < 20; i++, phi += 0.1 * M_PI) 
        DrawLineAA(real(mCenter) + r1 * cos(phi), imag(mCenter) + r1 * sin(phi),
            real(mCenter) + r2 * cos(phi), imag(mCenter) + r2 * sin(phi), black, 2);
}

bool cDarts::updateByKeyboard() {
    // check inner bull
    if (cKeyboard::instance()->pressKeyState(cDarts::POINT_KEY[sPoint::INNER_BULL]) == sKey::RELEASEDtoPRESSED) {
		mRadialPos = sRadialPos::INNER_BULL; mPoint = BULL_POINT; mPower = 2; mTotalPoint = 50; 
        mIsThrowed = true; return true;
    }

    // check outer bull
    if (cKeyboard::instance()->pressKeyState(cDarts::POINT_KEY[sPoint::OUTER_BULL]) == sKey::RELEASEDtoPRESSED) {
        mRadialPos = sRadialPos::OUTER_BULL; mPoint = BULL_POINT; mPower = 1; mIsThrowed = true;
        if (cGame::instance()->category() == cGame::sCategory::CRICKET) {
            mTotalPoint = BULL_POINT; return true;
        }
        mTotalPoint = 2 * BULL_POINT; return true;
    }

    // check miss
    if (cKeyboard::instance()->pressKeyState(cDarts::POINT_KEY[sPoint::MISS]) == sKey::RELEASEDtoPRESSED) {
        mIsThrowed = true; return true;
    }
    
    // check points 1 to 20
    for (int point = 1; point <= 20; point++) {
        if (cKeyboard::instance()->pressKeyState(cDarts::POINT_KEY[point]) == sKey::RELEASEDtoPRESSED) {
            if (cKeyboard::instance()->pressKeyState(KEY_INPUT_D) == sKey::PRESSED) {
                mRadialPos = sRadialPos::DOUBLE; mPower = 2;
            }
            else if (cKeyboard::instance()->pressKeyState(KEY_INPUT_T) == sKey::PRESSED) {
                mRadialPos = sRadialPos::TRIPLE; mPower = 3;
            }
            else if (cKeyboard::instance()->pressKeyState(KEY_INPUT_O) == sKey::PRESSED) {
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
        cMouse::instance()->x() - real(mCenter), imag(mCenter) - cMouse::instance()->y());
    float r = abs(cursor), theta = arg(cursor);

	// check radial position
    for (int radialPosNo = sRadialPos::INNER_BULL; radialPosNo < sRadialPos::NUM; radialPosNo++) {
        if (r < sRadialPos::RADIUS[radialPosNo]) {
            mRadialPos = radialPosNo; mPower = sRadialPos::POWER[radialPosNo];
            if (cMouse::instance()->clickState() == sKey::PRESSEDtoRELEASED) {
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
        mPoint = BULL_POINT; mTotalPoint = 2 * BULL_POINT; return;
    case sRadialPos::OUTER_BULL:
        mPoint = BULL_POINT;
        if (cGame::instance()->category() == cGame::sCategory::CRICKET) {
            mTotalPoint = BULL_POINT; return;
        }
        mTotalPoint = 2 * BULL_POINT; return;
    default:
        for (int i = 0; i < 21; i++, phi += 0.1 * M_PI) {
            if (theta < phi) {
                mPoint = cDarts::BOARD_POINT[i];
                mTotalPoint = mPower * mPoint;
                return;
            }
        }
        return;
    }
}

void cDarts::update() {
    cTimer::instance()->update();
    if (cTimer::instance()->isPaused()) {
        if (cControl::instance()->isRequested(cControl::RESUME)) { 
            cTimer::instance()->resume(); return;
        }
    }
    else {
        if (cControl::instance()->isRequested(cControl::PAUSE)) { 
            cTimer::instance()->pause(); return;
        }
    }
    // initialize darts
    mRadialPos = sRadialPos::OUTSIDE; mPoint = 0; mPower = 0; mTotalPoint = 0;
    mIsTouched = false; mIsThrowed = false;
	if (updateByKeyboard()) return;
	updateByMouse();
}

void cDarts::reset() {
    cTimer::instance()->restart();
}