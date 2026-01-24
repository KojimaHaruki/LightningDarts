// program for Lightning Darts ©2026 Haruki Kojima
#include "DxLib.h"
#include "Scene.hpp"
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
    cScene::instance()->init();
    while (!ScreenFlip() && !ProcessMessage() && !ClearDrawScreen()) {
		cScene::instance()->draw();
		cScene::instance()->update();
    }
    return -1;
}

