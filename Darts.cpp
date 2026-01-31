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
#include "Scene.hpp"
#include "Sound.hpp"

bool cDarts::setPointValidation(int point, bool isValid) {
    if (point == 25) {
        mIsValidPoint[sPoint::OUTER_BULL] = isValid; mIsValidPoint[sPoint::INNER_BULL] = isValid;
        return true;
    }
    if (point < 0 || point >= sPoint::NUM) return false;
    mIsValidPoint[point] = isValid; return true;
}

bool cDarts::isValidPoint(int point) {
    if (point == BULL_POINT) return mIsValidPoint[sPoint::OUTER_BULL] && mIsValidPoint[sPoint::INNER_BULL];
    if (point >= 0 && point < sPoint::NUM) return mIsValidPoint[point];
    return false;
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

void cDarts::init() {
    loadColor();
    loadFont();
    loadImage();
    mCenter = std::complex<float>(
        cScreen::instance()->box().left() + 0.25 * cScreen::instance()->box().width() + 5.0f, 
        cScreen::instance()->box().centerY());
	cTimer::instance()->reset();
    float lowerTheta = -1.05 * std::numbers::pi, upperTheta = -0.95 * std::numbers::pi;
    for (int i = 0; i < 20; i++, lowerTheta = upperTheta, upperTheta += 0.1 * std::numbers::pi) {
        pointThetaRange[BOARD_POINT[i]][0] = lowerTheta;
        pointThetaRange[BOARD_POINT[i]][1] = upperTheta;
    }
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
		cScreen::instance()->upperFrame().bottom() + 10, white, Sfont, cTimer::Mode::HMSmS);

    // draw game name
    DrawStringToHandle(cControl::instance()->icon(cControl::MUTE).box().right() + 5,
        cScreen::instance()->upperFrame().centerY() - MfontSize / 2,
        cGame::instance()->modeName().c_str(), white, Mfont);
    
    // draw base board
    DrawCircleAA(real(mCenter), imag(mCenter), RADIAL_POS_RADIUS[sRadialPos::OUTSIDE], 100, black);
    DrawCircleAA(real(mCenter), imag(mCenter), RADIAL_POS_RADIUS[sRadialPos::DOUBLE], 100, gray);

    DrawStringToHandle(0, cScreen::instance()->lowerFrame().top() - MfontSize - 5, 
        pointName().c_str(), white, Mfont);
    // draw point part
	float phi = -std::numbers::pi, r = RADIAL_POS_RADIUS[sRadialPos::OUTSIDE] - MfontSize + 2.0f,
        errorX = -11.0f, errorY = -10.0f;
    for (int i = 0; i < 20; i++, phi += 0.1 * std::numbers::pi) {
        if (mIsValidPoint[cDarts::BOARD_POINT[i]]) {
            DrawStringToHandle(real(mCenter) + r * cos(phi) + errorX, imag(mCenter) - r * sin(phi) + errorY,
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
            real(mCenter) + r * cos(phi) + errorX, imag(mCenter) - r * sin(phi) + errorY,
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
    DrawCircleAA(real(mCenter), imag(mCenter), RADIAL_POS_RADIUS[sRadialPos::OUTER_BULL], 100, color);

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
    DrawCircleAA(real(mCenter), imag(mCenter), RADIAL_POS_RADIUS[sRadialPos::INNER_BULL], 100, color);

	// draw circumferencial frame
    for (int posNo = 0; posNo < sRadialPos::NUM; posNo++) 
        DrawCircleAA(real(mCenter), imag(mCenter), RADIAL_POS_RADIUS[posNo], 100, black, FALSE, 2);
    
    // draw radial frame
    phi = 0.05 * std::numbers::pi;
	float r1 = RADIAL_POS_RADIUS[sRadialPos::OUTER_BULL], r2 = RADIAL_POS_RADIUS[sRadialPos::DOUBLE];
    for (int i = 0; i < 20; i++, phi += 0.1 * std::numbers::pi)
        DrawLineAA(real(mCenter) + r1 * cos(phi), imag(mCenter) + r1 * sin(phi),
            real(mCenter) + r2 * cos(phi), imag(mCenter) + r2 * sin(phi), black, 2);
}

bool cDarts::updateByKeyboard() {
    // check inner bull
    if (cKeyboard::instance()->pressKeyState(cDarts::POINT_KEY[sPoint::INNER_BULL]) == sKey::RELEASEDtoPRESSED) {
		mRadius = ((float)rand() / RAND_MAX) * RADIAL_POS_RADIUS[sRadialPos::INNER_BULL]; 
        mTheta = rand() % 360 * std::numbers::pi / 180.0f;
        mRadialPos = sRadialPos::INNER_BULL; mPoint = BULL_POINT; mPower = 2; mTotalPoint = 50; 
        mIsThrowed = true; return true;
    }

    // check outer bull
    if (cKeyboard::instance()->pressKeyState(cDarts::POINT_KEY[sPoint::OUTER_BULL]) == sKey::RELEASEDtoPRESSED) {
        mRadius = RADIAL_POS_RADIUS[sRadialPos::INNER_BULL] + ((float)rand() / RAND_MAX) *
            (RADIAL_POS_RADIUS[sRadialPos::OUTER_BULL] - RADIAL_POS_RADIUS[sRadialPos::INNER_BULL]);
        mTheta = rand() % 360 * std::numbers::pi / 180.0f;
        mRadialPos = sRadialPos::OUTER_BULL; mPoint = BULL_POINT; mPower = 1; mIsThrowed = true;
        if (cGame::instance()->category() == cGame::sCategory::CRICKET) {
            mTotalPoint = BULL_POINT; return true;
        }
        mTotalPoint = 2 * BULL_POINT; return true;
    }

    // check miss
    if (cKeyboard::instance()->pressKeyState(cDarts::POINT_KEY[sPoint::MISS]) == sKey::RELEASEDtoPRESSED) {
        mRadius = RADIAL_POS_RADIUS[sRadialPos::DOUBLE] + ((float)rand() / RAND_MAX) *
            (RADIAL_POS_RADIUS[sRadialPos::OUTSIDE] - RADIAL_POS_RADIUS[sRadialPos::DOUBLE]);
        mTheta = rand() % 360 * std::numbers::pi / 180.0f;
        mIsThrowed = true; return true;
    }
    
    // check points 1 to 20
    for (int point = 1; point <= 20; point++) {
        if (cKeyboard::instance()->pressKeyState(cDarts::POINT_KEY[point]) == sKey::RELEASEDtoPRESSED) {
            mTheta = rand() % 18 * std::numbers::pi / 180.0f + pointThetaRange[point][0];
            if (cKeyboard::instance()->pressKeyState(KEY_INPUT_D) == sKey::PRESSED) {
                mRadius = RADIAL_POS_RADIUS[sRadialPos::OUTER_SINGLE] + ((float)rand() / RAND_MAX) *
                    (RADIAL_POS_RADIUS[sRadialPos::DOUBLE] - RADIAL_POS_RADIUS[sRadialPos::OUTER_SINGLE]);
                mRadialPos = sRadialPos::DOUBLE; mPower = 2;
            }
            else if (cKeyboard::instance()->pressKeyState(KEY_INPUT_T) == sKey::PRESSED) {
                mRadius = RADIAL_POS_RADIUS[sRadialPos::INNER_SINGLE] + ((float)rand() / RAND_MAX) *
                    (RADIAL_POS_RADIUS[sRadialPos::TRIPLE] - RADIAL_POS_RADIUS[sRadialPos::INNER_SINGLE]);
                mRadialPos = sRadialPos::TRIPLE; mPower = 3;
            }
            else if (cKeyboard::instance()->pressKeyState(KEY_INPUT_O) == sKey::PRESSED) {
                mRadius = RADIAL_POS_RADIUS[sRadialPos::TRIPLE] + ((float)rand() / RAND_MAX) *
                    (RADIAL_POS_RADIUS[sRadialPos::OUTER_SINGLE] - RADIAL_POS_RADIUS[sRadialPos::TRIPLE]);
                mRadialPos = sRadialPos::OUTER_SINGLE; mPower = 1;
            }
            else {
                mRadius = RADIAL_POS_RADIUS[sRadialPos::OUTER_BULL] + ((float)rand() / RAND_MAX) *
                    (RADIAL_POS_RADIUS[sRadialPos::INNER_SINGLE] - RADIAL_POS_RADIUS[sRadialPos::OUTER_BULL]);
                mRadialPos = sRadialPos::INNER_SINGLE; mPower = 1;
            }
            mPoint = point; mTotalPoint = mPower * mPoint; mIsThrowed = true; return true;
        }
    }
	return false;
}

void cDarts::updateByMouse() {
    mArrowPos._Val[0] = cMouse::instance()->x(); mArrowPos._Val[1] = cMouse::instance()->y();
	std::complex<float> arrow(
        cMouse::instance()->x() - real(mCenter), imag(mCenter) - cMouse::instance()->y());
    mRadius = abs(arrow); mTheta = arg(arrow);

	// check radial position
    for (int radialPosNo = sRadialPos::INNER_BULL; radialPosNo < sRadialPos::NUM; radialPosNo++) {
        if (mRadius < RADIAL_POS_RADIUS[radialPosNo]) {
            mRadialPos = radialPosNo; mPower = RADIAL_POS_POWER[radialPosNo];
            if (cMouse::instance()->clickState() == sKey::PRESSEDtoRELEASED) {
                mIsThrowed = true;
                break;
            }
            mIsTouched = true;
            break;
        }
    }

    // check point
    float phi = -0.95 * std::numbers::pi;
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
        for (int point = 0; point < 20; point++, phi += 0.1 * std::numbers::pi) {
            if (mTheta < phi) {
                mPoint = BOARD_POINT[point];
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
    if (cControl::instance()->isRequested(cControl::CONFIG)) {
        cTimer::instance()->pause(); cScene::instance()->setScene(cScene::CONFIG); return;
    }
    // initialize darts
	mRadius = RADIAL_POS_RADIUS[sRadialPos::OUTSIDE]; mTheta = 0.0f;
    mRadialPos = sRadialPos::OUTSIDE; mPoint = 0; mPower = 0; mTotalPoint = 0;
    mIsTouched = false; mIsThrowed = false;
    if (updateByKeyboard()) {
        std::complex<float> arrow(mRadius * cos(mTheta), -mRadius * sin(mTheta));
        mArrowPos = mCenter + arrow;
    }
    else updateByMouse();
    if (mIsThrowed && isValidPoint(mPoint)) {
        cSound::instance()->playShotSE(mRadialPos);
        return;
    }
    cSound::instance()->playShotSE(cDarts::sRadialPos::OUTSIDE);
}

void cDarts::reset() {
    cTimer::instance()->restart();
}