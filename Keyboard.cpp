#include <DxLib.h>
#include "Keyboard.hpp"

void cKeyboard::loadKeyImage() {
    for (int i = 0; i < VALID_KEY_NUM; i++)
        mKeyImage[KeyNo[i]].load(KeyNo[i], "PNG");
}

cImage cKeyboard::keyImage(int keyCode) { 
    if (keyCode >= 0 && keyCode < KEY_NUM) return mKeyImage[keyCode];
    cImage empty; return empty;
}

cBox& cKeyboard::keyBox(int keyCode) {
    if (keyCode >= 0 && keyCode < KEY_NUM) return mKeyImage[keyCode].box();
    cImage empty; return empty.box();
}

bool cKeyboard::update() {
    char nowKeyStatus[KEY_NUM];
    GetHitKeyStateAll(nowKeyStatus);       //¡‚ÌƒL[‚Ì“ü—Íó‘Ô‚ðŽæ“¾
    for (int i = 0; i < KEY_NUM; i++) {
        if (nowKeyStatus[i]) { // if key is pressed,
            if (mPressKeyCount[i] < 0) { // if key is released before,
                mPressKeyState[i] = Key::RELEASEDtoPRESSED;
                mPressKeyCount[i] = 1;
            }
            else { // if key is pressed before,
                mPressKeyState[i] = Key::PRESSED;
                mPressKeyCount[i]++;
            }
        }
        else { // if key is released,
            if (mPressKeyCount[i] > 0) { // if key is pressed before,
                mPressKeyState[i] = Key::PRESSEDtoRELEASED;
                mPressKeyCount[i] = -1;
            }
            else { // if key is released before,
                mPressKeyState[i] = Key::RELEASED;
                mPressKeyCount[i]--;
            }
        }
    }
    return true;
}
int cKeyboard::pressKeyCount(int keyCode) {
    if (keyCode < 0 && keyCode >= KEY_NUM) return 0;
    switch (keyCode) {
    case KEY_INPUT_LSHIFT: case KEY_INPUT_RSHIFT:
        return max(mPressKeyCount[KEY_INPUT_LSHIFT], mPressKeyCount[KEY_INPUT_RSHIFT]);
    case KEY_INPUT_LCONTROL: case KEY_INPUT_RCONTROL:
        return max(mPressKeyCount[KEY_INPUT_LCONTROL], mPressKeyCount[KEY_INPUT_RCONTROL]);
    case KEY_INPUT_LALT: case KEY_INPUT_RALT:
        return max(mPressKeyCount[KEY_INPUT_LALT], mPressKeyCount[KEY_INPUT_RALT]);
    default:
        return mPressKeyCount[keyCode];
    }
}
int cKeyboard::pressKeyState(int keyCode) {
    if (keyCode < 0 && keyCode >= KEY_NUM) return 0;
    switch (keyCode) {
    case KEY_INPUT_LSHIFT: case KEY_INPUT_RSHIFT:
        return max(mPressKeyState[KEY_INPUT_LSHIFT], mPressKeyState[KEY_INPUT_RSHIFT]);
    case KEY_INPUT_LCONTROL: case KEY_INPUT_RCONTROL:
        return max(mPressKeyState[KEY_INPUT_LCONTROL], mPressKeyState[KEY_INPUT_RCONTROL]);
    case KEY_INPUT_LALT: case KEY_INPUT_RALT:
        return max(mPressKeyState[KEY_INPUT_LALT], mPressKeyState[KEY_INPUT_RALT]);
    default:
        return mPressKeyState[keyCode];
    }
}