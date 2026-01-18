#include "GameSelect.hpp"
#include "Mouse.hpp"
#include <string>
#include "Color.hpp"
#include "Sound.hpp"

GameSelect::GameSelect(ShareData shareData) {
    sd = shareData;
    mNowScene = GAME_SELECT;
    for (int category = 0, mode = 0; category < cGame::sCategory::NUM && mode < cGame::sMode::NUM; 
        category++) {
        gameCategoryBox[category].setSize(230, XLfontSize + 40);
        gameCategoryBox[category].setUpperLeft(
            screen.left() + screen.width() * category / 3, upperFrame.bottom());
        for (int categoryMode = 0; 
            categoryMode < cGame::CATEGORY_MODE_NUM[category] && mode < cGame::sMode::NUM; 
            categoryMode++, mode++) {
            gameModeBox[mode].setSize(250, MfontSize + 12);
            gameModeBox[mode].setUpperLeft(gameCategoryBox[category].left() + 15,
                gameCategoryBox[category].bottom() + screen.height() * categoryMode / 10);
        }
    }
}

void GameSelect::reset() {
    cScene::reset();
    cGame::instance()->init();
}

void GameSelect::draw() {
    cScene::draw();
    
    // icons
    sd.ctrl.forward.icon.draw();
    sd.ctrl.skip.icon.draw();
    
    // games
    for (int category = 0; category < cGame::sCategory::NUM; category++)
        DrawStringToHandle(gameCategoryBox[category].left() + 5,
            gameCategoryBox[category].center().y() - XLfontSize / 2,
            cGame::instance()->categoryName(category).c_str(), white, XLfont);
    unsigned int color = white;
    for (int game = 0; game < cGame::sMode::NUM; game++, color = white) {
        switch (cMouse::instance()->clickBoxState(gameModeBox[game])) {
        case sKey::RELEASED:
            color = touchColor; break;
        case sKey::RELEASEDtoPRESSED: case sKey::PRESSED:
            color = pressColor; break;
        case sKey::PRESSEDtoRELEASED:
            cGame::instance()->setMode(game); mNextScene = PLAYER_SELECT; break;
        default: break;
        }
        DrawStringToHandle(gameModeBox[game].left() + 20, gameModeBox[game].center().y() - MfontSize / 2,
            cGame::instance()->modeName(game).c_str(), color, Mfont);
    }

    // scene title
    DrawStringToHandle(sd.ctrl.mute[0].icon.box().right() + 5,
        upperFrame.center().y() - MfontSize / 2, "Game Select", white, Mfont);
}

void GameSelect::update() {
    cScene::update();
    if (ctrlRQ(sd.ctrl.forward) || ctrlRQ(sd.ctrl.skip)) mNextScene = PLAYER_SELECT;
    else if (ctrlRQ(sd.ctrl.back) || ctrlRQ(sd.ctrl.home)) mNextScene = HOME;
    else if (ctrlRQ(sd.ctrl.config)) mNextScene = CONFIG;
}
