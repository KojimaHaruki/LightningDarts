#pragma once
#include "Scene.hpp"
#include "Game.hpp"

class GameSelect : public cScene {
public:
    GameSelect(ShareData shareData);
    ~GameSelect() {}
    void reset();
    void draw();
    void update();
private:
    cBox gameCategoryBox[cGame::sCategory::NUM], gameModeBox[cGame::sMode::NUM];
};