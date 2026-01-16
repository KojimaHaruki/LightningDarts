#pragma once
#include "Scene.hpp"
class GameSelect : public cScene {
public:
    GameSelect(ShareData shareData);
    ~GameSelect() {}
    void reset();
    void draw();
    void update();
private:
    cBox mainGameBox[MAIN_GAME_NUM], gameBox[GAME_NUM];
};