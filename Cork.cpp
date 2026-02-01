#include "Cork.hpp"
#include <numbers>
#include "Darts.hpp"
#include "Game.hpp"
#include "Sound.hpp"
#include "Timer.hpp"
#include "Control.hpp"
#include "Scene.hpp"

cCork::cCork() {
	nTeam = cPlayer::inst()->nTeam();
	cDarts::inst()->reset();
	attempt = 0;
	maxAttempt = 0;
	now = {};
	now.arrow = 3;
	for (int team = 0; team < nTeam; team++) now.rank[team] = team;
	for (int point = 0; point < cDarts::sPoint::NUM; point++)
		cDarts::inst()->setPointValidation(point, true);

	space = 8;
	scoreBoxHeight = LfontSize + space;
	promptBoxHeight = LfontSize + space;

	// set team & table boxes
	teamBoxes.push_back(cBox());
	teamBoxes.front().setS(100, (cPlayer::inst()->teamType() + 1) * 100 + scoreBoxHeight);
	teamBoxes.front().setUL(screen.R() - cPlayer::MAX_DUO_TEAM_NUM * 100, upperFrame.B());
	teamBoxes.front().setColor(teamColor[0]);
	for (int team = 1, nTeam1 = min(nTeam, cPlayer::MAX_DUO_TEAM_NUM); team < nTeam1; team++) {
		cBox leftBox = teamBoxes.back();
		teamBoxes.push_back(leftBox);
		teamBoxes.back().setL(leftBox.R());
		teamBoxes.back().setColor(teamColor[team]);
	}
	tableBox = teamBoxes.front();
	tableBox.setX(screen.CX() + 10, teamBoxes.front().L());
	tableBox.setColor(tableColor);
	if (nTeam > cPlayer::MAX_DUO_TEAM_NUM) {
		teamBoxes.push_back(teamBoxes.front());
		teamBoxes.back().setT(teamBoxes.front().B() + promptBoxHeight);
		teamBoxes.back().setColor(teamColor[cPlayer::MAX_DUO_TEAM_NUM]);
		for (int team = cPlayer::MAX_DUO_TEAM_NUM + 1; team < nTeam; team++) {
			cBox leftBox = teamBoxes.back();
			teamBoxes.push_back(leftBox);
			teamBoxes.back().setL(leftBox.R());
			teamBoxes.back().setColor(teamColor[team]);
		}
		tableBox.setY2(teamBoxes.back().B());
	}

	// set team member image boxes
	for (int team = 0; team < nTeam; team++) {
		cPlayer::inst()->teamMemberImageBox(team, 0).setUL(teamBoxes.at(team).UL());
		if (cPlayer::inst()->nTeamMember(team) == 2) {
			cPlayer::inst()->teamMemberImageBox(team, 1).setUL(
				cPlayer::inst()->teamMemberImageBox(team, 0).LL());
		}
	}
	cControl::inst()->iconBox(cControl::SKIP).setR(screen.CX() + 10);
}

void cCork::reset() {
	cBaseScene::reset();
	cDarts::inst()->reset();
	attempt = 0;
	maxAttempt = 0;
	now = {};
	now.arrow = 1;
	for (int team = 0; team < nTeam; team++) now.rank[team] = team;
}

