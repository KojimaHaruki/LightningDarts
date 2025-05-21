#include <DxLib.h>
#include "Keyboard.hpp"

Keyboard::Keyboard() {}

bool Keyboard::update() {
    char nowKeyStatus[KEY_NUM];
    GetHitKeyStateAll(nowKeyStatus);       //ç°ÇÃÉLÅ[ÇÃì¸óÕèÛë‘ÇéÊìæ
    for (int i = 0; i < KEY_NUM; i++) {
        if (nowKeyStatus[i]) { // if key is pressed,
            if (key[i].pressCount < 0) { // if key is released before,
                key[i].pressState = Key::RELEASEDtoPRESSED;
                key[i].pressCount = 1;
            }
            else { // if key is pressed before,
                key[i].pressState = Key::PRESSED;
                key[i].pressCount++;
            }
        }
        else { // if key is released,
            if (key[i].pressCount > 0) { // if key is pressed before,
                key[i].pressState = Key::PRESSEDtoRELEASED;
                key[i].pressCount = -1;
            }
            else { // if key is released before,
                key[i].pressState = Key::RELEASED;
                key[i].pressCount--;
            }
        }
    }
    return true;
}
int Keyboard::getPressCount(int keyCode) {
    if (!isValidKey(keyCode)) return 0;
    switch (keyCode) {
    case KEY_INPUT_LSHIFT: case KEY_INPUT_RSHIFT:
        return max(key[KEY_INPUT_LSHIFT].pressCount, key[KEY_INPUT_RSHIFT].pressCount);
    case KEY_INPUT_LCONTROL: case KEY_INPUT_RCONTROL:
        return max(key[KEY_INPUT_LCONTROL].pressCount, key[KEY_INPUT_RCONTROL].pressCount);
    case KEY_INPUT_LALT: case KEY_INPUT_RALT:
        return max(key[KEY_INPUT_LALT].pressCount, key[KEY_INPUT_RALT].pressCount);
    default:
        return key[keyCode].pressCount;
    }
}
int Keyboard::getPressState(int keyCode) {
    if (!isValidKey(keyCode)) return 0;
    switch (keyCode) {
    case KEY_INPUT_LSHIFT: case KEY_INPUT_RSHIFT:
        return max(key[KEY_INPUT_LSHIFT].pressState, key[KEY_INPUT_RSHIFT].pressState);
    case KEY_INPUT_LCONTROL: case KEY_INPUT_RCONTROL:
        return max(key[KEY_INPUT_LCONTROL].pressState, key[KEY_INPUT_RCONTROL].pressState);
    case KEY_INPUT_LALT: case KEY_INPUT_RALT:
        return max(key[KEY_INPUT_LALT].pressState, key[KEY_INPUT_RALT].pressState);
    default:
        return key[keyCode].pressState;
    }
}