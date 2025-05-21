#include "GameSelect.hpp"
#include "Mouse.hpp"
#include <string>
GameSelect::GameSelect(ShareData shareData) {
    sd = shareData;
    mNowScene = GAME_SELECT;
    for (int mainGameNo = 0, gameNo = 0; mainGameNo < MAIN_GAME_NUM; mainGameNo++) {
        mainGameBox[mainGameNo].setSize(230, sd.font.title.size + 40);
        mainGameBox[mainGameNo].setUpperLeft(
            sd.screen.left() + sd.screen.width() * mainGameNo / 3, sd.obj.upperFrame.box.bottom());
        for (int subGameNo = 0; subGameNo < SUB_GAME_NUM; subGameNo++, gameNo++) {
            gameBox[gameNo].setSize(250, sd.font.normal.size + 12);
            gameBox[gameNo].setUpperLeft(mainGameBox[mainGameNo].left() + 15,
                mainGameBox[mainGameNo].bottom() + sd.screen.height() * subGameNo / 10);
        }
    }
}
void GameSelect::reset() {
    Scene::reset(); sd.game = -1;
}
void GameSelect::draw() {
    Scene::draw();
    for (int mainGameNo = 0; mainGameNo < MAIN_GAME_NUM; mainGameNo++)
        DrawStringToHandle(mainGameBox[mainGameNo].left() + 5,
            mainGameBox[mainGameNo].center().y() - sd.font.title.size / 2,
            mainGameName[mainGameNo].c_str(), sd.color.w, sd.font.title.handle);
    for (int gameNo = 0; gameNo < GAME_NUM; gameNo++) {
        switch (Mouse::getInstance()->getClickBoxState(gameBox[gameNo])) {
        case Key::RELEASED: 
            drawBoxObj(gameBox[gameNo], sd.color.touch, TRUE); break;
        case Key::RELEASEDtoPRESSED: case Key::PRESSED:
            drawBoxObj(gameBox[gameNo], sd.color.press, TRUE); break;
        case Key::PRESSEDtoRELEASED:
            sd.game = gameNo; mNextScene = PLAYER_SELECT; break;
        default: break;
        }
        DrawStringToHandle(gameBox[gameNo].left() + 15, gameBox[gameNo].center().y() - sd.font.normal.size / 2,
            gameName[gameNo].c_str(), sd.color.w, sd.font.normal.handle);
    }
    DrawStringToHandle(sd.ctrl.mute[sd.sound].icon.box.right() + 5,
        sd.obj.upperFrame.box.center().y() - sd.font.normal.size / 2, "Game Select", sd.color.w, sd.font.normal.handle);
    return;
}
void GameSelect::update() {
    Scene::update();
    if (sd.game >= 0 && sd.game < GAME_NUM) {
        drawImage(sd.ctrl.forward.icon);
        if (ctrlRQ(sd.ctrl.forward) || ctrlRQ(sd.ctrl.skip)) { mNextScene = PLAYER_SELECT; return; }
    }
    if (ctrlRQ(sd.ctrl.skip)) { sd.game = 0; mNextScene = PLAYER_SELECT; }
    else if (ctrlRQ(sd.ctrl.back)) mNextScene = HOME;
    return;
}
GameSelect::~GameSelect() {
}
