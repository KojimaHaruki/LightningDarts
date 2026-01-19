#include "Home.hpp"
#include "Color.hpp"
#include "Font.hpp"
#include "Sound.hpp"
#include <ctime>
#include "Control.hpp"
#include "Keyboard.hpp"

cHome::cHome() {
    mNowScene = NO_CHANGE;
    timeError = localtime_s(&nowLocalTime, &nowTime);
    cKeyboard::instance()->keyBox(cControl::instance()->keyCode(cControl::START)).setUpperLeft(
        screen.center().x() - 56, 11 * screen.center().y() / 8 - 12);
}

void cHome::draw() {
    cBaseScene::draw();
    cControl::instance()->icon(cControl::FORWARD).draw();
    DrawStringToHandle(screen.center().x() - 3 * XLfontSize,
        screen.center().y() - XLfontSize / 2, "Lightning Darts", yellow, XLfont);
	cKeyboard::instance()->keyImage(cControl::instance()->keyCode(cControl::START)).draw();
    int y = 11 * screen.center().y() / 8 - MfontSize / 2;
    DrawStringToHandle(11 * screen.center().x() / 16, y, "Press", white, Mfont);
    DrawStringToHandle(screen.center().x() - 30, y, "space", white, Mfont);
    DrawStringToHandle(19 * screen.center().x() / 16, y, "to start ...", white, Mfont);
    if (!timeError) {
        DrawFormatStringToHandle(screen.right() - 250, upperFrame.bottom() + 10,
            white, Mfont, "%4d/%02d/%02d(%3s) %02d:%02d:%02d",
            nowLocalTime.tm_year + 1900, nowLocalTime.tm_mon + 1, nowLocalTime.tm_mday,
            wday[nowLocalTime.tm_wday].c_str(), nowLocalTime.tm_hour, nowLocalTime.tm_min, nowLocalTime.tm_sec);
    }
    DrawStringToHandle(cControl::instance()->icon(cControl::MUTE).box().upperRight().x(),
        upperFrame.center().y() - MfontSize / 2, "Home", white, Mfont);
}
void cHome::update() {
    cBaseScene::update();
    if (cControl::instance()->isKeyTyped(cControl::START)) mNextScene = GAME_SELECT;
    else if (cControl::instance()->isRequested(cControl::CONFIG)) mNextScene = CONFIG;
    else if (cControl::instance()->isRequested(cControl::FORWARD)) mNextScene = GAME_SELECT;
}