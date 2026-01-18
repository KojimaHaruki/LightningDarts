#pragma once
#include "Singleton.hpp"
#include "Box.hpp"
#include "Image.hpp"

class cControl : public Singleton<cControl> {
    cControl() {}
    friend class Singleton<cControl>;
public:
    // key
    struct sKey {
        cImage image;
        int code = 0;
    };

    // data
    struct sData {
        sKey key;
        cImage icon;
        std::string name;
    };
    sData left, right, down, up, skill, home, gameSelect, playerSelect, reset,
        init, skip, quit, config, bgm, window[2], mute[2], pause[2], back, forward, start, yes, no;

    // Functions
    void resetKey() {
        home.key.code = KEY_INPUT_H;      back.key.code = KEY_INPUT_BACK;
        forward.key.code = KEY_INPUT_RETURN; quit.key.code = KEY_INPUT_ESCAPE;
        config.key.code = KEY_INPUT_C;      skill.key.code = KEY_INPUT_E;
        skip.key.code = KEY_INPUT_S;      init.key.code = KEY_INPUT_I;
        gameSelect.key.code = KEY_INPUT_G; playerSelect.key.code = KEY_INPUT_P;
        reset.key.code = KEY_INPUT_R; bgm.key.code = KEY_INPUT_B;
        left.key.code = KEY_INPUT_LEFT;   right.key.code = KEY_INPUT_RIGHT;
        up.key.code = KEY_INPUT_UP;     down.key.code = KEY_INPUT_DOWN;
        start.key.code = KEY_INPUT_SPACE;  yes.key.code = KEY_INPUT_Y;
        no.key.code = KEY_INPUT_N;
        for (int i = 0; i < 2; i++) {
            mute[i].key.code = KEY_INPUT_M;
            window[i].key.code = KEY_INPUT_W;
            pause[i].key.code = KEY_INPUT_PAUSE;
        }
    }
};