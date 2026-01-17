#include "StandardCricket.hpp"
#include <string>
#include <numbers>
#include <algorithm>
#include <random>
#include "Mouse.hpp"
#include "Darts.hpp"
#include "Color.hpp"
#include "Sound.hpp"
#include "Game.hpp"
#include "Team.hpp"

cStandardCricket::cStandardCricket(ShareData shareData) : attempt(0), maxAttempt(0) {
	sd = shareData;
	mNowScene = STANDARD_CRICKET;
	nTeam = sd.teams.size();
	reset();
	if (nTeam <= MAX_TEAM_NUM) {
		space = 6;
		for (int team = 0, x = sd.screen.right() - 400, y = sd.obj.upperFrame.bottom();
			team < nTeam; team++, x += 100) {
			teamBox.push_back(cBox());
			teamBox.at(team).setSize(
				100, (cTeam::instance()->type() + 1) * 100 + POINT_NUM * (MfontSize + space));
			teamBox.at(team).setUpperLeft(x, y);
			sd.teams.at(team).members.at(0).image.box.setUpperLeft(teamBox.at(team).upperLeft());
			if (sd.teams.at(team).members.size() == 2) {
				sd.teams.at(team).members.at(1).image.box.setUpperLeft(
					sd.teams.at(team).members.at(0).image.box.lowerLeft());
			}
		}
		pointBox.push_back(cBox());
		pointBox.at(0).setSize(100 * nTeam, MfontSize + space);
		pointBox.at(0).setUpperLeft(
			sd.screen.right() - 400, teamBox.at(0).top() + (cTeam::instance()->type() + 1) * 100);
		for (int pos = 1; pos < POINT_NUM; pos++) {
			pointBox.push_back(cBox());
			pointBox.at(pos).setSize(100 * nTeam, MfontSize + space);
			pointBox.at(pos).setUpperLeft(pointBox.at(pos - 1).lowerLeft());
		}
		for (int team = 0; team < nTeam; team++) {
			teamMarks.push_back(sPosBox());
			for (int pos = 0; pos < POS_NUM; pos++) {
				teamMarks.at(team).posBox[pos].setSize(MfontSize, MfontSize);
				teamMarks.at(team).posBox[pos].setCenter(
					teamBox.at(team).center().x(), pointBox.at(pos).center().y());
			}
		}
	}
	else {
		space = 2;
		for (int team = 0; team < nTeam; team++) {
			sd.teams.at(team).members.at(0).image.box.setSize(100, 70);
			teamBox.push_back(cBox());
			teamBox.at(team).setSize(100, 70 + POINT_NUM * (SfontSize + space));
		}
		for (int team = 0; team < MAX_TEAM_NUM; team++) {
			teamBox.at(team).setUpperLeft(
				sd.screen.right() + 100 * (team - MAX_TEAM_NUM), sd.obj.upperFrame.bottom());
		}
		for (int team = MAX_TEAM_NUM; team < nTeam; team++) {
			teamBox.at(team).setUpperLeft(
				teamBox.at(team - MAX_TEAM_NUM).left(),
				teamBox.at(team - MAX_TEAM_NUM).bottom() + MfontSize + space);
		}
		for (int team = 0; team < nTeam; team++) {
			sd.teams.at(team).members.at(0).image.box.setUpperLeft(teamBox.at(team).upperLeft());
		}
		pointBox.push_back(cBox());
		pointBox.at(0).setSize(100 * MAX_TEAM_NUM, SfontSize + space);
		pointBox.at(0).setUpperLeft(sd.screen.right() - 400, teamBox.at(0).top() + 70);
		for (int pos = 1; pos < POINT_NUM; pos++) {
			pointBox.push_back(cBox());
			pointBox.at(pos).setSize(100 * MAX_TEAM_NUM, SfontSize + space);
			pointBox.at(pos).setUpperLeft(pointBox.at(pos - 1).lowerLeft());
		}
		pointBox.push_back(cBox());
		pointBox.at(POINT_NUM).setSize(100 * (nTeam - MAX_TEAM_NUM), SfontSize + space);
		pointBox.at(POINT_NUM).setUpperLeft(sd.screen.right() - 400, teamBox.at(MAX_TEAM_NUM).top() + 70);
		for (int pos = POINT_NUM + 1; pos < 2 * POINT_NUM; pos++) {
			pointBox.push_back(cBox());
			pointBox.at(pos).setSize(100 * (nTeam - MAX_TEAM_NUM), SfontSize + space);
			pointBox.at(pos).setUpperLeft(pointBox.at(pos - 1).lowerLeft());
		}
		for (int team = 0; team < nTeam; team++) {
			teamMarks.push_back(sPosBox());
			for (int pos = 0; pos < POS_NUM; pos++) {
				teamMarks.at(team).posBox[pos].setSize(SfontSize, SfontSize);
				teamMarks.at(team).posBox[pos].setCenter(
					teamBox.at(team).center().x(),
					pointBox.at(pos + (team / MAX_TEAM_NUM) * POINT_NUM).center().y());
			}
		}
	}
	mem[attempt] = now;
}

