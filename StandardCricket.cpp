#include "StandardCricket.hpp"
#include "Darts.hpp"
#include "Timer.hpp"
#include "Control.hpp"

cStandardCricket::cStandardCricket(ShareData shareData) : attempt(0), maxAttempt(0) {
	sd = shareData;
	nTeam = cPlayer::instance()->nTeam();
	mNowScene = STANDARD_CRICKET;
	attempt = 0;
	maxAttempt = 0;
	now = {};
	now.arrow = 3;
	for (int team = 0; team < nTeam; team++) now.rank[team] = team;
	for (int point = cDarts::sPoint::MISS; point < cDarts::sPoint::MIN_CRICKET_POINT; point++)
		cDarts::instance()->setPointValidation(point, false);
	for (int point = cDarts::sPoint::MIN_CRICKET_POINT; point < cDarts::sPoint::NUM; point++)
		cDarts::instance()->setPointValidation(point, true);
	ranker.reserve(nTeam);
	if (nTeam <= cPlayer::MAX_DUO_TEAM_NUM) {
		space = 6;
		teamBox.push_back(cBox());
		teamBox.at(0).setSize(
			100, (cPlayer::instance()->teamType() + 1) * 100 + POINT_NUM * (MfontSize + space));
		teamBox.at(0).setUpperLeft(screen.right() - cPlayer::MAX_DUO_TEAM_NUM * 100, upperFrame.bottom());
		teamBox.at(0).setColor(tableColor);
		cPlayer::instance()->teamMemberImageBox(0, 0).setUpperLeft(teamBox.at(0).upperLeft());
		if (cPlayer::instance()->nTeamMember(0) == 2) {
			cPlayer::instance()->teamMemberImageBox(0, 1).setUpperLeft(
				cPlayer::instance()->teamMemberImageBox(0, 0).lowerLeft());
		}
		for (int team = 1; team < nTeam; team++) {
			teamBox.push_back(teamBox.at(0));
			teamBox.at(team).setLeft(teamBox.at(team - 1).right());
			cPlayer::instance()->teamMemberImageBox(team, 0).setUpperLeft(teamBox.at(team).upperLeft());
			if (cPlayer::instance()->nTeamMember(team) == 2) {
				cPlayer::instance()->teamMemberImageBox(team, 1).setUpperLeft(
					cPlayer::instance()->teamMemberImageBox(team, 0).lowerLeft());
			}
			if (team == now.team) {
				teamBox.at(team).setColor(white); continue;
			}
			teamBox.at(team).setColor(tableColor);
		}
		pointBox.push_back(cBox());
		pointBox.at(0).setSize(teamBox.back().right() - teamBox.at(0).left(), MfontSize + space);
		pointBox.at(0).setUpperLeft(
			teamBox.at(0).left(),
			teamBox.at(0).top() + (cPlayer::instance()->teamType() + 1) * 100);
		pointBox.at(0).setColor(tableColor);
		for (int pos = 1; pos < POINT_NUM; pos++) {
			pointBox.push_back(pointBox.at(0));
			pointBox.at(pos).setTop(pointBox.at(pos - 1).bottom());
		}
		for (int team = 0; team < nTeam; team++) {
			teamMarks.push_back(sPosBox());
			teamMarks.at(team).posBox[0].setSize(MfontSize, MfontSize);
			teamMarks.at(team).posBox[0].setCenter(teamBox.at(team).centerX(), pointBox.at(0).centerY());
			for (int pos = 1; pos < POS_NUM; pos++) {
				teamMarks.at(team).posBox[pos] = teamMarks.at(team).posBox[0];
				teamMarks.at(team).posBox[pos].setCenterY(pointBox.at(pos).centerY());
			}
		}
	}
	else {
		space = 2;
		for (int team = 0; team < nTeam; team++) {
			cPlayer::instance()->teamMemberImageBox(team, 0).setSize(100, 70);
			teamBox.push_back(cBox());
			teamBox.at(team).setSize(100, 70 + POINT_NUM * (SfontSize + space));
			if (team == now.team) {
				teamBox.at(team).setColor(white); continue;
			}
			teamBox.at(team).setColor(tableColor);
		}
		for (int team = 0; team < cPlayer::MAX_DUO_TEAM_NUM; team++) {
			teamBox.at(team).setUpperLeft(
				screen.right() + 100 * (team - cPlayer::MAX_DUO_TEAM_NUM), upperFrame.bottom());
		}
		for (int team = cPlayer::MAX_DUO_TEAM_NUM; team < nTeam; team++) {
			teamBox.at(team).setUpperLeft(
				teamBox.at(team - cPlayer::MAX_DUO_TEAM_NUM).left(),
				teamBox.at(team - cPlayer::MAX_DUO_TEAM_NUM).bottom() + MfontSize + space);
		}
		for (int team = 0; team < nTeam; team++) {
			cPlayer::instance()->teamMemberImageBox(team, 0).setUpperLeft(teamBox.at(team).upperLeft());
		}
		pointBox.push_back(cBox());
		pointBox.at(0).setSize(100 * cPlayer::MAX_DUO_TEAM_NUM, SfontSize + space);
		pointBox.at(0).setUpperLeft(screen.right() - 400, teamBox.at(0).top() + 70);
		pointBox.at(0).setColor(tableColor);
		for (int pos = 1; pos < POINT_NUM; pos++) {
			pointBox.push_back(cBox());
			pointBox.at(pos).setSize(100 * cPlayer::MAX_DUO_TEAM_NUM, SfontSize + space);
			pointBox.at(pos).setUpperLeft(pointBox.at(pos - 1).lowerLeft());
			pointBox.at(pos).setColor(tableColor);
		}
		pointBox.push_back(cBox());
		pointBox.at(POINT_NUM).setSize(100 * (nTeam - cPlayer::MAX_DUO_TEAM_NUM), SfontSize + space);
		pointBox.at(POINT_NUM).setUpperLeft(screen.right() - 400, teamBox.at(cPlayer::MAX_DUO_TEAM_NUM).top() + 70);
		pointBox.at(POINT_NUM).setColor(tableColor);
		for (int pos = POINT_NUM + 1; pos < 2 * POINT_NUM; pos++) {
			pointBox.push_back(cBox());
			pointBox.at(pos).setSize(100 * (nTeam - cPlayer::MAX_DUO_TEAM_NUM), SfontSize + space);
			pointBox.at(pos).setUpperLeft(pointBox.at(pos - 1).lowerLeft());
			pointBox.at(pos).setColor(tableColor);
		}
		for (int team = 0; team < nTeam; team++) {
			teamMarks.push_back(sPosBox());
			for (int pos = 0; pos < POS_NUM; pos++) {
				teamMarks.at(team).posBox[pos].setSize(SfontSize, SfontSize);
				teamMarks.at(team).posBox[pos].setCenter(
					teamBox.at(team).center().x(),
					pointBox.at(pos + (team / cPlayer::MAX_DUO_TEAM_NUM) * POINT_NUM).center().y());
			}
		}
	}
	mem[attempt] = now;
}

