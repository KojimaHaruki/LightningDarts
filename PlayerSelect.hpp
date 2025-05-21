#pragma once
#include "Scene.hpp"
class PlayerSelect : public Scene {
public:
    PlayerSelect(ShareData shareData);
    ~PlayerSelect();
    void reset();
    void draw();
    void update();
    bool chTeamType(int teamType);
private:
    int teamTypeMem;
    int playerChara[MAX_PLAYER_NUM];
    Box teamTypeBox[TeamType::NUM];
};
