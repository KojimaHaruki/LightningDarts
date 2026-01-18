#include "HiddenCricket.hpp"
#include <algorithm>
#include <random>
#include "Darts.hpp"
#include "Game.hpp"
#include "Timer.hpp"

HiddenCricket::HiddenCricket(ShareData shareData) : attempt(0), maxAttempt(0), selectPos(POS_NUM) {
	sd = shareData;
	nTeam = sd.teams.size();
	mNowScene = STANDARD_CRICKET;
	markPart[0].color = white;
	markPart[1].color = black;
	markPart[0].lineWidth = 3;
	markPart[1].lineWidth = 3;
	markPart[2].lineWidth = 2;
	reset();
	sChara chara;
	if (nTeam <= 4) {
		space = 8;
		for (int i = 0; i < 3; i++) {
			markPart[i].box.setSize(20, 20);
		}
		for (int team = 0, x = screen.right() - 400, y = upperFrame.bottom() + space;
			team < nTeam; team++, x += 100, y = upperFrame.bottom() + space) {
			teamBox[team].setSize(
				100, 100 + cTeam::instance()->type() * 100 + POINT_NUM * (SfontSize + space));
			teamBox[team].setUpperLeft(x, y);
			for (int member = 0; member < sd.teams.at(team).members.size(); member++, y += 100) {
				sd.teams.at(team).members[member].image.box.setUpperLeft(x, y);
			}
		}
	}
	else {
		space = 2;
		for (int player = 0; player < nTeam; player++) {
			sd.teams.at(player).members.at(0).image.box.setSize(100, 70);
			teamBox[player].setSize(100,
				sd.teams.at(player).members.at(0).image.box.size().y() + POINT_NUM * (SfontSize + space));
		}
		for (int player = 0; player < 4; player++) {
			sd.teams.at(player).members.at(0).image.box.setUpperLeft(
				screen.right() + 100 * (player - 4), upperFrame.bottom() + space);
			teamBox[player].setUpperLeft(sd.teams.at(player).members.at(0).image.box.upperLeft());
			if (player + 4 < nTeam) {
				sd.teams.at(player + 4).members.at(0).image.box.setUpperLeft(
					teamBox[player].left(), teamBox[player].bottom() + MfontSize + space);
				teamBox[player + 4].setUpperLeft(sd.teams.at(player + 4).members.at(0).image.box.upperLeft());
			}
		}
	}
	record[attempt] = now;
}

void HiddenCricket::reset() {
	cScene::reset();
	cTimer::instance()->restart();
	attempt = 0;
	maxAttempt = 0;
	now = {};
	now.arrow = 3;
	for (int team = 0; team < nTeam; team++) now.rank[team] = team;
	std::vector<int> randomPoint;
	for (int point = 1; point <= 20; point++) randomPoint.push_back(point);
	std::shuffle(randomPoint.begin(), randomPoint.end(), std::mt19937{ std::random_device{}() });
	switch (mGameMode) {
	case cGame::sMode::HIDDEN_CRICKET:
		for (int pos = 0, point = randomPoint.at(0); pos < POS_NUM - 1; pos++, point = randomPoint.at(pos)) {
			now.posScore[pos] = -point - 1;
		}
		now.posScore[POS_NUM - 1] = -25;
		for (int point = 0; point <= 20; point++) {
			cDarts::instance()->setPointValidation(point, true);
		}
		break;
	case cGame::sMode::RANDOM_CRICKET:
		now.posScore[POS_NUM - 1] = 25;
		cDarts::instance()->setPointValidation(0, true); // BULL is valid
		for (int pos = 0, point = randomPoint.at(0); pos < POS_NUM - 1; pos++, point = randomPoint.at(pos)) {
			now.posScore[pos] = point;
			cDarts::instance()->setPointValidation(point, true);
		}
		break;
	case cGame::sMode::SELECT_A_CRICKET:
		selectPos = 0;
		for (int point = 0; point <= 20; point++) {
			cDarts::instance()->setPointValidation(point, false);
		}
		now.arrow = 1;
		break;
	default:
		cDarts::instance()->setPointValidation(0, true); // BULL is valid
		for (int point = 15; point <= 20; point++) {
			cDarts::instance()->setPointValidation(point, true);
		}
		break;
	}
}

