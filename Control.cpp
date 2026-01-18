#include "Control.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "Screen.hpp"
#include "resource.h"

void cControl::init() {
    loadName();
    initKey();
    loadIcon();
    initIconBox();
}

void cControl::loadName() {
    mCtrl[HOME].name = "Home"; mCtrl[BACK].name = "Back"; mCtrl[FORWARD].name = "Forward";
    mCtrl[UNMUTE].name = "Unmute"; mCtrl[MUTE].name = "Mute"; mCtrl[QUIT].name = "Quit";
    mCtrl[ANOTHER_WINDOW].name = "Another Window"; mCtrl[MAXIMIZE_WINDOW].name = "Maximize Window";
    mCtrl[PAUSE].name = "Pause"; mCtrl[RESUME].name = "Resume"; mCtrl[CONFIG].name = "Config";
    mCtrl[PLAYER_SELECT].name = "Player select"; mCtrl[GAME_SELECT].name = "Game select";
    mCtrl[SKILL].name = "Skill Effect"; mCtrl[SKIP].name = "Skip";
    mCtrl[INITIALIZE].name = "Initialize";
    mCtrl[RESET].name = "Reset"; mCtrl[CHANGE_BGM].name = "Change BGM";
}

void cControl::initKey() {
    mCtrl[HOME].keyCode = KEY_INPUT_H;
    mCtrl[BACK].keyCode = KEY_INPUT_BACK; mCtrl[FORWARD].keyCode = KEY_INPUT_RETURN;
    mCtrl[SKILL].keyCode = KEY_INPUT_E; mCtrl[PLAYER_SELECT].keyCode = KEY_INPUT_P;
    mCtrl[GAME_SELECT].keyCode = KEY_INPUT_G;
    mCtrl[CONFIG].keyCode = KEY_INPUT_C; mCtrl[QUIT].keyCode = KEY_INPUT_ESCAPE;
    mCtrl[SKIP].keyCode = KEY_INPUT_S;
    mCtrl[INITIALIZE].keyCode = KEY_INPUT_I; mCtrl[RESET].keyCode = KEY_INPUT_R;
    mCtrl[CHANGE_BGM].keyCode = KEY_INPUT_B;
    mCtrl[LEFT].keyCode = KEY_INPUT_LEFT; mCtrl[RIGHT].keyCode = KEY_INPUT_RIGHT;
    mCtrl[UP].keyCode = KEY_INPUT_UP; mCtrl[DOWN].keyCode = KEY_INPUT_DOWN;
    mCtrl[START].keyCode = KEY_INPUT_SPACE; mCtrl[YES].keyCode = KEY_INPUT_Y;
    mCtrl[NO].keyCode = KEY_INPUT_N;
    for (int i = 0; i < 2; i++) {
        mCtrl[MUTE + i].keyCode = KEY_INPUT_M;
        mCtrl[ANOTHER_WINDOW + i].keyCode = KEY_INPUT_W;
        mCtrl[PAUSE + i].keyCode = KEY_INPUT_PAUSE;
    }
}

void cControl::loadIcon() {
    for (int ctrl = 0; ctrl < NUM; ctrl++) mCtrl[ctrl].icon.load(IDB_PNG68 + ctrl, "PNG");
}

void cControl::reloadIcon() {
    for (int ctrl = 0; ctrl < NUM; ctrl++) mCtrl[ctrl].icon.reload();
}

void cControl::initIconBox() {
    cCoordinate2d<int> DEFAULT_ICON_SIZE(DEFAULT_ICON_WIDTH, DEFAULT_ICON_HEIGHT);
    mCtrl[HOME].icon.box().setSize(30, DEFAULT_ICON_HEIGHT);
    mCtrl[HOME].icon.box().setUpperLeft(cScreen::instance()->box().upperLeft());
    mCtrl[BACK].icon.box().setSize(DEFAULT_ICON_SIZE);
    mCtrl[BACK].icon.box().setUpperLeft(mCtrl[HOME].icon.box().upperRight());
    mCtrl[FORWARD].icon.box().setSize(DEFAULT_ICON_SIZE);
    mCtrl[FORWARD].icon.box().setUpperLeft(mCtrl[BACK].icon.box().upperRight());
    for (int i = 0; i < 2; i++) {
        mCtrl[MUTE + i].icon.box().setSize(DEFAULT_ICON_SIZE);
        mCtrl[MUTE + i].icon.box().setUpperLeft(mCtrl[FORWARD].icon.box().upperRight());
    }
    mCtrl[QUIT].icon.box().setSize(DEFAULT_ICON_SIZE); mCtrl[QUIT].icon.box().setUpperRight(
        cScreen::instance()->box().upperRight());
    for (int i = 0; i < 2; i++) {
        mCtrl[ANOTHER_WINDOW + i].icon.box().setSize(DEFAULT_ICON_SIZE);
        mCtrl[ANOTHER_WINDOW + i].icon.box().setUpperRight(mCtrl[QUIT].icon.box().upperLeft());
    }
    mCtrl[CONFIG].icon.box().setSize(DEFAULT_ICON_SIZE);
    mCtrl[CONFIG].icon.box().setUpperRight(mCtrl[ANOTHER_WINDOW + 0].icon.box().upperLeft());
    mCtrl[GAME_SELECT].icon.box().setSize(DEFAULT_ICON_SIZE);
    mCtrl[GAME_SELECT].icon.box().setUpperRight(mCtrl[CONFIG].icon.box().upperLeft());
    mCtrl[PLAYER_SELECT].icon.box().setSize(DEFAULT_ICON_SIZE);
    mCtrl[PLAYER_SELECT].icon.box().setUpperRight(mCtrl[GAME_SELECT].icon.box().upperLeft());
    for (int i = 0; i < 2; i++) {
        mCtrl[PAUSE + i].icon.box().setSize(DEFAULT_ICON_SIZE);
        mCtrl[PAUSE + i].icon.box().setUpperRight(mCtrl[PLAYER_SELECT].icon.box().upperLeft());
    }
    mCtrl[SKILL].icon.box().setSize(DEFAULT_ICON_SIZE);
    mCtrl[SKILL].icon.box().setUpperRight(mCtrl[PAUSE].icon.box().upperLeft());
    mCtrl[SKIP].icon.box().setSize(76, DEFAULT_ICON_HEIGHT);
    mCtrl[SKIP].icon.box().setLowerRight(cScreen::instance()->lowerFrame().upperRight());
    mCtrl[INITIALIZE].icon.box().setSize(DEFAULT_ICON_SIZE);
    mCtrl[INITIALIZE].icon.box().setLowerLeft(cScreen::instance()->box().lowerLeft());
    mCtrl[RESET].icon.box().setSize(DEFAULT_ICON_SIZE);
    mCtrl[RESET].icon.box().setLowerLeft(mCtrl[INITIALIZE].icon.box().lowerRight());
    mCtrl[CHANGE_BGM].icon.box().setSize(DEFAULT_ICON_SIZE);
    mCtrl[CHANGE_BGM].icon.box().setLowerLeft(mCtrl[RESET].icon.box().lowerRight());
}

bool cControl::isRequested(int ctrl) {
    return cMouse::instance()->clickBoxState(mCtrl[ctrl].icon.box()) == sKey::PRESSEDtoRELEASED ||
        cKeyboard::instance()->pressKeyState(mCtrl[ctrl].keyCode) == sKey::RELEASEDtoPRESSED;
}
