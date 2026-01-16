#pragma once
#include "Scene.hpp"
class PlayerSelect : public cScene {
public:
    PlayerSelect(ShareData shareData);
    ~PlayerSelect() {}
    void reset();
    void draw();
    void update();
    void setTeamType(int teamType);
private:
    cBox playerBox[MAX_PLAYER_NUM], teamTypeBox[TEAM_TYPE_NUM];
    sBoxObj shuffle;
	std::vector<sChara> players, playersMem;
};
