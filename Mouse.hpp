#pragma once

#include "Singleton.hpp"
#include "Box.hpp"
#include "Key.hpp"

class Mouse : public Singleton<Mouse> {
    Mouse();
    friend Singleton< Mouse >;
    private:
        static constexpr int KEY_NUM = 8;
        Coordinate2d<int> pos;
        Key key[KEY_NUM];
        inline bool isValidKey(int keyCode) { return keyCode >= 0 && keyCode < KEY_NUM; }
    public:
        bool update();
        int getPressCount(int keyCode);
        int getPressState(int keyCode);
        inline int getClickCount() { return key[MOUSE_INPUT_LEFT].pressCount; }
        inline int getClickState() { return key[MOUSE_INPUT_LEFT].pressState; }
        inline int getRightClickCount() { return key[MOUSE_INPUT_RIGHT].pressCount; }
        inline int getRightClickState() { return key[MOUSE_INPUT_RIGHT].pressState; }
        inline bool isCursorInBox(Box box) { return box.isInBox(pos); }
        bool isCursorInBox(int x1, int y1, int x2, int y2);
        int getClickBoxCount(Box box);
        int getClickBoxCount(int x1, int y1, int x2, int y2);
        int getClickBoxState(Box box);
        int getClickBoxState(int x1, int y1, int x2, int y2);
        int getRightClickBoxCount(Box box);
        int getRightClickBoxCount(int x1, int y1, int x2, int y2);
        int getRightClickBoxState(Box box);
        int getRightClickBoxState(int x1, int y1, int x2, int y2);
        int getPressBoxCount(int keyCode, Box box);
        int getPressBoxCount(int keyCode, int x1, int y1, int x2, int y2);
        int getPressBoxState(int keyCode, Box box);
        int getPressBoxState(int keyCode, int x1, int y1, int x2, int y2);
        inline Coordinate2d<int> xy() { return pos; }
        inline int x() { return pos.x(); }
        inline int y() { return pos.y(); }
};
