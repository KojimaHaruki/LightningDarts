#pragma once
#include "Scene.hpp"
class ZeroOne : public Scene {
public:
	ZeroOne(ShareData shareData);
	~ZeroOne();
	void reset();
	void draw();
	void update();
	void fin();
private:
	static constexpr int MAX_TURN = 100;
	static constexpr int MAX_ATTEMPT = 500;
	int attempt, maxAttempt;
	struct RecordData {
		int team = 0;
		int arrow = 3;
		int turn = 0;
		int teamPoint[MAX_PLAYER_NUM][MAX_TURN] = {};
		int teamBill[MAX_PLAYER_NUM] = {};
		int rank[MAX_PLAYER_NUM] = {};
		bool isPlayerFin[MAX_PLAYER_NUM] = {};
		bool isGameFin = false;
		int finRank = 0;
	}; RecordData now, record[MAX_ATTEMPT];
	Box teamBox[MAX_PLAYER_NUM];
	int space;
	static constexpr int pointArray[21] = { 11, 8, 16, 7, 19, 3, 17, 2, 15, 10, 6, 13, 4, 18, 1, 20, 5, 12, 9, 14, 11 };
	static constexpr int initPoint[6] = { 301, 501, 701, 901, 1101, 1501 };
	int pointRecordNum;
	int pointNum;
};