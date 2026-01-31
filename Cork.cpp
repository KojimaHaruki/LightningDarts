#include "Cork.hpp"
#include <numbers>
#include "Darts.hpp"
#include "Game.hpp"
#include "Sound.hpp"
#include "Timer.hpp"
#include "Control.hpp"
#include "Scene.hpp"

cCork::cCork() {
	nTeam = cPlayer::instance()->nTeam();
	cDarts::instance()->reset();
	attempt = 0;
	maxAttempt = 0;
	now = {};
	now.arrow = 3;
	for (int team = 0; team < nTeam; team++) now.rank[team] = team;
	for (int point = 0; point < cDarts::sPoint::NUM; point++)
		cDarts::instance()->setPointValidation(point, true);

	space = 8;
	scoreBoxHeight = LfontSize + space;
	promptBoxHeight = LfontSize + space;

	// set team & table boxes
	teamBoxes.push_back(cBox());
	teamBoxes.front().setSize(100, (cPlayer::instance()->teamType() + 1) * 100 + scoreBoxHeight);
	teamBoxes.front().setUpperLeft(screen.right() - cPlayer::MAX_DUO_TEAM_NUM * 100, upperFrame.bottom());
	teamBoxes.front().setColor(teamColor[0]);
	for (int team = 1, nTeam1 = min(nTeam, cPlayer::MAX_DUO_TEAM_NUM); team < nTeam1; team++) {
		cBox leftBox = teamBoxes.back();
		teamBoxes.push_back(leftBox);
		teamBoxes.back().setLeft(leftBox.right());
		teamBoxes.back().setColor(teamColor[team]);
	}
	tableBox = teamBoxes.front();
	tableBox.setX(screen.centerX() + 10, teamBoxes.front().left());
	tableBox.setColor(tableColor);
	if (nTeam > cPlayer::MAX_DUO_TEAM_NUM) {
		teamBoxes.push_back(teamBoxes.front());
		teamBoxes.back().setTop(teamBoxes.front().bottom() + promptBoxHeight);
		teamBoxes.back().setColor(teamColor[cPlayer::MAX_DUO_TEAM_NUM]);
		for (int team = cPlayer::MAX_DUO_TEAM_NUM + 1; team < nTeam; team++) {
			cBox leftBox = teamBoxes.back();
			teamBoxes.push_back(leftBox);
			teamBoxes.back().setLeft(leftBox.right());
			teamBoxes.back().setColor(teamColor[team]);
		}
		tableBox.setY2(teamBoxes.back().bottom());
	}

	// set team member image boxes
	for (int team = 0; team < nTeam; team++) {
		cPlayer::instance()->teamMemberImageBox(team, 0).setUpperLeft(teamBoxes.at(team).upperLeft());
		if (cPlayer::instance()->nTeamMember(team) == 2) {
			cPlayer::instance()->teamMemberImageBox(team, 1).setUpperLeft(
				cPlayer::instance()->teamMemberImageBox(team, 0).lowerLeft());
		}
	}
	cControl::instance()->iconBox(cControl::SKIP).setRight(screen.centerX() + 10);
}

void cCork::reset() {
	cBaseScene::reset();
	cDarts::instance()->reset();
	attempt = 0;
	maxAttempt = 0;
	now = {};
	now.arrow = 1;
	for (int team = 0; team < nTeam; team++) now.rank[team] = team;
}