void HiddenCricket::draw() {
	cScene::draw();

	// draw icon
	if (cTimer::instance()->isPaused()) drawImage(sd.ctrl.pause[TRUE].icon);
	else drawImage(sd.ctrl.pause[FALSE].icon);
	if (attempt < maxAttempt) drawImage(sd.ctrl.forward.icon);
	drawImage(sd.ctrl.gameSelect.icon); drawImage(sd.ctrl.playerSelect.icon);
	drawImage(sd.ctrl.skill.icon); drawImage(sd.ctrl.skip.icon);

	// draw time
	cTimer::instance()->drawLapseTime(
		screen.left(), upperFrame.bottom() + 10, white, Sfont, cTimer::Mode::HMSmS);

	// draw game name
	DrawStringToHandle(sd.ctrl.mute[0].icon.box.right() + 5, upperFrame.center().y() - MfontSize / 2,
		cGame::instance()->modeName().c_str(), white, Mfont);

	// draw darts board
	cDarts::instance()->draw();

	// draw round
	DrawFormatStringToHandle(screen.center().x() - 80, upperFrame.bottom() + 10,
		white, Mfont, "Round%3d", now.round + 1);

	// draw score table
	sChara chara;
	if (nTeam <= 4) {
		DrawBox(screen.center().x() + 10, teamBox[0].top(),
			teamBox[nTeam - 1].right(), teamBox[0].bottom(), tableColor, TRUE);
		DrawBox(teamBox[now.team].left(), teamBox[now.team].top(),
			teamBox[now.team].right(), teamBox[now.team].bottom(), white, TRUE);
		for (int team = 0; team < nTeam; team++) {
			markPart[2].color = teamColor[team];
			for (int member = 0; member < sd.teams.at(team).members.size(); member++) {
				chara = sd.teams.at(team).members[member];
				drawImage(chara.image);
				DrawStringToHandle(chara.image.box.left() + 5 * max(0, 10 - chara.name.size()),
					chara.image.box.bottom() - SfontSize - 6, chara.name.c_str(),
					white, Sfont);
			}
			DrawStringToHandle(teamBox[team].left(), teamBox[team].top(), rankName[now.rank[team]].c_str(),
				white, Mfont);
			for (int pos = 0; pos < POINT_NUM - 1; pos++) {
				markPart[2].box.setCenter(chara.image.box.center().x(),
					chara.image.box.bottom() + (2 * pos + 1) * (SfontSize + space) / 2);
				markPart[0].box.setCenter(markPart[2].box.center().x() - 1, markPart[2].box.center().y());
				markPart[1].box.setCenter(markPart[2].box.center().x() + 1, markPart[2].box.center().y());
				for (int i = 0; i < 3; i++) {
					switch (now.teamPosScore[team][pos]) {
					case 3:
						DrawCircleAA(markPart[i].box.center().x(), markPart[i].box.center().y(),
							(MfontSize) / 2.0 - 2.0, 100, markPart[i].color, FALSE, markPart[i].lineWidth);
					case 2:
						DrawLine(markPart[i].box.upperRight().x(), markPart[i].box.upperRight().y(),
							markPart[i].box.lowerLeft().x(), markPart[i].box.lowerLeft().y(),
							markPart[i].color, markPart[i].lineWidth);
					case 1:
						DrawLine(markPart[i].box.upperLeft().x(), markPart[i].box.upperLeft().y(),
							markPart[i].box.lowerRight().x(), markPart[i].box.lowerRight().y(),
							markPart[i].color, markPart[i].lineWidth);
					default:
						break;
					}
				}
			}
			DrawFormatStringToHandle(chara.image.box.center().x() - 20,
				chara.image.box.bottom() + 7 * SfontSize + 15 * space / 2,
				white, Mfont, "%4d", now.teamBill[team]);
			DrawLine(teamBox[team].left(), teamBox[team].top(),
				teamBox[team].left(), teamBox[team].bottom(), black);
		}
		int y = sd.teams.at(0).members.at(cTeam::instance()->type()).image.box.bottom();
		for (int pos = 0; pos < selectPos; pos++) {
			DrawStringToHandle(screen.center().x() + 12,
				y + pos * SfontSize + (2 * pos + 1) * space / 2,
				cDarts::instance()->pointName(now.posScore[pos]).c_str(), white, Sfont);
		}
		for (int pointPos = 0, posY = 0; pointPos < POINT_NUM; pointPos++) {
			posY = y + pointPos * (SfontSize + space);
			DrawLine(screen.center().x() + 10, posY, teamBox[nTeam - 1].right(), posY, black);
		}
		if (nTeam <= 2) {
			DrawStringToHandle(screen.center().x() + 18, y + 7 * SfontSize + 15 * space / 2,
				"Pt", white, Sfont);
		}
		else {
			DrawStringToHandle(screen.center().x() + 12, y + 7 * SfontSize + 15 * space / 2,
				"Bill", white, Sfont);
		}
	}
	else {
		DrawBox(screen.center().x() + 10, teamBox[0].top(),
			teamBox[3].right(), teamBox[0].bottom(), tableColor, TRUE);
		DrawBox(screen.center().x() + 10, teamBox[4].top(),
			teamBox[nTeam - 1].right(), teamBox[4].bottom(), tableColor, TRUE);
		DrawBox(teamBox[now.team].left(), teamBox[now.team].top(),
			teamBox[now.team].right(), teamBox[now.team].bottom(), white, TRUE);
		for (int team = 0; team < nTeam; team++) {
			markPart[2].color = teamColor[team];
			chara = sd.teams.at(team).members.at(0);
			DrawRotaGraph(chara.image.box.center().x(), chara.image.box.center().y(),
				0.7, 0.0, chara.image.handle, TRUE);
			DrawStringToHandle(chara.image.box.left(), chara.image.box.top(),
				rankName[now.rank[team]].c_str(), white, Sfont);
			DrawStringToHandle(chara.image.box.left() + 5 * max(0, 10 - chara.name.size()),
				chara.image.box.bottom() - SfontSize - 6,
				chara.name.c_str(), white, Sfont);
			for (int posNo = 0; posNo < POINT_NUM - 1; posNo++) {
				markPart[2].box.setCenter(chara.image.box.center().x(),
					chara.image.box.bottom() + (2 * posNo + 1) * (SfontSize + space) / 2);
				markPart[0].box.setCenter(markPart[2].box.center().x() - 1, markPart[2].box.center().y());
				markPart[1].box.setCenter(markPart[2].box.center().x() + 1, markPart[2].box.center().y());
				for (int i = 0; i < 3; i++) {
					switch (now.teamPosScore[team][posNo]) {
					case 3:
						DrawCircleAA(markPart[i].box.center().x(), markPart[i].box.center().y(),
							(SfontSize) / 2.0 - 2.0, 100, markPart[i].color, FALSE, markPart[i].lineWidth);
					case 2:
						DrawLine(markPart[i].box.upperRight().x(), markPart[i].box.upperRight().y(),
							markPart[i].box.lowerLeft().x(), markPart[i].box.lowerLeft().y(),
							markPart[i].color, markPart[i].lineWidth);
					case 1:
						DrawLine(markPart[i].box.upperLeft().x(), markPart[i].box.upperLeft().y(),
							markPart[i].box.lowerRight().x(), markPart[i].box.lowerRight().y(),
							markPart[i].color, markPart[i].lineWidth);
					default:
						break;
					}
				}
			}
			DrawFormatStringToHandle(chara.image.box.center().x() - 20,
				chara.image.box.bottom() + 7 * SfontSize + 15 * space / 2,
				white, Sfont, "%4d", now.teamBill[team]);
			DrawLine(teamBox[team].left(), teamBox[team].top(),
				teamBox[team].left(), teamBox[team].bottom(), black);
		}
		for (int pointPos = 0, y = 0; pointPos < POINT_NUM; pointPos++) {
			y = sd.teams.at(0).members.at(0).image.box.bottom() + pointPos * (SfontSize + space);
			DrawLine(screen.center().x() + 10, y, teamBox[3].right(), y, black);
			y = sd.teams.at(4).members.at(0).image.box.bottom() + pointPos * (SfontSize + space);
			DrawLine(screen.center().x() + 10, y, teamBox[nTeam - 1].right(), y, black);
		}
		for (int i = 0; i < 2; i++) {
			chara = sd.teams.at(4 * i).members.at(0);
			for (int pos = 0; pos < selectPos; pos++) {
				DrawStringToHandle(screen.center().x() + 12,
					chara.image.box.bottom() + pos * SfontSize + (2 * pos + 1) * space / 2,
					cDarts::instance()->pointName(now.posScore[pos]).c_str(), white, Sfont);
			}
			DrawStringToHandle(screen.center().x() + 12,
				chara.image.box.bottom() + 7 * SfontSize + 15 * space / 2,
				"Bill", white, Sfont);
		}
	}
	DrawBox(screen.center().x() + 10, teamBox[0].bottom(),
		screen.right(), teamBox[0].bottom() + 2 * space + MfontSize, pressColor, TRUE);
	chara = sd.teams.at(now.team).members.at(now.member);
	for (int arrow = 0, x = chara.image.box.right(), y = chara.image.box.top();
		arrow < now.arrow; arrow++)
		DrawGraph(x - 10 * (arrow + 1), y, cDarts::instance()->arrowImage(), TRUE);
	if (selectPos < POS_NUM) {
		DrawStringToHandle(screen.center().x() + 120, teamBox[0].bottom() + space,
			(sd.teams.at(now.team).members.at(now.member).name + ", throw a dart!").c_str(),
			white, Mfont);
		return;
	}
	DrawStringToHandle(screen.center().x() + 120, teamBox[0].bottom() + space,
		(chara.name + ", throw darts!").c_str(), white, Mfont);
}

