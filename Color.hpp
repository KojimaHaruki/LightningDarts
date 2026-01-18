#pragma once
#include "Singleton.hpp"
#include "Team.hpp"

class cColor : public Singleton<cColor> {
    cColor() {}
    friend class Singleton<cColor>;
    unsigned int mWhite = 0U, mBlack = 0U, mGray = 0U, mBrown = 0U,
        mRed = 0U, mGreen = 0U, mBlue = 0U, mMagenta = 0U, mCyan = 0U, mYellow = 0U,
        mTableColor = 0U, mTouchColor = 0U, mPressColor = 0U, mExecuteColor = 0U,
        mTeamColor[cTeam::MAX_SOLO_PLAYER_NUM] = {}, mRankColor[cTeam::MAX_SOLO_PLAYER_NUM] = {};
public:
    void load();
    unsigned int white() { return mWhite; }
    unsigned int black() { return mBlack; }
    unsigned int gray() { return mGray; }
    unsigned int brown() { return mBrown; }
    unsigned int red() { return mRed; }
    unsigned int green() { return mGreen; }
    unsigned int blue() { return mBlue; }
	unsigned int magenta() { return mMagenta; }
    unsigned int cyan() { return mCyan; }
    unsigned int yellow() { return mYellow; }
	unsigned int tableColor() { return mTableColor; }
    unsigned int touchColor() { return mTouchColor; }
    unsigned int pressColor() { return mPressColor; }
    unsigned int executeColor() { return mExecuteColor; }
    unsigned int teamColor(int team) {
        return (team >= 0 || team < cTeam::MAX_SOLO_PLAYER_NUM) ? mTeamColor[team] : 0U;
    }
    unsigned int rankColor(int rank) {
        return (rank >= 0 || rank < cTeam::MAX_SOLO_PLAYER_NUM) ? mRankColor[rank] : 0U;
    }
};