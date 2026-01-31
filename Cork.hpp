#pragma once
#include "BaseScene.hpp"
#include <complex>

class cCork : public cBaseScene {
public:
    cCork();
    ~cCork();
    void reset();
    void update();
    void draw();
    bool throwDart();
    bool back();
    bool forward();
    bool skip();
	bool record();
	void updateRank();
	bool changeTeam();

private:
    static constexpr int MAX_ATTEMPT = 10;
    int attempt = 0, maxAttempt = 0;
	struct Memory {
		int team = 0;
		int arrow = 1;
		float teamScore[cPlayer::MAX_SOLO_PLAYER_NUM] = {};
		std::complex<float> teamArrowPos[cPlayer::MAX_SOLO_PLAYER_NUM] = {};
		int rank[cPlayer::MAX_SOLO_PLAYER_NUM] = {};
	}; Memory now, mem[MAX_ATTEMPT];

    // rank
    const std::string RANK_NAME[cPlayer::MAX_SOLO_PLAYER_NUM] = {
        "1st", "2nd", "3rd", "4th", "5th", "6th", "7th", "8th"
    };

    int space = 0, scoreBoxHeight = 0, promptBoxHeight = 0;
    std::vector<cBox> teamBoxes;
    cBox tableBox;
};
