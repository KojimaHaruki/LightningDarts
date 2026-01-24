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

    static constexpr int QUIT = 0;
    static constexpr int CONFIG = 1;
    static constexpr int HOME = 2;
    static constexpr int GAME_SELECT = 3;
    static constexpr int PLAYER_SELECT = 4;
    static constexpr int SCENE_NUM = 5;
    static constexpr int LEFT = 5;
    static constexpr int RIGHT = 6;
    static constexpr int DOWN = 7;
    static constexpr int UP = 8;
    static constexpr int SKILL = 9;
    static constexpr int INITIALIZE = 10;
    static constexpr int RESET = 11;
    static constexpr int SKIP = 12;
    static constexpr int CHANGE_BGM = 13;
    static constexpr int ANOTHER_WINDOW = 14;
    static constexpr int MAXIMIZE_WINDOW = 15;
    static constexpr int MUTE = 16;
    static constexpr int UNMUTE = 17;
    static constexpr int PAUSE = 18;
    static constexpr int RESUME = 19;
    static constexpr int BACK = 20;
    static constexpr int FORWARD = 21;
    static constexpr int ICON_NUM = 25;
    static constexpr int START = 22;
    static constexpr int YES = 23;
    static constexpr int NO = 24;
    static constexpr int NUM = 25;

    // setter
    void init();
    void initKey();
    void loadIcon();
    void reloadIcon();
    void initIconBox();

    // getter
    cImage icon(int ctrl);
    cBox& iconBox(int ctrl);
    int keyCode(int ctrl);
    std::string name(int ctrl);
    
    // Functions
    bool isIconClicked(int ctrl);
    bool isKeyTyped(int ctrl);
    bool isRequested(int ctrl);

private:
    // data
    cImage mIcon[ICON_NUM];
    int mKeyCode[NUM] = {};
    std::string mName[NUM] = { 
		"Quit", "Config", "Home", "Game select", "Player select", "Left", "Right", "Down", "Up",
        "Skill Effect", "Initialize", "Reset", "Skip", "Change BGM", "Another Window", "Maximize Window",
		"Mute", "Unmute", "Pause", "Resume", "Back", "Forward", "Start", "Yes", "No"
    };
};