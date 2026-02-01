// program for Lightning Darts ©2026 Haruki Kojima
#include "DxLib.h"
#include "Scene.hpp"
#include "resource.h"
#include "highlevelmonitorconfigurationapi.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    if (DxLib_Init()) return -1;
    SetSysCommandOffFlag(TRUE);
    SetWindowIconID(IDI_ICON1);
    SetWindowText("Lightning Darts");
    SetLogDrawOutFlag(FALSE);
    SRand(GetNowCount()); // initialize random number by the lapse time in Windows
    cScene::inst()->init();
    while (!ScreenFlip() && !ProcessMessage() && !ClearDrawScreen() &&
        cScene::inst()->currentScene() != cScene::QUIT) {
		cScene::inst()->draw();
		cScene::inst()->update();
    }
    StopSound();
    InitSoundMem();
    DxLib_End();
    return 0;
}

