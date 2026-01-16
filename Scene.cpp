#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "winmm.lib")   // MSVC —p
#include "Scene.hpp"
#include <random>
#include <filesystem>
#include <string>
#include "resource.h"
#include "Mouse.hpp"
#include "Sound.hpp"
#include "Darts.hpp"
#include "Color.hpp"
#include "Font.hpp"
namespace fs = std::filesystem;

cScene::cScene() : mNowScene(HOME), mNextScene(NO_CHANGE), mIsConfig(false), nowTime(time(NULL)),
mGame(cDarts::instance()->gameNo()) {
    loadColor();
    loadFont();
    timeError = localtime_s(&nowLocalTime, &nowTime);
    ICONSIZE_NORMAL.setXY(25, 25);
    // load team color
    for (int i = 0; i < MAX_PLAYER_NUM; i++) teamColor[i] = cColor::instance()->teamColor(i);
}

void cScene::loadColor() {
    white = cColor::instance()->white(); black = cColor::instance()->black(); 
    gray = cColor::instance()->gray(); red = cColor::instance()->red(); 
    green = cColor::instance()->green(); blue = cColor::instance()->blue();
    magenta = cColor::instance()->magenta(); cyan = cColor::instance()->cyan();
    yellow = cColor::instance()->yellow(); tableColor = cColor::instance()->tableColor();
    touchColor = cColor::instance()->touchColor(); pressColor = cColor::instance()->pressColor();
    executeColor = cColor::instance()->executeColor(); 
    for (int i = 0; i < MAX_PLAYER_NUM; i++) {
        teamColor[i] = cColor::instance()->teamColor(i);
        rankColor[i] = cColor::instance()->rankColor(i);
    }
}

void cScene::loadFont() {
    Sfont = cFont::instance()->Sfont(); SfontSize = cFont::instance()->SfontSize();
    Mfont = cFont::instance()->MFont(); MfontSize = cFont::instance()->MfontSize();
    XLfont = cFont::instance()->XLfont(); XLfontSize = cFont::instance()->XLfontSize();
}

void cScene::initCtrlKey() {
    sd.ctrl.home.key.code = KEY_INPUT_H;      sd.ctrl.back.key.code = KEY_INPUT_BACK;
    sd.ctrl.forward.key.code = KEY_INPUT_RETURN; sd.ctrl.quit.key.code = KEY_INPUT_ESCAPE;
    sd.ctrl.config.key.code = KEY_INPUT_C;      sd.ctrl.skill.key.code = KEY_INPUT_E;
    sd.ctrl.skip.key.code = KEY_INPUT_S;      sd.ctrl.init.key.code = KEY_INPUT_I;
    sd.ctrl.gameSelect.key.code = KEY_INPUT_G; sd.ctrl.playerSelect.key.code = KEY_INPUT_P;
    sd.ctrl.reset.key.code = KEY_INPUT_R; sd.ctrl.bgm.key.code = KEY_INPUT_B;
    sd.ctrl.left.key.code = KEY_INPUT_LEFT;   sd.ctrl.right.key.code = KEY_INPUT_RIGHT;
    sd.ctrl.up.key.code = KEY_INPUT_UP;     sd.ctrl.down.key.code = KEY_INPUT_DOWN;
    sd.ctrl.start.key.code = KEY_INPUT_SPACE;  sd.ctrl.yes.key.code = KEY_INPUT_Y;
    sd.ctrl.no.key.code = KEY_INPUT_N;
    for (int i = 0; i < 2; i++) {
        sd.ctrl.mute[i].key.code = KEY_INPUT_M;
        sd.ctrl.window[i].key.code = KEY_INPUT_W;
        sd.ctrl.pause[i].key.code = KEY_INPUT_SPACE;
    }
}

