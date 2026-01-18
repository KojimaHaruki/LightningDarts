#pragma once
#include "Singleton.hpp"
#include "Box.hpp"
#include "Image.hpp"

class cControl : public Singleton<cControl> {
    cControl() {}
    friend class Singleton<cControl>;
public:
    static constexpr int DEFAULT_ICON_WIDTH = 25;
    static constexpr int DEFAULT_ICON_HEIGHT = 25;

    static constexpr int LEFT = 0;
    static constexpr int RIGHT = 1;
    static constexpr int DOWN = 2;
    static constexpr int UP = 3;
    static constexpr int SKILL = 4;
    static constexpr int HOME = 5;
    static constexpr int GAME_SELECT = 6;
    static constexpr int PLAYER_SELECT = 7;
    static constexpr int RESET = 8;
    static constexpr int INITIALIZE = 9;
    static constexpr int SKIP = 10;
    static constexpr int QUIT = 11;
    static constexpr int CONFIG = 12;
    static constexpr int CHANGE_BGM = 13;
    static constexpr int ANOTHER_WINDOW = 14;
    static constexpr int MAXIMIZE_WINDOW = 15;
    static constexpr int MUTE = 16;
    static constexpr int UNMUTE = 17;
    static constexpr int PAUSE = 18;
    static constexpr int RESUME = 19;
    static constexpr int BACK = 20;
    static constexpr int FORWARD = 21;
    static constexpr int START = 22;
    static constexpr int YES = 23;
    static constexpr int NO = 24;
    static constexpr int NUM = 25;

    cImage icon(int ctrl) { return mCtrl[ctrl].icon; }
    cBox& iconBox(int ctrl) { return mCtrl[ctrl].icon.box(); }
    // Functions
    void init();
    void loadName();
    void initKey();
    void loadIcon();
    void reloadIcon();
    void initIconBox();
    bool isRequested(int ctrl);
private:

    // data
    struct sData {
        int keyCode = 0;
        cImage icon;
        std::string name = {};
    };
    sData mCtrl[NUM];
};