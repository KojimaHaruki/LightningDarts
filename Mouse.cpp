#include <DxLib.h>
#include "Mouse.hpp"

bool cMouse::update() {
    int nowButtonState = GetMouseInput();
    int inX = 0, inY = 0;
    GetMousePoint(&inX, &inY);
    cursor.setXY(inX, inY);
    for (int i = 0; i < KEY_NUM; i++) {
        if (nowButtonState & i) { // if key is pressed,
            if (mPressKeyCount[i] < 0) { // if key is released before,
                mPressKeyState[i] = sKey::RELEASEDtoPRESSED;
                mPressKeyCount[i] = 1;
            }
            else { // if key is pressed before,
                mPressKeyState[i] = sKey::PRESSED;
                mPressKeyCount[i]++;
            }
        }
        else { // if key is released,
            if (mPressKeyCount[i] > 0) { // if key is pressed before,
                mPressKeyState[i] = sKey::PRESSEDtoRELEASED;
                mPressKeyCount[i] = -1;
            }
            else { // if key is released before,
                mPressKeyState[i] = sKey::RELEASED;
                mPressKeyCount[i]--;
            }
        }
    }
    return true;
}
int cMouse::pressKeyCount(int keyCode) {
    return (keyCode >= 0 && keyCode < KEY_NUM) ? mPressKeyCount[keyCode] : 0;
}
int cMouse::pressKeyState(int keyCode) {
    return (keyCode >= 0 && keyCode < KEY_NUM) ? mPressKeyState[keyCode] : 0;
}
bool cMouse::isCursorInBox(int x1, int y1, int x2, int y2) {
    return cursor.x() > x1 && cursor.y() > y1 && cursor.x() < x2 && cursor.y() < y2;
}
int cMouse::clickBoxCount(cBox box) {
    if (!isCursorInBox(box)) return 0; return clickCount();
}
int cMouse::clickBoxCount(int x1, int y1, int x2, int y2) {
    if (!isCursorInBox(x1, y1, x2, y2)) return 0; return clickCount();
}
int cMouse::clickBoxState(cBox box) {
    if (!isCursorInBox(box)) return sKey::NO_SIGNAL; return clickState();
}
int cMouse::clickBoxState(int x1, int y1, int x2, int y2) {
    if (!isCursorInBox(x1, y1, x2, y2)) return sKey::NO_SIGNAL; return clickState();
}
int cMouse::rightClickBoxCount(cBox box) {
    if (!isCursorInBox(box)) return 0; return rightClickCount();
}
int cMouse::rightClickBoxCount(int x1, int y1, int x2, int y2) {
    if (!isCursorInBox(x1, y1, x2, y2)) return 0; return rightClickCount();
}
int cMouse::rightClickBoxState(cBox box) {
    if (!isCursorInBox(box)) return sKey::NO_SIGNAL; return rightClickState();
}
int cMouse::rightClickBoxState(int x1, int y1, int x2, int y2) {
    if (!isCursorInBox(x1, y1, x2, y2)) return sKey::NO_SIGNAL; return rightClickState();
}
int cMouse::pressBoxCount(int keyCode, cBox box) {
    if (!isCursorInBox(box)) return 0; return pressKeyCount(keyCode);
}
int cMouse::pressBoxCount(int keyCode, int x1, int y1, int x2, int y2) {
    if (!isCursorInBox(x1, y1, x2, y2)) return 0;
    return pressKeyCount(keyCode);
}
int cMouse::pressBoxState(int keyCode, cBox box) {
    if (!isCursorInBox(box)) return sKey::NO_SIGNAL; return pressKeyState(keyCode);
}
int cMouse::pressBoxState(int keyCode, int x1, int y1, int x2, int y2) {
    if (!isCursorInBox(x1, y1, x2, y2)) return sKey::NO_SIGNAL; return pressKeyState(keyCode);
}