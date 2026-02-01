#include "Config.hpp"
#include "Mouse.hpp"
#include "Sound.hpp"
#include "Timer.hpp"
#include "Keyboard.hpp"
#include "Control.hpp"
#include "Scene.hpp"

cConfig::cConfig() {
    set();
}

void cConfig::draw(int ctrl) {
    mIcon[ctrl].draw();
    DrawStringToHandle(mIcon[ctrl].box().R() + 5, mIcon[ctrl].box().C().y() - MfontSize / 2,
        cControl::inst()->name(ctrl).c_str(), white, Mfont);
    cKeyboard::inst()->keyImage(cControl::inst()->keyCode(ctrl)).draw();
}

void cConfig::reset() {
    cBaseScene::reset();
    cControl::inst()->initKey();
    initScreenSize();
    cSound::inst()->initSoundVol();
}

void cConfig::set() {
    for (int ctrl = 0; ctrl < cControl::NUM; ctrl++) 
        mIcon[ctrl] = cControl::inst()->icon(ctrl);
    mIcon[cControl::CHANGE_BGM].box().setUL(10, 210);
    cKeyboard::inst()->keyBox(cControl::inst()->keyCode(cControl::CHANGE_BGM)).setUL(
        200, 210);
    int iconX[2] = { screen.C().x(), screen.C().x() + 20 + screen.W() / 4 };
    int keyX[2] = { iconX[0] + 190, iconX[1] + 150 };
    mIcon[cControl::HOME].box().setUL(iconX[0], upperFrame.B() + XLfontSize + 30);
    cKeyboard::inst()->keyBox(cControl::inst()->keyCode(cControl::HOME)).setUL(
        keyX[0], mIcon[cControl::HOME].box().T());
    mIcon[cControl::SKILL].box().setUL(iconX[0], mIcon[cControl::HOME].box().B() + 5);
    cKeyboard::inst()->keyBox(cControl::inst()->keyCode(cControl::SKILL)).setUL(
        keyX[0], mIcon[cControl::SKILL].box().T());
    mIcon[cControl::CONFIG].box().setUL(iconX[0], mIcon[cControl::SKILL].box().B() + 5);
    cKeyboard::inst()->keyBox(cControl::inst()->keyCode(cControl::CONFIG)).setUL(
        keyX[0], mIcon[cControl::CONFIG].box().T());
    mIcon[cControl::QUIT].box().setUL(iconX[1], mIcon[cControl::CONFIG].box().T());
    cKeyboard::inst()->keyBox(cControl::inst()->keyCode(cControl::QUIT)).setUL(
        keyX[1], mIcon[cControl::CONFIG].box().T());
    mIcon[cControl::INITIALIZE].box().setUL(iconX[0], mIcon[cControl::CONFIG].box().B() + 5);
    cKeyboard::inst()->keyBox(cControl::inst()->keyCode(cControl::INITIALIZE)).setUL(
        keyX[0], mIcon[cControl::INITIALIZE].box().T());
    mIcon[cControl::SKIP].box().setUL(iconX[1], mIcon[cControl::INITIALIZE].box().T());
    cKeyboard::inst()->keyBox(cControl::inst()->keyCode(cControl::SKIP)).setUL(
        keyX[1], mIcon[cControl::INITIALIZE].box().T());
    for (int i = 0; i < 2; i++) {
        mIcon[cControl::MUTE + i].box().setUL(iconX[1], mIcon[cControl::HOME].box().T());
        cKeyboard::inst()->keyBox(cControl::inst()->keyCode(cControl::MUTE + i)).setUL(
            keyX[1], mIcon[cControl::HOME].box().T());
        mIcon[cControl::PAUSE + i].box().setUL(iconX[1], mIcon[cControl::SKILL].box().T());
        cKeyboard::inst()->keyBox(cControl::inst()->keyCode(cControl::PAUSE + i)).setUL(
            keyX[1], mIcon[cControl::SKILL].box().T());
        mIcon[cControl::ANOTHER_WINDOW + i].box().setUL(
            iconX[0], mIcon[cControl::INITIALIZE].box().B() + 5);
        cKeyboard::inst()->keyBox(cControl::inst()->keyCode(cControl::ANOTHER_WINDOW + i)).setUL(
            keyX[0], mIcon[cControl::ANOTHER_WINDOW + i].box().T());
    }
    mIcon[cControl::GAME_SELECT].box().setUL(iconX[1], mIcon[cControl::ANOTHER_WINDOW].box().T());
    cKeyboard::inst()->keyBox(cControl::inst()->keyCode(cControl::GAME_SELECT)).setUL(
        keyX[1], mIcon[cControl::GAME_SELECT].box().T());
    mIcon[cControl::PLAYER_SELECT].box().setUL(iconX[0], mIcon[cControl::GAME_SELECT].box().B() + 5);
    cKeyboard::inst()->keyBox(cControl::inst()->keyCode(cControl::PLAYER_SELECT)).setUL(
        keyX[0], mIcon[cControl::PLAYER_SELECT].box().T());
    mIcon[cControl::RESET].box().setUL(iconX[1], mIcon[cControl::PLAYER_SELECT].box().T());
    cKeyboard::inst()->keyBox(cControl::inst()->keyCode(cControl::RESET)).setUL(
        keyX[1], mIcon[cControl::RESET].box().T());
    mIcon[cControl::BACK].box().setUL(iconX[0], mIcon[cControl::RESET].box().B() + 5);
    cKeyboard::inst()->keyBox(cControl::inst()->keyCode(cControl::BACK)).setUL(
        keyX[0], mIcon[cControl::BACK].box().T());
    mIcon[cControl::FORWARD].box().setUL(iconX[1], mIcon[cControl::BACK].box().T());
    cKeyboard::inst()->keyBox(cControl::inst()->keyCode(cControl::FORWARD)).setUL(
        keyX[1], mIcon[cControl::BACK].box().T());
}