void cScene::initScreenSize() {
    sd.screen.setSize(900, 500); sd.screen.setUpperLeft(0, 0);
    changeWindow(sd.window);
    // set frame
    sd.obj.upperFrame.setSize(sd.screen.width(), ICONSIZE_NORMAL.y());
    sd.obj.upperFrame.setUpperLeft(sd.screen.upperLeft());
    sd.obj.lowerFrame.setSize(sd.screen.width(), ICONSIZE_NORMAL.y());
    sd.obj.lowerFrame.setLowerLeft(sd.screen.lowerLeft());
    // set icon
    sd.ctrl.home.icon.box.setSize(30, 25); sd.ctrl.home.icon.box.setUpperLeft(sd.screen.upperLeft());
    sd.ctrl.back.icon.box.setSize(ICONSIZE_NORMAL);
    sd.ctrl.back.icon.box.setUpperLeft(sd.ctrl.home.icon.box.upperRight());
    sd.ctrl.forward.icon.box.setSize(ICONSIZE_NORMAL);
    sd.ctrl.forward.icon.box.setUpperLeft(sd.ctrl.back.icon.box.upperRight());
    for (int i = 0; i < 2; i++) {
        sd.ctrl.mute[i].icon.box.setSize(ICONSIZE_NORMAL);
        sd.ctrl.mute[i].icon.box.setUpperLeft(sd.ctrl.forward.icon.box.upperRight());
    }
    sd.ctrl.quit.icon.box.setSize(ICONSIZE_NORMAL); sd.ctrl.quit.icon.box.setUpperRight(sd.screen.upperRight());
    for (int i = 0; i < 2; i++) {
        sd.ctrl.window[i].icon.box.setSize(ICONSIZE_NORMAL);
        sd.ctrl.window[i].icon.box.setUpperRight(sd.ctrl.quit.icon.box.upperLeft());
    }
    sd.ctrl.config.icon.box.setSize(ICONSIZE_NORMAL);
    sd.ctrl.config.icon.box.setUpperRight(sd.ctrl.window[0].icon.box.upperLeft());
    sd.ctrl.gameSelect.icon.box.setSize(ICONSIZE_NORMAL);
    sd.ctrl.gameSelect.icon.box.setUpperRight(sd.ctrl.config.icon.box.upperLeft());
    sd.ctrl.playerSelect.icon.box.setSize(ICONSIZE_NORMAL);
    sd.ctrl.playerSelect.icon.box.setUpperRight(sd.ctrl.gameSelect.icon.box.upperLeft());
    for (int i = 0; i < 2; i++) {
        sd.ctrl.pause[i].icon.box.setSize(ICONSIZE_NORMAL);
        sd.ctrl.pause[i].icon.box.setUpperRight(sd.ctrl.playerSelect.icon.box.upperLeft());
    }
    sd.ctrl.skill.icon.box.setSize(ICONSIZE_NORMAL);
    sd.ctrl.skill.icon.box.setUpperRight(sd.ctrl.pause[0].icon.box.upperLeft());
    sd.ctrl.skip.icon.box.setSize(76, 25);
    sd.ctrl.skip.icon.box.setLowerRight(sd.obj.lowerFrame.upperRight());
    sd.ctrl.init.icon.box.setSize(ICONSIZE_NORMAL);
    sd.ctrl.init.icon.box.setLowerLeft(sd.screen.lowerLeft());
    sd.ctrl.reset.icon.box.setSize(ICONSIZE_NORMAL);
    sd.ctrl.reset.icon.box.setLowerLeft(sd.ctrl.init.icon.box.lowerRight());
    sd.ctrl.bgm.icon.box.setSize(ICONSIZE_NORMAL);
    sd.ctrl.bgm.icon.box.setLowerLeft(sd.ctrl.reset.icon.box.lowerRight());
    // set image
    sd.pic.selected.image.box.setSize(100, 86);
    sd.pic.darts.image.box.setSize(900, 600);
    sd.pic.darts.image.box.setUpperLeft(0, 0);
    sd.pic.thunder.image.box.setSize(900, 945);
    sd.pic.thunder.image.box.setUpperLeft(-50, -200);
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
        DrawStringToHandle(sd.screen.right() - 180, sd.screen.bottom() - MfontSize - 10,
            "now loading...", white, Mfont);
        ScreenFlip();
        sd.ctrl.home.name = "Home"; sd.ctrl.back.name = "Back"; sd.ctrl.forward.name = "Forward";
        sd.ctrl.mute[0].name = "Unmute"; sd.ctrl.mute[1].name = "Mute"; sd.ctrl.quit.name = "Quit";
        sd.ctrl.window[0].name = "Another window"; sd.ctrl.window[1].name = "Maximize window";
        sd.ctrl.pause[0].name = "Pause"; sd.ctrl.pause[1].name = "Resume"; sd.ctrl.config.name = "Config";
        sd.ctrl.playerSelect.name = "Restart from Player select"; sd.ctrl.gameSelect.name = "Restart from Game select";
        sd.ctrl.skill.name = "Skill"; sd.ctrl.skip.name = "Skip"; sd.ctrl.init.name = "Initialize";
        sd.ctrl.reset.name = "Reset the current scene"; sd.ctrl.bgm.name = "Change BGM";
        // load BGM
        Sound::instance()->load();
        cDarts::instance()->setCenter(
            sd.screen.left() + 0.25 * sd.screen.width() + 5, sd.screen.center().y());
    }
    Sound::instance()->initSoundVol();
    StopSound();
    Sound::instance()->playBGM(0);
    // set game
    cDarts::instance()->setGameNo(cDarts::sGame::DEFAULT);
    // set player
    sd.teamType = sTeamType::SOLO;
    sd.teams.clear();
    sd.teams.reserve(MAX_GROUP_NUM);
    mNextScene = HOME;
}

