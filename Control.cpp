#include "Control.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "Screen.hpp"
#include "resource.h"
#include "Sound.hpp"

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
    mIcon[HOME].box().setS(30, DEFAULT_ICON_HEIGHT);
    mIcon[HOME].box().setUL(cScreen::inst()->box().UL());
    mIcon[BACK].box().setS(DEFAULT_ICON_SIZE);
    mIcon[BACK].box().setUL(mIcon[HOME].box().UR());
    mIcon[FORWARD].box().setS(DEFAULT_ICON_SIZE);
    mIcon[FORWARD].box().setUL(mIcon[BACK].box().UR());
    for (int i = 0; i < 2; i++) {
        mIcon[MUTE + i].box().setS(DEFAULT_ICON_SIZE);
        mIcon[MUTE + i].box().setUL(mIcon[FORWARD].box().UR());
    }
    mIcon[QUIT].box().setS(DEFAULT_ICON_SIZE); mIcon[QUIT].box().setUR(
        cScreen::inst()->box().UR());
    for (int i = 0; i < 2; i++) {
        mIcon[ANOTHER_WINDOW + i].box().setS(DEFAULT_ICON_SIZE);
        mIcon[ANOTHER_WINDOW + i].box().setUR(mIcon[QUIT].box().UL());
    }
    mIcon[CONFIG].box().setS(DEFAULT_ICON_SIZE);
    mIcon[CONFIG].box().setUR(mIcon[ANOTHER_WINDOW + 0].box().UL());
    mIcon[GAME_SELECT].box().setS(DEFAULT_ICON_SIZE);
    mIcon[GAME_SELECT].box().setUR(mIcon[CONFIG].box().UL());
    mIcon[PLAYER_SELECT].box().setS(DEFAULT_ICON_SIZE);
    mIcon[PLAYER_SELECT].box().setUR(mIcon[GAME_SELECT].box().UL());
    for (int i = 0; i < 2; i++) {
        mIcon[PAUSE + i].box().setS(DEFAULT_ICON_SIZE);
        mIcon[PAUSE + i].box().setUR(mIcon[PLAYER_SELECT].box().UL());
    }
    mIcon[SKILL].box().setS(DEFAULT_ICON_SIZE);
    mIcon[SKILL].box().setUR(mIcon[PAUSE].box().UL());
    mIcon[SKIP].box().setS(76, DEFAULT_ICON_HEIGHT);
    mIcon[SKIP].box().setLR(cScreen::inst()->lowerFrame().UR());
    mIcon[INITIALIZE].box().setS(DEFAULT_ICON_SIZE);
    mIcon[INITIALIZE].box().setLL(cScreen::inst()->box().LL());
    mIcon[RESET].box().setS(DEFAULT_ICON_SIZE);
    mIcon[RESET].box().setLL(mIcon[INITIALIZE].box().LR());
    mIcon[CHANGE_BGM].box().setS(DEFAULT_ICON_SIZE);
    mIcon[CHANGE_BGM].box().setLL(mIcon[RESET].box().LR());
}

bool cControl::isIconLclicked(int ctrl) {
    return isValidCtrlIcon(ctrl) ? cMouse::inst()->isBoxLclicked(mIcon[ctrl].box()) : false;
}

bool cControl::isKeyTyped(int ctrl) {
    return isValidCtrl(ctrl) ? cKeyboard::inst()->isKeyTyped(mKeyCode[ctrl]) : false;
}

bool cControl::isRequested(int ctrl) {
    return isIconLclicked(ctrl) || isKeyTyped(ctrl);
}

cImage& cControl::icon(int ctrl) {
    if (isValidCtrlIcon(ctrl)) return mIcon[ctrl];
    return mIcon[0];
}

cBox& cControl::iconBox(int ctrl) {
    if (isValidCtrlIcon(ctrl)) return mIcon[ctrl].box();
	return mIcon[0].box();
}

int cControl::keyCode(int ctrl) { 
    if (isValidCtrl(ctrl)) return mKeyCode[ctrl];
	return -1;
}

std::string cControl::name(int ctrl) { 
    if (isValidCtrl(ctrl)) return mName[ctrl];
    return "None";
}