void cStandardCricket::reset() {
	cScene::reset();
	cDarts::instance()->timer().restart();
	attempt = 0;
	maxAttempt = 0;
	now = {};
	now.arrow = 3;
	for (int team = 0; team < nTeam; team++) now.rank[team] = team;
	for (int point = cDarts::sPoint::MISS; point < cDarts::sPoint::MIN_CRICKET_POINT; point++)
		cDarts::instance()->setPointValidation(point, false);
	for (int point = cDarts::sPoint::MIN_CRICKET_POINT; point < cDarts::sPoint::NUM; point++)
		cDarts::instance()->setPointValidation(point, true);
}

void cStandardCricket::draw() {
	cScene::draw();
	drawImage(sd.ctrl.pause[cDarts::instance()->timer().isPaused()].icon);
	drawImage(sd.ctrl.skill.icon);
	cDarts::instance()->timer().drawLapseTime(
		sd.screen.left(), sd.obj.upperFrame.bottom() + 10, white, Sfont, Timer::Mode::HMSmS);
	DrawStringToHandle(
		sd.ctrl.mute[0].icon.box.right() + 5, sd.obj.upperFrame.center().y() - MfontSize / 2,
		(cGame::instance()->modeName() + " / " + cTeam::instance()->typeName()).c_str(),
		white, Mfont);
	cDarts::instance()->draw();
	drawImage(sd.ctrl.playerSelect.icon);
	drawImage(sd.ctrl.gameSelect.icon);
	drawImage(sd.ctrl.skip.icon);
	drawImage(sd.ctrl.home.icon);
	drawImage(sd.ctrl.back.icon);
	drawImage(sd.ctrl.mute[cSound::instance()->isBGMPlayed()].icon);
	drawImage(sd.ctrl.config.icon);
	drawImage(sd.ctrl.window[sd.window].icon);
	drawImage(sd.ctrl.quit.icon);
	drawImage(sd.ctrl.init.icon);
	drawImage(sd.ctrl.reset.icon);
	drawImage(sd.ctrl.bgm.icon);
	DrawFormatStringToHandle(sd.screen.center().x() - 80, sd.obj.upperFrame.bottom() + 10,
		white, Mfont, "Turn%3d", now.round + 1);
	sChara chara;
	if (nTeam <= MAX_TEAM_NUM) {
		DrawBox(sd.screen.center().x() + 10, teamBox.at(0).top(),
			teamBox.at(nTeam - 1).right(), teamBox.at(0).bottom(), tableColor, TRUE);
		drawBoxObj(teamBox.at(now.team), white);
		for (int pos = 0; pos < POS_NUM; pos++) {
			if (now.isPosFill[pos]) {
				drawBoxObj(pointBox.at(pos), gray);
			}
		}
		for (int team = 0; team < nTeam; team++) {
			if (now.isTeamFin[team]) {
				drawBoxObj(teamBox.at(team), gray);
			}
			for (int member = 0; member < sd.teams.at(team).members.size(); member++) {
				chara = sd.teams.at(team).members[member];
				drawImage(chara.image);
				unsigned int color = white;
				if (team == now.team && member == now.member) {
					color = touchColor;
				}
				DrawStringToHandle(chara.image.box.left() + 5 * max(0, 10 - chara.name.size()),
					chara.image.box.bottom() - SfontSize - 6, chara.name.c_str(), color, Sfont);
			}
			DrawStringToHandle(teamBox.at(team).left(), teamBox.at(team).top(),
				rankName[now.rank[team]].c_str(), rankColor[now.rank[team]], Mfont);
			DrawFormatStringToHandle(
				chara.image.box.center().x() - 20, pointBox.at(POS_NUM).top() + space / 2,
				white, Mfont, "%4d", now.teamBill[team]);
			DrawLine(teamBox.at(team).left(), teamBox.at(team).top(),
				teamBox.at(team).left(), teamBox.at(team).bottom(), black);
		}
		int y = sd.teams.at(0).members.at(cTeam::instance()->type()).image.box.bottom();
		for (int pos = 0; pos < POS_NUM - 1; pos++) {
			DrawStringToHandle(sd.screen.center().x() + 20, pointBox.at(pos).top() + space / 2,
				std::to_string(POS_POINT[pos]).c_str(), white, Mfont);
		}
		DrawStringToHandle(sd.screen.center().x() + 10, pointBox.at(POS_NUM - 1).top() + space / 2,
			"Bull", white, Mfont);
		for (int point = 0; point < POINT_NUM; point++) {
			DrawLine(sd.screen.center().x() + 10, pointBox.at(point).top(),
				teamBox.at(nTeam - 1).right(), pointBox.at(point).top(), black);
		}
		DrawStringToHandle(
			sd.screen.center().x() + 10, pointBox.at(POS_NUM).center().y() - SfontSize / 2,
			"Score", white, Sfont);
	}
	else {
		DrawBox(sd.screen.center().x() + 10, teamBox.at(0).top(),
			teamBox.at(MAX_TEAM_NUM - 1).right(), teamBox.at(0).bottom(), tableColor, TRUE);
		DrawBox(sd.screen.center().x() + 10, teamBox.at(MAX_TEAM_NUM).top(),
			teamBox.at(nTeam - 1).right(), teamBox.at(MAX_TEAM_NUM).bottom(), tableColor, TRUE);
		DrawBox(teamBox.at(now.team).left(), teamBox.at(now.team).top(),
			teamBox.at(now.team).right(), teamBox.at(now.team).bottom(), white, TRUE);
		for (int pos = 0; pos < POS_NUM; pos++) {
			if (now.isPosFill[pos]) {
				drawBoxObj(pointBox.at(pos), gray);
				drawBoxObj(pointBox.at(pos + POINT_NUM), gray);
			}
		}
		for (int team = 0; team < nTeam; team++) {
			if (now.isTeamFin[team]) {
				drawBoxObj(teamBox.at(team), gray);
			}
			chara = sd.teams.at(team).members.at(0);
			DrawRotaGraph(chara.image.box.center().x(), chara.image.box.center().y(),
				0.7, 0.0, chara.image.handle, TRUE);
			DrawStringToHandle(chara.image.box.left(), chara.image.box.top(),
				rankName[now.rank[team]].c_str(), white, Sfont);
			DrawStringToHandle(chara.image.box.left() + 5 * max(0, 10 - chara.name.size()),
				chara.image.box.bottom() - SfontSize - 6,
				chara.name.c_str(), white, Sfont);
			DrawFormatStringToHandle(chara.image.box.center().x() - 20,
				chara.image.box.bottom() + POS_NUM * SfontSize + 15 * space / 2,
				white, Sfont, "%4d", now.teamBill[team]);
			DrawLine(teamBox.at(team).left(), teamBox.at(team).top(),
				teamBox.at(team).left(), teamBox.at(team).bottom(), black);
		}
		for (int pointPos = 0, y = 0; pointPos < POINT_NUM; pointPos++) {
			y = sd.teams.at(0).members.at(0).image.box.bottom() + pointPos * (SfontSize + space);
			DrawLine(sd.screen.center().x() + 10, y, teamBox.at(MAX_TEAM_NUM - 1).right(), y, black);
			y = sd.teams.at(MAX_TEAM_NUM).members.at(0).image.box.bottom() +
				pointPos * (SfontSize + space);
			DrawLine(sd.screen.center().x() + 10, y, teamBox.at(nTeam - 1).right(), y, black);
		}
		for (int i = 0; i < 2; i++) {
			chara = sd.teams.at(MAX_TEAM_NUM * i).members.at(0);
			for (int pos = 0; pos < POS_NUM - 1; pos++) {
				DrawStringToHandle(sd.screen.center().x() + 20,
					chara.image.box.bottom() + pos * SfontSize + (2 * pos + 1) * space / 2,
					std::to_string(POS_POINT[pos]).c_str(), white, Sfont);
			}
			DrawStringToHandle(sd.screen.center().x() + 14,
				chara.image.box.bottom() + (POS_NUM - 1) * SfontSize + (2 * (POS_NUM - 1) + 1) * space / 2,
				"Bull", white, Sfont);
			DrawStringToHandle(sd.screen.center().x() + 10,
				chara.image.box.bottom() + POS_NUM * SfontSize + 15 * space / 2,
				"Score", white, Sfont);
		}
	}
	unsigned int colors[MARK_PART_NUM] = { white, black, 0U };
	for (int team = 0; team < nTeam; team++) {
		colors[2] = teamColor[team];
		for (int pos = 0; pos < POS_NUM; pos++) {
			for (int part = 0; part < MARK_PART_NUM; part++) {
				if (now.teamPosPower[team][pos] >= MAX_POWER) {
					DrawCircleAA(teamMarks.at(team).posBox[pos].center().x() + MARK_PART_ERROR[part],
						teamMarks.at(team).posBox[pos].center().y(),
						(teamMarks.at(team).posBox[pos].height() - space) / 2.0,
						100, colors[part], FALSE, MARK_PART_LINETHICK[part]);
				}
				if (now.teamPosPower[team][pos] >= 2) {
					DrawLine(
						teamMarks.at(team).posBox[pos].right() + MARK_PART_ERROR[part],
						teamMarks.at(team).posBox[pos].top(),
						teamMarks.at(team).posBox[pos].left() + MARK_PART_ERROR[part],
						teamMarks.at(team).posBox[pos].bottom(), colors[part], MARK_PART_LINETHICK[part]);
				}
				if (now.teamPosPower[team][pos] >= 2) {
					DrawLine(
						teamMarks.at(team).posBox[pos].left() + MARK_PART_ERROR[part],
						teamMarks.at(team).posBox[pos].top(),
						teamMarks.at(team).posBox[pos].right() + MARK_PART_ERROR[part],
						teamMarks.at(team).posBox[pos].bottom(), colors[part], MARK_PART_LINETHICK[part]);
				}
			}
		}
	}
	DrawBox(sd.screen.center().x() + 10, teamBox.at(0).bottom(),
		sd.screen.right(), teamBox.at(0).bottom() + space + MfontSize, pressColor, TRUE);
	chara = sd.teams.at(now.team).members.at(now.member);
	for (int arrow = 0, x = teamBox.at(now.team).right() - 10, y = teamBox.at(now.team).top();
		arrow < now.arrow; arrow++, x -= 10)
		DrawGraph(x, y, cDarts::instance()->arrowImage(), TRUE);
	DrawStringToHandle(sd.screen.center().x() + 120, teamBox.at(0).bottom() + space / 2,
		(chara.name + ", throw darts!").c_str(), white, Mfont);
}

