#pragma once
#include "DxLib.h"
#include "Box.hpp"
#include <string>
#include <vector>
#include "Player.hpp"
#include "Image.hpp"

class cScene {
public:
    static constexpr int MAX_CHARA_NUM = 28;
    static constexpr int MAX_GROUP_NUM = 10;
    static constexpr int DEFAULT_ICON_WIDTH = 25;
    static constexpr int DEFAULT_ICON_HEIGHT = 25;
protected:
    // Scene
    int mNowScene, mNextScene;

    // Screen
    cBox screen, upperFrame, lowerFrame;

    // Time
    time_t nowTime; tm nowLocalTime; errno_t timeError;

    // Skill
    std::string SkillName[5] = { "Single!", "Double!!", "Triple!!!", "BULL!!", "!!IN BULL!!" };

	// Color
    unsigned int white = 0U, black = 0U, gray = 0U,
		red = 0U, green = 0U, blue = 0U, magenta = 0U, cyan = 0U, yellow = 0U, tableColor = 0U,
        touchColor = 0U, pressColor = 0U, executeColor = 0U, teamColor[cPlayer::MAX_SOLO_PLAYER_NUM] = {},
        rankColor[cPlayer::MAX_SOLO_PLAYER_NUM] = {};

    // Font
	int Sfont = 0, SfontSize = 0, Mfont = 0, MfontSize = 0, XLfont = 0, XLfontSize = 0;

    // Character
    const std::string playerFolderPath = "C:/Users/miniy/OneDrive/‰æ‘œ/Darts Player";
    struct sCharaStatus {
        double winRate = 0;
        int rank = 0;
    };
    struct sChara {
        std::string name = {};
        cImage image;
        std::string group = {};
        sCharaStatus status;
    };
    struct sGroup {
        std::string name = {};
        std::vector<sChara> members = {};
    };

    // Team
    int nTeam = 0;

    struct ShareData {
        cImage selected, darts, thunder;
        std::vector<sGroup> groups, teams;
    }; ShareData sd;
    const char* ShareDataFileName = "ShareData.dat";

    // Functions
    void loadColor();
    void loadFont();
    void loadScreen();
    void initScreenSize();
    void changeWindow();
    void setWindow(int window);

public:
    cScene();
    virtual ~cScene();
    inline int currentScene() { return mNowScene; }
    inline int nextScene() { return mNextScene; }
    virtual void init();
    virtual void reset() {}
    virtual void draw();
    virtual void update();
    virtual void fin() {}
    static constexpr int NO_CHANGE     = -1;
    static constexpr int QUIT = 0;
    static constexpr int CONFIG        =  1;
    static constexpr int HOME          =  2;
    static constexpr int GAME_SELECT   =  3;
    static constexpr int PLAYER_SELECT =  4;
    static constexpr int GAME_START    =  5;
    static constexpr int ZERO_ONE      =  6;
    static constexpr int STANDARD_CRICKET =  7;
    static constexpr int COUNT_UP      =  8;
    ShareData shareData() { return sd; }
};