void cScene::changeWindow(int WindowModeFlag) {
    if (!WindowModeFlag) SetGraphMode(sd.screen.width(), sd.screen.height(), 32); // change screen size
    ChangeWindowMode(WindowModeFlag);
    SetDrawScreen(DX_SCREEN_BACK);
    SetMouseDispFlag(TRUE);
    // load font
    cFont::instance()->load();
    loadFont();
    // load image
    for (int i = 0; i < VALID_KEY_NUM; i++)
        sd.key[KeyNo[i]].image.handle = LoadGraphToResource(MAKEINTRESOURCE(KeyNo[i]), "PNG");
    sd.ctrl.home.key.image.handle = sd.key[sd.ctrl.home.key.code].image.handle;
    sd.ctrl.back.key.image.handle = sd.key[sd.ctrl.back.key.code].image.handle;
    sd.ctrl.forward.key.image.handle = sd.key[sd.ctrl.forward.key.code].image.handle;
    sd.ctrl.quit.key.image.handle = sd.key[sd.ctrl.quit.key.code].image.handle;
    sd.ctrl.config.key.image.handle = sd.key[sd.ctrl.config.key.code].image.handle;
    sd.ctrl.skill.key.image.handle = sd.key[sd.ctrl.skill.key.code].image.handle;
    sd.ctrl.skip.key.image.handle = sd.key[sd.ctrl.skip.key.code].image.handle;
    sd.ctrl.init.key.image.handle = sd.key[sd.ctrl.init.key.code].image.handle;
    sd.ctrl.gameSelect.key.image.handle = sd.key[sd.ctrl.gameSelect.key.code].image.handle;
    sd.ctrl.playerSelect.key.image.handle = sd.key[sd.ctrl.playerSelect.key.code].image.handle;
    sd.ctrl.reset.key.image.handle = sd.key[sd.ctrl.reset.key.code].image.handle;
    sd.ctrl.bgm.key.image.handle = sd.key[sd.ctrl.bgm.key.code].image.handle;
    sd.ctrl.left.key.image.handle = sd.key[sd.ctrl.left.key.code].image.handle;
    sd.ctrl.right.key.image.handle = sd.key[sd.ctrl.right.key.code].image.handle;
    sd.ctrl.up.key.image.handle = sd.key[sd.ctrl.up.key.code].image.handle;
    sd.ctrl.down.key.image.handle = sd.key[sd.ctrl.down.key.code].image.handle;
    sd.ctrl.start.key.image.handle = sd.key[sd.ctrl.start.key.code].image.handle;
    sd.ctrl.no.key.image.handle = sd.key[sd.ctrl.no.key.code].image.handle;
    sd.ctrl.yes.key.image.handle = sd.key[sd.ctrl.yes.key.code].image.handle;
    sd.ctrl.reset.key.image.handle = sd.key[sd.ctrl.reset.key.code].image.handle;
    for (int i = 0; i < 2; i++) {
        sd.ctrl.mute[i].key.image.handle = sd.key[sd.ctrl.mute[i].key.code].image.handle;
        sd.ctrl.window[i].key.image.handle = sd.key[sd.ctrl.window[i].key.code].image.handle;
        sd.ctrl.pause[i].key.image.handle = sd.key[sd.ctrl.pause[i].key.code].image.handle;
    }

    // load players
    if (sd.groups.size() > 0) {
        for (int group = 0; group < sd.groups.size(); group++) {
            for (int member = 0; member < sd.groups.at(group).members.size(); member++) {
                sd.groups.at(group).members.at(member).image.handle =
                    LoadGraph(sd.groups.at(group).members.at(member).path.c_str());
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
                    sd.groups.at(group).members.push_back(
                        sChara(name, path, sImage(cBox(), LoadGraph(path.c_str()), TRUE),
                            sd.groups.at(group).name, sCharaStatus()));
                }
            }
        }
        sd.groups.shrink_to_fit();
        if (err) {
            std::cout << err.value() << std::endl;
            std::cout << err.message() << std::endl;
        }
    }
    
    sd.ctrl.left.icon.handle = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG68), "PNG");
    sd.ctrl.right.icon.handle = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG69), "PNG");
    sd.ctrl.skill.icon.handle = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG70), "PNG");
    sd.ctrl.home.icon.handle = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG71), "PNG");
    sd.ctrl.init.icon.handle = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG72), "PNG");
    sd.ctrl.gameSelect.icon.handle = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG73), "PNG");
    sd.ctrl.playerSelect.icon.handle = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG74), "PNG");
    sd.ctrl.reset.icon.handle = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG75), "PNG");
    sd.ctrl.skip.icon.handle = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG76), "PNG");
    sd.ctrl.quit.icon.handle = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG77), "PNG");
    sd.ctrl.config.icon.handle = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG78), "PNG");
    sd.ctrl.bgm.icon.handle = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG79), "PNG");
    for (int i = 0; i < 2; i++) {
        sd.ctrl.window[i].icon.handle = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG80 + i), "PNG");
        sd.ctrl.mute[i].icon.handle = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG82 + i), "PNG");
        sd.ctrl.pause[i].icon.handle = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG84 + i), "PNG");
    }
    sd.ctrl.back.icon.handle = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG86), "PNG");
    sd.ctrl.forward.icon.handle = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG87), "PNG");
    sd.ctrl.down.icon.handle = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG88), "PNG");
    sd.ctrl.up.icon.handle = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG89), "PNG");
    sd.pic.darts.image.handle = LoadGraphToResource(MAKEINTRESOURCE(IDB_JPG1), "JPG");
    sd.pic.selected.image.handle = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG90), "PNG");
    sd.pic.thunder.image.handle = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG91), "PNG");
    cDarts::instance()->load();
    sd.window = GetWindowModeFlag(); return;
}

