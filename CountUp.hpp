#pragma once
#include "Scene.hpp"
class CountUp : public Scene {
public:
	CountUp(ShareData shareData);
	~CountUp();
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
		int teamRoundScore[MAX_PLAYER_NUM][ROUND_NUM] = {};
		int teamScore[MAX_PLAYER_NUM] = {};
		int rank[MAX_PLAYER_NUM] = {};
		bool isGameFin = false;
	}; RecordData now, record[MAX_ATTEMPT];
	Box teamBox[MAX_PLAYER_NUM];
	int space;
	static constexpr int pointArray[21] = { 11, 8, 16, 7, 19, 3, 17, 2, 15, 10, 6, 13, 4, 18, 1, 20, 5, 12, 9, 14, 11 };
	static constexpr int CRICKET_NUMBER_SCORE[CRICKET_NUMBER_NUM] = { 20, 19, 18, 17, 16, 15, 0 };
};