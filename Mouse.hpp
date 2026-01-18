#pragma once

#include "Singleton.hpp"
#include "Box.hpp"
#include "Key.hpp"

class cMouse : public Singleton<cMouse> {
    cMouse() {}
    friend Singleton< cMouse >;
    private:
        static constexpr int KEY_NUM = 8;
        cCoordinate2d<int> cursor;
        int mPressKeyCount[KEY_NUM] = {}; // positive: pressed, negative: released
        int mPressKeyState[KEY_NUM] = {};
    public:
        bool update();
        int pressKeyCount(int keyCode);
        int pressKeyState(int keyCode);
        inline int clickCount() { return mPressKeyCount[MOUSE_INPUT_LEFT]; }
        inline int clickState() { return mPressKeyState[MOUSE_INPUT_LEFT]; }
        inline int rightClickCount() { return mPressKeyCount[MOUSE_INPUT_RIGHT]; }
        inline int rightClickState() { return mPressKeyState[MOUSE_INPUT_RIGHT]; }
        inline bool isCursorInBox(cBox box) { return box.isInBox(cursor); }
        bool isCursorInBox(int x1, int y1, int x2, int y2);
        int clickBoxCount(cBox box);
        int clickBoxCount(int x1, int y1, int x2, int y2);
        int clickBoxState(cBox box);
        int clickBoxState(int x1, int y1, int x2, int y2);
        int rightClickBoxCount(cBox box);
        int rightClickBoxCount(int x1, int y1, int x2, int y2);
        int rightClickBoxState(cBox box);
        int rightClickBoxState(int x1, int y1, int x2, int y2);
        int pressBoxCount(int keyCode, cBox box);
        int pressBoxCount(int keyCode, int x1, int y1, int x2, int y2);
        int pressBoxState(int keyCode, cBox box);
        int pressBoxState(int keyCode, int x1, int y1, int x2, int y2);
        inline cCoordinate2d<int> xy() { return cursor; }
        inline int x() { return cursor.x(); }
        inline int y() { return cursor.y(); }
};