void cStandardCricket::update() {
	cScene::update();
	cDarts::instance()->update();
	bool isPaused = cDarts::instance()->timer().isPaused();
	int point = cDarts::instance()->point(), power = cDarts::instance()->power();
	if (attempt < maxAttempt) {
		drawImage(sd.ctrl.forward.icon);
		if (ctrlRQ(sd.ctrl.forward)) {
			attempt++;
			now = mem[attempt];
			for (int pos = 0; pos < POS_NUM; pos++) {
				cDarts::instance()->setPointValidation(POS_POINT[pos], !now.isPosFill[pos]);
			}
			return;
		}
	}
	if (!now.isGameFin) {
		if (ctrlRQ(sd.ctrl.skip)) {
			changeTeam();
			record();
		}
		else if (cDarts::instance()->isThrowed()) {
			now.arrow--;
			int throwedPos = -1;
			for (int pos = 0; pos < POS_NUM; pos++) {
				if (point == POS_POINT[pos]) {
					if (!now.isPosFill[pos]) {
						throwedPos = pos;
					}
					break;
				}
			}
			if (throwedPos >= 0) {
				now.teamPosPower[now.team][throwedPos] += power;
				if (now.teamPosPower[now.team][throwedPos] >= MAX_POWER) {
					now.isTeamPosFilled[now.team][throwedPos] = true;
					checkPosFill(throwedPos);
					addDamage(throwedPos,
						POS_POINT[throwedPos] * (now.teamPosPower[now.team][throwedPos] - MAX_POWER));
				}
				updateRank();
				checkTeamFin();
				// If team has finished, set arrows to 0
				if (now.isTeamFin[now.team]) { now.arrow = 0; }
				checkGameFin();
			}
			if (now.arrow < 1) {
				changeTeam();
			}
			record();
		}
	}
	if (ctrlRQ(sd.ctrl.back)) {
		if (attempt > 0) {
			attempt--;
			now = mem[attempt];
			for (int pos = 0; pos < POS_NUM; pos++) {
				cDarts::instance()->setPointValidation(POS_POINT[pos], !now.isPosFill[pos]);
			}
			return;
		}
		mNextScene = GAME_START;
	}
	else if (ctrlRQ(sd.ctrl.playerSelect)) mNextScene = PLAYER_SELECT;
	else if (ctrlRQ(sd.ctrl.gameSelect)) mNextScene = GAME_SELECT;
	else if (ctrlRQ(sd.ctrl.home)) mNextScene = HOME;
	else if (ctrlRQ(sd.ctrl.config)) {
		mNextScene = CONFIG;
		cDarts::instance()->timer().stop();
	}
	else if (!isPaused && ctrlRQ(sd.ctrl.pause[FALSE])) cDarts::instance()->timer().stop();
	else if (isPaused && ctrlRQ(sd.ctrl.pause[TRUE])) cDarts::instance()->timer().resume();
}

