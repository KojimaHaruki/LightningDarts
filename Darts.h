#pragma once
#include "Mouse.h"
#include "Keyboard.h"
#include "Timer.h"
#include "Coordinate.h"
#include "Box.h"
#include <iostream>
#include <string>
#include <complex>
struct Scene {
    static constexpr int Home       = 0;
    static constexpr int GameSelect = 1;
    static constexpr int CharSelect = 2;
    static constexpr int GameStart  = 3;
    static constexpr int Game       = 4;
    static constexpr int Pause      = 5;
    static constexpr int Result     = 6;
    static constexpr int Num        = 7;
    static constexpr int Quit       = 8;
};
struct ConfigID {
    static constexpr int Home       = -7;
    static constexpr int ModeSelect = -6;
    static constexpr int CharSelect = -5;
    static constexpr int GameStart  = -4;
    static constexpr int Game       = -3;
    static constexpr int Pause      = -2;
    static constexpr int Result     = -1;
    static constexpr int Num        =  7;
};
class Darts {
public:
    Darts();
    ~Darts();
    void BackSystem();
    int DrawScene();
private:
    void ChangeWindow(int WindowModeFlag);
    bool ChangeSound(int SoundModeFlag);
    bool ChangeSoundVol(int SoundNo, int Vol);
    bool PlayBGM(int BGMNo);
    struct CtrlIcon {
        Box box;
        int handle = 0;
        int trans = TRUE;
    };
    Coordinate2d<int> ICONSIZE_NORMAL;
    struct Image {
        Coordinate2d<int> pos;
        int handle = 0;
        int trans = TRUE;
    };
    struct CtrlKey {
        Image image;
        int code = 0;
    };
    struct Ctrl {
        CtrlKey key;
        CtrlIcon icon;
        std::string name;
    };
    struct CtrlKind {
        Ctrl left, right, down, up, skill, home, reGame, init, skip, quit, config, chBGM, chWindow[2],
            mute[2], pause[2], back, forward, start, yes, no, reset;
    }; CtrlKind ctrl;
    struct Picture {
        Image image;
    }; 
    Picture selected, darts, thunder;
    // Scene
    int scene; const std::string SceneName[3] = { "Home", "Game Select", "Char Select" };
    // BGM:1
    std::string bgmFolderPath = "C:/Users/miniyoshi/Music/the very best of fripSide 2009-2020";
    static constexpr int MAX_BGM_NUM = 10;
    struct BGM {
        int handle = 0;
        std::string name = {};
    }; BGM bgm[MAX_BGM_NUM];
    int PlayingBGMNo, BGMVol;
    struct BGMModeID {
        static constexpr int Loop = 0;
        static constexpr int Ascending = 1;
        static constexpr int Descending = 2;
        static constexpr int Random = 3;
        static constexpr int Num = 4;
    };
    const std::string BGMModeName[BGMModeID::Num] = { "Loop", "Ascending order", "Descending order", "Random" };
    struct SEID {
        static constexpr int Num = 4;
        static constexpr int ComboNum = 7;
    };
    int SE[SEID::Num], ComboSE[SEID::ComboNum], SEVol;
    struct MuteModeID {
        static constexpr int Mute = 0; static constexpr int Unmute = 1;
        static constexpr int Num = 2;
    };
    int MuteMode;
    struct SoundID {
        static constexpr int Total = 0;
        static constexpr int BGM   = 1;
        static constexpr int SE    = 2;
        static constexpr int NUM   = 3;
    };
    struct SoundVol {
        static constexpr int Min = 0;
        static constexpr int Max = 200;
    };
    const std::string SoundName[SoundID::NUM] = { "Volume", " BGM", " SE" };
    // indication of Ghost block & Skill name
    bool SkillMode; std::string SkillName[4] = { "   Single!", "  Double!!", "  Triple!!!", "!!TETRIS!!" };
    // window
    int WindowMode;
    // game mode
    static constexpr int GAME_KIND_NUM = 3;
    static constexpr int GAME_NUM = 6;
    struct Mode {
        int main = 0;
        int sub = 0;
        bool isSelected = false;
    };
    const std::string gameKindName[GAME_KIND_NUM] = { "01", "Cricket", "Others" };
    const std::string gameName[GAME_KIND_NUM][GAME_NUM] = {
        { "301", "501", "701", "901", "1101", "1501" },
        { "Standard Cricket", "Random Cricket", "Hidden Cricket", 
          "Select-a-Cricket", "All numbers Cricket", "Cricket Count-up"},
        { "Count-up", "Eagle's eye", "Half-it", "Shoot out", "Sevens heaven", "Big bull"}
    };
    // chara
    std::string playerFolderPath = "Image/Player/";
    static constexpr int MAX_CHARA_NUM = 28;
    static constexpr int GRADE_NUM = 4;
    static constexpr int MAX_PLAYER_NUM = 8;
    int charaNum, gradeCharaNum[GRADE_NUM];
    struct CharaStatus {
        double winRate = 0;
        int rank = 0;
    };
    struct Chara {
        Image image;
        std::string name = {};
        int grade = 0;
        bool isPlayer = false;
        CharaStatus status;
    }; Chara chara[MAX_CHARA_NUM];
    int player[MAX_PLAYER_NUM], playerNum;
    const std::string gradeName[GRADE_NUM] = { "Guest", "B4", "M1", "M2" };
    const std::string playStyleName[GRADE_NUM] = { "Solo", "Duo", "Tournament" };
    // Progress of Game
    struct PauseID {
        static constexpr int Resume = 0; static constexpr int Pause = 1;
        static constexpr int Num    = 2;
    };
    int PauseMode;
    // Game system
    int Turn, Shuttle, Phase, Score, Combo, CT, MaxCT, HP, MaxHP;
    Timer t; static constexpr int StartCount = 3000;