void cCork::draw() {
	cBaseScene::draw();

	// control icons
	if (attempt < maxAttempt) cControl::inst()->icon(cControl::FORWARD).draw();

	// darts board
	cDarts::inst()->draw();

	// darts positions
	if (now.team < nTeam) {
		DrawCircleAA(real(cDarts::inst()->arrowPos()), imag(cDarts::inst()->arrowPos()),
			5, 1000, teamColor[now.team]);
		DrawCircleAA(real(cDarts::inst()->center()), imag(cDarts::inst()->center()),
			cDarts::inst()->radius(), 1000, teamColor[now.team], FALSE);
	}
	for (int team = 0; team < now.team; team++) {
		DrawCircleAA(real(now.teamArrowPos[team]), imag(now.teamArrowPos[team]), 5, 1000, teamColor[team]);
		DrawCircleAA(real(cDarts::inst()->center()), imag(cDarts::inst()->center()),
			now.teamScore[team], 1000, teamColor[team], FALSE);
	}

	// scene name
	DrawStringToHandle(cControl::inst()->icon(cControl::MUTE).box().R() + 5,
		upperFrame.C().y() - MfontSize / 2,
		(cGame::inst()->modeName() + " < Cork").c_str(), white, Mfont);

	// boxes
	tableBox.draw();
	for (int team = 0; team < nTeam; team++) teamBoxes.at(team).draw();
	DrawBox(screen.CX() + 10, teamBoxes.front().B(),
		screen.R(), teamBoxes.front().B() + promptBoxHeight, pressColor, TRUE);

	cPlayer::sChara chara = {};
	unsigned int color = 0U;
	for (int team = 0; team < nTeam; team++) {
		for (int member = 0; member < cPlayer::inst()->nTeamMember(team); member++) {
			chara = cPlayer::inst()->teamMember(team, member);
			chara.image.draw();
			color = white;
			if (team == now.team && !member) {
				color = touchColor;
				// arrows
				DrawGraph(chara.image.box().R() - 10, chara.image.box().T(), 
					cDarts::inst()->arrowImage(), TRUE);
				// prompt
				DrawStringToHandle(screen.CX() + 120, teamBoxes.front().B() + space / 2,
					(chara.name + ", throw darts!").c_str(), white, Lfont);
			}
			DrawStringToHandle(chara.image.box().L() + 5 * max(0, 10 - (int)chara.name.size()),
				chara.image.box().B() - SfontSize - 6, chara.name.c_str(), color, Sfont);
		}
		DrawStringToHandle(teamBoxes.at(team).L(), teamBoxes.at(team).T(),
			RANK_NAME[now.rank[team]].c_str(), rankColor[now.rank[team]], Mfont);
		DrawFormatStringToHandle(
			chara.image.box().CX() - 20, teamBoxes.at(team).B() - scoreBoxHeight + space / 2,
			white, Lfont, "%6.2f", now.teamScore[team]);
	}
	// finish prompt
	if (now.team >= nTeam) {
		DrawStringToHandle(screen.CX() + 100, teamBoxes.front().B() + space / 2,
			"The order has been decided!", white, Lfont);
	}
}

void cCork::update() {
	cBaseScene::update();
	cDarts::inst()->update();
	if (cDarts::inst()->isThrowed()) throwDart();
	else if (cControl::inst()->isRequested(cControl::SKIP)) skip();
	else if (cControl::inst()->isRequested(cControl::FORWARD)) forward();
	else if (cControl::inst()->isRequested(cControl::BACK)) back();
	else if (now.team >= nTeam && cControl::inst()->isRequested(cControl::YES)) {
		std::vector<cPlayer::sGroup> tmpTeams = cPlayer::inst()->teams();
		cPlayer::inst()->teams().clear();
		for (int rank = 0; rank < nTeam; rank++) {
			for (int team = 0; team < nTeam; team++) {
				if (now.rank[team] == rank) {
					cPlayer::inst()->teams().push_back(tmpTeams.at(team));
				}
			}
		}
		cScene::inst()->setScene(cScene::GAME_START);
	}
}

bool cCork::throwDart() {
	if (now.team >= nTeam) return false;
	now.teamScore[now.team] = cDarts::inst()->radius();
	now.teamArrowPos[now.team] = cDarts::inst()->arrowPos();
	changeTeam(); record(); return true;
}

bool cCork::back() {
	if (attempt <= 0) { cScene::inst()->setScene(cScene::PLAYER_SELECT); return false; }
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
	now.teamArrowPos[now.team] = cDarts::inst()->center() + arrow;
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
	cControl::inst()->iconBox(cControl::SKIP).setR(screen.R());
}
