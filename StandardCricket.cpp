#include "StandardCricket.hpp"
#include "Darts.hpp"
#include "Control.hpp"
#include "Scene.hpp"

cStandardCricket::cStandardCricket() : attempt(0), maxAttempt(0) {
	nTeam = cPlayer::instance()->nTeam();
	attempt = 0;
	maxAttempt = 0;
	now = {};
	now.arrow = 3;
	for (int team = 0; team < nTeam; team++) now.rank[team] = team;
	ranker = cPlayer::instance()->teams();
	for (int point = cDarts::sPoint::MISS; point < cDarts::sPoint::MIN_CRICKET_POINT; point++)
		cDarts::instance()->setPointValidation(point, false);
	for (int point = cDarts::sPoint::MIN_CRICKET_POINT; point < cDarts::sPoint::NUM; point++)
		cDarts::instance()->setPointValidation(point, true);
	if (nTeam <= cPlayer::MAX_DUO_TEAM_NUM) {
		nTable = 1;
		space = 8;
		posBoxHeight = LfontSize + space;
		promptBoxHeight = XLfontSize + space;

		// set team boxes
		teamBoxes.push_back(cBox());
		teamBoxes.front().setSize(
			100, (cPlayer::instance()->teamType() + 1) * 100 + POINT_NUM * posBoxHeight);
		teamBoxes.front().setUpperLeft(
			screen.right() - cPlayer::MAX_DUO_TEAM_NUM * 100, upperFrame.bottom());
		teamBoxes.front().setColor(tableColor);
		for (int team = 1; team < nTeam; team++) {
			cBox leftBox = teamBoxes.back();
			teamBoxes.push_back(leftBox);
			teamBoxes.back().setLeft(leftBox.right());
		}

		// set team member image boxes
		for (int team = 0; team < nTeam; team++) {
			cPlayer::instance()->teamMemberImageBox(team, 0).setUpperLeft(teamBoxes.at(team).upperLeft());
			if (cPlayer::instance()->nTeamMember(team) == 2) {
				cPlayer::instance()->teamMemberImageBox(team, 1).setUpperLeft(
					cPlayer::instance()->teamMemberImageBox(team, 0).lowerLeft());
			}
		}

		// set table boxes
		tableBoxes.push_back(teamBoxes.front());
		tableBoxes.front().setX(screen.centerX() + 10, teamBoxes.front().left());

		// set fill boxes
		filledBoxes.push_back(sPosBox());
		filledBoxes.front().posBox[0].setX(teamBoxes.front().left(), teamBoxes.back().right());
		filledBoxes.front().posBox[0].setY1(
			teamBoxes.front().top() + 100 * (cPlayer::instance()->teamType() + 1));
		filledBoxes.front().posBox[0].setHeight(posBoxHeight);
		filledBoxes.front().posBox[0].setColor(gray);
		filledBoxes.front().posBox[0].setDraw(false);
		for (int pos = 1; pos < POS_NUM; pos++) {
			cBox upperBox = filledBoxes.front().posBox[pos - 1];
			filledBoxes.front().posBox[pos] = upperBox;
			filledBoxes.front().posBox[pos].setTop(upperBox.bottom());
		}

		// set team mark boxes
		teamMarkBoxes.push_back(sPosBox());
		teamMarkBoxes.front().posBox[0].setSize(LfontSize, LfontSize);
		teamMarkBoxes.back().posBox[0].setCenter(
			teamBoxes.front().centerX(), filledBoxes.front().posBox[0].centerY());
		for (int pos = 1; pos < POS_NUM; pos++) {
			teamMarkBoxes.front().posBox[pos] = teamMarkBoxes.front().posBox[0];
			teamMarkBoxes.front().posBox[pos].setCenterY(filledBoxes.front().posBox[pos].centerY());
		}
		for (int team = 1; team < nTeam; team++) {
			teamMarkBoxes.push_back(teamMarkBoxes.front());
			for (int pos = 0; pos < POS_NUM; pos++) {
				teamMarkBoxes.back().posBox[pos].setCenterX(teamBoxes.at(team).centerX());
			}
		}
	}
	else {
		nTable = 2;
		space = 4;
		posBoxHeight = SfontSize + space;
		promptBoxHeight = MfontSize + space;

		// set team boxes
		teamBoxes.push_back(cBox());
		teamBoxes.front().setSize(100, 70 + POINT_NUM * posBoxHeight);
		teamBoxes.front().setColor(tableColor);
		teamBoxes.front().setUpperLeft(
			screen.right() - 100 * cPlayer::MAX_DUO_TEAM_NUM, upperFrame.bottom());
		for (int team = 1; team < cPlayer::MAX_DUO_TEAM_NUM; team++) {
			cBox leftBox = teamBoxes.back();
			teamBoxes.push_back(leftBox);
			teamBoxes.back().setLeft(leftBox.right());
		}
		teamBoxes.push_back(teamBoxes.front());
		teamBoxes.back().setTop(teamBoxes.front().bottom() + promptBoxHeight);
		for (int team = cPlayer::MAX_DUO_TEAM_NUM; team < nTeam; team++) {
			cBox leftBox = teamBoxes.back();
			teamBoxes.push_back(leftBox);
			teamBoxes.back().setLeft(leftBox.right());
		}

		// set team member image boxes
		for (int team = 0; team < nTeam; team++) {
			cPlayer::instance()->teamMemberImageBox(team, 0).setHeight(70);
			cPlayer::instance()->teamMemberImageBox(team, 0).setUpperLeft(teamBoxes.at(team).upperLeft());
		}

		// set table boxes
		for (int table = 0, team = 0; table < nTable; table++, team += cPlayer::MAX_DUO_TEAM_NUM) {
			tableBoxes.push_back(teamBoxes.at(team));
			tableBoxes.back().setX(screen.centerX() + 10, teamBoxes.at(team).left());
		}

		// set fill boxes
		filledBoxes.push_back(sPosBox());
		filledBoxes.front().posBox[0].setSize(100 * cPlayer::MAX_DUO_TEAM_NUM, SfontSize + space);
		filledBoxes.front().posBox[0].setUpperLeft(screen.right() - 400, teamBoxes.at(0).top() + 70);
		filledBoxes.front().posBox[0].setColor(gray);
		filledBoxes.front().posBox[0].setDraw(false);
		for (int pos = 1; pos < POS_NUM; pos++) {
			filledBoxes.front().posBox[pos] = filledBoxes.front().posBox[0];
			filledBoxes.front().posBox[pos].setTop(filledBoxes.front().posBox[pos - 1].bottom());
		}
		filledBoxes.push_back(filledBoxes.front());
		filledBoxes.back().posBox[0].setWidth(100 * (nTeam - cPlayer::MAX_DUO_TEAM_NUM));
		filledBoxes.back().posBox[0].setTop(teamBoxes.at(cPlayer::MAX_DUO_TEAM_NUM).top() + 70);
		for (int pos = 1; pos < POS_NUM; pos++) {
			filledBoxes.back().posBox[pos] = filledBoxes.back().posBox[0];
			filledBoxes.back().posBox[pos].setTop(filledBoxes.back().posBox[pos - 1].bottom());
		}
		for (int team = 0, table = 0; team < nTeam; team++) {
			teamMarkBoxes.push_back(sPosBox());
			if (team == cPlayer::MAX_DUO_TEAM_NUM) {
				table++;
			}
			for (int pos = 0; pos < POS_NUM; pos++) {
				teamMarkBoxes.at(team).posBox[pos].setSize(SfontSize, SfontSize);
				teamMarkBoxes.at(team).posBox[pos].setCenter(
					teamBoxes.at(team).centerX(), filledBoxes.at(table).posBox[pos].centerY());
			}
		}
	}
	teamBoxes.front().setColor(white);
	cControl::instance()->iconBox(cControl::SKIP).setRight(screen.centerX() + 10);
	mem[attempt] = now;
}

