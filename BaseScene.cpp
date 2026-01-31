#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "winmm.lib")   // MSVC —p
#include "BaseScene.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "Sound.hpp"
#include "Darts.hpp"
#include "Color.hpp"
#include "Font.hpp"
#include "Game.hpp"
#include "Screen.hpp"
#include "Control.hpp"
#include "Scene.hpp"
#include "resource.h"

cBaseScene::cBaseScene() : nowTime(time(NULL)), nTeam(cPlayer::instance()->nTeam()){
    loadColor(); loadFont(); loadScreen();
    timeError = localtime_s(&nowLocalTime, &nowTime);
}

void cBaseScene::loadColor() {
    white = cColor::instance()->white(); black = cColor::instance()->black();
    gray = cColor::instance()->gray(); red = cColor::instance()->red();
    green = cColor::instance()->green(); blue = cColor::instance()->blue();
    magenta = cColor::instance()->magenta(); cyan = cColor::instance()->cyan();
    yellow = cColor::instance()->yellow(); tableColor = cColor::instance()->tableColor();
    touchColor = cColor::instance()->touchColor(); pressColor = cColor::instance()->pressColor();
    executeColor = cColor::instance()->executeColor();
    for (int i = 0; i < cPlayer::MAX_SOLO_PLAYER_NUM; i++) {
        teamColor[i] = cColor::instance()->teamColor(i);
        rankColor[i] = cColor::instance()->rankColor(i);
    }
}

void cBaseScene::loadFont() {
    Sfont = cFont::instance()->Sfont(); SfontSize = cFont::instance()->SfontSize();
    Mfont = cFont::instance()->MFont(); MfontSize = cFont::instance()->MfontSize();
    Lfont = cFont::instance()->Lfont(); LfontSize = cFont::instance()->LfontSize();
    XLfont = cFont::instance()->XLfont(); XLfontSize = cFont::instance()->XLfontSize();
}

void cBaseScene::loadScreen() {
    screen = cScreen::instance()->box();
    upperFrame = cScreen::instance()->upperFrame();
    lowerFrame = cScreen::instance()->lowerFrame();
}

void cBaseScene::initScreenSize() {
    cScreen::instance()->initSize();
    loadScreen();
    setWindow(FALSE);
    cControl::instance()->initIconBox();
}

void cBaseScene::init() {
    initScreenSize();
    cSound::instance()->init();
    cControl::instance()->initKey();
    cGame::instance()->init();
    cPlayer::instance()->initTeam();
    cScene::instance()->setScene(cScene::HOME);
}

void cBaseScene::changeWindow() {
    if (GetWindowModeFlag() == TRUE) setWindow(FALSE);
    else setWindow(TRUE);
}

void cBaseScene::setWindow(int window) {
    ChangeWindowMode(window);
    SetDrawScreen(DX_SCREEN_BACK);
    SetMouseDispFlag(TRUE);

    // load fonts
    cFont::instance()->load();
    cDarts::instance()->loadFont();
    loadFont();

    // reload images
    cKeyboard::instance()->reloadKeyImage();
    cControl::instance()->reloadIcon();
    cScreen::instance()->reloadImage();
    cPlayer::instance()->loadImage();
    cDarts::instance()->loadImage();
}

void cBaseScene::draw() {
	cScreen::instance()->draw();
    cControl::instance()->icon(cControl::BACK).draw();
    for (int scene = cScene::HOME; scene <= cScene::PLAYER_SELECT; scene++) {
        if (scene <= cScene::instance()->maxScene()) {
            cControl::instance()->icon(scene).draw();
        }
    }
    cControl::instance()->icon(cControl::MUTE + cSound::instance()->isBGMPlayed()).draw();
    cControl::instance()->icon(cControl::CONFIG).draw();
    cControl::instance()->icon(cControl::ANOTHER_WINDOW + GetWindowModeFlag()).draw();
    cControl::instance()->icon(cControl::QUIT).draw();
    cControl::instance()->icon(cControl::INITIALIZE).draw();
    cControl::instance()->icon(cControl::RESET).draw();
    cControl::instance()->icon(cControl::CHANGE_BGM).draw();
    DrawStringToHandle(cControl::instance()->icon(cControl::CHANGE_BGM).box().right(),
        cScreen::instance()->lowerFrame().center().y() - SfontSize / 2,
        cSound::instance()->playingBGMName().c_str(), white, Sfont);
    DrawStringToHandle(screen.right() - 340, lowerFrame.center().y() - SfontSize / 2,
        "Lightning Darts C 2025 Haruki Kojima", yellow, Sfont);
    DrawCircleAA(screen.right() - 210.5f, (float)lowerFrame.center().y(),
        SfontSize / 2.0f, 1000, black, FALSE, 3.0f);
    DrawCircleAA(screen.right() - 210.5f, (float)lowerFrame.center().y(),
        SfontSize / 2.0f, 1000, yellow, FALSE, 1.0f);
    if (!timeError) {
        DrawFormatStringToHandle(
            screen.right() - 42, lowerFrame.center().y() - SfontSize / 2,
            white, Sfont, "%02d:%02d", nowLocalTime.tm_hour, nowLocalTime.tm_min);
    }
}

void cBaseScene::update() {
    cMouse::instance()->update();
    cKeyboard::instance()->update();
    cSound::instance()->update();
    nowTime = time(NULL);
    timeError = localtime_s(&nowLocalTime, &nowTime);
    for (int scene = cScene::HOME; scene <= cScene::PLAYER_SELECT; scene++) {
        if (scene <= cScene::instance()->maxScene() && scene != cScene::instance()->currentScene() &&
            cControl::instance()->isRequested(scene)) {
            cScene::instance()->setScene(scene); return;
        }
    }
    if (cControl::instance()->isRequested(cControl::INITIALIZE)) init();
    else if (cControl::instance()->isRequested(cControl::RESET)) reset();
    else if (cControl::instance()->isRequested(cControl::QUIT)) 
        cScene::instance()->setScene(cScene::QUIT);
    else if (cControl::instance()->isRequested(cControl::MUTE + cSound::instance()->isBGMPlayed())) {
        if (cSound::instance()->isBGMPlayed()) {
            cSound::instance()->mute();
            return;
        }
        cSound::instance()->unmute();
    }
    else if (cControl::instance()->isRequested(cControl::ANOTHER_WINDOW + GetWindowModeFlag()))
        changeWindow();
    else if (cControl::instance()->isRequested(cControl::CHANGE_BGM)) {
        if (cKeyboard::instance()->pressKeyState(KEY_INPUT_LSHIFT) == sKey::PRESSED) {
            cSound::instance()->playLastBGM();
        }
        else {
            cSound::instance()->playNextBGM();
        }
    }
}

cBaseScene::~cBaseScene() {
    //FILE* osdf; errno_t error = fopen_s(&osdf, ShareDataFileName, "wb+"); // open data file
    //if (!error) { fwrite(&sd, sizeof(sd), 1, osdf); fclose(osdf); }
    // PlaySound(NULL, 0, 0); InitSoundMem(); DxLib_End();
    /*StopSound();
    InitSoundMem();
    DxLib_End();*/
}
