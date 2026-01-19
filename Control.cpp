#include "Control.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "Screen.hpp"
#include "resource.h"

void cControl::init() {
    initKey();
    loadIcon();
    initIconBox();
}

void cControl::initKey() {
    mKeyCode[HOME] = KEY_INPUT_H;
    mKeyCode[BACK] = KEY_INPUT_BACK;
    mKeyCode[FORWARD] = KEY_INPUT_RETURN;
    mKeyCode[SKILL] = KEY_INPUT_E;
    mKeyCode[PLAYER_SELECT] = KEY_INPUT_P;
    mKeyCode[GAME_SELECT] = KEY_INPUT_G;
    mKeyCode[CONFIG] = KEY_INPUT_C;
    mKeyCode[QUIT] = KEY_INPUT_ESCAPE;
    mKeyCode[SKIP] = KEY_INPUT_S;
    mKeyCode[INITIALIZE] = KEY_INPUT_I;
    mKeyCode[RESET] = KEY_INPUT_R;
    mKeyCode[CHANGE_BGM] = KEY_INPUT_B;
    mKeyCode[LEFT] = KEY_INPUT_LEFT;
    mKeyCode[RIGHT] = KEY_INPUT_RIGHT;
    mKeyCode[UP] = KEY_INPUT_UP;
    mKeyCode[DOWN] = KEY_INPUT_DOWN;
    mKeyCode[START] = KEY_INPUT_SPACE;
    mKeyCode[YES] = KEY_INPUT_Y;
    mKeyCode[NO] = KEY_INPUT_N;
    for (int i = 0; i < 2; i++) {
        mKeyCode[MUTE + i] = KEY_INPUT_M;
        mKeyCode[ANOTHER_WINDOW + i] = KEY_INPUT_W;
        mKeyCode[PAUSE + i] = KEY_INPUT_V;
    }
}

void cControl::loadIcon() {
    for (int ctrl = 0; ctrl < NUM; ctrl++) mIcon[ctrl].load(IDB_PNG68 + ctrl, "PNG");
}

void cControl::reloadIcon() {
    for (int ctrl = 0; ctrl < NUM; ctrl++) mIcon[ctrl].reload();
}

void cControl::initIconBox() {
    cCoordinate2d<int> DEFAULT_ICON_SIZE(DEFAULT_ICON_WIDTH, DEFAULT_ICON_HEIGHT);
    mIcon[HOME].box().setSize(30, DEFAULT_ICON_HEIGHT);
    mIcon[HOME].box().setUpperLeft(cScreen::instance()->box().upperLeft());
    mIcon[BACK].box().setSize(DEFAULT_ICON_SIZE);
    mIcon[BACK].box().setUpperLeft(mIcon[HOME].box().upperRight());
    mIcon[FORWARD].box().setSize(DEFAULT_ICON_SIZE);
    mIcon[FORWARD].box().setUpperLeft(mIcon[BACK].box().upperRight());
    for (int i = 0; i < 2; i++) {
        mIcon[MUTE + i].box().setSize(DEFAULT_ICON_SIZE);
        mIcon[MUTE + i].box().setUpperLeft(mIcon[FORWARD].box().upperRight());
    }
    mIcon[QUIT].box().setSize(DEFAULT_ICON_SIZE); mIcon[QUIT].box().setUpperRight(
        cScreen::instance()->box().upperRight());
    for (int i = 0; i < 2; i++) {
        mIcon[ANOTHER_WINDOW + i].box().setSize(DEFAULT_ICON_SIZE);
        mIcon[ANOTHER_WINDOW + i].box().setUpperRight(mIcon[QUIT].box().upperLeft());
    }
    mIcon[CONFIG].box().setSize(DEFAULT_ICON_SIZE);
    mIcon[CONFIG].box().setUpperRight(mIcon[ANOTHER_WINDOW + 0].box().upperLeft());
    mIcon[GAME_SELECT].box().setSize(DEFAULT_ICON_SIZE);
    mIcon[GAME_SELECT].box().setUpperRight(mIcon[CONFIG].box().upperLeft());
    mIcon[PLAYER_SELECT].box().setSize(DEFAULT_ICON_SIZE);
    mIcon[PLAYER_SELECT].box().setUpperRight(mIcon[GAME_SELECT].box().upperLeft());
    for (int i = 0; i < 2; i++) {
        mIcon[PAUSE + i].box().setSize(DEFAULT_ICON_SIZE);
        mIcon[PAUSE + i].box().setUpperRight(mIcon[PLAYER_SELECT].box().upperLeft());
    }
    mIcon[SKILL].box().setSize(DEFAULT_ICON_SIZE);
    mIcon[SKILL].box().setUpperRight(mIcon[PAUSE].box().upperLeft());
    mIcon[SKIP].box().setSize(76, DEFAULT_ICON_HEIGHT);
    mIcon[SKIP].box().setLowerRight(cScreen::instance()->lowerFrame().upperRight());
    mIcon[INITIALIZE].box().setSize(DEFAULT_ICON_SIZE);
    mIcon[INITIALIZE].box().setLowerLeft(cScreen::instance()->box().lowerLeft());
    mIcon[RESET].box().setSize(DEFAULT_ICON_SIZE);
    mIcon[RESET].box().setLowerLeft(mIcon[INITIALIZE].box().lowerRight());
    mIcon[CHANGE_BGM].box().setSize(DEFAULT_ICON_SIZE);
    mIcon[CHANGE_BGM].box().setLowerLeft(mIcon[RESET].box().lowerRight());
}

bool cControl::isIconClicked(int ctrl) {
    if (ctrl >= 0 || ctrl < NUM)
        return cMouse::instance()->clickBoxState(mIcon[ctrl].box()) == sKey::PRESSEDtoRELEASED;
    return false;
}

bool cControl::isKeyTyped(int ctrl) {
    if (ctrl >= 0 || ctrl < NUM)
        return cKeyboard::instance()->pressKeyState(mKeyCode[ctrl]) == sKey::RELEASEDtoPRESSED;
    return false;
}

bool cControl::isRequested(int ctrl) {
    return isIconClicked(ctrl) || isKeyTyped(ctrl);
}

cImage cControl::icon(int ctrl) {
    if (ctrl >= 0 && ctrl < ICON_NUM) return mIcon[ctrl];
    cImage empty; return empty;
}

cBox& cControl::iconBox(int ctrl) {
    if (ctrl >= 0 && ctrl < ICON_NUM) return mIcon[ctrl].box(); 
	cBox empty; return empty;
}

int cControl::keyCode(int ctrl) { 
    if (ctrl >= 0 && ctrl < NUM) return mKeyCode[ctrl]; 
	return -1;
}

std::string cControl::name(int ctrl) { 
    if (ctrl >= 0 && ctrl < NUM) return mName[ctrl];
    return "None";
}
