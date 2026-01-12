#pragma once
#include "Scene.hpp"
class PlayerSelect : public Scene {
public:
    PlayerSelect(ShareData shareData);
    ~PlayerSelect();
    void reset();
    void draw();
    void update();
    void chTeamType(int teamType);
private:
    Box playerBox[MAX_PLAYER_NUM], teamTypeBox[TEAM_TYPE_NUM];
	std::vector<Chara> players;
    int nSelectedPlayer = 0;
};