void cStandardCricket::reset() {
	cBaseScene::reset();
	cTimer::instance()->restart();
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
	cBaseScene::draw();

	// draw control icons
	if (attempt < maxAttempt) cControl::instance()->icon(cControl::FORWARD).draw();

	// draw darts board
	cDarts::instance()->draw();

	// draw round
	DrawFormatStringToHandle(screen.center().x() - 80, upperFrame.bottom() + 10,
		white, Mfont, "Round%3d", now.round + 1);

	// draw score table
	cPlayer::sChara chara;
	unsigned int color = 0U;
	if (nTeam <= cPlayer::MAX_DUO_TEAM_NUM) {
		DrawBox(screen.center().x() + 10, teamBox.at(0).top(),
			teamBox.at(0).left(), teamBox.at(0).bottom(), tableColor, TRUE);
		for (int team = 0; team < nTeam; team++) { teamBox.at(team).draw(); }
		for (int pos = 0; pos < POS_NUM; pos++) {
			if (now.isPosFill[pos]) { pointBox.at(pos).draw(); }
		}
		for (int team = 0; team < nTeam; team++) {
			for (int member = 0; member < cPlayer::instance()->nTeamMember(team); member++) {
				chara = cPlayer::instance()->teamMember(team, member);
				chara.image.draw();
				color = white;
				if (team == now.team && member == min(now.member, cPlayer::instance()->nTeamMember(team) - 1)) {
					color = touchColor;
				}
				DrawStringToHandle(chara.image.box().left() + 5 * max(0, 10 - chara.name.size()),
					chara.image.box().bottom() - SfontSize - 6, chara.name.c_str(), color, Sfont);
			}
			DrawStringToHandle(teamBox.at(team).left(), teamBox.at(team).top(),
				rankName[now.rank[team]].c_str(), rankColor[now.rank[team]], Mfont);
			DrawFormatStringToHandle(
				chara.image.box().center().x() - 20, pointBox.at(POS_NUM).top() + space / 2,
				white, Mfont, "%4d", now.teamScore[team]);
			DrawLine(teamBox.at(team).left(), teamBox.at(team).top(),
				teamBox.at(team).left(), teamBox.at(team).bottom(), black);
		}
		int y = cPlayer::instance()->teamMemberImage(0, cPlayer::instance()->teamType()).box().bottom();
		for (int pos = 0; pos < POS_NUM - 1; pos++) {
			color = white;
			if (now.isPosFill[pos]) {
				color = gray;
			}
			DrawStringToHandle(screen.center().x() + 20, pointBox.at(pos).top() + space / 2,
				std::to_string(POS_POINT[pos]).c_str(), color, Mfont);
		}
		color = white;
		if (now.isPosFill[POS_NUM - 1]) {
			color = gray;
		}
		DrawStringToHandle(screen.center().x() + 10, pointBox.at(POS_NUM - 1).top() + space / 2,
			"Bull", color, Mfont);
		for (int point = 0; point < POINT_NUM; point++) {
			DrawLine(screen.center().x() + 10, pointBox.at(point).top(),
				teamBox.at(nTeam - 1).right(), pointBox.at(point).top(), black);
		}
		DrawStringToHandle(
			screen.center().x() + 10, pointBox.at(POS_NUM).center().y() - SfontSize / 2,
			"Score", white, Sfont);
	}
	else {
		DrawBox(screen.center().x() + 10, teamBox.at(0).top(),
			teamBox.at(cPlayer::MAX_DUO_TEAM_NUM - 1).right(), teamBox.at(0).bottom(), tableColor, TRUE);
		DrawBox(screen.center().x() + 10, teamBox.at(cPlayer::MAX_DUO_TEAM_NUM).top(),
			teamBox.at(nTeam - 1).right(), teamBox.at(cPlayer::MAX_DUO_TEAM_NUM).bottom(), tableColor, TRUE);
		DrawBox(teamBox.at(now.team).left(), teamBox.at(now.team).top(),
			teamBox.at(now.team).right(), teamBox.at(now.team).bottom(), white, TRUE);
		for (int team = 0; team < nTeam; team++) {
			teamBox.at(team).draw();
		}
		for (int pos = 0; pos < POS_NUM; pos++) {
			if (now.isPosFill[pos]) {
				pointBox.at(pos).draw();
				pointBox.at(pos + POINT_NUM).draw();
			}
		}
		for (int team = 0; team < nTeam; team++) {
			chara = cPlayer::instance()->teamMember(team, 0);
			DrawRotaGraph(chara.image.box().center().x(), chara.image.box().center().y(),
				0.7, 0.0, chara.image.handle(), TRUE);
			DrawStringToHandle(chara.image.box().left(), chara.image.box().top(),
				rankName[now.rank[team]].c_str(), rankColor[now.rank[team]], Sfont);
			DrawStringToHandle(chara.image.box().left() + 5 * max(0, 10 - chara.name.size()),
				chara.image.box().bottom() - SfontSize - 6,
				chara.name.c_str(), white, Sfont);
			DrawFormatStringToHandle(chara.image.box().center().x() - 20,
				chara.image.box().bottom() + POS_NUM * SfontSize + 15 * space / 2,
				white, Sfont, "%4d", now.teamScore[team]);
			DrawLine(teamBox.at(team).left(), teamBox.at(team).top(),
				teamBox.at(team).left(), teamBox.at(team).bottom(), black);
		}
		for (int pointPos = 0, y = 0; pointPos < POINT_NUM; pointPos++) {
			y = cPlayer::instance()->teamMember(0, 0).image.box().bottom() + pointPos * (SfontSize + space);
			DrawLine(screen.center().x() + 10, y, teamBox.at(cPlayer::MAX_DUO_TEAM_NUM - 1).right(), y, black);
			y = cPlayer::instance()->teams().at(cPlayer::MAX_DUO_TEAM_NUM).members.at(0).image.box().bottom() +
				pointPos * (SfontSize + space);
			DrawLine(screen.center().x() + 10, y, teamBox.at(nTeam - 1).right(), y, black);
		}
		for (int i = 0; i < 2; i++) {
			chara = cPlayer::instance()->teams().at(cPlayer::MAX_DUO_TEAM_NUM * i).members.at(0);
			for (int pos = 0; pos < POS_NUM - 1; pos++) {
				DrawStringToHandle(screen.center().x() + 20,
					chara.image.box().bottom() + pos * SfontSize + (2 * pos + 1) * space / 2,
					std::to_string(POS_POINT[pos]).c_str(), white, Sfont);
			}
			DrawStringToHandle(screen.center().x() + 14,
				chara.image.box().bottom() + (POS_NUM - 1) * SfontSize + (2 * (POS_NUM - 1) + 1) * space / 2,
				"Bull", white, Sfont);
			DrawStringToHandle(screen.center().x() + 10,
				chara.image.box().bottom() + POS_NUM * SfontSize + 15 * space / 2,
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
	DrawBox(screen.center().x() + 10, teamBox.at(0).bottom(),
		screen.right(), teamBox.at(0).bottom() + space + MfontSize, pressColor, TRUE);
	chara = cPlayer::instance()->teamMember(
		now.team, min(now.member, cPlayer::instance()->nTeamMember(now.team) - 1));
	for (int arrow = 0, x = teamBox.at(now.team).right() - 10, y = chara.image.box().top();
		arrow < now.arrow; arrow++, x -= 10)
		DrawGraph(x, y, cDarts::instance()->arrowImage(), TRUE);
	if (now.isGameFin) {
		DrawStringToHandle(screen.center().x() + 120, teamBox.at(0).bottom() + space / 2,
			(ranker.at(0) + " Win!").c_str(), white, Mfont);
		return;
	}
	DrawStringToHandle(screen.center().x() + 120, teamBox.at(0).bottom() + space / 2,
		(chara.name + ", throw darts!").c_str(), white, Mfont);
}

void cStandardCricket::update() {
	cBaseScene::update();
	cDarts::instance()->update();
	cTimer::instance()->update();

	if (cDarts::instance()->isThrowed()) throwDart();
	else if (cControl::instance()->isRequested(cControl::SKIP)) skip();
	else if (cControl::instance()->isRequested(cControl::FORWARD)) forward();
	else if (cControl::instance()->isRequested(cControl::BACK)) back();
	else if (cControl::instance()->isRequested(cControl::PLAYER_SELECT)) mNextScene = PLAYER_SELECT;
	else if (cControl::instance()->isRequested(cControl::GAME_SELECT)) mNextScene = GAME_SELECT;
	else if (cControl::instance()->isRequested(cControl::HOME)) mNextScene = HOME;
	else if (cControl::instance()->isRequested(cControl::CONFIG)) {
		mNextScene = CONFIG; cTimer::instance()->pause();
	}
}

bool cStandardCricket::throwDart() {
	if (now.isTeamFin[now.team]) return false;
	now.arrow--;
	int point = cDarts::instance()->point();
	for (int pos = 0; pos < POS_NUM; pos++) {
		if (point == POS_POINT[pos] && !now.isPosFill[pos]) {
			addScore(pos); checkPosFill(pos); updateRank(); checkTeamFin(); checkGameFin();
			// If team has finished, set arrows to 0
			if (now.isTeamFin[now.team]) { now.arrow = 0; }
			break;
		}
	}
	if (now.arrow < 1) changeTeam();
	record();
}

bool cStandardCricket::back() {
	if (attempt <= 0) { mNextScene = GAME_START; return false; }
	attempt--; now = mem[attempt]; updateScoreBox();
	return true;
}

bool cStandardCricket::forward() {
	if (attempt >= maxAttempt) return false;
	attempt++; now = mem[attempt]; updateScoreBox();
	return true;
}

bool cStandardCricket::skip() {
	if (!changeTeam()) return false;
	record(); return true;
}

void cStandardCricket::updatePointBox() {
	unsigned int color = 0U;
	for (int pos = 0; pos < POS_NUM; pos++) {
		cDarts::instance()->setPointValidation(POS_POINT[pos], !now.isPosFill[pos]);
		if (now.isPosFill[pos]) {
			color = gray;
		}
		else {
			color = tableColor;
		}
		pointBox.at(pos).setColor(color);
		if (pointBox.size() == 2 * POINT_NUM) {
			pointBox.at(pos + POINT_NUM).setColor(color);
		}
	}
}

void cStandardCricket::updateTeamBox() {
	for (int team = 0; team < nTeam; team++) {
		if (now.isTeamFin[team]) {
			teamBox.at(team).setColor(gray); continue;
		}
		if (team == now.team) {
			teamBox.at(team).setColor(white); continue;
		}
		teamBox.at(team).setColor(tableColor);
	}
}

void cStandardCricket::updateScoreBox() {
	updatePointBox(); updateTeamBox();
}

bool cStandardCricket::record() {
	if (attempt >= MAX_ATTEMPT - 2) return false;
	attempt++; maxAttempt = attempt; mem[attempt] = now; return true;
}

void cStandardCricket::checkPosFill(int pos) {
	for (int team = 0; team < nTeam; team++) {
		if (!now.isTeamPosFill[team][pos]) {
			now.isPosFill[pos] = false;
			return;
		}
	}
	now.isPosFill[pos] = true;
	cDarts::instance()->setPointValidation(POS_POINT[pos], true);
	pointBox.at(pos).setColor(gray);
	if (pointBox.size() == 2 * POINT_NUM) pointBox.at(pos + POINT_NUM).setColor(gray);
}

bool cStandardCricket::addScore(int pos) {
	now.teamPosPower[now.team][pos] += cDarts::instance()->power();
	int damage = POS_POINT[pos] * (now.teamPosPower[now.team][pos] - MAX_POWER);
	if (damage >= 0) now.isTeamPosFill[now.team][pos] = true;
	if (damage <= 0) return false;
	now.teamPosPower[now.team][pos] = MAX_POWER;

	if (nTeam <= 2) {
		int opponent = (now.team + 1) % nTeam;
		// Add damage to opponent if opponent has not closed this position
		if (!now.isTeamPosFill[opponent][pos]) {
			now.teamScore[now.team] += damage;
			return true;
		}
		return false;
	}
	// Add damage to all opponents who have not closed this position
	for (int opponent = (now.team + 1) % nTeam; opponent != now.team; opponent = (opponent + 1) % nTeam) {
		if (!now.isTeamPosFill[opponent][pos]) {
			now.teamScore[opponent] += damage;
		}
	}
	return true;
}

bool cStandardCricket::changeTeam() {
	if (!now.isTeamFin[now.team]) teamBox.at(now.team).setColor(tableColor);
	for (int i = 0; i < nTeam; i++) {
		now.team++;
		if (now.team >= nTeam) {
			now.team = 0;
			now.round++;
			now.member++;
		}
		if (now.member > cPlayer::instance()->teamType()) {
			now.member = 0;
		}
		if (!now.isTeamFin[now.team]) {
			now.arrow = 3;
			teamBox.at(now.team).setColor(white);
			return true;
		}
	}
	return false;
}

void cStandardCricket::updateRank() {
	if (nTeam <= 2) {
		int opponent = (now.team + 1) % nTeam;
		if (now.teamScore[now.team] == now.teamScore[opponent]) {
			now.rank[now.team] = 0; now.rank[opponent] = 0;
		}
		else if (now.teamScore[now.team] > now.teamScore[opponent]) {
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
				if (now.teamScore[team] > now.teamScore[opponent] || now.isTeamFin[opponent] ||
					(now.teamScore[team] == now.teamScore[opponent] && team > opponent)) {
					now.rank[team]++;
				}
			}
		}
	}
}

void cStandardCricket::checkTeamFin() {
	for (int team = 0; team < nTeam; team++) {
		if (!now.isTeamFin[team]) {
			for (int pos = 0; pos < POS_NUM; pos++) {
				if (!now.isTeamPosFill[team][pos]) {
					return;
				}
			}
			for (int opponent = (team + 1) % nTeam; opponent != team; opponent = (opponent + 1) % nTeam) {
				// If any opponent has a higher rank and is not finished, this team is not finished
				if (now.rank[team] >= now.rank[opponent] && !now.isTeamFin[opponent]) {
					return;
				}
			}
			now.isTeamFin[team] = true;
			teamBox.at(team).setColor(gray);
		}
	}
}

void cStandardCricket::checkGameFin() {
	for (int team = 0, notFinCount = 0; team < nTeam; team++) {
		if (!now.isTeamFin[team]) {
			notFinCount++;
			if (notFinCount >= 2) {
				now.isGameFin = false; return;
			}
		}
	}
	now.isGameFin = true;
	ranker.clear();
	for (int rank = 0; rank < nTeam; rank++) {
		for (int team = 0; team < nTeam; team++) {
			if (now.rank[team] == rank) {
				ranker.push_back(cPlayer::instance()->teams().at(team).name);
			}
		}
	}
}

void cStandardCricket::fin() {
	cControl::instance()->icon(cControl::SKIP).box().setLowerRight(screen.right(), lowerFrame.top());
	if (nTeam > cPlayer::MAX_DUO_TEAM_NUM) {
		for (int player = 0; player < nTeam; player++) {
			cPlayer::instance()->teams().at(player).members.at(0).image.box().setHeight(100);
		}
	}
}