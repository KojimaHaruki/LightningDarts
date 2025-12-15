#pragma once
#define _SILENCE_NONFLOATING_COMPLEX_DEPRECATION_WARNING
#include "Singleton.hpp"
#include "Box.hpp"
#include "Key.hpp"

class Mouse : public Singleton<Mouse> {
    Mouse();
    friend Singleton< Mouse >;
    private:
        static constexpr int KEY_NUM = 8;
        std::complex<int> pos;
        Key key[KEY_NUM];
        inline bool isValidKey(int keyCode) { return keyCode >= 0 && keyCode < KEY_NUM; }
    public:
        bool update();
        int getPressCount(int keyCode);
        int getPressState(int keyCode);
        const int getClickCount() { return key[MOUSE_INPUT_LEFT].pressCount; }
        const int getClickState() { return key[MOUSE_INPUT_LEFT].pressState; }
        const int getRightClickCount() { return key[MOUSE_INPUT_RIGHT].pressCount; }
        const int getRightClickState() { return key[MOUSE_INPUT_RIGHT].pressState; }
        const bool isCursorInBox(Box box) { return box.isInBox(pos); }
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
        std::complex<int> xy() { return pos; }
        std::complex<float> floatxy() { std::complex<float> xy(real(pos), imag(pos)); return xy; }
        const int x() { return real(pos); }
        const int y() { return imag(pos); }
};
