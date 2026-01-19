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
namespace fs = std::filesystem;

cScene::cScene() : mNowScene(HOME), mNextScene(NO_CHANGE), nowTime(time(NULL)) {
    loadColor();
    loadFont();
    loadScreen();
    timeError = localtime_s(&nowLocalTime, &nowTime);
    // load team color
    for (int i = 0; i < cTeam::MAX_SOLO_PLAYER_NUM; i++) teamColor[i] = cColor::instance()->teamColor(i);
}

void cScene::loadColor() {
    white = cColor::instance()->white(); black = cColor::instance()->black();
    gray = cColor::instance()->gray(); red = cColor::instance()->red();
    green = cColor::instance()->green(); blue = cColor::instance()->blue();
    magenta = cColor::instance()->magenta(); cyan = cColor::instance()->cyan();
    yellow = cColor::instance()->yellow(); tableColor = cColor::instance()->tableColor();
    touchColor = cColor::instance()->touchColor(); pressColor = cColor::instance()->pressColor();
    executeColor = cColor::instance()->executeColor();
    for (int i = 0; i < cTeam::MAX_SOLO_PLAYER_NUM; i++) {
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

void cScene::initCtrlKey() {
    sd.ctrl.home.keyCode = KEY_INPUT_H;      sd.ctrl.back.keyCode = KEY_INPUT_BACK;
    sd.ctrl.forward.keyCode = KEY_INPUT_RETURN; sd.ctrl.quit.keyCode = KEY_INPUT_ESCAPE;
    sd.ctrl.config.keyCode = KEY_INPUT_C;      sd.ctrl.skill.keyCode = KEY_INPUT_E;
    sd.ctrl.skip.keyCode = KEY_INPUT_S;      sd.ctrl.init.keyCode = KEY_INPUT_I;
    sd.ctrl.gameSelect.keyCode = KEY_INPUT_G; sd.ctrl.playerSelect.keyCode = KEY_INPUT_P;
    sd.ctrl.reset.keyCode = KEY_INPUT_R; sd.ctrl.bgm.keyCode = KEY_INPUT_B;
    sd.ctrl.left.keyCode = KEY_INPUT_LEFT;   sd.ctrl.right.keyCode = KEY_INPUT_RIGHT;
    sd.ctrl.up.keyCode = KEY_INPUT_UP;     sd.ctrl.down.keyCode = KEY_INPUT_DOWN;
    sd.ctrl.start.keyCode = KEY_INPUT_SPACE;  sd.ctrl.yes.keyCode = KEY_INPUT_Y;
    sd.ctrl.no.keyCode = KEY_INPUT_N;
    for (int i = 0; i < 2; i++) {
        sd.ctrl.mute[i].keyCode = KEY_INPUT_M;
        sd.ctrl.window[i].keyCode = KEY_INPUT_W;
        sd.ctrl.pause[i].keyCode = KEY_INPUT_PAUSE;
    }
}

void cScene::initScreenSize() {
    cScreen::instance()->init();
    loadScreen();
    changeWindow(sd.window);
    // set icon
    cCoordinate2d<int> DEFAULT_ICON_SIZE(DEFAULT_ICON_WIDTH, DEFAULT_ICON_HEIGHT);
    sd.ctrl.home.icon.box().setSize(30, DEFAULT_ICON_HEIGHT);
    sd.ctrl.home.icon.box().setUpperLeft(screen.upperLeft());
    sd.ctrl.back.icon.box().setSize(DEFAULT_ICON_SIZE);
    sd.ctrl.back.icon.box().setUpperLeft(sd.ctrl.home.icon.box().upperRight());
    sd.ctrl.forward.icon.box().setSize(DEFAULT_ICON_SIZE);
    sd.ctrl.forward.icon.box().setUpperLeft(sd.ctrl.back.icon.box().upperRight());
    for (int i = 0; i < 2; i++) {
        sd.ctrl.mute[i].icon.box().setSize(DEFAULT_ICON_SIZE);
        sd.ctrl.mute[i].icon.box().setUpperLeft(sd.ctrl.forward.icon.box().upperRight());
    }
    sd.ctrl.quit.icon.box().setSize(DEFAULT_ICON_SIZE); sd.ctrl.quit.icon.box().setUpperRight(
        screen.upperRight());
    for (int i = 0; i < 2; i++) {
        sd.ctrl.window[i].icon.box().setSize(DEFAULT_ICON_SIZE);
        sd.ctrl.window[i].icon.box().setUpperRight(sd.ctrl.quit.icon.box().upperLeft());
    }
    sd.ctrl.config.icon.box().setSize(DEFAULT_ICON_SIZE);
    sd.ctrl.config.icon.box().setUpperRight(sd.ctrl.window[0].icon.box().upperLeft());
    sd.ctrl.gameSelect.icon.box().setSize(DEFAULT_ICON_SIZE);
    sd.ctrl.gameSelect.icon.box().setUpperRight(sd.ctrl.config.icon.box().upperLeft());
    sd.ctrl.playerSelect.icon.box().setSize(DEFAULT_ICON_SIZE);
    sd.ctrl.playerSelect.icon.box().setUpperRight(sd.ctrl.gameSelect.icon.box().upperLeft());
    for (int i = 0; i < 2; i++) {
        sd.ctrl.pause[i].icon.box().setSize(DEFAULT_ICON_SIZE);
        sd.ctrl.pause[i].icon.box().setUpperRight(sd.ctrl.playerSelect.icon.box().upperLeft());
    }
    sd.ctrl.skill.icon.box().setSize(DEFAULT_ICON_SIZE);
    sd.ctrl.skill.icon.box().setUpperRight(sd.ctrl.pause[0].icon.box().upperLeft());
    sd.ctrl.skip.icon.box().setSize(76, DEFAULT_ICON_HEIGHT);
    sd.ctrl.skip.icon.box().setLowerRight(lowerFrame.upperRight());
    sd.ctrl.init.icon.box().setSize(DEFAULT_ICON_SIZE);
    sd.ctrl.init.icon.box().setLowerLeft(screen.lowerLeft());
    sd.ctrl.reset.icon.box().setSize(DEFAULT_ICON_SIZE);
    sd.ctrl.reset.icon.box().setLowerLeft(sd.ctrl.init.icon.box().lowerRight());
    sd.ctrl.bgm.icon.box().setSize(DEFAULT_ICON_SIZE);
    sd.ctrl.bgm.icon.box().setLowerLeft(sd.ctrl.reset.icon.box().lowerRight());
    // set image
    sd.selected.box().setSize(100, 86);
    sd.darts.box().setSize(900, 600);
    sd.darts.box().setUpperLeft(0, 0);
    sd.thunder.box().setSize(900, 945);
    sd.thunder.box().setUpperLeft(-50, -200);
}

void cScene::init() {
    initCtrlKey();
    initScreenSize();
    // load color
    if (mNowScene == NO_CHANGE) {
        int cpp = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG92), "PNG");
        int dxlib = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG93), "PNG");
        cColor::instance()->load();
        loadColor();
        // draw start screen
        ClearDrawScreen();
        DrawStringToHandle(100, 50, "Powered by", white, XLfont);
        DrawGraph(200, 100 + XLfontSize, cpp, TRUE);
        DrawGraph(500, 100 + XLfontSize, dxlib, TRUE);
        DrawStringToHandle(screen.right() - 180, screen.bottom() - MfontSize - 10,
            "now loading...", white, Mfont);
        ScreenFlip();
        sd.ctrl.home.name = "Home"; sd.ctrl.back.name = "Back"; sd.ctrl.forward.name = "Forward";
        sd.ctrl.mute[0].name = "Unmute"; sd.ctrl.mute[1].name = "Mute"; sd.ctrl.quit.name = "Quit";
        sd.ctrl.window[0].name = "Another window"; sd.ctrl.window[1].name = "Maximize window";
        sd.ctrl.pause[0].name = "Pause"; sd.ctrl.pause[1].name = "Resume"; sd.ctrl.config.name = "Config";
        sd.ctrl.playerSelect.name = "Player select"; sd.ctrl.gameSelect.name = "Game select";
        sd.ctrl.skill.name = "Skill"; sd.ctrl.skip.name = "Skip"; sd.ctrl.init.name = "Initialize";
        sd.ctrl.reset.name = "Reset"; sd.ctrl.bgm.name = "Change BGM";
        // load BGM
        cSound::instance()->load();
        cDarts::instance()->setCenter(screen.left() + 0.25 * screen.width() + 5, screen.center().y());
    }
    cSound::instance()->initSoundVol();
    StopSound();
    cSound::instance()->playBGM(0);
    cGame::instance()->init();
    sd.teams.clear();
    sd.teams.reserve(MAX_GROUP_NUM);
    mNextScene = HOME;
}

