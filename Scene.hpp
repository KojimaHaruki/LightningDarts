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
    static constexpr int MAIN_GAME_NUM = 3;
    static constexpr int SUB_GAME_NUM = 6;
    static constexpr int GAME_NUM = MAIN_GAME_NUM * SUB_GAME_NUM;
    static constexpr int TEAM_TYPE_NUM = 2;
    static constexpr int SOLO_MEMBER_NUM = 1;
    static constexpr int DUO_MEMBER_NUM = 2;
    static constexpr int VALID_KEY_NUM = 67;
protected:
    // Scene
    int mNowScene, mNextScene;
    bool mIsConfig;

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
	int mGame = 0;
    const std::string mainGameName[MAIN_GAME_NUM] = { "01", "Cricket", "Others" };
    const std::string gameName[GAME_NUM] = {
        "301", "501", "701", "901", "1101", "1501",
        "Standard Cricket", "Random Cricket", "Hidden Cricket",
        "Select-a-Cricket", "All numbers Cricket", "Cricket Count-up",
        "Count-up", "Eagle's eye", "Half-it", "Shoot out", "Sevens heaven", "Big bull"
    };

    // Team type
    struct sTeamType {
        static constexpr int SOLO       = 0;
        static constexpr int DUO        = 1;
    };
    const std::string teamTypeName[TEAM_TYPE_NUM] = { "Solo", "Duo" };

    // Character
    const std::string playerFolderPath = "./Image/Player";
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

    // Key
    const int KeyNo[VALID_KEY_NUM] = { 
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 28, 29, 30, 31, 32, 33, 34,
        35, 36, 37, 38, 42, 44, 45, 46, 47, 48, 49, 50, 54, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 87, 88,
        157, 184, 197, 200, 203, 205, 208, 210, 211 };
    struct sKeyImage {
        sImage image;
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
        sKeyImage key[Keyboard::KEY_NUM];
        std::vector<sGroup> groups, teams;
        int teamType = sTeamType::SOLO;
        int SkillMode = TRUE;
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
