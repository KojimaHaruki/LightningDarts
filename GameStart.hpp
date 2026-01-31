#pragma once
#include "BaseScene.hpp"

class cGameStart : public cBaseScene {
public:
    cGameStart();
    ~cGameStart() {}
    void reset();
    void update();
    void draw();
private:
    time_t nowTime, startTime;
    const time_t TIME_ENTRYtoSTART = 3;
    // rank
    const std::string RANK_NAME[cPlayer::MAX_SOLO_PLAYER_NUM] = {
        "1st", "2nd", "3rd", "4th", "5th", "6th", "7th", "8th"
    };
};
