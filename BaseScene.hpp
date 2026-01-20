#pragma once
#include "Box.hpp"
#include <string>
#include "Image.hpp"
#include "Player.hpp"

class cBaseScene {
protected:
    // Scene
    int mNowScene, mNextScene;

    // Screen
    cBox screen, upperFrame, lowerFrame;

    // Time
    time_t nowTime; tm nowLocalTime; errno_t timeError;

	// Color
    unsigned int white = 0U, black = 0U, gray = 0U,
		red = 0U, green = 0U, blue = 0U, magenta = 0U, cyan = 0U, yellow = 0U, tableColor = 0U,
        touchColor = 0U, pressColor = 0U, executeColor = 0U, teamColor[cPlayer::MAX_SOLO_PLAYER_NUM] = {},
        rankColor[cPlayer::MAX_SOLO_PLAYER_NUM] = {};

    // Font
	int Sfont = 0, SfontSize = 0, Mfont = 0, MfontSize = 0, 
        Lfont = 0, LfontSize = 0, XLfont = 0, XLfontSize = 0;

    // Team
    int nTeam = 0;

    // Functions
    void loadColor();
    void loadFont();
    void loadScreen();
    void initScreenSize();
    void changeWindow();
    void setWindow(int window);

public:
    cBaseScene();
    virtual ~cBaseScene();
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
};
