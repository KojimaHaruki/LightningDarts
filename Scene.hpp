#pragma once
#include "DxLib.h"
#include "Box.hpp"
#include "Keyboard.hpp"
#include "Timer.hpp"
#include <iostream>

class Scene {
protected:
    // Scene
    int mNowScene, mNextScene;
    bool isConfig;

    // BGM
    std::string bgmFolderPath = "BGM/";
    static constexpr int MAX_BGM_NUM = 30;
    struct BGM {
        int handle = 0;
        std::string name = {};
    };
    struct BGMMode {
        static constexpr int LOOP    = 0;
        static constexpr int ASCEND  = 1;
        static constexpr int DESCEND = 2;
        static constexpr int RANDAM  = 3;
        static constexpr int NUM     = 4;
    };
    const std::string BGMModeName[BGMMode::NUM] = { "Loop", "Ascending order", "Descending order", "Random" };

    // SE
    static constexpr int SE_NUM = 4;
    static constexpr int COMBO_SE_NUM = 7;

    // Sound
    struct Sound {
        static constexpr int TOTAL = 0;
        static constexpr int BGM   = 1;
        static constexpr int SE    = 2;
        static constexpr int NUM   = 3;
    };
    struct SoundVol {
        static constexpr int MIN =   0;
        static constexpr int MAX = 200;
    };
    const std::string SoundName[Sound::NUM] = { "Total", "BGM", "SE" };

    // Skill
    std::string SkillName[5] = { "Single!", "Double!!", "Triple!!!", "BULL!!", "!!IN BULL!!" };

    // Image
    struct Image {
        Box box;
        int handle = 0;
        int trans = TRUE;
    };
    Coordinate2d<int> ICONSIZE_NORMAL;

    // Key for control
    struct CtrlKey {
        Image image;
        int code = 0;
    };

    // Control
    struct Ctrl {
        CtrlKey key;
        Image icon;
        std::string name;
    };
    struct CtrlKind {
        Ctrl left, right, down, up, skill, home, gameSelect, playerSelect, reset,
            init, skip, quit, config, bgm, window[2], mute[2], pause[2], back, forward, start, yes, no;
    };

    // Picture
    struct Picture {
        Image image;
    };
    struct PictureKind {
        Picture selected, darts, thunder;
    };

    // Screen
    struct BoxObj {
        Box box;
        int color = 0;
        int fill = TRUE;
    };
    struct ScreenObjKind {
        BoxObj upperFrame, lowerFrame;
    };

    // Game
    static constexpr int MAIN_GAME_NUM = 3;
    static constexpr int SUB_GAME_NUM = 6;
    static constexpr int GAME_NUM = MAIN_GAME_NUM * SUB_GAME_NUM;
    const std::string mainGameName[MAIN_GAME_NUM] = { "01", "Cricket", "Others" };
    const std::string gameName[GAME_NUM] = {
        "301", "501", "701", "901", "1101", "1501",
        "Standard Cricket", "Random Cricket", "Hidden Cricket",
        "Select-a-Cricket", "All numbers Cricket", "Cricket Count-up",
        "Count-up", "Eagle's eye", "Half-it", "Shoot out", "Sevens heaven", "Big bull"
    };

    // Play mode
    struct TeamType {
        static constexpr int SOLO       = 0;
        static constexpr int DUO        = 1;
        static constexpr int TOURNAMENT = 2;
        static constexpr int NUM        = 3;
    };
    const std::string playModeName[TeamType::NUM] = { "Solo", "Duo", "Tournament" };

    // Character
    const std::string playerFolderPath = "Image/Player/";
    static constexpr int MAX_CHARA_NUM = 28;
    static constexpr int MAX_GROUP_NUM = 10;
    static constexpr int MAX_PLAYER_NUM = 8;
    static constexpr int MAX_TEAM_NUM = 4;
    static constexpr int DUO_MEMBER_NUM = 2;
    struct CharaStatus {
        double winRate = 0;
        int rank = 0;
    };
    struct Chara {
        Image image;
        std::string name = {};
        std::string groupName = {};
        bool isPlayer = false;
        CharaStatus status;
    };

    // Key
    static constexpr int VALID_KEY_NUM = 67;
    const int KeyNo[VALID_KEY_NUM] = { 
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 28, 29, 30, 31, 32, 33, 34,
        35, 36, 37, 38, 42, 44, 45, 46, 47, 48, 49, 50, 54, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 87, 88,
        157, 184, 197, 200, 203, 205, 208, 210, 211 };
    struct KeyImage {
        Image image;
    };

    // Font
    struct Font {
        std::string name;
        int thick = 0;
        int size = 0;
        int type = DX_FONTTYPE_NORMAL;
        int handle = 0;
    };
    struct FontKind {
        Font title = { "Brush Script MT Italic", 5, 60, DX_FONTTYPE_ANTIALIASING_EDGE, 0 },
            normal = { "Calisto MT Bold", 5, 18, DX_FONTTYPE_ANTIALIASING_EDGE, 0 },
            chara = { "Calisto MT Bold", 1, 16, DX_FONTTYPE_ANTIALIASING_EDGE, 0 };
    };

    // Color
    struct ColorHandle {
        unsigned int w = 0, k = 0, gy = 0, r = 0, g = 0, b = 0, touch = 0, press = 0;
        unsigned int team[MAX_PLAYER_NUM] = {};
    };