void cScene::changeWindow(int WindowModeFlag) {
    ChangeWindowMode(WindowModeFlag);
    SetDrawScreen(DX_SCREEN_BACK);
    SetMouseDispFlag(TRUE);
    // load font
    cFont::instance()->load();
    loadFont();
    cKeyboard::instance()->loadKeyImage();

    // load players
    if (sd.groups.size() > 0) {
        for (int group = 0; group < sd.groups.size(); group++) {
            for (int member = 0; member < sd.groups.at(group).members.size(); member++) {
                sd.groups.at(group).members.at(member).image.reload();
            }
        }
    }
    else {
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
    if (nTeam > 0) {
        for (int team = 0; team < sd.teams.size(); team++) {
            for (int member = 0; member < sd.teams.at(team).members.size(); member++) {
                sd.teams.at(team).members.at(member).image.reload();
            }
        }
    }

    sd.ctrl.left.icon.load(IDB_PNG68, "PNG");
    sd.ctrl.right.icon.load(IDB_PNG69, "PNG");
    sd.ctrl.skill.icon.load(IDB_PNG70, "PNG");
    sd.ctrl.home.icon.load(IDB_PNG71, "PNG");
    sd.ctrl.init.icon.load(IDB_PNG72, "PNG");
    sd.ctrl.gameSelect.icon.load(IDB_PNG73, "PNG");
    sd.ctrl.playerSelect.icon.load(IDB_PNG74, "PNG");
    sd.ctrl.reset.icon.load(IDB_PNG75, "PNG");
    sd.ctrl.skip.icon.load(IDB_PNG76, "PNG");
    sd.ctrl.quit.icon.load(IDB_PNG77, "PNG");
    sd.ctrl.config.icon.load(IDB_PNG78, "PNG");
    sd.ctrl.bgm.icon.load(IDB_PNG79, "PNG");
    for (int i = 0; i < 2; i++) {
        sd.ctrl.window[i].icon.load(IDB_PNG80 + i, "PNG");
        sd.ctrl.mute[i].icon.load(IDB_PNG82 + i, "PNG");
        sd.ctrl.pause[i].icon.load(IDB_PNG84 + i, "PNG");
    }
    sd.ctrl.back.icon.load(IDB_PNG86, "PNG");
    sd.ctrl.forward.icon.load(IDB_PNG87, "PNG");
    sd.ctrl.down.icon.load(IDB_PNG88, "PNG");
    sd.ctrl.up.icon.load(IDB_PNG89, "PNG");
    sd.darts.load(IDB_JPG1, "JPG");
    sd.selected.load(IDB_PNG90, "PNG");
    sd.thunder.load(IDB_PNG91, "PNG");
    cDarts::instance()->loadImage();
    sd.window = GetWindowModeFlag();
}

void cScene::reset() {
}

void cScene::draw() {
    sd.darts.draw(); sd.thunder.draw();
    sd.ctrl.home.icon.draw(); sd.ctrl.back.icon.draw();
    sd.ctrl.mute[cSound::instance()->isBGMPlayed()].icon.draw(); 
    sd.ctrl.config.icon.draw(); sd.ctrl.window[sd.window].icon.draw();
    sd.ctrl.quit.icon.draw(); sd.ctrl.init.icon.draw(); sd.ctrl.reset.icon.draw(); sd.ctrl.bgm.icon.draw();
    DrawStringToHandle(
        sd.ctrl.bgm.icon.box().right(), cScreen::instance()->lowerFrame().center().y() - SfontSize / 2,
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
void cScene::fin() {
}
void cScene::update() {
    cMouse::instance()->update();
    cKeyboard::instance()->update();
    cSound::instance()->update();
    nowTime = time(NULL);
    timeError = localtime_s(&nowLocalTime, &nowTime);
    if (ctrlRQ(sd.ctrl.init)) init();
    else if (ctrlRQ(sd.ctrl.reset)) reset();
    else if (ctrlRQ(sd.ctrl.quit)) mNextScene = QUIT;
    else if (ctrlRQ(sd.ctrl.mute[cSound::instance()->isBGMPlayed()])) {
        if (cSound::instance()->isBGMPlayed()) {
            cSound::instance()->mute();
            return;
        }
        cSound::instance()->unmute();
    }
    else if (ctrlRQ(sd.ctrl.window[sd.window])) changeWindow((sd.window + 1) % 2);
    else if (ctrlRQ(sd.ctrl.bgm)) {
        if (cKeyboard::instance()->pressKeyState(KEY_INPUT_LSHIFT) == sKey::PRESSED) {
            cSound::instance()->playLastBGM();
        }
        else {
            cSound::instance()->playNextBGM();
        }
    }
}

bool cScene::isClicked(cBox box) {
    return cMouse::instance()->clickBoxState(box) == sKey::PRESSEDtoRELEASED;
}

bool cScene::isBoxClicked(int x1, int y1, int x2, int y2) {
    return cMouse::instance()->clickBoxState(x1, y1, x2, y2) == sKey::PRESSEDtoRELEASED;
}

bool cScene::isClicked(cImage image) {
    return isClicked(image.box());
}

bool cScene::isKeyTyped(int keyCode) {
    return cKeyboard::instance()->pressKeyState(keyCode) == sKey::RELEASEDtoPRESSED;
}

bool cScene::isTyped(sCtrl ctrl) {
    return isKeyTyped(ctrl.keyCode);
}

bool cScene::ctrlRQ(sCtrl ctrl) {
    return isClicked(ctrl.icon) || isTyped(ctrl);
}

cScene::~cScene() {
    FILE* osdf; errno_t error = fopen_s(&osdf, ShareDataFileName, "wb+"); // open data file
    if (!error) { fwrite(&sd, sizeof(sd), 1, osdf); fclose(osdf); }
    PlaySound(NULL, 0, 0); InitSoundMem(); DxLib_End();
}