void cCork::draw() {
	cBaseScene::draw();

	// control icons
	if (attempt < maxAttempt) cControl::instance()->icon(cControl::FORWARD).draw();

	// darts board
	cDarts::instance()->draw();

	// darts positions
	if (now.team < nTeam) {
		DrawCircleAA(real(cDarts::instance()->arrowPos()), imag(cDarts::instance()->arrowPos()),
			5, 1000, teamColor[now.team]);
		DrawCircleAA(real(cDarts::instance()->center()), imag(cDarts::instance()->center()),
			cDarts::instance()->radius(), 1000, teamColor[now.team], FALSE);
	}
	for (int team = 0; team < now.team; team++) {
		DrawCircleAA(real(now.teamArrowPos[team]), imag(now.teamArrowPos[team]), 5, 1000, teamColor[team]);
		DrawCircleAA(real(cDarts::instance()->center()), imag(cDarts::instance()->center()),
			now.teamScore[team], 1000, teamColor[team], FALSE);
	}

	// scene name
	DrawStringToHandle(cControl::instance()->icon(cControl::MUTE).box().right() + 5,
		upperFrame.center().y() - MfontSize / 2,
		(cGame::instance()->modeName() + " < Cork").c_str(), white, Mfont);

	// boxes
	tableBox.draw();
	for (int team = 0; team < nTeam; team++) teamBoxes.at(team).draw();
	DrawBox(screen.centerX() + 10, teamBoxes.front().bottom(),
		screen.right(), teamBoxes.front().bottom() + promptBoxHeight, pressColor, TRUE);

	cPlayer::sChara chara = {};
	unsigned int color = 0U;
	for (int team = 0; team < nTeam; team++) {
		for (int member = 0; member < cPlayer::instance()->nTeamMember(team); member++) {
			chara = cPlayer::instance()->teamMember(team, member);
			chara.image.draw();
			color = white;
			if (team == now.team && !member) {
				color = touchColor;
				// arrows
				DrawGraph(chara.image.box().right() - 10, chara.image.box().top(), 
					cDarts::instance()->arrowImage(), TRUE);
				// prompt
				DrawStringToHandle(screen.centerX() + 120, teamBoxes.front().bottom() + space / 2,
					(chara.name + ", throw darts!").c_str(), white, Lfont);
			}
			DrawStringToHandle(chara.image.box().left() + 5 * max(0, 10 - (int)chara.name.size()),
				chara.image.box().bottom() - SfontSize - 6, chara.name.c_str(), color, Sfont);
		}
		DrawStringToHandle(teamBoxes.at(team).left(), teamBoxes.at(team).top(),
			RANK_NAME[now.rank[team]].c_str(), rankColor[now.rank[team]], Mfont);
		DrawFormatStringToHandle(
			chara.image.box().centerX() - 20, teamBoxes.at(team).bottom() - scoreBoxHeight + space / 2,
			white, Lfont, "%6.2f", now.teamScore[team]);
	}
	// finish prompt
	if (now.team >= nTeam) {
		DrawStringToHandle(screen.centerX() + 100, teamBoxes.front().bottom() + space / 2,
			"The order has been decided!", white, Lfont);
	}
}

void cCork::update() {
	cBaseScene::update();
	cDarts::instance()->update();
	if (cDarts::instance()->isThrowed()) throwDart();
	else if (cControl::instance()->isRequested(cControl::SKIP)) skip();
	else if (cControl::instance()->isRequested(cControl::FORWARD)) forward();
	else if (cControl::instance()->isRequested(cControl::BACK)) back();
	else if (now.team >= nTeam && cControl::instance()->isRequested(cControl::YES)) {
		std::vector<cPlayer::sGroup> tmpTeams = cPlayer::instance()->teams();
		cPlayer::instance()->teams().clear();
		for (int rank = 0; rank < nTeam; rank++) {
			for (int team = 0; team < nTeam; team++) {
				if (now.rank[team] == rank) {
					cPlayer::instance()->teams().push_back(tmpTeams.at(team));
				}
			}
		}
		cScene::instance()->setScene(cScene::GAME_START);
	}
}

bool cCork::throwDart() {
	if (now.team >= nTeam) return false;
	now.teamScore[now.team] = cDarts::instance()->radius();
	now.teamArrowPos[now.team] = cDarts::instance()->arrowPos();
	changeTeam(); record(); return true;
}

bool cCork::back() {
	if (attempt <= 0) { cScene::instance()->setScene(cScene::PLAYER_SELECT); return false; }
	attempt--; now = mem[attempt]; return true;
}

bool cCork::forward() {
	if (attempt >= maxAttempt) return false;
	attempt++; now = mem[attempt]; return true;
}

bool cCork::skip() {
	if (now.team >= nTeam) return false;
	float radius = ((float)rand() / RAND_MAX) * cDarts::RADIAL_POS_RADIUS[cDarts::sRadialPos::OUTSIDE],
		theta = rand() % 360 * std::numbers::pi / 180.0f;
	now.teamScore[now.team] = radius;
	std::complex<float> arrow(radius * cos(theta), -radius * sin(theta));
	now.teamArrowPos[now.team] = cDarts::instance()->center() + arrow;
	changeTeam();
	record(); return true;
}

bool cCork::record() {
	if (attempt >= MAX_ATTEMPT - 2) return false;
	attempt++; maxAttempt = attempt; mem[attempt] = now; return true;
}

void cCork::updateRank() {
	for (int team = 0; team < nTeam; team++) {
		now.rank[team] = 0;
		for (int opponent = 0; opponent < nTeam; opponent++) {
			if (opponent == team) {
				continue;
			}
			if (now.teamScore[team] > now.teamScore[opponent] ||
				(now.teamScore[team] == now.teamScore[opponent] && team > opponent)) {
				now.rank[team]++; // If opponent has lower score or is finished, increase rank
			}
		}
	}
}

bool cCork::changeTeam() {
	updateRank();
	now.team++;
	if (now.team >= nTeam) return false;
	now.arrow = 1; // reset arrows
	return true;
}

cCork::~cCork() {
	cControl::instance()->iconBox(cControl::SKIP).setRight(screen.right());
}
