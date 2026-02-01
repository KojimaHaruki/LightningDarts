#pragma once
#include "Singleton.hpp"
#include "Box.hpp"
#include "Key.hpp"
#include "DxLib.h"

class cMouse : public Singleton<cMouse> {
    cMouse() {}
    friend Singleton< cMouse >;
    private:
        static constexpr int KEY_NUM = 8;
        cCoordinate2d<int> cursor;
        int mKeyCount[KEY_NUM] = {}; // positive: pressed, negative: released
        static constexpr int TYPE_KEY_STATE = sKeyState::PRESStoRELEASE;

    public:
        void update();

        bool isValidKey(int key) { return 0 <= key && key < KEY_NUM; }

        int keyCount(int key) { return isValidKey(key) ? mKeyCount[key] : 0; }
        int LclickCount() { return mKeyCount[MOUSE_INPUT_LEFT]; }
        int RclickCount() { return mKeyCount[MOUSE_INPUT_RIGHT]; }
        int MclickCount() { return mKeyCount[MOUSE_INPUT_MIDDLE]; }

        int keyState(int key);
        int LclickState();
        int RclickState();
        int MclickState();

        bool isKeyTyped(int key);
        bool isLclicked();
        bool isRclicked();
        bool isMclicked();

        bool isCursorInBox(cBox box) { return box.isInBox(cursor); }
        bool isCursorInBox(int x1, int y1, int x2, int y2);

        int pressBoxCount(int key, cBox box);
        int LclickBoxCount(cBox box);
        int RclickBoxCount(cBox box);
        int MclickBoxCount(cBox box);
        
        int pressBoxCount(int key, int x1, int y1, int x2, int y2);
        int LclickBoxCount(int x1, int y1, int x2, int y2);
        int RclickBoxCount(int x1, int y1, int x2, int y2);
        int MclickBoxCount(int x1, int y1, int x2, int y2);

        int pressBoxState(int key, cBox box);
        int LclickBoxState(cBox box);
        int RclickBoxState(cBox box);
        int MclickBoxState(cBox box);

        int pressBoxState(int key, int x1, int y1, int x2, int y2);
        int LclickBoxState(int x1, int y1, int x2, int y2);
        int RclickBoxState(int x1, int y1, int x2, int y2);
        int MclickBoxState(int x1, int y1, int x2, int y2);

        bool isBoxPressed(int key, cBox box);
        bool isBoxLclicked(cBox box);
        bool isBoxRclicked(cBox box);
        bool isBoxMclicked(cBox box);

        bool isBoxPressed(int key, int x1, int y1, int x2, int y2);
        bool isBoxLclicked(int x1, int y1, int x2, int y2);
        bool isBoxRclicked(int x1, int y1, int x2, int y2);
        bool isBoxMclicked(int x1, int y1, int x2, int y2);

        cCoordinate2d<int> xy() { return cursor; }
        int x() { return cursor.x(); }
        int y() { return cursor.y(); }
};
