#pragma once
#include "Scene.hpp"
class cGameStart : public cScene {
public:
    cGameStart(ShareData shareData);
    ~cGameStart() {}
    void reset();
    void update();
    void draw();
private:
    time_t nowTime, startTime;
    const time_t timeFromEntryToStart = 3;
};
