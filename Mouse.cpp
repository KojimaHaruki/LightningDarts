#include <DxLib.h>
#include "Mouse.hpp"

Mouse::Mouse() {
}
bool Mouse::update() {
    int nowButtonState = GetMouseInput();
    int inX = 0, inY = 0;
    GetMousePoint(&inX, &inY);
    pos.setXY(inX, inY);
    for (int i = 0; i < KEY_NUM; i++) {
        if (nowButtonState & i) { // if key is pressed,
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
int Mouse::getPressCount(int keyCode) { 
    if (!isValidKey(keyCode)) return 0; return key[keyCode].pressCount; 
}
int Mouse::getPressState(int keyCode) { 
    if (!isValidKey(keyCode)) return 0; return key[keyCode].pressState; 
}
bool Mouse::isCursorInBox(int x1, int y1, int x2, int y2) {
    return pos.x() > x1 && pos.y() > y1 && pos.x() < x2 && pos.y() < y2;
}
int Mouse::getClickBoxCount(Box box) {
    if (!isCursorInBox(box)) return 0; return getClickCount();
}
int Mouse::getClickBoxCount(int x1, int y1, int x2, int y2) {
    if (!isCursorInBox(x1, y1, x2, y2)) return 0; return getClickCount();
}
int Mouse::getClickBoxState(Box box) {
    if (!isCursorInBox(box)) return Key::NO_SIGNAL; return getClickState();
}
int Mouse::getClickBoxState(int x1, int y1, int x2, int y2) {
    if (!isCursorInBox(x1, y1, x2, y2)) return Key::NO_SIGNAL; return getClickState();
}
int Mouse::getRightClickBoxCount(Box box) {
    if (!isCursorInBox(box)) return 0; return getRightClickCount();
}
int Mouse::getRightClickBoxCount(int x1, int y1, int x2, int y2) {
    if (!isCursorInBox(x1, y1, x2, y2)) return 0; return getRightClickCount();
}
int Mouse::getRightClickBoxState(Box box) {
    if (!isCursorInBox(box)) return Key::NO_SIGNAL; return getRightClickState();
}
int Mouse::getRightClickBoxState(int x1, int y1, int x2, int y2) {
    if (!isCursorInBox(x1, y1, x2, y2)) return Key::NO_SIGNAL; return getRightClickState();
}
int Mouse::getPressBoxCount(int keyCode, Box box) {
    if (!isCursorInBox(box)) return 0; return getPressCount(keyCode);
}
int Mouse::getPressBoxCount(int keyCode, int x1, int y1, int x2, int y2) {
    if (!isCursorInBox(x1, y1, x2, y2)) return 0; 
    return getPressCount(keyCode);
}
int Mouse::getPressBoxState(int keyCode, Box box) {
    if (!isCursorInBox(box)) return Key::NO_SIGNAL; return getPressState(keyCode);
}
int Mouse::getPressBoxState(int keyCode, int x1, int y1, int x2, int y2) {
    if (!isCursorInBox(x1, y1, x2, y2)) return Key::NO_SIGNAL; return getPressState(keyCode);
}