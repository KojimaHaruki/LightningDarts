// program for Lightning Darts ©2025 Haruki Kojima
#include "DxLib.h"
#include "Home.hpp"
#include "GameSelect.hpp"
#include "PlayerSelect.hpp"
#include "GameStart.hpp"
#include "ZeroOne.hpp"
#include "StandardCricket.hpp"
#include "CountUp.hpp"
#include "Config.hpp"
#include "resource.h"
#include <opencv2/opencv.hpp>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    if (DxLib_Init()) return -1;
    SetSysCommandOffFlag(TRUE);
    SetWindowIconID(IDI_ICON1);
    SetWindowText("Lightning Darts");
    SetLogDrawOutFlag(FALSE);
    SRand(GetNowCount()); // initialize random number by the lapse time in Windows
    SetDrawScreen(DX_SCREEN_BACK);
    SetMouseDispFlag(TRUE);
    cScene *scene = new Home();
    while (!ScreenFlip() && !ProcessMessage() && !ClearDrawScreen()) {
        scene->init();
        if (scene->nextScene() == cScene::HOME) { break; }
    }
    while (!ScreenFlip() && !ProcessMessage() && !ClearDrawScreen()) {
        scene->draw();
        scene->update();
        if (scene->nextScene() != cScene::NO_CHANGE) {
            scene->fin();
        }
        switch (scene->nextScene()) {
        case cScene::NO_CHANGE: break;
        case cScene::CONFIG: scene = new Config(scene->currentScene(), scene->shareData()); break;
        case cScene::HOME:          scene = new Home(scene->shareData());                   break;
        case cScene::GAME_SELECT:   scene = new GameSelect(scene->shareData());             break;
        case cScene::PLAYER_SELECT: scene = new PlayerSelect(scene->shareData());           break;
        case cScene::GAME_START:    scene = new cGameStart(scene->shareData());             break;
        case cScene::ZERO_ONE:      scene = new ZeroOne(scene->shareData());                break;
        case cScene::STANDARD_CRICKET: scene = new StandardCricket(scene->shareData());     break;
        case cScene::COUNT_UP:      scene = new CountUp(scene->shareData());                break;
        case cScene::QUIT:          delete scene; return 0;
        default: return -1;
        }
    }
    return -1;
}