void cConfig::draw() {
    cBaseScene::draw();

    // draw icon
    if (cTimer::inst()->isPaused()) cControl::inst()->icon(cControl::PAUSE).draw();
    else cControl::inst()->icon(cControl::RESUME).draw();
    cControl::inst()->icon(cControl::SKILL).draw();
    if (cScene::inst()->lastScene() >= cScene::PLAYER_SELECT)
        cControl::inst()->icon(cControl::PLAYER_SELECT).draw();
    if (cScene::inst()->lastScene() >= cScene::GAME_SELECT) 
        cControl::inst()->icon(cControl::GAME_SELECT).draw();

    // control setting
    draw(cControl::HOME);
    draw(cControl::MUTE + cSound::inst()->isBGMPlayed());
    draw(cControl::PLAYER_SELECT);
    draw(cControl::GAME_SELECT);
    draw(cControl::SKILL);
    if (cTimer::inst()->isPaused()) draw(cControl::PAUSE); 
    else draw(cControl::RESUME);
    draw(cControl::CONFIG);
    draw(cControl::ANOTHER_WINDOW + GetWindowModeFlag());
    draw(cControl::QUIT);
    draw(cControl::INITIALIZE);
    draw(cControl::RESET);
    draw(cControl::CHANGE_BGM);
    draw(cControl::SKIP);
    draw(cControl::BACK);
    draw(cControl::FORWARD);

    // music setting 
    DrawStringToHandle(5, upperFrame.B() + 10, "Sound", white, XLfont);
    DrawStringToHandle(screen.C().x(), upperFrame.B() + 10, "System", white, XLfont);
    DrawStringToHandle(10, 180, ("Play mode: " + cSound::inst()->bgmPlayModeName()).c_str(),
        white, Mfont);
    DrawGraph(355, 175, cControl::inst()->icon(cControl::DOWN).handle(), TRUE);
    DrawGraph(380, 175, cControl::inst()->icon(cControl::UP).handle(), TRUE);
    for (int i = 0; i < cSound::sKind::NUM; i++) {
        DrawStringToHandle(10, 90 + 30 * i,
            cSound::inst()->name(i).c_str(), white, Mfont);
        int vol = cSound::inst()->vol(i);
        DrawBox(100, 95 + 30 * i, 100 + vol, 105 + 30 * i, white, TRUE);
        DrawBox(100 + vol, 95 + 30 * i, 300, 105 + 30 * i, black, TRUE);
        DrawFormatStringToHandle(315, 90 + 30 * i, white, Mfont, "%3d", vol);
        DrawGraph(355, 85 + 30 * i, cControl::inst()->icon(cControl::DOWN).handle(), TRUE);
        DrawGraph(380, 85 + 30 * i, cControl::inst()->icon(cControl::UP).handle(), TRUE);
        switch (cMouse::inst()->LclickBoxState(95, 90 + 30 * i, 305, 110 + 30 * i)) {
        case sKeyState::RELEASE:
            DrawBox(95 + vol, 90 + 30 * i, 105 + vol, 110 + 30 * i, touchColor, TRUE);
            break;
        case sKeyState::RELEASEtoPRESS: case sKeyState::PRESS:
            DrawBox(95 + vol, 90 + 30 * i, 105 + vol, 110 + 30 * i, pressColor, TRUE);
            cSound::inst()->setVol(i, cMouse::inst()->x() - 100);
            break;
        default:
            DrawBox(95 + vol, 90 + 30 * i, 105 + vol, 110 + 30 * i, gray, TRUE);
            break;
        }
        if (cMouse::inst()->LclickBoxCount(355, 85 + 30 * i, 380, 110 + 30 * i) > 10) {
            cSound::inst()->setVol(i, vol - 1);
        }
        else if (cMouse::inst()->LclickBoxCount(380, 85 + 30 * i, 405, 110 + 30 * i) > 10) {
            cSound::inst()->setVol(i, vol + 1);
        }
        else if (cMouse::inst()->LclickBoxCount(355, 85 + 30 * i, 380, 110 + 30 * i) == -1) {
            cSound::inst()->setVol(i, vol - 4);
        }
        else if (cMouse::inst()->LclickBoxCount(380, 85 + 30 * i, 405, 110 + 30 * i) == -1) {
            cSound::inst()->setVol(i, vol + 4);
        }
    }
    DrawStringToHandle(
        cControl::inst()->icon(cControl::MUTE + cSound::inst()->isBGMPlayed()).box().R(),
        upperFrame.C().y() - MfontSize / 2, "Config", white, Mfont);
}

void cConfig::update() {
    cBaseScene::update();
    if (cMouse::inst()->LclickBoxState(355, 175, 380, 200) == sKeyState::RELEASEtoPRESS) {
        cSound::inst()->playSelectSE();
        cSound::inst()->setBgmPlayMode(
            (cSound::inst()->bgmPlayMode() - 1 + cSound::sPlayMode::NUM) % cSound::sPlayMode::NUM);
    }
    else if (cMouse::inst()->LclickBoxState(380, 175, 405, 200) == sKeyState::RELEASEtoPRESS) {
        cSound::inst()->playSelectSE();
        cSound::inst()->setBgmPlayMode(
            (cSound::inst()->bgmPlayMode() + 1) % cSound::sPlayMode::NUM);
    }
    else if (cControl::inst()->isRequested(cControl::BACK)) {
        cScene::inst()->setScene(cScene::inst()->lastScene());
        if (cScene::inst()->lastScene() == cScene::GAME) {
            cTimer::inst()->resume();
        }
    }
    else if (cControl::inst()->isRequested(cControl::ANOTHER_WINDOW) ||
        cControl::inst()->isRequested(cControl::MAXIMIZE_WINDOW)) set();
    else if (cControl::inst()->isRequested(cControl::RESET)) reset();
}