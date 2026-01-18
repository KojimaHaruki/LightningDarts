#pragma once
#include "Scene.hpp"

class CountUp : public cScene {
public:
	CountUp(ShareData shareData);
	~CountUp() {}
	void reset();
	void draw();
	void update();
	void fin();
private:
	static constexpr int MAX_ATTEMPT = 500;
	static constexpr int ROUND_NUM = 8;
	static constexpr int SCORE_NUM = 9;
	static constexpr int CRICKET_NUMBER_NUM = 7;
	int attempt, maxAttempt;
	struct RecordData {
		int team = 0;
		int member = 0;
		int arrow = 3;
		int round = 0;
		int teamRoundScore[cTeam::MAX_SOLO_PLAYER_NUM][ROUND_NUM] = {};
		int teamScore[cTeam::MAX_SOLO_PLAYER_NUM] = {};
		int rank[cTeam::MAX_SOLO_PLAYER_NUM] = {};
		bool isGameFin = false;
	}; RecordData now, record[MAX_ATTEMPT];
	cBox teamBox[cTeam::MAX_SOLO_PLAYER_NUM];
	int space = 0;
	static constexpr int CRICKET_NUMBER_SCORE[CRICKET_NUMBER_NUM] = { 20, 19, 18, 17, 16, 15, 25 };
	const std::string rankName[cTeam::MAX_SOLO_PLAYER_NUM] = {
		"1st", "2nd", "3rd", "4th", "5th", "6th", "7th", "8th"
	};
};