void cStandardCricket::record() {
	if (attempt < MAX_ATTEMPT - 1) {
		attempt++;
		maxAttempt = attempt;
	}
	mem[attempt] = now;
}

void cStandardCricket::checkPosFill(int pos) {
	now.isPosFill[pos] = true;
	for (int team = 0; team < nTeam; team++) {
		if (!now.isTeamPosFilled[team][pos]) {
			now.isPosFill[pos] = false;
			break;
		}
	}
	if (now.isPosFill[pos]) cDarts::instance()->setPointValidation(POS_POINT[pos], false);
}

bool cStandardCricket::addDamage(int pos, int damage) {
	now.teamPosPower[now.team][pos] = MAX_POWER;
	if (now.isPosFill[pos] && damage <= 0) return false;
	if (nTeam <= 2) {
		int opponent = (now.team + 1) % nTeam;
		// Add damage to opponent if opponent has not closed this position
		if (!now.isTeamPosFilled[opponent][pos]) {
			now.teamBill[now.team] += damage;
		}
		return true;
	}
	// Add damage to all opponents who have not closed this position
	for (int opponent = (now.team + 1) % nTeam; opponent != now.team; opponent = (opponent + 1) % nTeam) {
		if (!now.isTeamPosFilled[opponent][pos]) {
			now.teamBill[opponent] += damage;
		}
	}
	return true;
}