void cStandardCricket::reset() {
	cBaseScene::reset();
	cDarts::instance()->reset();
	attempt = 0;
	maxAttempt = 0;
	now = {};
	now.arrow = cDarts::MAX_ARROW_NUM;
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
	DrawFormatStringToHandle(screen.centerX() - 80, upperFrame.bottom() + 10,
		white, Mfont, "Round%3d", now.round + 1);

	// draw score table
	cPlayer::sChara chara;
	unsigned int color = 0U;

	// draw boxes
	for (int table = 0; table < nTable; table++) tableBoxes.at(table).draw();
	for (int team = 0; team < nTeam; team++) teamBoxes.at(team).draw();
	for (int table = 0; table < nTable; table++) {
		for (int pos = 0; pos < POS_NUM; pos++) {
			filledBoxes.at(table).posBox[pos].draw();
		}
	}
	
	if (nTeam <= cPlayer::MAX_DUO_TEAM_NUM) {
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
			DrawStringToHandle(teamBoxes.at(team).left(), teamBoxes.at(team).top(),
				RANK_NAME[now.rank[team]].c_str(), rankColor[now.rank[team]], Mfont);
			DrawFormatStringToHandle(
				chara.image.box().centerX() - 20, teamBoxes.at(team).bottom() - posBoxHeight + space / 2,
				white, Lfont, "%4d", now.teamScore[team]);
		}
		int y = cPlayer::instance()->teamMemberImage(0, cPlayer::instance()->teamType()).box().bottom();
		for (int pos = 0; pos < POS_NUM - 1; pos++) {
			color = white;
			if (now.isPosFill[pos]) {
				color = gray;
			}
			DrawStringToHandle(screen.centerX() + 18, filledBoxes.back().posBox[pos].top() + space / 2,
				POS_NAME[pos].c_str(), color, Lfont);
		}
		color = white;
		if (now.isPosFill[POS_NUM - 1]) {
			color = gray;
		}
		DrawStringToHandle(screen.centerX() + 10, 
			filledBoxes.at(0).posBox[POS_NUM - 1].centerY() - MfontSize / 2, "Bull", color, Mfont);
		DrawStringToHandle(screen.centerX() + 10, 
			filledBoxes.at(0).posBox[POS_NUM - 1].bottom() + (posBoxHeight - SfontSize) / 2,
			"Score", white, Sfont);
	}
	else {
		for (int team = 0; team < nTeam; team++) {
			chara = cPlayer::instance()->teamMember(team, 0);
			DrawRotaGraph(chara.image.box().centerX(), chara.image.box().centerY(),
				0.7, 0.0, chara.image.handle(), TRUE);
			color = white;
			if (team == now.team) {
				color = touchColor;
			}
			DrawStringToHandle(chara.image.box().left() + 5 * max(0, 10 - chara.name.size()),
				chara.image.box().bottom() - SfontSize - 6,
				chara.name.c_str(), color, Sfont);
			DrawStringToHandle(chara.image.box().left(), chara.image.box().top(),
				RANK_NAME[now.rank[team]].c_str(), rankColor[now.rank[team]], Mfont);
			DrawFormatStringToHandle(chara.image.box().centerX() - 20,
				teamBoxes.at(team).bottom() - posBoxHeight + space / 2,
				white, Sfont, "%4d", now.teamScore[team]);
		}
		for (int table = 0; table < nTable; table++) {
			for (int pos = 0; pos < POS_NUM; pos++) {
				DrawStringToHandle(screen.centerX() + 26 - 3 * POS_NAME[pos].size(),
					filledBoxes.at(table).posBox[pos].top() + space / 2,
					POS_NAME[pos].c_str(), white, Sfont);
			}
			DrawStringToHandle(screen.centerX() + 10,
				filledBoxes.at(table).posBox[POS_NUM - 1].bottom() + space / 2,
				"Score", white, Sfont);
		}
	}

	// draw marks
	unsigned int colors[MARK_PART_NUM] = { white, black, 0U };
	for (int team = 0; team < nTeam; team++) {
		colors[2] = teamColor[team];
		for (int pos = 0; pos < POS_NUM; pos++) {
			for (int part = 0; part < MARK_PART_NUM; part++) {
				if (now.teamPosPower[team][pos] >= MAX_POWER) {
					DrawCircleAA(teamMarkBoxes.at(team).posBox[pos].centerX() + MARK_PART_ERROR[part],
						teamMarkBoxes.at(team).posBox[pos].centerY(),
						(teamMarkBoxes.at(team).posBox[pos].height() - 4) / 2.0,
						100, colors[part], FALSE, MARK_PART_LINETHICK[part]);
				}
				if (now.teamPosPower[team][pos] >= 1) {
					DrawLine(
						teamMarkBoxes.at(team).posBox[pos].right() + MARK_PART_ERROR[part],
						teamMarkBoxes.at(team).posBox[pos].top(),
						teamMarkBoxes.at(team).posBox[pos].left() + MARK_PART_ERROR[part],
						teamMarkBoxes.at(team).posBox[pos].bottom(), colors[part], MARK_PART_LINETHICK[part]);
				}
				if (now.teamPosPower[team][pos] >= 2) {
					DrawLine(
						teamMarkBoxes.at(team).posBox[pos].left() + MARK_PART_ERROR[part],
						teamMarkBoxes.at(team).posBox[pos].top(),
						teamMarkBoxes.at(team).posBox[pos].right() + MARK_PART_ERROR[part],
						teamMarkBoxes.at(team).posBox[pos].bottom(), colors[part], MARK_PART_LINETHICK[part]);
				}
			}
		}
	}

	// draw lines
	for (int team = 0; team < nTeam; team++) 
		DrawLine(teamBoxes.at(team).left(), teamBoxes.at(team).top(),
			teamBoxes.at(team).left(), teamBoxes.at(team).bottom(), black);
	for (int table = 0; table < nTable; table++) {
		for (int pos = 0; pos < POS_NUM; pos++) {
			DrawLine(screen.centerX() + 10, filledBoxes.at(table).posBox[pos].top(),
				filledBoxes.at(table).posBox[0].right(), filledBoxes.at(table).posBox[pos].top(), black);
		}
		DrawLine(screen.centerX() + 10, filledBoxes.at(table).posBox[POS_NUM - 1].bottom(),
			filledBoxes.at(table).posBox[0].right(), filledBoxes.at(table).posBox[POS_NUM - 1].bottom(), 
			black);
	}

	// draw arrows
	cPlayer::sChara player = cPlayer::instance()->teamMember(
		now.team, min(now.member, cPlayer::instance()->nTeamMember(now.team) - 1));
	for (int arrow = 0, x = teamBoxes.at(now.team).right() - 10, y = player.image.box().top();
		arrow < now.arrow; arrow++, x -= 10)
		DrawGraph(x, y, cDarts::instance()->arrowImage(), TRUE);

	// draw prompt
	DrawBox(screen.centerX() + 10, teamBoxes.at(0).bottom(),
		screen.right(), teamBoxes.at(0).bottom() + space + MfontSize, pressColor, TRUE);
	if (now.isGameFin) {
		DrawStringToHandle(screen.centerX() + 120, teamBoxes.at(0).bottom() + space / 2,
			(ranker.at(0).name + " Win!").c_str(), white, Mfont);
		return;
	}
	DrawStringToHandle(screen.centerX() + 120, teamBoxes.at(0).bottom() + space / 2,
		(player.name + ", throw darts!").c_str(), white, Mfont);
}

