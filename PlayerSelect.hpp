#pragma once
#include "Scene.hpp"
#include "Team.hpp"

class sPlayerSelect : public cScene {
public:
    sPlayerSelect(ShareData shareData);
    ~sPlayerSelect() {}
    void reset();
    void draw();
    void update();
    void setTeamType(int teamType);
private:
    cBox playerBox[MAX_PLAYER_NUM], teamTypeBox[cTeam::sType::NUM];
    sBoxObj shuffle;
	std::vector<sChara> players, playersMem;
};
