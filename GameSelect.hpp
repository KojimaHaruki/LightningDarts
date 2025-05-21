#pragma once
#include "Scene.hpp"
class GameSelect : public Scene {
public:
    GameSelect(ShareData shareData);
    ~GameSelect();
    void reset();
    void draw();
    void update();
private:
    Box mainGameBox[MAIN_GAME_NUM], gameBox[GAME_NUM];
};