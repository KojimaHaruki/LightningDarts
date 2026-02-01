#include "Mouse.hpp"
#include "Sound.hpp"

void cMouse::update() {
    int keyState = GetMouseInput();
    int inX = 0, inY = 0;
    GetMousePoint(&inX, &inY);
    cursor.setXY(inX, inY);
    for (int key = 0; key < KEY_NUM; key++) {
        if (keyState & key) { // if key is pressed,
            if (mKeyCount[key] < 0) { // if key is released before,
                mKeyCount[key] = 1;
            }
            else { // if key is pressed before,
                mKeyCount[key]++;
            }
        }
        else { // if key is released,
            if (mKeyCount[key] > 0) { // if key is pressed before,
                mKeyCount[key] = -1;
            }
            else { // if key is released before,
                mKeyCount[key]--;
            }
        }
    }
}

int cMouse::keyState(int key) {
    int count = keyCount(key);
    if (count == 1) return sKeyState::RELEASEtoPRESS;
    if (count == -1) return sKeyState::PRESStoRELEASE;
    if (count > 0) return sKeyState::PRESS;
    if (count < 0) return sKeyState::RELEASE;
    return sKeyState::NO_SIGNAL;
}

int cMouse::LclickState() {
    int count = mKeyCount[MOUSE_INPUT_LEFT];
    if (count == 1) return sKeyState::RELEASEtoPRESS;
    if (count == -1) return sKeyState::PRESStoRELEASE;
    if (count > 0) return sKeyState::PRESS;
    if (count < 0) return sKeyState::RELEASE;
    return sKeyState::NO_SIGNAL;
}

int cMouse::RclickState() {
    int count = mKeyCount[MOUSE_INPUT_RIGHT];
    if (count == 1) return sKeyState::RELEASEtoPRESS;
    if (count == -1) return sKeyState::PRESStoRELEASE;
    if (count > 0) return sKeyState::PRESS;
    if (count < 0) return sKeyState::RELEASE;
    return sKeyState::NO_SIGNAL;
}

int cMouse::MclickState() {
    int count = mKeyCount[MOUSE_INPUT_MIDDLE];
    if (count == 1) return sKeyState::RELEASEtoPRESS;
    if (count == -1) return sKeyState::PRESStoRELEASE;
    if (count > 0) return sKeyState::PRESS;
    if (count < 0) return sKeyState::RELEASE;
    return sKeyState::NO_SIGNAL;
}

bool cMouse::isKeyTyped(int key) {
    if (keyState(key) != TYPE_KEY_STATE) return false;
    cSound::inst()->playSelectSE();
    return true;
}

bool cMouse::isLclicked() {
    if (LclickState() != TYPE_KEY_STATE) return false;
    cSound::inst()->playSelectSE();
    return true;
}

bool cMouse::isRclicked() {
    if (RclickState() != TYPE_KEY_STATE) return false;
    cSound::inst()->playSelectSE();
    return true;
}

bool cMouse::isMclicked() {
    if (MclickState() != TYPE_KEY_STATE) return false;
    cSound::inst()->playSelectSE();
    return true;
}

bool cMouse::isCursorInBox(int x1, int y1, int x2, int y2) {
    return cursor.x() > x1 && cursor.y() > y1 && cursor.x() < x2 && cursor.y() < y2;
}

int cMouse::pressBoxCount(int key, cBox box) {
    return isCursorInBox(box) ? keyCount(key) : 0;
}

int cMouse::LclickBoxCount(cBox box) {
    return isCursorInBox(box) ? LclickCount() : 0;
}

int cMouse::RclickBoxCount(cBox box) {
    return isCursorInBox(box) ? RclickCount() : 0;
}

int cMouse::MclickBoxCount(cBox box) {
    return isCursorInBox(box) ? MclickCount() : 0;
}

int cMouse::pressBoxCount(int key, int x1, int y1, int x2, int y2) {
    return isCursorInBox(x1, y1, x2, y2) ? keyCount(key) : 0;
}

int cMouse::LclickBoxCount(int x1, int y1, int x2, int y2) {
    return isCursorInBox(x1, y1, x2, y2) ? LclickCount() : 0;
}

int cMouse::RclickBoxCount(int x1, int y1, int x2, int y2) {
    return isCursorInBox(x1, y1, x2, y2) ? RclickCount() : 0;
}

int cMouse::MclickBoxCount(int x1, int y1, int x2, int y2) {
    return isCursorInBox(x1, y1, x2, y2) ? MclickCount() : 0;
}

int cMouse::pressBoxState(int key, cBox box) {
    return isCursorInBox(box) ? keyState(key) : sKeyState::NO_SIGNAL;
}

int cMouse::LclickBoxState(cBox box) {
    return isCursorInBox(box) ? LclickState() : sKeyState::NO_SIGNAL;
}

int cMouse::RclickBoxState(cBox box) {
    return isCursorInBox(box) ? RclickState() : sKeyState::NO_SIGNAL;
}

int cMouse::MclickBoxState(cBox box) { 
    return isCursorInBox(box) ? MclickState() : sKeyState::NO_SIGNAL;
}

int cMouse::pressBoxState(int key, int x1, int y1, int x2, int y2) { 
    return isCursorInBox(x1, y1, x2, y2) ? keyState(key) : sKeyState::NO_SIGNAL;
}

int cMouse::LclickBoxState(int x1, int y1, int x2, int y2) {
    return isCursorInBox(x1, y1, x2, y2) ? LclickState() : sKeyState::NO_SIGNAL;
}

int cMouse::RclickBoxState(int x1, int y1, int x2, int y2) {
    return isCursorInBox(x1, y1, x2, y2) ? RclickState() : sKeyState::NO_SIGNAL;
}

int cMouse::MclickBoxState(int x1, int y1, int x2, int y2) {
    return isCursorInBox(x1, y1, x2, y2) ? MclickState() : sKeyState::NO_SIGNAL;
}

bool cMouse::isBoxPressed(int key, cBox box) {
    return isCursorInBox(box) && isKeyTyped(key);
}

bool cMouse::isBoxLclicked(cBox box) {
    return isCursorInBox(box) && isLclicked();
}

bool cMouse::isBoxRclicked(cBox box) {
    return isCursorInBox(box) && isRclicked();
}

bool cMouse::isBoxMclicked(cBox box) {
    return isCursorInBox(box) && isMclicked();
}

bool cMouse::isBoxPressed(int key, int x1, int y1, int x2, int y2) {
    return isCursorInBox(x1, y1, x2, y2) && isKeyTyped(key);
}

bool cMouse::isBoxLclicked(int x1, int y1, int x2, int y2) {
    return isCursorInBox(x1, y1, x2, y2) && isLclicked();
}

bool cMouse::isBoxRclicked(int x1, int y1, int x2, int y2) {
    return isCursorInBox(x1, y1, x2, y2) && isRclicked();
}

bool cMouse::isBoxMclicked(int x1, int y1, int x2, int y2) {
    return isCursorInBox(x1, y1, x2, y2) && isMclicked();
}