void HiddenCricket::update() {
	cScene::update();
	cDarts::instance()->update();
	cTimer::instance()->update();
	bool isPaused = cTimer::instance()->isPaused();
	int point = cDarts::instance()->point(), power = cDarts::instance()->power(),
		totalPoint = cDarts::instance()->totalPoint();
	if (selectPos < POS_NUM) { // select-a-clicket
		if (now.posScore[selectPos] > 0) {
			drawImage(sd.ctrl.forward.icon);
			if (ctrlRQ(sd.ctrl.forward)) {
				if (now.posScore[selectPos] == 25) {
					cDarts::instance()->setPointValidation(0, true);
				}
				else {
					cDarts::instance()->setPointValidation(now.posScore[selectPos], true);
				}
				selectPos++;
				return;
			}
		}
		if (cDarts::instance()->isThrowed()) {
			now.arrow--;
			if (point > 0) {
				if (!cDarts::instance()->isValidPoint(point)) {
					now.posScore[selectPos] = point;
					cDarts::instance()->setPointValidation(point, true);
					selectPos++;
				}
			}
			if (!now.arrow) {
				now.team++;
				now.arrow = 1;
			}
			if (now.team >= nTeam) {
				now.team = 0;
				now.member++;
			}
			if (now.member >= sd.teams.at(now.team).members.size()) {
				now.member = 0;
			}
			if (selectPos == POS_NUM) {
				now.team = 0;
				now.member = 0;
				now.arrow = 3;
				record[attempt] = now;
			}
			return;
		}
	}
	else if (attempt < maxAttempt) {
		drawImage(sd.ctrl.forward.icon);
		if (ctrlRQ(sd.ctrl.forward)) {
			attempt++;
			now = record[attempt];
			return;
		}
	}
	if (cDarts::instance()->isThrowed()) {
		if (attempt < MAX_ATTEMPT - 1) {
			attempt++;
			maxAttempt = attempt;
		}
		now.arrow--;
		int cricketPos = -1;
		for (int posNo = 0; posNo < POS_NUM; posNo++) {
			if (point == abs(now.posScore[posNo])) {
				cricketPos = posNo;
				break;
			}
		}
		if (cricketPos >= 0 && cricketPos < POS_NUM) {
			if (now.posScore[cricketPos] < 0) { // hidden cricket
				now.posScore[cricketPos] = -now.posScore[cricketPos];
				power++;
			}
			now.teamPosScore[now.team][cricketPos] += power;
			if (now.teamPosScore[now.team][cricketPos] > 3) {
				int damage = POS_POINT[cricketPos] * (now.teamPosScore[now.team][cricketPos] - 3);
				now.teamPosScore[now.team][cricketPos] = 3;
				if (nTeam <= 2) {
					int opponent = (now.team + 1) % nTeam;
					if (now.teamPosScore[opponent][cricketPos] < 3) {
						now.teamBill[now.team] += damage;
					}
					if (now.teamBill[now.team] > now.teamBill[opponent]) {
						now.rank[now.team] = 0;
						now.rank[opponent] = 1;
					}
				}
				else {
					for (int opponent = (now.team + 1) % nTeam; opponent != now.team;
						opponent = (opponent + 1) % nTeam) {
						if (now.teamPosScore[opponent][cricketPos] < 3) {
							now.teamBill[opponent] += damage;
						}
					}
				}
			}
			now.isTeamFin[now.team] = true;
			for (int posNo = 0; posNo < POS_NUM; posNo++) {
				if (now.teamPosScore[now.team][posNo] < 3) {
					now.isTeamFin[now.team] = false;
					break;
				}
			}
			if (nTeam <= 2) {
				for (int opponent = (now.team + 1) % nTeam; opponent != now.team;
					opponent = (opponent + 1) % nTeam) {
					if (!now.isTeamFin[opponent] && now.teamBill[now.team] < now.teamBill[opponent]) {
						now.isTeamFin[now.team] = false;
					}
				}
			}
			else {
				for (int opponent = (now.team + 1) % nTeam; opponent != now.team;
					opponent = (opponent + 1) % nTeam) {
					if (!now.isTeamFin[opponent] && now.teamBill[now.team] > now.teamBill[opponent]) {
						now.isTeamFin[now.team] = false;
					}
				}
			}
		}
		if (now.isTeamFin[now.team]) {
			now.arrow = 0;
			now.rank[now.team] = now.finRank;
			now.finRank++;
			if (now.finRank == nTeam) {
				now.isGameFin = true;
			}
		}
		if (nTeam > 2) {
			for (int team = 0; team < nTeam; team++) {
				if (!now.isTeamFin[team]) {
					now.rank[team] = 0;
					for (int opponent = (team + 1) % nTeam; opponent != team;
						opponent = (opponent + 1) % nTeam) {
						if (now.teamBill[team] > now.teamBill[opponent] || now.isTeamFin[opponent] ||
							(now.teamBill[team] == now.teamBill[opponent] && team > opponent)) {
							now.rank[team]++;
						}
					}
				}
			}
		}
		record[attempt] = now;
	}
	if (!now.isGameFin) {
		if (ctrlRQ(sd.ctrl.skip)) {
			if (attempt < MAX_ATTEMPT - 1) {
				attempt++;
				maxAttempt = attempt;
			}
			now.arrow = 0;
		}
		if (now.arrow < 1) {
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
			record[attempt] = now;
			return;
		}
	}
	if (ctrlRQ(sd.ctrl.back)) {
		if (attempt > 0) {
			attempt--;
			now = record[attempt];
			return;
		}
		if (mGameMode == cGame::sMode::SELECT_A_CRICKET && selectPos > 0) {
			now.arrow = 1;
			selectPos--;
			if (now.posScore[selectPos] == 25) {
				cDarts::instance()->setPointValidation(0, false);
				return;
			}
			cDarts::instance()->setPointValidation(now.posScore[selectPos], false);
			return;
		}
		mNextScene = GAME_START;
	}
	else if (ctrlRQ(sd.ctrl.playerSelect)) mNextScene = PLAYER_SELECT;
	else if (ctrlRQ(sd.ctrl.gameSelect)) mNextScene = GAME_SELECT;
	else if (ctrlRQ(sd.ctrl.home)) mNextScene = HOME;
	else if (ctrlRQ(sd.ctrl.config)) {
		mNextScene = CONFIG;
		cTimer::instance()->stop();
	}
	else if (!isPaused && ctrlRQ(sd.ctrl.pause[FALSE])) cTimer::instance()->stop();
	else if (isPaused && ctrlRQ(sd.ctrl.pause[TRUE])) cTimer::instance()->resume();
}

void HiddenCricket::fin() {
	sd.ctrl.skip.icon.box.setLowerRight(screen.right(), lowerFrame.top());
	if (nTeam > 4) {
		for (int player = 0; player < nTeam; player++) {
			sd.teams.at(player).members.at(0).image.box.setSize(100, 100);
		}
	}
}