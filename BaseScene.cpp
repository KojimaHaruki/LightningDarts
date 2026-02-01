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

cBaseScene::cBaseScene() : nowTime(time(NULL)), nTeam(cPlayer::inst()->nTeam()){
    loadColor(); loadFont(); loadScreen();
    timeError = localtime_s(&nowLocalTime, &nowTime);
}

void cBaseScene::loadColor() {
    white = cColor::inst()->white(); black = cColor::inst()->black();
    gray = cColor::inst()->gray(); red = cColor::inst()->red();
    green = cColor::inst()->green(); blue = cColor::inst()->blue();
    magenta = cColor::inst()->magenta(); cyan = cColor::inst()->cyan();
    yellow = cColor::inst()->yellow(); tableColor = cColor::inst()->tableColor();
    touchColor = cColor::inst()->touchColor(); pressColor = cColor::inst()->pressColor();
    executeColor = cColor::inst()->executeColor();
    for (int i = 0; i < cPlayer::MAX_SOLO_PLAYER_NUM; i++) {
        teamColor[i] = cColor::inst()->teamColor(i);
        rankColor[i] = cColor::inst()->rankColor(i);
    }
}

void cBaseScene::loadFont() {
    Sfont = cFont::inst()->Sfont(); SfontSize = cFont::inst()->SfontSize();
    Mfont = cFont::inst()->MFont(); MfontSize = cFont::inst()->MfontSize();
    Lfont = cFont::inst()->Lfont(); LfontSize = cFont::inst()->LfontSize();
    XLfont = cFont::inst()->XLfont(); XLfontSize = cFont::inst()->XLfontSize();
}

void cBaseScene::loadScreen() {
    screen = cScreen::inst()->box();
    upperFrame = cScreen::inst()->upperFrame();
    lowerFrame = cScreen::inst()->lowerFrame();
}

void cBaseScene::initScreenSize() {
    cScreen::inst()->initSize();
    loadScreen();
    setWindow(FALSE);
    cControl::inst()->initIconBox();
}

void cBaseScene::init() {
    initScreenSize();
    cSound::inst()->init();
    cControl::inst()->initKey();
    cGame::inst()->init();
    cPlayer::inst()->initTeam();
    cScene::inst()->setScene(cScene::HOME);
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
    cFont::inst()->load();
    cDarts::inst()->loadFont();
    loadFont();

    // reload images
    cKeyboard::inst()->reloadKeyImage();
    cControl::inst()->reloadIcon();
    cScreen::inst()->reloadImage();
    cPlayer::inst()->loadImage();
    cDarts::inst()->loadImage();
}

void cBaseScene::draw() {
	cScreen::inst()->draw();
    cControl::inst()->icon(cControl::BACK).draw();
    for (int scene = cScene::HOME; scene <= cScene::PLAYER_SELECT; scene++) {
        if (scene <= cScene::inst()->maxScene()) {
            cControl::inst()->icon(scene).draw();
        }
    }
    cControl::inst()->icon(cControl::MUTE + cSound::inst()->isBGMPlayed()).draw();
    cControl::inst()->icon(cControl::CONFIG).draw();
    cControl::inst()->icon(cControl::ANOTHER_WINDOW + GetWindowModeFlag()).draw();
    cControl::inst()->icon(cControl::QUIT).draw();
    cControl::inst()->icon(cControl::INITIALIZE).draw();
    cControl::inst()->icon(cControl::RESET).draw();
    cControl::inst()->icon(cControl::CHANGE_BGM).draw();
    DrawStringToHandle(cControl::inst()->icon(cControl::CHANGE_BGM).box().R(),
        cScreen::inst()->lowerFrame().C().y() - SfontSize / 2,
        cSound::inst()->playingBGMName().c_str(), white, Sfont);
    DrawStringToHandle(screen.R() - 340, lowerFrame.C().y() - SfontSize / 2,
        "Lightning Darts C 2025 Haruki Kojima", yellow, Sfont);
    DrawCircleAA(screen.R() - 210.5f, (float)lowerFrame.C().y(),
        SfontSize / 2.0f, 1000, black, FALSE, 3.0f);
    DrawCircleAA(screen.R() - 210.5f, (float)lowerFrame.C().y(),
        SfontSize / 2.0f, 1000, yellow, FALSE, 1.0f);
    if (!timeError) {
        DrawFormatStringToHandle(
            screen.R() - 42, lowerFrame.C().y() - SfontSize / 2,
            white, Sfont, "%02d:%02d", nowLocalTime.tm_hour, nowLocalTime.tm_min);
    }
}

void cBaseScene::update() {
    cMouse::inst()->update();
    cKeyboard::inst()->update();
    cSound::inst()->update();
    nowTime = time(NULL);
    timeError = localtime_s(&nowLocalTime, &nowTime);
    for (int scene = cScene::HOME; scene <= cScene::PLAYER_SELECT; scene++) {
        if (scene <= cScene::inst()->maxScene() && scene != cScene::inst()->currentScene() &&
            cControl::inst()->isRequested(scene)) {
            cScene::inst()->setScene(scene); return;
        }
    }
    if (cControl::inst()->isRequested(cControl::INITIALIZE)) init();
    else if (cControl::inst()->isRequested(cControl::RESET)) reset();
    else if (cControl::inst()->isRequested(cControl::QUIT)) 
        cScene::inst()->setScene(cScene::QUIT);
    else if (cControl::inst()->isRequested(cControl::MUTE + cSound::inst()->isBGMPlayed())) {
        if (cSound::inst()->isBGMPlayed()) {
            cSound::inst()->mute();
            return;
        }
        cSound::inst()->unmute();
    }
    else if (cControl::inst()->isRequested(cControl::ANOTHER_WINDOW + GetWindowModeFlag()))
        changeWindow();
    else if (cControl::inst()->isRequested(cControl::CHANGE_BGM)) {
        if (cKeyboard::inst()->keyState(KEY_INPUT_LSHIFT) == sKeyState::PRESS) {
            cSound::inst()->playLastBGM();
        }
        else {
            cSound::inst()->playNextBGM();
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
