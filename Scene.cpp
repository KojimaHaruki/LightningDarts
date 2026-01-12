#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#pragma comment(lib, "winmm.lib")   // MSVC —p
#include "Scene.hpp"
#include "resource.h"
#include "Mouse.hpp"
#include "Sound.hpp"
#include "colorlib.hpp"
#include <numbers>
#include <random>
#include <filesystem>
#include <string>
namespace cl = colorlib;
namespace fs = std::filesystem;

Scene::Scene() : mNowScene(HOME), mNextScene(NO_CHANGE), isConfig(false), nowTime(time(NULL)),
isValidPoint() {
    timeError = localtime_s(&nowLocalTime, &nowTime);
    ICONSIZE_NORMAL.setXY(25, 25);
}

void Scene::initCtrlKey() {
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
    return;
}

void Scene::initScreenSize() {
    sd.screen.setSize(900, 500); sd.screen.setUpperLeft(0, 0);
    changeWindow(sd.window);
    // set frame
    sd.obj.upperFrame.box.setSize(sd.screen.width(), ICONSIZE_NORMAL.y());
    sd.obj.upperFrame.box.setUpperLeft(sd.screen.upperLeft());
    sd.obj.upperFrame.color = cl::srgb("CocoaBrown");
    sd.obj.lowerFrame.box.setSize(sd.screen.width(), ICONSIZE_NORMAL.y());
    sd.obj.lowerFrame.box.setLowerLeft(sd.screen.lowerLeft());
    sd.obj.lowerFrame.color = sd.obj.upperFrame.color;
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
    sd.ctrl.skip.icon.box.setSize(76, 25); sd.ctrl.skip.icon.box.setLowerRight(sd.obj.lowerFrame.box.upperRight());
    sd.ctrl.init.icon.box.setSize(ICONSIZE_NORMAL); sd.ctrl.init.icon.box.setLowerLeft(sd.screen.lowerLeft());
    sd.ctrl.reset.icon.box.setSize(ICONSIZE_NORMAL);
    sd.ctrl.reset.icon.box.setLowerLeft(sd.ctrl.init.icon.box.lowerRight());
    sd.ctrl.bgm.icon.box.setSize(ICONSIZE_NORMAL); sd.ctrl.bgm.icon.box.setLowerLeft(sd.ctrl.reset.icon.box.lowerRight());
    // set image
    sd.pic.selected.image.box.setSize(100, 86);
    sd.pic.darts.image.box.setSize(900, 600);
    sd.pic.darts.image.box.setUpperLeft(0, 0);
    sd.pic.thunder.image.box.setSize(900, 945);
    sd.pic.thunder.image.box.setUpperLeft(-50, -200);
    return;
}