void cStandardCricket::changeTeam() {
	for (int i = 0; i < nTeam; i++) {
		now.team++;
		if (now.team >= nTeam) {
			now.team = 0;
			now.round++;
			now.member++;
		}
		if (now.member >= sd.teams.at(now.team).members.size()) {
			now.member = 0;
		}
		if (!now.isTeamFin[now.team]) {
			now.arrow = 3;
			break;
		}
	}
}

void cStandardCricket::updateRank() {
	if (nTeam <= 2) {
		int opponent = (now.team + 1) % nTeam;
		if (now.teamBill[now.team] == now.teamBill[opponent]) {
			now.rank[now.team] = 0; now.rank[opponent] = 0;
		}
		else if (now.teamBill[now.team] > now.teamBill[opponent]) {
			now.rank[now.team] = 0; now.rank[opponent] = 1;
		}
		else {
			now.rank[now.team] = 1; now.rank[opponent] = 0;
		}
		return;
	}
	for (int team = 0; team < nTeam; team++) {
		if (!now.isTeamFin[team]) {
			now.rank[team] = 0;
			for (int opponent = (team + 1) % nTeam; opponent != team; opponent = (opponent + 1) % nTeam) {
				// Increase rank if opponent has a lower bill or is finished
				if (now.teamBill[team] > now.teamBill[opponent] ||
					now.isTeamFin[opponent] ||
					(now.teamBill[team] == now.teamBill[opponent] &&
						team > opponent)) {
					now.rank[team]++;
				}
			}
		}
	}
}

void cStandardCricket::checkTeamFin() {
	for (int team = 0; team < nTeam; team++) {
		if (!now.isTeamFin[team]) {
			now.isTeamFin[team] = true;
			for (int pos = 0; pos < POS_NUM; pos++) {
				if (!now.isTeamPosFilled[team][pos]) {
					now.isTeamFin[team] = false;
					break;
				}
			}
			for (int opponent = (team + 1) % nTeam; opponent != team; opponent = (opponent + 1) % nTeam) {
				// If any opponent has a higher rank and is not finished, this team is not finished
				if (now.rank[team] >= now.rank[opponent] && !now.isTeamFin[opponent]) {
					now.isTeamFin[team] = false;
					break;
				}
			}
		}
	}
}

void cStandardCricket::checkGameFin() {
	now.isGameFin = true;
	for (int team = 0; team < nTeam; team++) {
		if (!now.isTeamFin[team]) {
			now.isGameFin = false;
		}
	}
}

void cStandardCricket::fin() {
	sd.ctrl.skip.icon.box.setLowerRight(sd.screen.right(), sd.obj.lowerFrame.top());
	if (nTeam > MAX_TEAM_NUM) {
		for (int player = 0; player < nTeam; player++) {
			sd.teams.at(player).members.at(0).image.box.setSize(100, 100);
		}
	}
}