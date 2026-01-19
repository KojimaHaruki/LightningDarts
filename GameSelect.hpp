#pragma once
#include "BaseScene.hpp"
#include "Game.hpp"

class cGameSelect : public cBaseScene {
public:
    cGameSelect();
    ~cGameSelect() {}
    void reset();
    void draw();
    void update();
private:
    cBox gameCategoryBox[cGame::sCategory::NUM], gameModeBox[cGame::sMode::NUM];
};