void Scene::init() {
    //FILE* isdf; errno_t error = fopen_s(&isdf, ShareDataFileName, "rb"); // open data file
    //if (!error) { fread(&sd, sizeof(sd), 1, isdf); fclose(isdf); }
    initCtrlKey();
    initScreenSize();
    // load color
    if (mNowScene == NO_CHANGE) {
        int cpp = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG92), "PNG");
        int dxlib = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG93), "PNG");
        sd.color.w = cl::srgb("w"); 
        // draw start screen
        ClearDrawScreen();
        DrawStringToHandle(100, 50, "Powered by", sd.color.w, sd.font.xl.handle);
        DrawGraph(200, 100 + sd.font.xl.size, cpp, TRUE);
        DrawGraph(500, 100 + sd.font.xl.size, dxlib, TRUE);
        DrawStringToHandle(sd.screen.right() - 180, sd.screen.bottom() - sd.font.m.size - 10, "now loading...",
            sd.color.w, sd.font.m.handle);
        ScreenFlip();
        sd.color.k = cl::srgb("k"); sd.color.gy = cl::srgb("AshGrey");
        sd.color.r = cl::srgb("r"); sd.color.g = cl::srgb("g"); sd.color.b = cl::srgb("b");
		sd.color.m = cl::srgb("m"); sd.color.c = cl::srgb("c"); sd.color.y = cl::srgb("y");
		sd.color.touch = sd.color.y; sd.color.press = cl::srgb("Strawberry"); 
        sd.color.execute = cl::srgb("Violet");
        sd.color.team[0] = sd.color.b; sd.color.team[1] = sd.color.r; sd.color.team[2] = sd.color.g;
        sd.color.team[3] = sd.color.y; sd.color.team[4] = sd.color.c; sd.color.team[5] = sd.color.m;
        sd.color.team[6] = sd.color.execute; sd.color.team[7] = cl::srgb("Marigold");
        sd.ctrl.home.name = "Home"; sd.ctrl.back.name = "Back"; sd.ctrl.forward.name = "Forward";
        sd.ctrl.mute[0].name = "Unmute"; sd.ctrl.mute[1].name = "Mute"; sd.ctrl.quit.name = "Quit";
        sd.ctrl.window[0].name = "Another window"; sd.ctrl.window[1].name = "Maximize window";
        sd.ctrl.pause[0].name = "Pause"; sd.ctrl.pause[1].name = "Resume"; sd.ctrl.config.name = "Config";
        sd.ctrl.playerSelect.name = "Restart from Player select"; sd.ctrl.gameSelect.name = "Restart from Game select";
        sd.ctrl.skill.name = "Skill"; sd.ctrl.skip.name = "Skip"; sd.ctrl.init.name = "Initialize";
        sd.ctrl.reset.name = "Reset the current scene"; sd.ctrl.bgm.name = "Change BGM";
        // load BGM
		Sound::instance()->loadBGM();
    }
	Sound::instance()->initSoundVol();
    StopSound();
    Sound::instance()->playBGM(0);
    // set game
    sd.game = -1;
    // set player
    sd.teamType = TeamType::SOLO;
	sd.teams.clear();
	sd.teams.reserve(MAX_GROUP_NUM);
    mNextScene = HOME;
    return;
}

void Scene::changeWindow(int WindowModeFlag) {
    if (!WindowModeFlag) SetGraphMode(sd.screen.width(), sd.screen.height(), 32); // change screen size
    ChangeWindowMode(WindowModeFlag);
    SetDrawScreen(DX_SCREEN_BACK);
    SetMouseDispFlag(TRUE);
    // load font
    sd.font.xl.handle = CreateFontToHandle(sd.font.xl.name.c_str(), sd.font.xl.size,
        sd.font.xl.thick, sd.font.xl.type);
    sd.font.m.handle = CreateFontToHandle(sd.font.m.name.c_str(), sd.font.m.size,
        sd.font.m.thick, sd.font.m.type);
    sd.font.s.handle = CreateFontToHandle(sd.font.s.name.c_str(), sd.font.s.size,
        sd.font.s.thick, sd.font.s.type);
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
    std::error_code err;
	sd.groups.clear();
    sd.groups.push_back(Group("Guest"));
    for (fs::directory_iterator iter(playerFolderPath + "/"), end;
        iter != end && !err && sd.groups.size() < MAX_GROUP_NUM; iter.increment(err)) {
        const fs::directory_entry entry = *iter;
        // if found path is valid folder,
        if (!entry.path().has_extension() && entry.path().filename().string() != "Guest") {
            sd.groups.push_back(Group(entry.path().filename().string())); // get group name
        }
		sd.groups.shrink_to_fit();
    }
    for (int group = 0, chara = 0; group < sd.groups.size(); group++) {
		sd.groups[group].members.clear();
        for (fs::directory_iterator iter(playerFolderPath + "/" + sd.groups[group].name + "/"), end;
            iter != end && !err && chara < MAX_CHARA_NUM; iter.increment(err), chara++) {
            const fs::directory_entry entry = *iter;
            std::string extension = entry.path().extension().string();
            if (extension == ".jpg" || extension == ".png") { // if found file is image,
				std::string name = entry.path().filename().string();
				name.erase(name.length() - extension.length(), extension.length());
				sd.groups[group].members.push_back(
                    Chara(name, Image(Box(), LoadGraph(entry.path().string().c_str()), TRUE),
						sd.groups[group].name, CharaStatus()));
            }
        }
		sd.groups[group].members.shrink_to_fit();
    }
    if (err) {
        std::cout << err.value() << std::endl;
        std::cout << err.message() << std::endl;
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
    for (int posNo = 0; posNo < 4; posNo++) {
        for (int colorNo = 0; colorNo < 4; colorNo++) {
            sd.dartsBoard[posNo][colorNo] = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG94 + 4 * posNo + colorNo), "PNG");
        }
    }
    sd.dartsArrow = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG110), "PNG");
    sd.window = GetWindowModeFlag(); return;
}

