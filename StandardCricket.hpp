#pragma once
#include "Scene.hpp"

class cStandardCricket : public cScene {
public:
	cStandardCricket(ShareData shareData);
	~cStandardCricket() {}

	// Functions
	void reset();
	void draw();
	void update();
	bool throwDart();
	bool back();
	bool forward();
	bool skip();
	bool record();
	void checkPosFill(int pos);
	bool addScore(int pos);
	bool changeTeam();
	void updateRank();
	void checkTeamFin();
	void checkGameFin();
	void fin();
	
private:
	static constexpr int BULL      = 6;
	static constexpr int POS_NUM   = 7;
	static constexpr int POINT_NUM = 8;
	static constexpr int MAX_POWER = 3;
	static constexpr int MAX_ATTEMPT = 500;
	int attempt = 0, maxAttempt = 0;
	static constexpr int POS_POINT[POS_NUM] = { 20, 19, 18, 17, 16, 15, 25 };
	struct Memory {
		int team = 0;
		int member = 0;
		int arrow = 3;
		int round = 0;
		int teamPosPower[cTeam::MAX_SOLO_PLAYER_NUM][POS_NUM] = {};
		int teamScore[cTeam::MAX_SOLO_PLAYER_NUM] = {};
		bool isTeamPosFill[cTeam::MAX_SOLO_PLAYER_NUM][POS_NUM] = {};
		bool isPosFill[POS_NUM] = {};
		int rank[cTeam::MAX_SOLO_PLAYER_NUM] = {};
		bool isTeamFin[cTeam::MAX_SOLO_PLAYER_NUM] = {};
		bool isGameFin = false;
	}; Memory now, mem[MAX_ATTEMPT];
	struct sPosBox {
		cBox posBox[POS_NUM];
	};
	int nTable = 0;
	std::vector<cBox> teamBox, tableBoxes;
	std::vector<cBox> pointBox;
	int space;
	static constexpr int MARK_PART_NUM = 3;
	static constexpr int MARK_PART_ERROR[MARK_PART_NUM] = { -1, 1, 0 };
	static constexpr int MARK_PART_LINETHICK[MARK_PART_NUM] = { 3, 3, 2 };
	std::vector<sPosBox> teamMarks;
	const std::string rankName[cTeam::MAX_SOLO_PLAYER_NUM] = {
		"1st", "2nd", "3rd", "4th", "5th", "6th", "7th", "8th"
	};
	std::vector<std::string> ranker;
};