    // Key
    static constexpr int KEY_NUM = 67;
    const int KeyNo[KEY_NUM] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 28, 
                                29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 42, 44, 45, 46, 47, 48, 49, 50, 54, 56, 57, 58, 
                                59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 87, 88, 157, 184, 197, 200, 203, 205, 208, 210, 
                                211 };
    struct KeyImage {
        Image image;
    };
    KeyImage key[Keyboard::KEY_NUM];

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
    FontKind font;

    // Color
    struct ColorHandle {
        unsigned int w = 0;
        unsigned int k = 0;
        unsigned int y = 0;
        unsigned int Reset = 0;
        unsigned int OK = 0;
        unsigned int player[MAX_PLAYER_NUM] = {};
    }; ColorHandle color;

    // Screen
    struct BoxObj {
        Box box;
        int color = 0;
        int fill = TRUE;
    };
    Box screen;
    struct ScreenObjKind {
        BoxObj upperFrame, lowerFrame;
    };
    ScreenObjKind obj;
    
    // SaveData
    struct ConfigData {
        int gameKind = 0; 
        int game = 0;
        int SoundVol[SoundID::NUM] = {};         // SoundVol[SoundNo] SoundNo 0: Total, 1: BGM, 2: SE
        int BGMMode = BGMModeID::Loop;           // BGMStyle 0: Loop, 1: Ascending order, 2: Decending order, 3: Random
        CharaStatus cs[MAX_CHARA_NUM];
    }; ConfigData cd; 
    const char* DataFileName = "ConfigData.dat";

    // Function of each part
    void Config();
    void Home();
    void GameSelect();
    void CharSelect();
    void GameStart();
    void InitScene();
    void Game();
    void updateScene(int TargetScene);
    int DrawBoxObj(BoxObj obj) {
        return DrawBox(obj.box.left(), obj.box.top(), obj.box.right(), obj.box.bottom(), obj.color, obj.fill);
    }
    int DrawIcon(CtrlIcon icon) { 
        return DrawGraph(icon.box.upperLeft().x(), icon.box.upperLeft().y(), icon.handle, icon.trans);
    }
    inline int DrawCtrlIcon(Ctrl ctrl) { return DrawIcon(ctrl.icon); }
    inline int DrawImage(Image image) { return DrawGraph(image.pos.x(), image.pos.y(), image.handle, image.trans); }
    inline int DrawPicture(Picture pic) { return DrawImage(pic.image); }
    inline bool isBoxClicked(Box box) { return Mouse::getInstance()->getClickBoxState(box) == Key::PRESSEDtoRELEASED; }
    inline bool isBoxClicked(int x1, int y1, int x2, int y2) { 
        return Mouse::getInstance()->getClickBoxState(x1, y1, x2, y2) == Key::PRESSEDtoRELEASED; 
    }
    inline bool isCtrlIconClicked(Ctrl ctrl) { DrawCtrlIcon(ctrl); return isBoxClicked(ctrl.icon.box); }
    inline bool isKeyTyped(int keyCode) { return Keyboard::getInstance()->getPressState(keyCode) == Key::PRESSEDtoRELEASED; }
    inline bool isCtrlKeyTyped(Ctrl ctrl) { return isKeyTyped(ctrl.key.code); };
    inline bool isCtrled(Ctrl ctrl) { return isCtrlIconClicked(ctrl) || isCtrlKeyTyped(ctrl); }
};
