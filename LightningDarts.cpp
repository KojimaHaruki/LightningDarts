// program for Lightning Darts ©2025 Haruki Kojima
#include "DxLib.h"
#include "Home.hpp"
#include "GameSelect.hpp"
#include "PlayerSelect.hpp"
#include "GameStart.hpp"
#include "ZeroOne.hpp"
#include "Cricket.hpp"
#include "Config.hpp"
#include "resource.h"
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    if (DxLib_Init()) return -1;
    SetSysCommandOffFlag(TRUE);
    SetWindowIconID(IDI_ICON1);
    SetWindowText("Lightning Darts");
    SetLogDrawOutFlag(FALSE);
    SRand(GetNowCount()); // initialize random number by the lapse time in Windows
    SetDrawScreen(DX_SCREEN_BACK);
    SetMouseDispFlag(TRUE);
    Scene *scene = new Home();
    while (!ScreenFlip() && !ProcessMessage() && !ClearDrawScreen()) {
        scene->init();
        if (scene->nextScene() == Scene::HOME) { break; }
    }
    while (!ScreenFlip() && !ProcessMessage() && !ClearDrawScreen()) {
        scene->draw();
        scene->update();
        if (scene->nextScene() != Scene::NO_CHANGE) {
            scene->fin();
        }
        switch (scene->nextScene()) {
        case Scene::NO_CHANGE: break;
        case Scene::CONFIG: scene = new Config(scene->currentScene(), scene->shareData()); break;
        case Scene::HOME:          scene = new Home(scene->shareData());                   break;
        case Scene::GAME_SELECT:   scene = new GameSelect(scene->shareData());             break;
        case Scene::PLAYER_SELECT: scene = new PlayerSelect(scene->shareData());           break;
        case Scene::GAME_START:    scene = new GameStart(scene->shareData());              break;
        case Scene::ZERO_ONE:      scene = new ZeroOne(scene->shareData());                break;
        case Scene::CRICKET:       scene = new Cricket(scene->shareData());                break;
        case Scene::QUIT:          delete scene; return 0;
        default: return -1;
        }
    }
    return -1;
}

