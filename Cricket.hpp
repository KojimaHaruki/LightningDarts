#pragma once
#include "Scene.hpp"
class Cricket : public Scene {
public:
	Cricket(ShareData shareData);
	~Cricket();
	void init(int nowScene, ShareData shareData);
	void reset();
	void draw();
	void update();
	void fin();
private:
	static constexpr int BULL      = 6;
	static constexpr int POS_NUM   = 7;
	static constexpr int POINT_NUM = 8;
	int selectPos;
	static constexpr int MAX_ATTEMPT = 500;
	int attempt, maxAttempt;
	static constexpr int POS_POINT[POS_NUM] = { 20, 19, 18, 17, 16, 15, 25 };
	struct RecordData {
		int team = 0;
		int arrow = 3;
		int turn = 0;
		int posPoint[POS_NUM] = { 20, 19, 18, 17, 16, 15, 25 };
		int teamPoint[MAX_PLAYER_NUM][POS_NUM] = {};
		int teamBill[MAX_PLAYER_NUM] = {};
		int rank[MAX_PLAYER_NUM];
		bool isTeamFin[MAX_PLAYER_NUM] = {};
		bool isGameFin = false;
		int finRank = 0;
	}; RecordData now, record[MAX_ATTEMPT];
	Box teamBox[MAX_PLAYER_NUM];
	int space;
	struct MarkPart {
		Box box;
		int color;
		int lineWidth;
	}; MarkPart markPart[3];
};
