#include <DxLib.h>
#include "Keyboard.hpp"
#include "Control.hpp"
#include "Sound.hpp"

void cKeyboard::loadKeyImage() {
    for (int i = 0; i < VALID_KEY_NUM; i++) {
        keys[KeyNo[i]].image.load(KeyNo[i], "PNG");
        keys[KeyNo[i]].image.box().setS(cControl::DEFAULT_ICON_WIDTH, cControl::DEFAULT_ICON_HEIGHT);
    }
}

void cKeyboard::reloadKeyImage() {
    for (int i = 0; i < VALID_KEY_NUM; i++) keys[KeyNo[i]].image.reload();
}

cImage& cKeyboard::keyImage(int key) { 
    return isValidKey(key) ? keys[key].image : keys[0].image;
}

cBox& cKeyboard::keyBox(int key) {
    return isValidKey(key) ? keys[key].image.box() : keys[0].image.box();
}

void cKeyboard::update() {
    char keyStatus[KEY_NUM];
    GetHitKeyStateAll(keyStatus);
    for (int key = 0; key < KEY_NUM; key++) {
        if (keyStatus[key]) { // if key is pressed,
            if (keys[key].count < 0) { // if key is released before,
                keys[key].count = 1;
            }
            else { // if key is pressed before,
                keys[key].count++;
            }
        }
        else { // if key is released,
            if (keys[key].count > 0) { // if key is pressed before,
                keys[key].count = -1;
            }
            else { // if key is released before,
                keys[key].count--;
            }
        }
    }
}

int cKeyboard::keyCount(int key) {
    if (!isValidKey(key)) return 0;
    switch (key) {
    case KEY_INPUT_LSHIFT: case KEY_INPUT_RSHIFT:
        return max(keys[KEY_INPUT_LSHIFT].count, keys[KEY_INPUT_RSHIFT].count);
    case KEY_INPUT_LCONTROL: case KEY_INPUT_RCONTROL:
        return max(keys[KEY_INPUT_LCONTROL].count, keys[KEY_INPUT_RCONTROL].count);
    case KEY_INPUT_LALT: case KEY_INPUT_RALT:
        return max(keys[KEY_INPUT_LALT].count, keys[KEY_INPUT_RALT].count);
    default:
        return keys[key].count;
    }
}

int cKeyboard::keyState(int key) {
    int count = keyCount(key);
    if (count == 1) return sKeyState::RELEASEtoPRESS;
    if (count == -1) return sKeyState::PRESStoRELEASE;
    if (count > 0) return sKeyState::PRESS;
    if (count < 0) return sKeyState::RELEASE;
    return sKeyState::NO_SIGNAL;
}

bool cKeyboard::isKeyTyped(int key) {
    if (keyState(key) != TYPE_KEY_STATE) return false;
    cSound::inst()->playSelectSE();
    return true;
}