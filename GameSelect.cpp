#include "GameSelect.hpp"
#include "Mouse.hpp"
#include <string>
#include "Color.hpp"
#include "Darts.hpp"

GameSelect::GameSelect(ShareData shareData) {
    sd = shareData;
    mNowScene = GAME_SELECT;
    for (int mainGameNo = 0, gameNo = 0; mainGameNo < MAIN_GAME_NUM; mainGameNo++) {
        mainGameBox[mainGameNo].setSize(230, XLfontSize + 40);
        mainGameBox[mainGameNo].setUpperLeft(
            sd.screen.left() + sd.screen.width() * mainGameNo / 3, sd.obj.upperFrame.bottom());
        for (int subGameNo = 0; subGameNo < SUB_GAME_NUM; subGameNo++, gameNo++) {
            gameBox[gameNo].setSize(250, MfontSize + 12);
            gameBox[gameNo].setUpperLeft(mainGameBox[mainGameNo].left() + 15,
                mainGameBox[mainGameNo].bottom() + sd.screen.height() * subGameNo / 10);
        }
    }
}

void GameSelect::reset() {
    cScene::reset();
    cDarts::instance()->setGameNo(cDarts::sGame::DEFAULT);
}

void GameSelect::draw() {
    cScene::draw();
    for (int mainGameNo = 0; mainGameNo < MAIN_GAME_NUM; mainGameNo++)
        DrawStringToHandle(mainGameBox[mainGameNo].left() + 5,
            mainGameBox[mainGameNo].center().y() - XLfontSize / 2,
            mainGameName[mainGameNo].c_str(), white, XLfont);
    unsigned int color = white;
    for (int game = 0; game < cDarts::sGame::NUM; game++, color = white) {
        switch (Mouse::instance()->getClickBoxState(gameBox[game])) {
        case Key::RELEASED:
            color = touchColor;
            break;
        case Key::RELEASEDtoPRESSED: case Key::PRESSED:
            color = pressColor;
            break;
        case Key::PRESSEDtoRELEASED:
            cDarts::instance()->setGameNo(game); mNextScene = PLAYER_SELECT;
        default:
            break;
        }
        DrawStringToHandle(gameBox[game].left() + 20, gameBox[game].center().y() - MfontSize / 2,
            gameName[game].c_str(), color, Mfont);
    }
    DrawStringToHandle(sd.ctrl.mute[0].icon.box.right() + 5,
        sd.obj.upperFrame.center().y() - MfontSize / 2, "Game Select", white, Mfont);
}

void GameSelect::update() {
    cScene::update();
    if (mGame >= 0 && mGame < GAME_NUM) {
        drawImage(sd.ctrl.forward.icon);
        if (ctrlRQ(sd.ctrl.forward) || ctrlRQ(sd.ctrl.skip)) { 
            mNextScene = PLAYER_SELECT; 
            return; 
        }
    }
    if (ctrlRQ(sd.ctrl.skip)) { 
        cDarts::instance()->setGameNo(cDarts::sGame::STANDARD_CRICKET); 
        mNextScene = PLAYER_SELECT; 
    }
    else if (ctrlRQ(sd.ctrl.back) || ctrlRQ(sd.ctrl.home)) mNextScene = HOME;
    else if (ctrlRQ(sd.ctrl.config)) mNextScene = CONFIG;
}
