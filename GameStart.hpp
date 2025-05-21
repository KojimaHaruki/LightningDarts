#pragma once
#include "Scene.hpp"
class GameStart : public Scene {
public:
    GameStart(ShareData shareData);
    ~GameStart();
    void reset();
    void update();
    void draw();
private:
    time_t nowTime, startTime;
    const time_t timeFromEntryToStart = 3;
};