void cStandardCricket::update() {
	cBaseScene::update();
	cDarts::instance()->update();

	if (cDarts::instance()->isThrowed()) throwDart();
	else if (cControl::instance()->isRequested(cControl::SKIP)) skip();
	else if (cControl::instance()->isRequested(cControl::FORWARD)) forward();
	else if (cControl::instance()->isRequested(cControl::BACK)) back();
}

bool cStandardCricket::throwDart() {
	if (now.isTeamFin[now.team]) return false;
	now.arrow--;
	int point = cDarts::instance()->point();
	for (int pos = 0; pos < POS_NUM; pos++) {
		if (point == POS_POINT[pos] && !now.isPosFill[pos]) {
			addScore(pos); checkPosFill(pos); updateRank(); checkGameFin();
			if (now.isTeamFin[now.team]) { // If team has finished,
				now.arrow = 0; // set arrows to 0
			}
			break;
		}
	}
	if (now.arrow < 1) changeTeam();
	record();
	return true;
}

bool cStandardCricket::back() {
	if (attempt <= 0) { cScene::instance()->setScene(cScene::GAME_START); return false; }
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

void cStandardCricket::updateScoreBox() {
	// Update filled boxes
	for (int pos = 0; pos < POS_NUM; pos++) {
		// Update point validation
		cDarts::instance()->setPointValidation(POS_POINT[pos], !now.isPosFill[pos]);
		for (int table = 0; table < nTable; table++) {
			// Update whether draw fill box
			filledBoxes.at(table).posBox[pos].setDraw(now.isPosFill[pos]);
		}
	}

	// Update team box colors
	for (int team = 0; team < nTeam; team++) {
		if (now.isTeamFin[team]) { // If team has finished,
			teamBoxes.at(team).setColor(gray); // set team box gray color
			continue;
		}
		if (team == now.team) { // If team is playing,
			teamBoxes.at(team).setColor(white); // set team box white color
			continue;
		} 
		// If team is not playing,
		teamBoxes.at(team).setColor(tableColor); // set team box table color
	}
}

bool cStandardCricket::record() {
	if (attempt >= MAX_ATTEMPT - 2) return false;
	attempt++; maxAttempt = attempt; mem[attempt] = now; return true;
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
	for (int opponent = 0; opponent < nTeam; opponent++) {
		if (opponent == now.team) {
			continue;
		}
		if (!now.isTeamPosFill[opponent][pos]) {
			now.teamScore[opponent] += damage;
		}
	}
	return true;
}

void cStandardCricket::checkPosFill(int pos) {
	for (int team = 0; team < nTeam; team++) {
		if (!now.isTeamPosFill[team][pos]) { // If any team has not filled this position,
			return; // the position is not filled
		}
	}
	// If all teams have filled this position,
	now.isPosFill[pos] = true;
	cDarts::instance()->setPointValidation(POS_POINT[pos], false); // disable point
	for (int table = 0; table < nTable; table++) {
		filledBoxes.at(table).posBox[pos].setDraw(true); // set filled box to draw
	}
}

bool cStandardCricket::changeTeam() {
	if (!now.isTeamFin[now.team]) teamBoxes.at(now.team).setColor(tableColor);
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
		if (!now.isTeamFin[now.team]) { // If the next team is not finished,
			now.arrow = cDarts::MAX_ARROW_NUM; // reset arrows
			teamBoxes.at(now.team).setColor(white); // set team box to white
			updateRank();
			return true;
		}
	}
	return false;
}

