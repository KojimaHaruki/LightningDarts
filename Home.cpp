#include "Home.hpp"
Home::Home() : nowTime(time(NULL)) {
    mNowScene = NO_CHANGE; 
    timeError = localtime_s(&nowLocalTime, &nowTime);
}
Home::Home(ShareData shareData) : nowTime(time(NULL)) {
    sd = shareData;
    mNowScene = HOME; 
    timeError = localtime_s(&nowLocalTime, &nowTime); 
}
void Home::draw() {
    Scene::draw();
    DrawStringToHandle(sd.screen.center().x() - 3 * sd.font.title.size,
        sd.screen.center().y() - sd.font.title.size / 2, "Lightning Darts", sd.color.touch, sd.font.title.handle);
    DrawGraph(sd.screen.center().x() - 56, 11 * sd.screen.center().y() / 8 - 12, sd.ctrl.start.key.image.handle, TRUE);
    int y = 11 * sd.screen.center().y() / 8 - sd.font.normal.size / 2;
    DrawStringToHandle(11 * sd.screen.center().x() / 16, y, "Press", sd.color.w, sd.font.normal.handle);
    DrawStringToHandle(sd.screen.center().x() - 30, y, "space", sd.color.w, sd.font.normal.handle);
    DrawStringToHandle(19 * sd.screen.center().x() / 16, y, "to start ...", sd.color.w, sd.font.normal.handle);
    if (!timeError) {
        DrawFormatStringToHandle(sd.screen.right() - 250, sd.obj.upperFrame.box.bottom() + 10,
            sd.color.w, sd.font.chara.handle, "%4d/%02d/%02d(%3s) %02d:%02d:%02d",
            nowLocalTime.tm_year + 1900, nowLocalTime.tm_mon + 1, nowLocalTime.tm_mday,
            wday[nowLocalTime.tm_wday].c_str(), nowLocalTime.tm_hour, nowLocalTime.tm_min, nowLocalTime.tm_sec);
    }
    DrawStringToHandle(sd.ctrl.mute[sd.sound].icon.box.upperRight().x(),
        sd.obj.upperFrame.box.center().y() - sd.font.normal.size / 2, "Home", sd.color.w, sd.font.normal.handle);
    return;
}
void Home::update() {
    Scene::update(); nowTime = time(NULL); timeError = localtime_s(&nowLocalTime, &nowTime);
    if (isTyped(sd.ctrl.start.key)) mNextScene = GAME_SELECT;
    else if (ctrlRQ(sd.ctrl.back)) mNextScene = QUIT;
    else if (ctrlRQ(sd.ctrl.config)) mNextScene = CONFIG; 
    else if (sd.game >= 0 && sd.game < GAME_NUM) {
        drawImage(sd.ctrl.forward.icon); 
        if (ctrlRQ(sd.ctrl.forward)) { mNextScene = GAME_SELECT; }
    }
    return;
}
Home::~Home() {
}
