#include "StandardCricket.hpp"
#include "Darts.hpp"
#include "Control.hpp"
#include "Scene.hpp"

cStandardCricket::cStandardCricket() : attempt(0), maxAttempt(0) {
	now = {};
	now.arrow = 3;
	for (int team = 0; team < nTeam; team++) now.rank[team] = team;
	ranker = cPlayer::inst()->teams();
	for (int point = cDarts::sPoint::MISS; point < cDarts::sPoint::MIN_CRICKET_POINT; point++)
		cDarts::inst()->setPointValidation(point, false);
	for (int point = cDarts::sPoint::MIN_CRICKET_POINT; point < cDarts::sPoint::NUM; point++)
		cDarts::inst()->setPointValidation(point, true);
	if (nTeam <= cPlayer::MAX_DUO_TEAM_NUM) {
		nTable = 1;
		space = 8;
		posBoxHeight = LfontSize + space;
		promptBoxHeight = XLfontSize + space;

		// set team boxes
		teamBoxes.push_back(cBox());
		teamBoxes.front().setS(100, (cPlayer::inst()->teamType() + 1) * 100 + POINT_NUM * posBoxHeight);
		teamBoxes.front().setUL(screen.R() - cPlayer::MAX_DUO_TEAM_NUM * 100, upperFrame.B());
		teamBoxes.front().setColor(tableColor);
		for (int team = 1; team < nTeam; team++) {
			cBox leftBox = teamBoxes.back();
			teamBoxes.push_back(leftBox);
			teamBoxes.back().setL(leftBox.R());
		}

		// set team member image boxes
		for (int team = 0; team < nTeam; team++) {
			cPlayer::inst()->teamMemberImageBox(team, 0).setUL(teamBoxes.at(team).UL());
			if (cPlayer::inst()->nTeamMember(team) == 2) {
				cPlayer::inst()->teamMemberImageBox(team, 1).setUL(
					cPlayer::inst()->teamMemberImageBox(team, 0).LL());
			}
		}

		// set table boxes
		tableBox = teamBoxes.front();
		tableBox.setX(screen.CX() + 10, teamBoxes.front().L());

		// set fill boxes
		filledBoxes.push_back(sPosBox());
		filledBoxes.front().posBox[0].setX(teamBoxes.front().L(), teamBoxes.back().R());
		filledBoxes.front().posBox[0].setY1(
			teamBoxes.front().T() + 100 * (cPlayer::inst()->teamType() + 1));
		filledBoxes.front().posBox[0].setH(posBoxHeight);
		filledBoxes.front().posBox[0].setColor(gray);
		filledBoxes.front().posBox[0].setDrawFlag(false);
		for (int pos = 1; pos < POS_NUM; pos++) {
			cBox upperBox = filledBoxes.front().posBox[pos - 1];
			filledBoxes.front().posBox[pos] = upperBox;
			filledBoxes.front().posBox[pos].setT(upperBox.B());
		}

		// set team mark boxes
		teamMarkBoxes.push_back(sPosBox());
		teamMarkBoxes.front().posBox[0].setS(LfontSize, LfontSize);
		teamMarkBoxes.back().posBox[0].setC(teamBoxes.front().CX(), filledBoxes.front().posBox[0].CY());
		for (int pos = 1; pos < POS_NUM; pos++) {
			teamMarkBoxes.front().posBox[pos] = teamMarkBoxes.front().posBox[0];
			teamMarkBoxes.front().posBox[pos].setCY(filledBoxes.front().posBox[pos].CY());
		}
		for (int team = 1; team < nTeam; team++) {
			teamMarkBoxes.push_back(teamMarkBoxes.front());
			for (int pos = 0; pos < POS_NUM; pos++) {
				teamMarkBoxes.back().posBox[pos].setCX(teamBoxes.at(team).CX());
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
		teamBoxes.front().setS(100, 70 + POINT_NUM * posBoxHeight);
		teamBoxes.front().setColor(tableColor);
		teamBoxes.front().setUL(screen.R() - 100 * cPlayer::MAX_DUO_TEAM_NUM, upperFrame.B());
		for (int team = 1; team < cPlayer::MAX_DUO_TEAM_NUM; team++) {
			cBox leftBox = teamBoxes.back();
			teamBoxes.push_back(leftBox);
			teamBoxes.back().setL(leftBox.R());
		}
		teamBoxes.push_back(teamBoxes.front());
		teamBoxes.back().setT(teamBoxes.front().B() + promptBoxHeight);
		for (int team = cPlayer::MAX_DUO_TEAM_NUM + 1; team < nTeam; team++) {
			cBox leftBox = teamBoxes.back();
			teamBoxes.push_back(leftBox);
			teamBoxes.back().setL(leftBox.R());
		}

		// set team member image boxes
		for (int team = 0; team < nTeam; team++) {
			cPlayer::inst()->teamMemberImageBox(team, 0).setH(70);
			cPlayer::inst()->teamMemberImageBox(team, 0).setUL(teamBoxes.at(team).UL());
		}

		// set table boxes
		tableBox = teamBoxes.front();
		tableBox.setX(screen.CX() + 10, teamBoxes.front().L());
		tableBox.setY2(teamBoxes.back().B());

		// set fill boxes
		filledBoxes.push_back(sPosBox());
		filledBoxes.front().posBox[0].setS(100 * cPlayer::MAX_DUO_TEAM_NUM, SfontSize + space);
		filledBoxes.front().posBox[0].setUL(screen.R() - 400, teamBoxes.at(0).T() + 70);
		filledBoxes.front().posBox[0].setColor(gray);
		filledBoxes.front().posBox[0].setDrawFlag(false);
		for (int pos = 1; pos < POS_NUM; pos++) {
			filledBoxes.front().posBox[pos] = filledBoxes.front().posBox[0];
			filledBoxes.front().posBox[pos].setT(filledBoxes.front().posBox[pos - 1].B());
		}
		filledBoxes.push_back(filledBoxes.front());
		filledBoxes.back().posBox[0].setW(100 * (nTeam - cPlayer::MAX_DUO_TEAM_NUM));
		filledBoxes.back().posBox[0].setT(teamBoxes.at(cPlayer::MAX_DUO_TEAM_NUM).T() + 70);
		for (int pos = 1; pos < POS_NUM; pos++) {
			filledBoxes.back().posBox[pos] = filledBoxes.back().posBox[0];
			filledBoxes.back().posBox[pos].setT(filledBoxes.back().posBox[pos - 1].B());
		}
		for (int team = 0, table = 0; team < nTeam; team++) {
			teamMarkBoxes.push_back(sPosBox());
			if (team == cPlayer::MAX_DUO_TEAM_NUM) {
				table++;
			}
			for (int pos = 0; pos < POS_NUM; pos++) {
				teamMarkBoxes.at(team).posBox[pos].setS(SfontSize, SfontSize);
				teamMarkBoxes.at(team).posBox[pos].setC(
					teamBoxes.at(team).CX(), filledBoxes.at(table).posBox[pos].CY());
			}
		}
	}
	teamBoxes.front().setColor(white);
	cControl::inst()->iconBox(cControl::SKIP).setR(screen.CX() + 10);
	mem[attempt] = now;
}

void cStandardCricket::reset() {
	cBaseScene::reset();
	cDarts::inst()->reset();
	attempt = 0;
	maxAttempt = 0;
	now = {};
	now.arrow = cDarts::MAX_ARROW_NUM;
	for (int team = 0; team < nTeam; team++) now.rank[team] = team;
	for (int point = cDarts::sPoint::MISS; point < cDarts::sPoint::MIN_CRICKET_POINT; point++)
		cDarts::inst()->setPointValidation(point, false);
	for (int point = cDarts::sPoint::MIN_CRICKET_POINT; point < cDarts::sPoint::NUM; point++)
		cDarts::inst()->setPointValidation(point, true);
	updateScoreBox();
}

void cStandardCricket::draw() {
	cBaseScene::draw();

	// control icons
	if (attempt < maxAttempt) cControl::inst()->icon(cControl::FORWARD).draw();

	// darts board
	cDarts::inst()->draw();

	// round
	DrawFormatStringToHandle(screen.CX() - 80, upperFrame.B() + 10,
		white, Mfont, "Round%3d", now.round + 1);

	// score table
	cPlayer::sChara chara;
	unsigned int color = 0U;

	// boxes
	tableBox.draw();
	for (int team = 0; team < nTeam; team++) teamBoxes.at(team).draw();
	for (int table = 0; table < nTable; table++) {
		for (int pos = 0; pos < POS_NUM; pos++) {
			filledBoxes.at(table).posBox[pos].draw();
		}
	}
	DrawBox(screen.CX() + 10, teamBoxes.at(0).B(), screen.R(), teamBoxes.at(0).B() + space + MfontSize, 
		pressColor, TRUE);
	
	if (nTeam <= cPlayer::MAX_DUO_TEAM_NUM) {
		for (int team = 0; team < nTeam; team++) {
			for (int member = 0; member < cPlayer::inst()->nTeamMember(team); member++) {
				chara = cPlayer::inst()->teamMember(team, member);
				chara.image.draw();
				color = white;
				if (!now.isGameFin && team == now.team &&
					member == min(now.member, cPlayer::inst()->nTeamMember(team) - 1)) {
					color = touchColor;
					// arrows
					for (int arrow = 0, x = chara.image.box().R() - 10, y = chara.image.box().T();
						arrow < now.arrow; arrow++, x -= 10) {
						DrawGraph(x, y, cDarts::inst()->arrowImage(), TRUE);
					}
					// prompt
					DrawStringToHandle(screen.CX() + 120, teamBoxes.at(0).B() + space / 2,
						(chara.name + ", throw darts!").c_str(), white, Mfont);
				}
				DrawStringToHandle(chara.image.box().L() + 5 * max(0, 10 - (int)chara.name.size()),
					chara.image.box().B() - SfontSize - 6, chara.name.c_str(), color, Sfont);
			}
			DrawStringToHandle(teamBoxes.at(team).L(), teamBoxes.at(team).T(),
				RANK_NAME[now.rank[team]].c_str(), rankColor[now.rank[team]], Lfont);
			DrawFormatStringToHandle(
				chara.image.box().CX() - 20, teamBoxes.at(team).B() - posBoxHeight + space / 2,
				white, Lfont, "%4d", now.teamScore[team]);
		}
		int y = cPlayer::inst()->teamMemberImage(0, cPlayer::inst()->teamType()).box().B();
		for (int pos = 0; pos < POS_NUM - 1; pos++) {
			color = white;
			if (now.isPosFill[pos]) {
				color = gray;
			}
			DrawStringToHandle(screen.CX() + 18, filledBoxes.back().posBox[pos].T() + space / 2,
				POS_NAME[pos].c_str(), color, Lfont);
		}
		color = white;
		if (now.isPosFill[POS_NUM - 1]) {
			color = gray;
		}
		DrawStringToHandle(screen.CX() + 10, filledBoxes.at(0).posBox[POS_NUM - 1].CY() - MfontSize / 2, 
			"Bull", color, Mfont);
		DrawStringToHandle(
			screen.CX() + 10, filledBoxes.at(0).posBox[POS_NUM - 1].B() + (posBoxHeight - SfontSize) / 2,
			"Score", white, Sfont);
	}
	else {
		for (int team = 0; team < nTeam; team++) {
			chara = cPlayer::inst()->teamMember(team, 0);
			DrawRotaGraph(chara.image.box().CX(), chara.image.box().CY(),
				0.7, 0.0, chara.image.handle(), TRUE);
			color = white;
			if (!now.isGameFin && team == now.team) {
				color = touchColor;
				// arrows
				for (int arrow = 0, x = chara.image.box().R() - 10, y = chara.image.box().T();
					arrow < now.arrow; arrow++, x -= 10) {
					DrawGraph(x, y, cDarts::inst()->arrowImage(), TRUE);
				}
				// prompt
				DrawStringToHandle(screen.CX() + 120, teamBoxes.at(0).B() + space / 2,
					(chara.name + ", throw darts!").c_str(), white, Mfont);
			}
			DrawStringToHandle(chara.image.box().L() + 5 * max(0, 10 - (int)chara.name.size()),
				chara.image.box().B() - SfontSize - 6,
				chara.name.c_str(), color, Sfont);
			DrawStringToHandle(chara.image.box().L(), chara.image.box().T(),
				RANK_NAME[now.rank[team]].c_str(), rankColor[now.rank[team]], Lfont);
			DrawFormatStringToHandle(chara.image.box().CX() - 20,
				teamBoxes.at(team).B() - posBoxHeight + space / 2,
				white, Sfont, "%4d", now.teamScore[team]);
		}
		for (int table = 0; table < nTable; table++) {
			for (int pos = 0; pos < POS_NUM; pos++) {
				DrawStringToHandle(screen.CX() + 26 - 3 * POS_NAME[pos].size(),
					filledBoxes.at(table).posBox[pos].T() + space / 2,
					POS_NAME[pos].c_str(), white, Sfont);
			}
			DrawStringToHandle(screen.CX() + 10, filledBoxes.at(table).posBox[POS_NUM - 1].B() + space / 2,
				"Score", white, Sfont);
		}
	}

	// marks
	unsigned int colors[MARK_PART_NUM] = { white, black, 0U };
	for (int team = 0; team < nTeam; team++) {
		colors[2] = teamColor[team];
		for (int pos = 0; pos < POS_NUM; pos++) {
			for (int part = 0; part < MARK_PART_NUM; part++) {
				if (now.teamPosPower[team][pos] >= MAX_POWER) {
					DrawCircleAA(teamMarkBoxes.at(team).posBox[pos].CX() + MARK_PART_ERROR[part],
						teamMarkBoxes.at(team).posBox[pos].CY(),
						(teamMarkBoxes.at(team).posBox[pos].H() - 4) / 2.0,
						100, colors[part], FALSE, MARK_PART_LINETHICK[part]);
				}
				if (now.teamPosPower[team][pos] >= 1) {
					DrawLine(
						teamMarkBoxes.at(team).posBox[pos].R() + MARK_PART_ERROR[part],
						teamMarkBoxes.at(team).posBox[pos].T(),
						teamMarkBoxes.at(team).posBox[pos].L() + MARK_PART_ERROR[part],
						teamMarkBoxes.at(team).posBox[pos].B(), colors[part], MARK_PART_LINETHICK[part]);
				}
				if (now.teamPosPower[team][pos] >= 2) {
					DrawLine(
						teamMarkBoxes.at(team).posBox[pos].L() + MARK_PART_ERROR[part],
						teamMarkBoxes.at(team).posBox[pos].T(),
						teamMarkBoxes.at(team).posBox[pos].R() + MARK_PART_ERROR[part],
						teamMarkBoxes.at(team).posBox[pos].B(), colors[part], MARK_PART_LINETHICK[part]);
				}
			}
		}
	}

	// lines
	for (int team = 0; team < nTeam; team++) 
		DrawLine(teamBoxes.at(team).L(), teamBoxes.at(team).T(),
			teamBoxes.at(team).L(), teamBoxes.at(team).B(), black);
	for (int table = 0; table < nTable; table++) {
		for (int pos = 0; pos < POS_NUM; pos++) {
			DrawLine(screen.CX() + 10, filledBoxes.at(table).posBox[pos].T(),
				filledBoxes.at(table).posBox[0].R(), filledBoxes.at(table).posBox[pos].T(), black);
		}
		DrawLine(screen.CX() + 10, filledBoxes.at(table).posBox[POS_NUM - 1].B(),
			filledBoxes.at(table).posBox[0].R(), filledBoxes.at(table).posBox[POS_NUM - 1].B(), black);
	}
	// finish prompt
	if (now.isGameFin) {
		DrawStringToHandle(screen.CX() + 120, teamBoxes.at(0).B() + space / 2,
			(ranker.at(0).name + " Win!").c_str(), white, Mfont);
	}
}

void cStandardCricket::update() {
	cBaseScene::update();
	cDarts::inst()->update();

	if (cDarts::inst()->isThrowed()) throwDart();
	else if (cControl::inst()->isRequested(cControl::SKIP)) skip();
	else if (cControl::inst()->isRequested(cControl::FORWARD)) forward();
	else if (cControl::inst()->isRequested(cControl::BACK)) back();
}

bool cStandardCricket::throwDart() {
	if (now.isTeamFin[now.team]) return false;
	now.arrow--;
	int point = cDarts::inst()->point();
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
	if (attempt <= 0) { cScene::inst()->setScene(cScene::GAME_START); return false; }
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
		cDarts::inst()->setPointValidation(POS_POINT[pos], !now.isPosFill[pos]);
		for (int table = 0; table < nTable; table++) {
			// Update whether draw fill box
			filledBoxes.at(table).posBox[pos].setDrawFlag(now.isPosFill[pos]);
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
		teamBoxes.at(team).setColor(tableColor); // If team is not playing, set team box table color
	}
}

bool cStandardCricket::record() {
	if (attempt >= MAX_ATTEMPT - 2) return false;
	attempt++; maxAttempt = attempt; mem[attempt] = now; return true;
}

bool cStandardCricket::addScore(int pos) {
	now.teamPosPower[now.team][pos] += cDarts::inst()->power();
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
	cDarts::inst()->setPointValidation(POS_POINT[pos], false); // disable point
	for (int table = 0; table < nTable; table++) 
		filledBoxes.at(table).posBox[pos].setDrawFlag(true); // set filled box to draw
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
		if (now.member > cPlayer::inst()->teamType()) {
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
			ranker.at(now.rank[team]) = cPlayer::inst()->teams().at(team);
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
		ranker.at(now.rank[team]) = cPlayer::inst()->teams().at(team);
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

cStandardCricket::~cStandardCricket() {
	cControl::inst()->iconBox(cControl::SKIP).setR(screen.R());
	if (nTeam > cPlayer::MAX_DUO_TEAM_NUM) {
		for (int team = 0; team < nTeam; team++) {
			cPlayer::inst()->teams().at(team).members.at(0).image.box().setH(100);
		}
	}
}