void cStandardCricket::updateRank() {
	if (now.isGameFin) return;
	if (nTeam <= 2) {
		int opponent = (now.team + 1) % nTeam;
		if (now.teamScore[now.team] >= now.teamScore[opponent]) {
			now.rank[now.team] = 0; now.rank[opponent] = 1;
		}
		else {
			now.rank[now.team] = 1; now.rank[opponent] = 0;
		}
		for (int team = 0; team < 2; team++) {
			ranker.at(now.rank[team]) = cPlayer::instance()->teams().at(team);
		}
		return;
	}
	for (int team = 0, teamOrder = nTeam - now.team; team < nTeam; team++, teamOrder++) {
		if (teamOrder >= nTeam) {
			teamOrder -= nTeam;
		}
		if (now.isTeamFin[team]) {
			continue;
		}
		now.rank[team] = 0;
		for (int opponent = 0, opponentOrder = nTeam - now.team; opponent < nTeam;
			opponent++, opponentOrder++) {
			if (opponentOrder >= nTeam) {
				opponentOrder -= nTeam;
			}
			if (opponent == team) {
				continue;
			}
			if (now.teamScore[team] > now.teamScore[opponent] ||
				(now.teamScore[team] == now.teamScore[opponent] &&
					(teamOrder > opponentOrder || now.isTeamFin[opponent]))) {
				now.rank[team]++; // If opponent has lower score or is finished, increase rank
			}
		}
		ranker.at(now.rank[team]) = cPlayer::instance()->teams().at(team);
	}
}

