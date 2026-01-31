#pragma once
#include "Box.hpp"
#include "Player.hpp"

class cBaseScene {
protected:
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
    virtual void init();
    virtual void reset() {}
    virtual void draw();
    virtual void update();
};