void cScene::reset() {
}

void cScene::draw() {
    drawImage(sd.pic.darts.image);
    drawImage(sd.pic.thunder.image);
    DrawStringToHandle(
        sd.ctrl.bgm.icon.box.right(), sd.obj.lowerFrame.center().y() - SfontSize / 2,
        Sound::instance()->playingBGMName().c_str(), white, Sfont);
    DrawStringToHandle(sd.screen.right() - 340, sd.obj.lowerFrame.center().y() - SfontSize / 2,
        "Lightning Darts C 2025 Haruki Kojima", yellow, Sfont);
    DrawCircleAA(sd.screen.right() - 210.5f, (float)sd.obj.lowerFrame.center().y(),
        SfontSize / 2.0f, 1000, black, FALSE, 3.0f);
    DrawCircleAA(sd.screen.right() - 210.5f, (float)sd.obj.lowerFrame.center().y(),
        SfontSize / 2.0f, 1000, yellow, FALSE, 1.0f);
    if (!timeError) {
        DrawFormatStringToHandle(
            sd.screen.right() - 42, sd.obj.lowerFrame.center().y() - SfontSize / 2,
            white, Sfont, "%02d:%02d", nowLocalTime.tm_hour, nowLocalTime.tm_min);
    }
    // draw icon & darts board
    switch (mNowScene) {
    case ZERO_ONE: case STANDARD_CRICKET: case COUNT_UP:
        drawImage(sd.ctrl.pause[cDarts::instance()->timer().isPaused()].icon);
        drawImage(sd.ctrl.skill.icon);
        if (!mIsConfig) {
            cDarts::instance()->timer().drawLapseTime(
                sd.screen.left(), sd.obj.upperFrame.bottom() + 10, white, Sfont, Timer::Mode::HMSmS);
            DrawStringToHandle(
                sd.ctrl.mute[0].icon.box.right() + 5, sd.obj.upperFrame.center().y() - MfontSize / 2,
                (gameName[cDarts::instance()->gameNo()] + " / " + teamTypeName[sd.teamType]).c_str(),
                white, Mfont);
            cDarts::instance()->draw();
        }
    case GAME_START:
    case PLAYER_SELECT:
        drawImage(sd.ctrl.playerSelect.icon);
    case GAME_SELECT:
        drawImage(sd.ctrl.gameSelect.icon);
        drawImage(sd.ctrl.skip.icon);
    default:
        drawImage(sd.ctrl.home.icon);
        drawImage(sd.ctrl.back.icon);
        drawImage(sd.ctrl.mute[Sound::instance()->isBGMPlayed()].icon);
        drawImage(sd.ctrl.config.icon);
        drawImage(sd.ctrl.window[sd.window].icon);
        drawImage(sd.ctrl.quit.icon);
        drawImage(sd.ctrl.init.icon);
        drawImage(sd.ctrl.reset.icon);
        drawImage(sd.ctrl.bgm.icon);
        break;
    }
    return;
}
void cScene::fin() {
}
void cScene::update() {
    Mouse::instance()->update();
    Keyboard::instance()->update();
    Sound::instance()->update();
    nowTime = time(NULL);
    timeError = localtime_s(&nowLocalTime, &nowTime);
    if (ctrlRQ(sd.ctrl.init)) init();
    else if (ctrlRQ(sd.ctrl.reset)) reset();
    else if (ctrlRQ(sd.ctrl.quit)) mNextScene = QUIT;
    else if (ctrlRQ(sd.ctrl.mute[Sound::instance()->isBGMPlayed()])) {
        if (Sound::instance()->isBGMPlayed()) {
            Sound::instance()->mute();
            return;
        }
        Sound::instance()->unmute();
    }
    else if (ctrlRQ(sd.ctrl.window[sd.window])) changeWindow((sd.window + 1) % 2);
    else if (ctrlRQ(sd.ctrl.bgm)) {
        if (Keyboard::instance()->getPressState(KEY_INPUT_LSHIFT) == Key::PRESSED) {
            Sound::instance()->playLastBGM();
        }
        else {
            Sound::instance()->playNextBGM();
        }
    }
    if (mIsConfig) {
        switch (mNowScene) {
        case ZERO_ONE: case STANDARD_CRICKET: case COUNT_UP:
        case GAME_START:
        case PLAYER_SELECT:
            if (ctrlRQ(sd.ctrl.playerSelect)) { mNextScene = PLAYER_SELECT; return; }
        case GAME_SELECT:
            if (ctrlRQ(sd.ctrl.gameSelect)) { mNextScene = GAME_SELECT;   return; }
        case HOME:
            if (ctrlRQ(sd.ctrl.home)) { mNextScene = HOME;          return; }
        default:
            break;
        }
    }
}

