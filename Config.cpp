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
    DrawStringToHandle(mIcon[ctrl].box().right() + 5, mIcon[ctrl].box().center().y() - MfontSize / 2,
        cControl::instance()->name(ctrl).c_str(), white, Mfont);
    cKeyboard::instance()->keyImage(cControl::instance()->keyCode(ctrl)).draw();
}

void cConfig::reset() {
    cBaseScene::reset();
    cControl::instance()->initKey();
    initScreenSize();
    cSound::instance()->initSoundVol();
}

void cConfig::set() {
    for (int ctrl = 0; ctrl < cControl::NUM; ctrl++) 
        mIcon[ctrl] = cControl::instance()->icon(ctrl);
    mIcon[cControl::CHANGE_BGM].box().setUpperLeft(10, 210);
    cKeyboard::instance()->keyBox(cControl::instance()->keyCode(cControl::CHANGE_BGM)).setUpperLeft(
        200, 210);
    int iconX[2] = { screen.center().x(), screen.center().x() + 20 + screen.width() / 4 };
    int keyX[2] = { iconX[0] + 190, iconX[1] + 150 };
    mIcon[cControl::HOME].box().setUpperLeft(iconX[0], upperFrame.bottom() + XLfontSize + 30);
    cKeyboard::instance()->keyBox(cControl::instance()->keyCode(cControl::HOME)).setUpperLeft(
        keyX[0], mIcon[cControl::HOME].box().top());
    mIcon[cControl::SKILL].box().setUpperLeft(iconX[0], mIcon[cControl::HOME].box().bottom() + 5);
    cKeyboard::instance()->keyBox(cControl::instance()->keyCode(cControl::SKILL)).setUpperLeft(
        keyX[0], mIcon[cControl::SKILL].box().top());
    mIcon[cControl::CONFIG].box().setUpperLeft(iconX[0], mIcon[cControl::SKILL].box().bottom() + 5);
    cKeyboard::instance()->keyBox(cControl::instance()->keyCode(cControl::CONFIG)).setUpperLeft(
        keyX[0], mIcon[cControl::CONFIG].box().top());
    mIcon[cControl::QUIT].box().setUpperLeft(iconX[1], mIcon[cControl::CONFIG].box().top());
    cKeyboard::instance()->keyBox(cControl::instance()->keyCode(cControl::QUIT)).setUpperLeft(
        keyX[1], mIcon[cControl::CONFIG].box().top());
    mIcon[cControl::INITIALIZE].box().setUpperLeft(iconX[0], mIcon[cControl::CONFIG].box().bottom() + 5);
    cKeyboard::instance()->keyBox(cControl::instance()->keyCode(cControl::INITIALIZE)).setUpperLeft(
        keyX[0], mIcon[cControl::INITIALIZE].box().top());
    mIcon[cControl::SKIP].box().setUpperLeft(iconX[1], mIcon[cControl::INITIALIZE].box().top());
    cKeyboard::instance()->keyBox(cControl::instance()->keyCode(cControl::SKIP)).setUpperLeft(
        keyX[1], mIcon[cControl::INITIALIZE].box().top());
    for (int i = 0; i < 2; i++) {
        mIcon[cControl::MUTE + i].box().setUpperLeft(iconX[1], mIcon[cControl::HOME].box().top());
        cKeyboard::instance()->keyBox(cControl::instance()->keyCode(cControl::MUTE + i)).setUpperLeft(
            keyX[1], mIcon[cControl::HOME].box().top());
        mIcon[cControl::PAUSE + i].box().setUpperLeft(iconX[1], mIcon[cControl::SKILL].box().top());
        cKeyboard::instance()->keyBox(cControl::instance()->keyCode(cControl::PAUSE + i)).setUpperLeft(
            keyX[1], mIcon[cControl::SKILL].box().top());
        mIcon[cControl::ANOTHER_WINDOW + i].box().setUpperLeft(
            iconX[0], mIcon[cControl::INITIALIZE].box().bottom() + 5);
        cKeyboard::instance()->keyBox(cControl::instance()->keyCode(cControl::ANOTHER_WINDOW + i)).setUpperLeft(
            keyX[0], mIcon[cControl::ANOTHER_WINDOW + i].box().top());
    }
    mIcon[cControl::GAME_SELECT].box().setUpperLeft(iconX[1], mIcon[cControl::ANOTHER_WINDOW].box().top());
    cKeyboard::instance()->keyBox(cControl::instance()->keyCode(cControl::GAME_SELECT)).setUpperLeft(
        keyX[1], mIcon[cControl::GAME_SELECT].box().top());
    mIcon[cControl::PLAYER_SELECT].box().setUpperLeft(iconX[0], mIcon[cControl::GAME_SELECT].box().bottom() + 5);
    cKeyboard::instance()->keyBox(cControl::instance()->keyCode(cControl::PLAYER_SELECT)).setUpperLeft(
        keyX[0], mIcon[cControl::PLAYER_SELECT].box().top());
    mIcon[cControl::RESET].box().setUpperLeft(iconX[1], mIcon[cControl::PLAYER_SELECT].box().top());
    cKeyboard::instance()->keyBox(cControl::instance()->keyCode(cControl::RESET)).setUpperLeft(
        keyX[1], mIcon[cControl::RESET].box().top());
    mIcon[cControl::BACK].box().setUpperLeft(iconX[0], mIcon[cControl::RESET].box().bottom() + 5);
    cKeyboard::instance()->keyBox(cControl::instance()->keyCode(cControl::BACK)).setUpperLeft(
        keyX[0], mIcon[cControl::BACK].box().top());
    mIcon[cControl::FORWARD].box().setUpperLeft(iconX[1], mIcon[cControl::BACK].box().top());
    cKeyboard::instance()->keyBox(cControl::instance()->keyCode(cControl::FORWARD)).setUpperLeft(
        keyX[1], mIcon[cControl::BACK].box().top());
}

