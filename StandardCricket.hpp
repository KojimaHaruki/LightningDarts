#pragma once
#include "BaseScene.hpp"

class cStandardCricket : public cBaseScene {
public:
	cStandardCricket();
	~cStandardCricket();

	// Functions
	void reset();
	void draw();
	void update();
	bool throwDart();
	bool back();
	bool forward();
	bool skip();
	void updateScoreBox();
	bool record();
	bool addScore(int pos);
	void checkPosFill(int pos);
	bool changeTeam();
	void updateRank();
	void checkTeamFin(int team);
	void checkGameFin();
	
private:
	static constexpr int BULL      = 6;
	static constexpr int POS_NUM   = 7;
	static constexpr int POINT_NUM = POS_NUM + 1;
	static constexpr int MAX_POWER = 3;
	static constexpr int MAX_ATTEMPT = 500;
	int attempt = 0, maxAttempt = 0;
	static constexpr int POS_POINT[POS_NUM] = { 20, 19, 18, 17, 16, 15, 25 };
	const std::string POS_NAME[POS_NUM] = { "20", "19", "18", "17", "16", "15", "Bull" };
	struct Memory {
		int team = 0;
		int member = 0;
		int arrow = 3;
		int round = 0;
		int teamPosPower[cPlayer::MAX_SOLO_PLAYER_NUM][POS_NUM] = {};
		int teamScore[cPlayer::MAX_SOLO_PLAYER_NUM] = {};
		bool isTeamPosFill[cPlayer::MAX_SOLO_PLAYER_NUM][POS_NUM] = {};
		bool isPosFill[POS_NUM] = {};
		int rank[cPlayer::MAX_SOLO_PLAYER_NUM] = {};
		bool isTeamFin[cPlayer::MAX_SOLO_PLAYER_NUM] = {};
		bool isGameFin = false;
	}; Memory now, mem[MAX_ATTEMPT];
	struct sPosBox {
		cBox posBox[POS_NUM];
	};
	int space = 0, posBoxHeight = 0, promptBoxHeight = 0;
	std::vector<cBox> teamBoxes, tableBoxes;
	int nTable = 0;
	std::vector<sPosBox> filledBoxes;
	
	// team mark
	static constexpr int MARK_PART_NUM = 3;
	static constexpr int MARK_PART_ERROR[MARK_PART_NUM] = { -1, 1, 0 };
	static constexpr int MARK_PART_LINETHICK[MARK_PART_NUM] = { 3, 3, 2 };
	std::vector<sPosBox> teamMarkBoxes;

	// rank
	const std::string RANK_NAME[cPlayer::MAX_SOLO_PLAYER_NUM] = {
		"1st", "2nd", "3rd", "4th", "5th", "6th", "7th", "8th"
	};
	std::vector<cPlayer::sGroup> ranker;
};