void cStandardCricket::checkTeamFin(int team) {
	if (now.isTeamFin[team]) return;
	for (int pos = 0; pos < POS_NUM; pos++) {
		if (!now.isTeamPosFill[team][pos]) { // If any position is not filled
			return; // the team is not finished
		}
	}
	for (int opponent = 0; opponent < nTeam; opponent++) {
		if (opponent == team) {
			continue;
		}
		if (now.rank[team] > now.rank[opponent] && !now.isTeamFin[opponent]) {
			return; // If any opponent has a higher rank and is not finished, the team is not finished
		}
	}
	// If the team has filled all positions and has a higher rank than all unfinished opponents,
	now.isTeamFin[team] = true;
	teamBoxes.at(team).setColor(gray); // If the team is finished, set team box to gray
}

void cStandardCricket::checkGameFin() {
	int notFinCount = 0;
	for (int team = 0; team < nTeam; team++) {
		checkTeamFin(team);
		if (!now.isTeamFin[team]) { // If team is not finished, 
			notFinCount++; // add notFinCount
		}
	}
	if (notFinCount > 1) return; // If more than one team is not finished, the game is not finished
	now.isGameFin = true; // If only one team is not finished, the game is finished
}

void cStandardCricket::fin() {
	cControl::instance()->iconBox(cControl::SKIP).setRight(screen.right());
	if (nTeam > cPlayer::MAX_DUO_TEAM_NUM) {
		for (int team = 0; team < nTeam; team++) {
			cPlayer::instance()->teams().at(team).members.at(0).image.box().setHeight(100);
		}
	}
}