void Scene::reset() {
}

void Scene::draw() {
    drawImage(sd.pic.darts.image); 
    drawImage(sd.pic.thunder.image);
    DrawStringToHandle(
        sd.ctrl.bgm.icon.box.right(), sd.obj.lowerFrame.box.center().y() - sd.font.s.size / 2,
        Sound::instance()->playingBGMName().c_str(), sd.color.w, sd.font.s.handle);
    DrawStringToHandle(sd.screen.right() - 340, sd.obj.lowerFrame.box.center().y() - sd.font.s.size / 2, 
        "Lightning Darts C 2025 Haruki Kojima", sd.color.touch, sd.font.s.handle);
    DrawCircleAA(sd.screen.right() - 210.5f, (float)sd.obj.lowerFrame.box.center().y(), 
        sd.font.s.size / 2.0f, 1000, sd.color.k, FALSE, 3.0f);
    DrawCircleAA(sd.screen.right() - 210.5f, (float)sd.obj.lowerFrame.box.center().y(), 
        sd.font.s.size / 2.0f, 1000, sd.color.touch, FALSE, 1.0f);
    if (!timeError) {
        DrawFormatStringToHandle(
            sd.screen.right() - 42, sd.obj.lowerFrame.box.center().y() - sd.font.s.size / 2,
            sd.color.w, sd.font.s.handle, "%02d:%02d", nowLocalTime.tm_hour, nowLocalTime.tm_min);
    }
    float theta = -M_PI;
    // draw icon & darts board
    switch (mNowScene) {
	case ZERO_ONE: case CRICKET: case COUNT_UP:
        drawImage(sd.ctrl.pause[sd.gameTime.isPaused()].icon);
        drawImage(sd.ctrl.skill.icon);
        if (!isConfig) {
            sd.gameTime.drawLapseTime(sd.screen.left(), sd.obj.upperFrame.box.bottom() + 10,
                sd.color.w, sd.font.s.handle, Timer::Mode::HMSmS);
            DrawStringToHandle(sd.ctrl.mute[Sound::instance()->isBGMPlayed()].icon.box.right() + 5,
                sd.obj.upperFrame.box.center().y() - sd.font.m.size / 2,
                (gameName[sd.game] + " / " + teamTypeName[sd.teamType]).c_str(),
                sd.color.w, sd.font.m.handle);
            // draw darts board
            DrawCircleAA(darts.center.x(), darts.center.y(), 226, 100, sd.color.k);
            DrawCircleAA(darts.center.x(), darts.center.y(), DartsRadialPos::Radius[DartsRadialPos::DOUBLE], 100, 
                sd.color.gy);
            for (int i = 0; i < 20; i++, theta += 0.1 * M_PI) {
                if (isValidPoint[Darts::BOARD_POINT[i]]) {
                    DrawStringToHandle(
                        darts.center.x() + 212.0 * cos(theta) - 18.0, darts.center.y() - 212.0 * sin(theta) - 10.0,
                        darts.pointName[Darts::BOARD_POINT[i]].c_str(), sd.color.w, sd.font.m.handle);
                    for (int posNo = DartsRadialPos::DOUBLE; posNo <= DartsRadialPos::INNER_SINGLE; posNo++) {
                        if (darts.point == Darts::BOARD_POINT[i] && darts.radialPos == posNo) {
                            switch (Mouse::instance()->getClickState()) {
                            case Key::RELEASED:
                                DrawCircleGauge(darts.center.x(), darts.center.y(), 77.5 - 5.0 * i, sd.dartsBoard[posNo][2], 72.5 - 5.0 * i); 
                                break;
                            case Key::RELEASEDtoPRESSED: case Key::PRESSED:
                                DrawCircleGauge(darts.center.x(), darts.center.y(), 77.5 - 5.0 * i, sd.dartsBoard[posNo][3], 72.5 - 5.0 * i); 
                                break;
                            default: 
                                break;
                            }
                            continue;
                        }
                        DrawCircleGauge(darts.center.x(), darts.center.y(),
                            77.5 - 5.0 * i, sd.dartsBoard[posNo][i % 2], 72.5 - 5.0 * i);
                    }
                    continue;
                }
                DrawStringToHandle(
                    darts.center.x() + 212.0 * cos(theta) - 18.0, darts.center.y() - 212.0 * sin(theta) - 10.0,
                    darts.pointName[Darts::BOARD_POINT[i]].c_str(), sd.color.gy, sd.font.m.handle);
            }
            if (!isValidPoint[0]) {
                DrawCircle(darts.center.x(), darts.center.y(), 22, sd.color.gy);
            }
            else {
                if (darts.radialPos == DartsRadialPos::BULL) {
                    if (Keyboard::instance()->getPressState(Darts::POINT_KEY[Darts::BULL]) != Key::RELEASED) {
                        DrawCircleAA(darts.center.x(), darts.center.y(), 
                            DartsRadialPos::Radius[DartsRadialPos::BULL], 100, sd.color.press);
                    }
                    else {
                        switch (Mouse::instance()->getClickState()) {
                        case Key::RELEASED:
                            DrawCircleAA(darts.center.x(), darts.center.y(), 
                                DartsRadialPos::Radius[DartsRadialPos::BULL], 100, sd.color.touch); break;
                        case Key::RELEASEDtoPRESSED: case Key::PRESSED:
                            DrawCircleAA(darts.center.x(), darts.center.y(), 
                                DartsRadialPos::Radius[DartsRadialPos::BULL], 100, sd.color.press); break;
                        default:
                            DrawCircleAA(darts.center.x(), darts.center.y(), 
                                DartsRadialPos::Radius[DartsRadialPos::BULL], 100, sd.color.r); break;
                        }
                    }
                    DrawStringToHandle(sd.screen.left(), sd.obj.lowerFrame.box.top() - sd.font.m.size - 5,
                        darts.pointName[Darts::BULL].c_str(), sd.color.w, sd.font.m.handle);
                }
                else { 
                    DrawCircleAA(darts.center.x(), darts.center.y(), 
                        DartsRadialPos::Radius[DartsRadialPos::BULL], 100, sd.color.r);
                }
                if (darts.radialPos == DartsRadialPos::INNER_BULL) {
                    if (Keyboard::instance()->getPressState(Darts::POINT_KEY[Darts::INNER_BULL]) != Key::RELEASED) {
                        DrawCircle(darts.center.x(), darts.center.y(), 
                            DartsRadialPos::Radius[DartsRadialPos::INNER_BULL], sd.color.press);
                    }
                    else {
                        switch (Mouse::instance()->getClickState()) {
                        case Key::RELEASED:
                            DrawCircleAA(darts.center.x(), darts.center.y(), 
                                DartsRadialPos::Radius[DartsRadialPos::INNER_BULL], 100, sd.color.touch); break;
                        case Key::RELEASEDtoPRESSED: case Key::PRESSED:
                            DrawCircleAA(darts.center.x(), darts.center.y(), 
                                DartsRadialPos::Radius[DartsRadialPos::INNER_BULL], 100, sd.color.press); break;
                        default:
                            DrawCircleAA(darts.center.x(), darts.center.y(), 
                                DartsRadialPos::Radius[DartsRadialPos::INNER_BULL], 100, sd.color.k); break;
                        }
                    }
                    DrawStringToHandle(sd.screen.left(), sd.obj.lowerFrame.box.top() - sd.font.m.size - 5,
                        darts.pointName[Darts::INNER_BULL].c_str(), sd.color.w, sd.font.m.handle);
                }
                else { 
                    DrawCircleAA(darts.center.x(), darts.center.y(), 
                        DartsRadialPos::Radius[DartsRadialPos::INNER_BULL], 100, sd.color.k); }
            }
            for (int posNo = 0; posNo < DartsRadialPos::NUM; posNo++) {
                DrawCircleAA(darts.center.x(), darts.center.y(), DartsRadialPos::Radius[posNo], 100, 0, FALSE, 2);
            }
            theta = 0.05 * M_PI;
            for (int i = 0; i < 20; i++, theta += 0.1 * M_PI) {
                DrawLineAA(darts.center.x() + 22.0 * cos(theta), sd.screen.center().y() + 22.0 * sin(theta),
                    darts.center.x() + 196.85 * cos(theta), sd.screen.center().y() + 196.85 * sin(theta), 0, 2);
            }
            if (darts.radialPos > DartsRadialPos::OUTSIDE && darts.radialPos < DartsRadialPos::BULL) {
                DrawStringToHandle(sd.screen.left(), sd.obj.lowerFrame.box.top() - sd.font.m.size - 5,
                    (std::to_string(darts.point) + darts.radialPosName[darts.radialPos]).c_str(),
                    sd.color.w, sd.font.m.handle);
            }
            if (darts.radialPos > DartsRadialPos::OUTSIDE && darts.radialPos < DartsRadialPos::BULL) {
                DrawStringToHandle(sd.screen.left(), sd.obj.lowerFrame.box.top() - sd.font.m.size - 5,
                    (std::to_string(darts.point) + darts.radialPosName[darts.radialPos]).c_str(),
                    sd.color.w, sd.font.m.handle);
            }
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
void Scene::fin() {
}
void Scene::update() {
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
    if (isConfig) {
        switch (mNowScene) {
        case ZERO_ONE: case CRICKET: case COUNT_UP:
        case GAME_START:    
        case PLAYER_SELECT: 
            if (ctrlRQ(sd.ctrl.playerSelect)) { mNextScene = PLAYER_SELECT; return; }
        case GAME_SELECT:   
            if (ctrlRQ(sd.ctrl.gameSelect))   { mNextScene = GAME_SELECT;   return; }
        case HOME:          
            if (ctrlRQ(sd.ctrl.home))         { mNextScene = HOME;          return; }
        default:            
            break;
        }
    }
    else {
        Coordinate2d<float> cursor;
        cursor.setXY(Mouse::instance()->x() - darts.center.x(), darts.center.y() - Mouse::instance()->y());
        Polar<float> cursorPolar = cursor.polar();
        switch (mNowScene) {
        case ZERO_ONE: case CRICKET: case COUNT_UP:
            sd.gameTime.update();
            if    (!sd.gameTime.isPaused() && ctrlRQ(sd.ctrl.pause[FALSE])) { sd.gameTime.stop();   return; }
            else if (sd.gameTime.isPaused() && ctrlRQ(sd.ctrl.pause[TRUE])) { sd.gameTime.resume(); return; }
            // update darts
            darts.point = -1;
            darts.power = 0;
            darts.radialPos = DartsRadialPos::OUTSIDE;
            
            for (int point = 1; point <= 20; point++) { // keyboard input
                if (Keyboard::instance()->getPressState(Darts::POINT_KEY[point]) != Key::RELEASED) {
                    darts.point = point;
                    break;
                }
            }
            if (darts.point >= 0) {
                if (Keyboard::instance()->getPressState(KEY_INPUT_D) == Key::PRESSED) {
                    darts.power = 2;
                    darts.radialPos = DartsRadialPos::DOUBLE;
                }
                else if (Keyboard::instance()->getPressState(KEY_INPUT_T) == Key::PRESSED) {
                    darts.power = 3;
                    darts.radialPos = DartsRadialPos::TRIPLE;
                }
                else {
                    darts.power = 1;
                    darts.radialPos = DartsRadialPos::INNER_SINGLE;
                }
                darts.totalPoint = darts.power * darts.point;
            }
            else if (cursorPolar.r < DartsRadialPos::Radius[DartsRadialPos::INNER_BULL] || // mouse input
                Keyboard::instance()->getPressState(Darts::POINT_KEY[Darts::INNER_BULL]) != Key::RELEASED) { // keyboard input
                darts.point = 25;
                darts.power = 2;
                darts.radialPos = DartsRadialPos::INNER_BULL;
                darts.totalPoint = 50;
            }
            else if (cursorPolar.r < DartsRadialPos::Radius[DartsRadialPos::BULL] || // mouse input
                Keyboard::instance()->getPressState(Darts::POINT_KEY[Darts::BULL]) != Key::RELEASED) { // keyboard input
                darts.point = 25;
                darts.power = 1;
                darts.radialPos = DartsRadialPos::BULL;
                if (mNowScene == ZERO_ONE || mNowScene == COUNT_UP) {
                    darts.totalPoint = 50;
                }
                else {
                    darts.totalPoint = 25;
                }
            }
            else if (cursorPolar.r < DartsRadialPos::Radius[DartsRadialPos::DOUBLE]) {
                float theta = -M_PI + 0.05 * M_PI;
                for (int i = 0; i < 21; i++, theta += 0.1 * M_PI) { // mouse input
                    if (cursorPolar.theta < theta) {
                        darts.point = Darts::BOARD_POINT[i];
                        break;
                    }
                }
                for (int radialPosNo = DartsRadialPos::INNER_SINGLE; radialPosNo > DartsRadialPos::OUTSIDE; radialPosNo--) {
                    if (cursorPolar.r < DartsRadialPos::Radius[radialPosNo]) {
                        darts.power = DartsRadialPos::Power[radialPosNo];
                        darts.radialPos = radialPosNo;
                        darts.totalPoint = darts.point * darts.power;
                        break;
                    }
                }
            }
            else if (cursorPolar.r < 226 || // mouse input
                Keyboard::instance()->getPressState(Darts::POINT_KEY[Darts::OUTSIDE]) != Key::RELEASED) { // keyboard input
                darts.point = 0;
                darts.totalPoint = 0;
            }
        case GAME_START:    
            if (ctrlRQ(sd.ctrl.playerSelect)) { mNextScene = PLAYER_SELECT;              return; }
        case PLAYER_SELECT: 
            if (ctrlRQ(sd.ctrl.gameSelect))   { mNextScene = GAME_SELECT;                return; }
        case GAME_SELECT:   
            if (ctrlRQ(sd.ctrl.home))         { mNextScene = HOME;                       return; }
        case HOME:          
            if (ctrlRQ(sd.ctrl.config))       { mNextScene = CONFIG; sd.gameTime.stop(); return; }
        default:            
            break;
        }
    }
}

int Scene::drawBoxObj(Box box, int color, int fill) {
    return DrawBox(box.left(), box.top(), box.right(), box.bottom(), color, fill);
}

int Scene::drawBoxObj(BoxObj obj) {
    return DrawBox(obj.box.left(), obj.box.top(), obj.box.right(), obj.box.bottom(), obj.color, obj.fill);
}

int Scene::drawImage(Image image) { 
    return DrawGraph(image.box.left(), image.box.top(), image.handle, image.trans);
}

bool Scene::isClicked(Box box) { 
    return Mouse::instance()->getClickBoxState(box) == Key::PRESSEDtoRELEASED; 
}

bool Scene::isBoxClicked(int x1, int y1, int x2, int y2) {
    return Mouse::instance()->getClickBoxState(x1, y1, x2, y2) == Key::PRESSEDtoRELEASED;
}

bool Scene::isClicked(Image image) { 
    return isClicked(image.box); 
}

bool Scene::isKeyTyped(int keyCode) { 
    return Keyboard::instance()->getPressState(keyCode) == Key::RELEASEDtoPRESSED; 
}

bool Scene::isTyped(CtrlKey key) {
    return isKeyTyped(key.code); 
}

bool Scene::ctrlRQ(Ctrl ctrl) { 
    return isClicked(ctrl.icon) || isTyped(ctrl.key); 
}

Scene::~Scene() {
    FILE* osdf; errno_t error = fopen_s(&osdf, ShareDataFileName, "wb+"); // open data file
    if (!error) { fwrite(&sd, sizeof(sd), 1, osdf); fclose(osdf); }
    PlaySound(NULL, 0, 0); InitSoundMem(); DxLib_End();
}
