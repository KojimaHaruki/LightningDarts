#pragma once
#include "Scene.hpp"
#include "Player.hpp"

class cGameStart : public cBaseScene {
public:
    cGameStart(ShareData shareData);
    ~cGameStart() {}
    void reset();
    void update();
    void draw();
private:
    time_t nowTime, startTime;
    const time_t timeFromEntryToStart = 3;
    const std::string PLAYER_NAME[cPlayer::MAX_SOLO_PLAYER_NUM] = {
        "1P", "2P", "3P", "4P", "5P", "6P", "7P", "8P"
    };
};
