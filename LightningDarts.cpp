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
    cBaseScene *scene = new Home();
    while (!ScreenFlip() && !ProcessMessage() && !ClearDrawScreen()) {
        scene->init();
        if (scene->nextScene() == cBaseScene::HOME) { break; }
    }
    while (!ScreenFlip() && !ProcessMessage() && !ClearDrawScreen()) {
        scene->draw();
        scene->update();
        if (scene->nextScene() != cBaseScene::NO_CHANGE) {
            scene->fin();
        }
        switch (scene->nextScene()) {
        case cBaseScene::NO_CHANGE: break;
        case cBaseScene::CONFIG: scene = new Config(scene->currentScene(), scene->shareData()); break;
        case cBaseScene::HOME:          scene = new Home(scene->shareData());                   break;
        case cBaseScene::GAME_SELECT:   scene = new GameSelect(scene->shareData());             break;
        case cBaseScene::PLAYER_SELECT: scene = new sPlayerSelect(scene->shareData());           break;
        case cBaseScene::GAME_START:    scene = new cGameStart(scene->shareData());             break;
        case cBaseScene::ZERO_ONE:      scene = new ZeroOne(scene->shareData());                break;
        case cBaseScene::STANDARD_CRICKET: scene = new cStandardCricket(scene->shareData());     break;
        case cBaseScene::COUNT_UP:      scene = new CountUp(scene->shareData());                break;
        case cBaseScene::QUIT:          delete scene; return 0;
        default: return -1;
        }
    }
    return -1;
}