    // Darts
    struct Game {
        static constexpr int DEFAULT            = -1;
        static constexpr int ZERO_ONE_NUM       =  6;
        static constexpr int STANDARD_CRICKET   =  6;
        static constexpr int RANDAM_CRICKET     =  7;
        static constexpr int HIDDEN_CRICKET     =  8;
        static constexpr int SELECT_A_CRICKET   =  9;
        static constexpr int ALL_NUMBER_CRICKET = 10;
        static constexpr int CRICKET_COUNT_UP   = 11;
    };
    struct DartsRadialPos {
		static constexpr int OUTSIDE      = -1;
		static constexpr int DOUBLE       =  0;
		static constexpr int OUTER_SINGLE =  1;
		static constexpr int TRIPLE       =  2;
		static constexpr int INNER_SINGLE =  3;
		static constexpr int BULL         =  4;
		static constexpr int INNER_BULL   =  5;
        static constexpr int NUM          =  6;
        static constexpr float Radius[NUM] = { 196.85, 178.85, 126.77, 108.77, 22, 8 };
        static constexpr int Power[NUM] = { 2, 1, 3, 1, 1, 2 };
    };
    bool isValidPoint[21];
    const std::string rankName[MAX_PLAYER_NUM] = { "1st", "2nd", "3rd", "4th", "5th", "6th", "7th", "8th" };
    struct Darts {
        Coordinate2d<float> center;
        int point = 0;
        int power = 0;
        int radialPos = 0;
        int totalPoint = 0;
        static constexpr int BOARD_POINT[21] = { 11, 8, 16, 7, 19, 3, 17, 2, 15, 10,
                                                  6, 13, 4, 18, 1, 20, 5, 12, 9, 14, 11 };
        const std::string radialPosName[DartsRadialPos::NUM] = {
            "Double", "Single", "Triple", "Single", "Bull", "Inner Bull"
        };
        static constexpr int OUTSIDE = 0;
        static constexpr int BULL = 21;
        static constexpr int INNER_BULL = 22;
        static constexpr int POINT_NUM = 23;
        const std::string pointName[POINT_NUM] = { "",
            "  1", "  2", "  3", "  4", "  5",
            "  6", "  7", "  8", "  9", " 10",
            " 11", " 12", " 13", " 14", " 15",
            " 16", " 17", " 18", " 19", " 20",
            "Bull", "Inner Bull" 
        };
        static constexpr int POINT_KEY[POINT_NUM] = {
            KEY_INPUT_SPACE,
            KEY_INPUT_1, KEY_INPUT_2, KEY_INPUT_3, KEY_INPUT_4, KEY_INPUT_5,
            KEY_INPUT_6, KEY_INPUT_7, KEY_INPUT_8, KEY_INPUT_9, KEY_INPUT_0,
            KEY_INPUT_F1, KEY_INPUT_F2, KEY_INPUT_F3, KEY_INPUT_F4, KEY_INPUT_F5,
            KEY_INPUT_F6, KEY_INPUT_F7, KEY_INPUT_F8, KEY_INPUT_F9, KEY_INPUT_F10,
            KEY_INPUT_F11, KEY_INPUT_F12
        };
    }; Darts darts;

    struct ShareData {
        Timer gameTime;
        int sound = TRUE;
        BGM bgm[MAX_BGM_NUM];
        int se[SE_NUM] = {};
        int comboSE[COMBO_SE_NUM] = {};
        int bgmNum = 0;
        int playingBGM = 0;
        int bgmVol = 0;
        int seVol = 0;
        int soundVol[Sound::NUM] = {}; // SoundVol[SoundNo] SoundNo 0: Total, 1: BGM, 2: SE
        int bgmMode = BGMMode::RANDAM;           // BGMStyle 0: Loop, 1: Ascending order, 2: Decending order, 3: Random
        int window = FALSE;
        Box screen;
        CtrlKind ctrl;
        PictureKind pic;
        int dartsBoard[4][4] = {};
        int dartsArrow = 0;
        ScreenObjKind obj;
        KeyImage key[Keyboard::KEY_NUM];
        FontKind font;
        int game = Game::DEFAULT;
        Chara chara[MAX_CHARA_NUM];
        int charaNum = 0;
        int groupNum = 0;
        int groupCharaNum[MAX_GROUP_NUM] = {};
        int teamChara[MAX_PLAYER_NUM][DUO_MEMBER_NUM] = {};
        int playerNum = 0;
        int teamNum = 0;
        int teamType = TeamType::SOLO;
        int SkillMode = TRUE;
        ColorHandle color;
    }; ShareData sd;
    const char* ShareDataFileName = "ShareData.dat";
    // SaveData
    struct ConfigData {
        CharaStatus cs[MAX_CHARA_NUM];
    }; ConfigData cd;
    const char* ConfigDataFileName = "ConfigData.dat";

    void initCtrlKey();
    void initScreenSize();
    void initSoundVol();
    void changeWindow(int WindowModeFlag);
    bool changeSound(int SoundModeFlag);
    bool changeSoundVol(int SoundNo, int Vol);
    bool playBGM(int BGMNo);
    int drawBoxObj(Box box, int color, int fill = TRUE);
    int drawBoxObj(BoxObj obj);
    int drawImage(Image imag);
    bool isClicked(Box box);
    bool isBoxClicked(int x1, int y1, int x2, int y2);
    bool isClicked(Image imag);
    bool isKeyTyped(int keyCode);
    bool isTyped(CtrlKey key);
    bool ctrlRQ(Ctrl ctrl);

public:
    Scene();
    virtual ~Scene();
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
    static constexpr int CRICKET       =  6;
    static constexpr int OTHER_GAME    =  7;
    static constexpr int QUIT          =  8;
    ShareData shareData() { return sd; }
};
