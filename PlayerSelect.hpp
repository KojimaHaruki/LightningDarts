#pragma once
#include "Scene.hpp"
#include "Player.hpp"

class sPlayerSelect : public cBaseScene {
public:
    sPlayerSelect(ShareData shareData);
    ~sPlayerSelect() {}
    void reset();
    void draw();
    void update();
    void setTeamType(int teamType);
private:
    cBox playerBox[cPlayer::MAX_SOLO_PLAYER_NUM], teamTypeBox[cPlayer::sTeamType::NUM], shuffle;
	std::vector<cPlayer::sChara> players, playersMem;
    const std::string PLAYER_NAME[cPlayer::MAX_SOLO_PLAYER_NUM] = {
        "1P", "2P", "3P", "4P", "5P", "6P", "7P", "8P"
    };
};
