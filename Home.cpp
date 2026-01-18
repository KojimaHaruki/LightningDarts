#include "Home.hpp"
#include "Color.hpp"
#include "Font.hpp"
#include "Sound.hpp"
#include <ctime>

Home::Home() {
    mNowScene = NO_CHANGE;
    timeError = localtime_s(&nowLocalTime, &nowTime);
}

Home::Home(ShareData shareData) {
    sd = shareData;
    mNowScene = HOME;
    timeError = localtime_s(&nowLocalTime, &nowTime);
}

void Home::draw() {
    cScene::draw();
    drawImage(sd.ctrl.forward.icon);
    DrawStringToHandle(screen.center().x() - 3 * XLfontSize,
        screen.center().y() - XLfontSize / 2, "Lightning Darts", yellow, XLfont);
    DrawGraph(screen.center().x() - 56, 11 * screen.center().y() / 8 - 12, sd.ctrl.start.key.image.handle, TRUE);
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
    DrawStringToHandle(sd.ctrl.mute[0].icon.box.upperRight().x(),
        upperFrame.center().y() - MfontSize / 2, "Home", white, Mfont);
}
void Home::update() {
    cScene::update();
    if (isTyped(sd.ctrl.start.key)) mNextScene = GAME_SELECT;
    else if (ctrlRQ(sd.ctrl.back)) mNextScene = QUIT;
    else if (ctrlRQ(sd.ctrl.config)) mNextScene = CONFIG;
    else if (ctrlRQ(sd.ctrl.forward)) mNextScene = GAME_SELECT;
}