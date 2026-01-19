#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "winmm.lib")   // MSVC —p
#include "Scene.hpp"
#include <random>
#include <filesystem>
#include <iostream>
#include "resource.h"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "Sound.hpp"
#include "Darts.hpp"
#include "Color.hpp"
#include "Font.hpp"
#include "Game.hpp"
#include "Screen.hpp"
#include "Control.hpp"
namespace fs = std::filesystem;

cScene::cScene() : mNowScene(HOME), mNextScene(NO_CHANGE), nowTime(time(NULL)) {
    loadColor(); loadFont(); loadScreen();
    timeError = localtime_s(&nowLocalTime, &nowTime);
}

void cScene::loadColor() {
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

void cScene::loadFont() {
    Sfont = cFont::instance()->Sfont(); SfontSize = cFont::instance()->SfontSize();
    Mfont = cFont::instance()->MFont(); MfontSize = cFont::instance()->MfontSize();
    XLfont = cFont::instance()->XLfont(); XLfontSize = cFont::instance()->XLfontSize();
}

void cScene::loadScreen() {
    screen = cScreen::instance()->box();
    upperFrame = cScreen::instance()->upperFrame();
    lowerFrame = cScreen::instance()->lowerFrame();
}

void cScene::initScreenSize() {
    cScreen::instance()->init();
    cDarts::instance()->loadScreen();
    loadScreen();
    setWindow(FALSE);
    // set icon
    cControl::instance()->initIconBox();
    // set image
    sd.selected.box().setSize(100, 86);
    sd.darts.box().setSize(900, 600);
    sd.darts.box().setUpperLeft(0, 0);
    sd.thunder.box().setSize(900, 945);
    sd.thunder.box().setUpperLeft(-50, -200);
}

void cScene::init() {
    initScreenSize();
    if (mNowScene == NO_CHANGE) {
        int cpp = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG92), "PNG");
        int dxlib = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG93), "PNG");
        cColor::instance()->load();
		cDarts::instance()->loadColor();
        loadColor();
        // draw start screen
        ClearDrawScreen();
        DrawStringToHandle(100, 50, "Powered by", white, XLfont);
        DrawGraph(200, 100 + XLfontSize, cpp, TRUE);
        DrawGraph(500, 100 + XLfontSize, dxlib, TRUE);
        DrawStringToHandle(screen.right() - 180, screen.bottom() - MfontSize - 10,
            "now loading...", white, Mfont);
        ScreenFlip();
        cSound::instance()->load();
        cDarts::instance()->setCenter(screen.left() + 0.25 * screen.width() + 5, screen.center().y());
    }
    cControl::instance()->initKey();
    cSound::instance()->initSoundVol();
    StopSound();
    cSound::instance()->playBGM(0);
    cGame::instance()->init();
    sd.teams.clear();
    sd.teams.reserve(MAX_GROUP_NUM);
    mNextScene = HOME;
}

void cScene::changeWindow() {
    if (GetWindowModeFlag() == TRUE) setWindow(FALSE);
    else setWindow(TRUE);
}

void cScene::setWindow(int window) {
    ChangeWindowMode(window);
    SetDrawScreen(DX_SCREEN_BACK);
    SetMouseDispFlag(TRUE);
    cFont::instance()->load();
    cDarts::instance()->loadFont();
    loadFont();

    if (sd.groups.size() > 0) {
		// reload images
        cKeyboard::instance()->reloadKeyImage();
        cControl::instance()->reloadIcon();
        sd.darts.reload();
        sd.selected.reload();
        sd.thunder.reload();
        for (int group = 0; group < sd.groups.size(); group++) {
            for (int member = 0; member < sd.groups.at(group).members.size(); member++) {
                sd.groups.at(group).members.at(member).image.reload();
            }
        }
    }
    else {
		// load images
        cKeyboard::instance()->loadKeyImage();
        cControl::instance()->loadIcon();
        sd.darts.load(IDB_JPG1, "JPG");
        sd.selected.load(IDB_PNG90, "PNG");
        sd.thunder.load(IDB_PNG91, "PNG");
        std::error_code err;
        sd.groups.clear();
        sd.groups.push_back(sGroup("Guest"));
        for (fs::directory_iterator iter(playerFolderPath + "/"), end;
            iter != end && !err && sd.groups.size() < MAX_GROUP_NUM; iter.increment(err)) {
            const fs::directory_entry entry = *iter;
            // if found path is valid folder,
            if (!entry.path().has_extension() && entry.path().filename().string() != "Guest") {
                sd.groups.push_back(sGroup(entry.path().filename().string())); // get group name
            }
        }
        for (int group = 0, chara = 0; group < sd.groups.size(); group++) {
            for (fs::directory_iterator iter(playerFolderPath + "/" + sd.groups[group].name + "/"), end;
                iter != end && !err && chara < MAX_CHARA_NUM; iter.increment(err), chara++) {
                const fs::directory_entry entry = *iter;
                std::string extension = entry.path().extension().string();
                if (extension == ".jpg" || extension == ".png") { // if found file is image,
                    std::string name = entry.path().filename().string(),
                        path = entry.path().string();
                    name.erase(name.length() - extension.length(), extension.length());
                    sd.groups.at(group).members.push_back(sChara());
                    sd.groups.at(group).members.back().name = name;
                    sd.groups.at(group).members.back().image.load(path);
                    sd.groups.at(group).members.back().group = sd.groups.at(group).name;
                }
            }
        }
        sd.groups.shrink_to_fit();
        if (err) {
            std::cout << err.value() << std::endl;
            std::cout << err.message() << std::endl;
        }
    }
    if (sd.teams.size() > 0) {
        for (int team = 0; team < sd.teams.size(); team++) {
            for (int member = 0; member < sd.teams.at(team).members.size(); member++) {
                sd.teams.at(team).members.at(member).image.reload();
            }
        }
    }
    cDarts::instance()->loadImage();
}

void cScene::draw() {
    sd.darts.draw(); sd.thunder.draw();
    cControl::instance()->icon(cControl::HOME).draw(); 
    cControl::instance()->icon(cControl::BACK).draw();
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

void cScene::update() {
    cMouse::instance()->update();
    cKeyboard::instance()->update();
    cSound::instance()->update();
    nowTime = time(NULL);
    timeError = localtime_s(&nowLocalTime, &nowTime);
    if (cControl::instance()->isRequested(cControl::INITIALIZE)) init();
    else if (cControl::instance()->isRequested(cControl::RESET)) reset();
    else if (cControl::instance()->isRequested(cControl::QUIT)) mNextScene = QUIT;
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

cScene::~cScene() {
    FILE* osdf; errno_t error = fopen_s(&osdf, ShareDataFileName, "wb+"); // open data file
    if (!error) { fwrite(&sd, sizeof(sd), 1, osdf); fclose(osdf); }
    PlaySound(NULL, 0, 0); InitSoundMem(); DxLib_End();
}
