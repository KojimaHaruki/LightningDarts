#include "GameSelect.hpp"
#include "Mouse.hpp"
#include "Control.hpp"
#include "Scene.hpp"

cGameSelect::cGameSelect() {
    for (int category = 0, mode = 0; category < cGame::sCategory::NUM && mode < cGame::sMode::NUM;
        category++) {
        gameCategoryBox[category].setS(230, XLfontSize + 40);
        gameCategoryBox[category].setUL(
            screen.L() + screen.W() * category / 3, upperFrame.B());
        for (int categoryMode = 0;
            categoryMode < cGame::CATEGORY_MODE_NUM[category] && mode < cGame::sMode::NUM;
            categoryMode++, mode++) {
            gameModeBox[mode].setS(250, MfontSize + 12);
            gameModeBox[mode].setUL(gameCategoryBox[category].L() + 15,
                gameCategoryBox[category].B() + screen.H() * categoryMode / 10);
        }
    }
}

void cGameSelect::reset() {
    cBaseScene::reset();
    cGame::inst()->init();
}

void cGameSelect::draw() {
    cBaseScene::draw();

    // icons
    if (cScene::inst()->lastScene() >= cScene::PLAYER_SELECT)
        cControl::inst()->icon(cControl::FORWARD).draw();
    cControl::inst()->icon(cControl::SKIP).draw();

    // games
    for (int category = 0; category < cGame::sCategory::NUM; category++)
        DrawStringToHandle(gameCategoryBox[category].L() + 5,
            gameCategoryBox[category].C().y() - XLfontSize / 2,
            cGame::inst()->categoryName(category).c_str(), white, XLfont);
    unsigned int color = white;
    for (int game = 0; game < cGame::sMode::NUM; game++, color = white) {
        switch (cMouse::inst()->LclickBoxState(gameModeBox[game])) {
        case sKeyState::RELEASE:
            color = touchColor; break;
        case sKeyState::RELEASEtoPRESS: case sKeyState::PRESS:
            color = pressColor; break;
        case sKeyState::PRESStoRELEASE:
            cGame::inst()->setMode(game); cScene::inst()->setScene(cScene::PLAYER_SELECT); break;
        default: break;
        }
        DrawStringToHandle(gameModeBox[game].L() + 20, gameModeBox[game].C().y() - LfontSize / 2,
            cGame::inst()->modeName(game).c_str(), color, Lfont);
    }

    // scene title
    DrawStringToHandle(cControl::inst()->icon(cControl::MUTE).box().R() + 5,
        upperFrame.C().y() - MfontSize / 2, "Game Select", white, Mfont);
}

void cGameSelect::update() {
    cBaseScene::update();
    if ((cScene::inst()->lastScene() >= cScene::PLAYER_SELECT && 
        cControl::inst()->isRequested(cControl::FORWARD)) ||
        cControl::inst()->isRequested(cControl::SKIP))
        cScene::inst()->setScene(cScene::PLAYER_SELECT);
    else if (cControl::inst()->isRequested(cControl::BACK)) 
        cScene::inst()->setScene(cScene::HOME);
    else if (cControl::inst()->isRequested(cControl::CONFIG)) 
        cScene::inst()->setScene(cScene::CONFIG);
}
