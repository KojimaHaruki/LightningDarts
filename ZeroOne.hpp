#pragma once
#include "Scene.hpp"
#include "Game.hpp"

class ZeroOne : public cScene {
public:
	ZeroOne(ShareData shareData);
	~ZeroOne() {}
	void reset();
	void draw();
	void update();
	void fin();
private:
	int arrowImage = 0;
	static constexpr int INIT_POINT[cGame::sMode::ZERO_ONE_NUM] = { 301, 501, 701, 901, 1101, 1501 };
	static constexpr int MAX_ROUND = 100;
	static constexpr int MAX_ATTEMPT = 500;
	int attempt, maxAttempt;
	struct RecordData {
		int team = 0;
		int member = 0;
		int arrow = 3;
		int round = 0;
		int teamRoundScore[cTeam::MAX_SOLO_PLAYER_NUM][MAX_ROUND] = {};
		int teamRemain[cTeam::MAX_SOLO_PLAYER_NUM] = {};
		int rank[cTeam::MAX_SOLO_PLAYER_NUM] = {};
		bool isTeamFin[cTeam::MAX_SOLO_PLAYER_NUM] = {};
		bool isGameFin = false;
		int finRank = 0;
	}; RecordData now, record[MAX_ATTEMPT];
	cBox teamBox[cTeam::MAX_SOLO_PLAYER_NUM];
	int space = 0;
	int nRound = 0, nScore = 0;
};