int cScene::drawBoxObj(cBox box, int color, int fill) {
    return DrawBox(box.left(), box.top(), box.right(), box.bottom(), color, fill);
}

int cScene::drawBoxObj(sBoxObj obj) {
    return DrawBox(obj.box.left(), obj.box.top(), obj.box.right(), obj.box.bottom(), obj.color, obj.fill);
}

int cScene::drawImage(sImage image) {
    return DrawGraph(image.box.left(), image.box.top(), image.handle, image.trans);
}

bool cScene::isClicked(cBox box) {
    return Mouse::instance()->getClickBoxState(box) == Key::PRESSEDtoRELEASED;
}

bool cScene::isBoxClicked(int x1, int y1, int x2, int y2) {
    return Mouse::instance()->getClickBoxState(x1, y1, x2, y2) == Key::PRESSEDtoRELEASED;
}

bool cScene::isClicked(sImage image) {
    return isClicked(image.box);
}

bool cScene::isKeyTyped(int keyCode) {
    return Keyboard::instance()->getPressState(keyCode) == Key::RELEASEDtoPRESSED;
}

bool cScene::isTyped(sCtrlKey key) {
    return isKeyTyped(key.code);
}

bool cScene::ctrlRQ(sCtrl ctrl) {
    return isClicked(ctrl.icon) || isTyped(ctrl.key);
}

cScene::~cScene() {
    FILE* osdf; errno_t error = fopen_s(&osdf, ShareDataFileName, "wb+"); // open data file
    if (!error) { fwrite(&sd, sizeof(sd), 1, osdf); fclose(osdf); }
    PlaySound(NULL, 0, 0); InitSoundMem(); DxLib_End();
}