void cConfig::draw() {
    cBaseScene::draw();

    // draw icon
    if (cTimer::instance()->isPaused()) cControl::instance()->icon(cControl::PAUSE).draw();
    else cControl::instance()->icon(cControl::RESUME).draw();
    cControl::instance()->icon(cControl::SKILL).draw();
    if (cScene::instance()->lastScene() >= cScene::PLAYER_SELECT)
        cControl::instance()->icon(cControl::PLAYER_SELECT).draw();
    if (cScene::instance()->lastScene() >= cScene::GAME_SELECT) 
        cControl::instance()->icon(cControl::GAME_SELECT).draw();

    // control setting
    draw(cControl::HOME);
    draw(cControl::MUTE + cSound::instance()->isBGMPlayed());
    draw(cControl::PLAYER_SELECT);
    draw(cControl::GAME_SELECT);
    draw(cControl::SKILL);
    if (cTimer::instance()->isPaused()) draw(cControl::PAUSE); 
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
    DrawStringToHandle(5, upperFrame.bottom() + 10, "Sound", white, XLfont);
    DrawStringToHandle(screen.center().x(), upperFrame.bottom() + 10, "System", white, XLfont);
    DrawStringToHandle(10, 180, ("Play mode: " + cSound::instance()->bgmPlayModeName()).c_str(),
        white, Mfont);
    DrawGraph(355, 175, cControl::instance()->icon(cControl::DOWN).handle(), TRUE);
    DrawGraph(380, 175, cControl::instance()->icon(cControl::UP).handle(), TRUE);
    for (int i = 0; i < cSound::sKind::NUM; i++) {
        DrawStringToHandle(10, 90 + 30 * i,
            cSound::instance()->name(i).c_str(), white, Mfont);
        int vol = cSound::instance()->vol(i);
        DrawBox(100, 95 + 30 * i, 100 + vol, 105 + 30 * i, white, TRUE);
        DrawBox(100 + vol, 95 + 30 * i, 300, 105 + 30 * i, black, TRUE);
        DrawFormatStringToHandle(315, 90 + 30 * i, white, Mfont, "%3d", vol);
        DrawGraph(355, 85 + 30 * i, cControl::instance()->icon(cControl::DOWN).handle(), TRUE);
        DrawGraph(380, 85 + 30 * i, cControl::instance()->icon(cControl::UP).handle(), TRUE);
        switch (cMouse::instance()->clickBoxState(95, 90 + 30 * i, 305, 110 + 30 * i)) {
        case sKey::RELEASED:
            DrawBox(95 + vol, 90 + 30 * i, 105 + vol, 110 + 30 * i, touchColor, TRUE);
            break;
        case sKey::RELEASEDtoPRESSED: case sKey::PRESSED:
            DrawBox(95 + vol, 90 + 30 * i, 105 + vol, 110 + 30 * i, pressColor, TRUE);
            cSound::instance()->setVol(i, cMouse::instance()->x() - 100);
            break;
        default:
            DrawBox(95 + vol, 90 + 30 * i, 105 + vol, 110 + 30 * i, gray, TRUE);
            break;
        }
        if (cMouse::instance()->clickBoxCount(355, 85 + 30 * i, 380, 110 + 30 * i) > 10) {
            cSound::instance()->setVol(i, vol - 1);
        }
        else if (cMouse::instance()->clickBoxCount(380, 85 + 30 * i, 405, 110 + 30 * i) > 10) {
            cSound::instance()->setVol(i, vol + 1);
        }
        else if (cMouse::instance()->clickBoxCount(355, 85 + 30 * i, 380, 110 + 30 * i) == -1) {
            cSound::instance()->setVol(i, vol - 4);
        }
        else if (cMouse::instance()->clickBoxCount(380, 85 + 30 * i, 405, 110 + 30 * i) == -1) {
            cSound::instance()->setVol(i, vol + 4);
        }
    }
    DrawStringToHandle(
        cControl::instance()->icon(cControl::MUTE + cSound::instance()->isBGMPlayed()).box().right(),
        upperFrame.center().y() - MfontSize / 2, "Config", white, Mfont);
}

void cConfig::update() {
    cBaseScene::update();
    if (cMouse::instance()->clickBoxState(355, 175, 380, 200) == sKey::RELEASEDtoPRESSED) {
        cSound::instance()->playShotSE(cDarts::sRadialPos::OUTER_SINGLE);
        cSound::instance()->setBgmPlayMode(
            (cSound::instance()->bgmPlayMode() - 1 + cSound::sPlayMode::NUM) % cSound::sPlayMode::NUM);
    }
    else if (cMouse::instance()->clickBoxState(380, 175, 405, 200) == sKey::RELEASEDtoPRESSED) {
        cSound::instance()->playShotSE(cDarts::sRadialPos::OUTER_SINGLE);
        cSound::instance()->setBgmPlayMode(
            (cSound::instance()->bgmPlayMode() + 1) % cSound::sPlayMode::NUM);
    }
    else if (cControl::instance()->isRequested(cControl::BACK)) {
        cScene::instance()->setScene(cScene::instance()->lastScene());
        if (cScene::instance()->lastScene() == cScene::GAME) {
            cTimer::instance()->resume();
        }
    }
    else if (cControl::instance()->isRequested(cControl::ANOTHER_WINDOW) ||
        cControl::instance()->isRequested(cControl::MAXIMIZE_WINDOW)) set();
    else if (cControl::instance()->isRequested(cControl::RESET)) reset();
}