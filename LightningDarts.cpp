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
    cBaseScene *scene = new cHome();
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
        case cBaseScene::CONFIG: scene = new cConfig(scene->currentScene()); break;
        case cBaseScene::HOME:          scene = new cHome();                   break;
        case cBaseScene::GAME_SELECT:   scene = new cGameSelect();             break;
        case cBaseScene::PLAYER_SELECT: scene = new cPlayerSelect();           break;
        case cBaseScene::GAME_START:    scene = new cGameStart();             break;
        case cBaseScene::ZERO_ONE:      scene = new cZeroOne();                break;
        case cBaseScene::STANDARD_CRICKET: scene = new cStandardCricket();     break;
        case cBaseScene::COUNT_UP:      scene = new cCountUp();                break;
        case cBaseScene::QUIT:          delete scene; return 0;
        default: return -1;
        }
    }
    return -1;
}

