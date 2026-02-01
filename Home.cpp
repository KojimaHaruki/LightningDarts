#include "Home.hpp"
#include "DxLib.h"
#include <ctime>
#include "Control.hpp"
#include "Scene.hpp"
#include "Keyboard.hpp"
#include "Camera.hpp"

cHome::cHome() {
    timeError = localtime_s(&nowLocalTime, &nowTime);
    cKeyboard::inst()->keyBox(cControl::inst()->keyCode(cControl::START)).setUL(
        screen.C().x() - 56, 11 * screen.C().y() / 8 - 12);
}

void cHome::draw() {
    cBaseScene::draw();
    cCamera::inst()->draw();
    if (cControl::inst()->isRequested(cControl::RESET))
        cCamera::inst()->reset();
    else if (cScene::inst()->lastScene() > cScene::HOME)
        cControl::inst()->icon(cControl::FORWARD).draw();
    DrawStringToHandle(screen.C().x() - 3 * XLfontSize,
        screen.C().y() - XLfontSize / 2, "Lightning Darts", yellow, XLfont);
	cKeyboard::inst()->keyImage(cControl::inst()->keyCode(cControl::START)).draw();
    int y = 11 * screen.C().y() / 8 - MfontSize / 2;
    DrawStringToHandle(11 * screen.C().x() / 16, y, "Press", white, Mfont);
    DrawStringToHandle(screen.C().x() - 30, y, "space", white, Mfont);
    DrawStringToHandle(19 * screen.C().x() / 16, y, "to start ...", white, Mfont);
    if (!timeError) {
        DrawFormatStringToHandle(screen.R() - 250, upperFrame.B() + 10,
            white, Mfont, "%4d/%02d/%02d(%3s) %02d:%02d:%02d",
            nowLocalTime.tm_year + 1900, nowLocalTime.tm_mon + 1, nowLocalTime.tm_mday,
            wday[nowLocalTime.tm_wday].c_str(), nowLocalTime.tm_hour, nowLocalTime.tm_min, nowLocalTime.tm_sec);
    }
    DrawStringToHandle(cControl::inst()->icon(cControl::MUTE).box().UR().x(),
        upperFrame.C().y() - MfontSize / 2, "Home", white, Mfont);
}

void cHome::update() {
    cBaseScene::update();
    cCamera::inst()->update();
    if (cControl::inst()->isKeyTyped(cControl::START))
        cScene::inst()->setScene(cScene::GAME_SELECT);
    else if (cControl::inst()->isRequested(cControl::CONFIG))
        cScene::inst()->setScene(cScene::CONFIG);
	else if (cScene::inst()->lastScene() > cScene::HOME &&
        cControl::inst()->isRequested(cControl::FORWARD))
        cScene::inst()->setScene(cScene::GAME_SELECT);
}

cHome::~cHome() {
}