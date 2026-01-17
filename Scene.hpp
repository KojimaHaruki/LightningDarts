#pragma once
#include "DxLib.h"
#include "Box.hpp"
#include "Keyboard.hpp"
#include "Timer.hpp"
#include <iostream>
#include <vector>

class cScene {
public:
    static constexpr int MAX_CHARA_NUM = 28;
    static constexpr int MAX_GROUP_NUM = 10;
    static constexpr int MAX_PLAYER_NUM = 8;
    static constexpr int MAX_TEAM_NUM = 4;
    static constexpr int SOLO_MEMBER_NUM = 1;
    static constexpr int DUO_MEMBER_NUM = 2;
protected:
    // Scene
    int mNowScene, mNextScene;

    // Time
    const std::string wday[7] = { "Sun","Mon","Tue","Wed","Thu", "Fri","Sat" };
    time_t nowTime;
    tm nowLocalTime;
    errno_t timeError;

    // Skill
    std::string SkillName[5] = { "Single!", "Double!!", "Triple!!!", "BULL!!", "!!IN BULL!!" };

    // Image
    struct sImage {
        cBox box;
        int handle = 0;
        int trans = TRUE;
    };
    Coordinate2d<int> ICONSIZE_NORMAL;

    // Key for control
    struct sCtrlKey {
        sImage image;
        int code = 0;
    };

    // Control
    struct sCtrl {
        sCtrlKey key;
        sImage icon;
        std::string name;
    };
    struct sCtrlKind {
        sCtrl left, right, down, up, skill, home, gameSelect, playerSelect, reset,
            init, skip, quit, config, bgm, window[2], mute[2], pause[2], back, forward, start, yes, no;
    };

    // Picture
    struct sPicture {
        sImage image;
    };
    struct PictureKind {
        sPicture selected, darts, thunder;
    };

    // Screen
    struct sBoxObj {
        cBox box;
        int color = 0;
        int fill = TRUE;
    };
    struct sScreenObjKind {
        cBox upperFrame, lowerFrame;
    };

	// Color
    unsigned int white = 0U, black = 0U, gray = 0U,
		red = 0U, green = 0U, blue = 0U, magenta = 0U, cyan = 0U, yellow = 0U, tableColor = 0U,
        touchColor = 0U, pressColor = 0U, executeColor = 0U, teamColor[cScene::MAX_PLAYER_NUM] = {},
        rankColor[MAX_PLAYER_NUM] = {};

    // Font
	int Sfont = 0, SfontSize = 0, Mfont = 0, MfontSize = 0, XLfont = 0, XLfontSize = 0;

    // Game
	int mGameMode = 0;

    // Character
    const std::string playerFolderPath = "C:/Users/miniy/OneDrive/‰æ‘œ/Darts Player";
    struct sCharaStatus {
        double winRate = 0;
        int rank = 0;
    };
    struct sChara {
        std::string name = {};
        std::string path = {};
        sImage image;
        std::string group = {};
        sCharaStatus status;
    };
    struct sGroup {
        std::string name = {};
        std::vector<sChara> members = {};
    };

    // Font
    struct sFont {
        std::string name;
        int thick = 0;
        int size = 0;
        int type = DX_FONTTYPE_NORMAL;
        int handle = 0;
    };

    // Darts
    const std::string rankName[MAX_PLAYER_NUM] = { "1st", "2nd", "3rd", "4th", "5th", "6th", "7th", "8th" };

    struct ShareData {
        int window = FALSE;
        cBox screen;
        sCtrlKind ctrl;
        PictureKind pic;
        sScreenObjKind obj;
        std::vector<sGroup> groups, teams;
        int SkillEffectMode = TRUE;
    }; ShareData sd;
    const char* ShareDataFileName = "ShareData.dat";
    // SaveData
    struct ConfigData {
        sCharaStatus cs[MAX_CHARA_NUM];
    }; ConfigData cd;
    const char* ConfigDataFileName = "ConfigData.dat";

    // Functions
    void loadColor();
    void loadFont();
    void initCtrlKey();
    void initScreenSize();
    void changeWindow(int WindowModeFlag);
    int drawBoxObj(cBox box, int color, int fill = TRUE);
    int drawBoxObj(sBoxObj obj);
    int drawImage(sImage imag);
    bool isClicked(cBox box);
    bool isBoxClicked(int x1, int y1, int x2, int y2);
    bool isClicked(sImage imag);
    bool isKeyTyped(int keyCode);
    bool isTyped(sCtrlKey key);
    bool ctrlRQ(sCtrl ctrl);

public:
    cScene();
    virtual ~cScene();
    inline int currentScene() { return mNowScene; }
    inline int nextScene() { return mNextScene; }
    virtual void init();
    virtual void reset();
    virtual void draw();
    virtual void update();
    virtual void fin();
    static constexpr int NO_CHANGE     = -1;
    static constexpr int CONFIG        =  0;
    static constexpr int HOME          =  1;
    static constexpr int GAME_SELECT   =  2;
    static constexpr int PLAYER_SELECT =  3;
    static constexpr int GAME_START    =  4;
    static constexpr int ZERO_ONE      =  5;
    static constexpr int STANDARD_CRICKET       =  6;
    static constexpr int COUNT_UP      =  7;
    static constexpr int OTHER_GAME    =  8;
    static constexpr int QUIT          =  9;
    ShareData shareData() { return sd; }
};
