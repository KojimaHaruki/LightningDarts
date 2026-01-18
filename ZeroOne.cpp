#include "ZeroOne.hpp"
#include "Darts.hpp"
#include "Game.hpp"
#include "Timer.hpp"

ZeroOne::ZeroOne(ShareData shareData) : attempt(0), maxAttempt(0) {
	mNowScene = ZERO_ONE;
	sd = shareData;
	nTeam = sd.teams.size();
	arrowImage = cDarts::instance()->arrowImage();
	sd.ctrl.skip.icon.box.setLowerRight(screen.center().x() + 10, lowerFrame.top());
	attempt = 0;
	maxAttempt = 0;
	now = {};
	now.arrow = 3;
	for (int playerNo = 0; playerNo < nTeam; playerNo++) {
		now.teamRemain[playerNo] = INIT_POINT[mGameMode];
		now.rank[playerNo] = playerNo;
	}
	for (int point = 0; point < cDarts::sPoint::NUM; point++) 
		cDarts::instance()->setPointValidation(point, true);
	if (nTeam <= 4) {
		space = 4;
		nRound = 9;
		nScore = nRound + 1;
		for (int team = 0, x = screen.right() - 400, y = upperFrame.bottom() + space;
			team < nTeam; team++, x += 100, y = upperFrame.bottom() + space) {
			teamBox[team].setSize(
				100, 100 + cTeam::instance()->type() * 100 + nScore * (MfontSize + space));
			teamBox[team].setUpperLeft(x, y);
			for (int member = 0; member < sd.teams.at(team).members.size(); member++, y += 100) {
				sd.teams.at(team).members.at(member).image.box.setUpperLeft(x, y);
			}
		}
	}
	else {
		space = 2;
		nRound = 7;
		nScore = nRound + 1;
		for (int player = 0; player < nTeam; player++) {
			sd.teams.at(player).members.at(0).image.box.setSize(100, 70);
			teamBox[player].setSize(100,
				sd.teams.at(player).members.at(0).image.box.size().y() + nScore * (SfontSize + space));
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

void ZeroOne::reset() {
	cScene::reset();
	cTimer::instance()->restart();
	attempt = 0;
	maxAttempt = 0;
	now = {};
	now.arrow = 3;
	for (int playerNo = 0; playerNo < nTeam; playerNo++) {
		now.teamRemain[playerNo] = INIT_POINT[mGameMode];
		now.rank[playerNo] = playerNo;
	}
}

void ZeroOne::draw() {
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
	int recordNo = 0;
	sChara chara;
	if (nTeam <= 4) {
		DrawBox(screen.center().x() + 10, teamBox[0].top(),
			teamBox[nTeam - 1].right(), teamBox[0].bottom(), tableColor, TRUE);
		DrawBox(teamBox[now.team].left(), teamBox[now.team].top(),
			teamBox[now.team].right(), teamBox[now.team].bottom(), white, TRUE);
		for (int team = 0; team < nTeam; team++) {
			for (int member = 0; member < sd.teams.at(team).members.size(); member++) {
				chara = sd.teams.at(team).members.at(member);
				drawImage(chara.image);
				DrawStringToHandle(chara.image.box.left() + 5 * max(0, 10 - chara.name.size()),
					chara.image.box.bottom() - SfontSize - 6, chara.name.c_str(), white, Sfont);
			}
			DrawStringToHandle(teamBox[team].left(), teamBox[team].top(),
				rankName[now.rank[team]].c_str(), white, Mfont);
			if (now.round < nRound) {
				recordNo = 0;
			}
			else {
				recordNo = now.round - nRound + 1;
			}
			for (int i = 0; recordNo <= now.round; i++, recordNo++) {
				if (recordNo == now.round && team > now.team) {
					break;
				}
				if (now.teamRoundScore[team][recordNo] == -1) {
					DrawStringToHandle(chara.image.box.center().x() - 25,
						chara.image.box.bottom() + space / 2 + i * (MfontSize + space),
						"BUST", white, Mfont);
					continue;
				}
				DrawFormatStringToHandle(chara.image.box.center().x() - 25,
					chara.image.box.bottom() + space / 2 + i * (MfontSize + space),
					white, Mfont, "%4d", now.teamRoundScore[team][recordNo]);
			}
			DrawFormatStringToHandle(chara.image.box.center().x() - 25,
				chara.image.box.bottom() + space / 2 + nRound * (MfontSize + space),
				white, Mfont, "%4d", now.teamRemain[team]);
			DrawLine(teamBox[team].left(), teamBox[team].top(),
				teamBox[team].left(), teamBox[team].bottom(), black);
		}
		int y = sd.teams.at(0).members.at(cTeam::instance()->type()).image.box.bottom();
		if (now.round < nRound) {
			recordNo = 0;
		}
		else {
			recordNo = now.round - nRound + 1;
		}
		for (int i = 0; recordNo <= now.round; i++, recordNo++) {
			DrawFormatStringToHandle(screen.center().x() + 16,
				y + space / 2 + i * (MfontSize + space),
				white, Mfont, "%2d", recordNo + 1);
		}
		for (int recordNo = 0, posY = 0; recordNo < nScore; recordNo++) {
			posY = y + recordNo * (MfontSize + space);
			DrawLine(screen.center().x() + 10, posY, teamBox[nTeam - 1].right(), posY, black);
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
			chara = sd.teams.at(team).members.at(0);
			DrawRotaGraph(chara.image.box.center().x(), chara.image.box.center().y(),
				0.7, 0.0, chara.image.handle, TRUE);
			DrawStringToHandle(chara.image.box.left(), chara.image.box.top(),
				rankName[now.rank[team]].c_str(), white, Sfont);
			DrawStringToHandle(chara.image.box.left() + 5 * max(0, 10 - chara.name.size()),
				chara.image.box.bottom() - SfontSize - 6,
				chara.name.c_str(), white, Sfont);
			if (now.round < nRound) {
				recordNo = 0;
			}
			else {
				recordNo = now.round - nRound + 1;
			}
			for (int i = 0; recordNo <= now.round; i++, recordNo++) {
				if (recordNo == now.round && team > now.team) {
					break;
				}
				if (now.teamRoundScore[team][recordNo] == -1) {
					DrawStringToHandle(chara.image.box.center().x() - 20,
						chara.image.box.bottom() + space / 2 + i * (SfontSize + space),
						"BUST", white, Sfont);
					continue;
				}
				DrawFormatStringToHandle(chara.image.box.center().x() - 20,
					chara.image.box.bottom() + space / 2 + i * (SfontSize + space),
					white, Sfont, "%4d", now.teamRoundScore[team][recordNo]);
			}
			DrawFormatStringToHandle(chara.image.box.center().x() - 20,
				chara.image.box.bottom() + space / 2 + nRound * (SfontSize + space),
				white, Sfont, "%4d", now.teamRemain[team]);
			DrawLine(teamBox[team].left(), teamBox[team].top(),
				teamBox[team].left(), teamBox[team].bottom(), black);
		}
		for (int pointNo = 0, y = 0; pointNo < nScore; pointNo++) {
			y = sd.teams.at(0).members.at(0).image.box.bottom() + pointNo * (SfontSize + space);
			DrawLine(screen.center().x() + 10, y, teamBox[3].right(), y, black);
			y = sd.teams.at(4).members.at(0).image.box.bottom() + pointNo * (SfontSize + space);
			DrawLine(screen.center().x() + 10, y, teamBox[nTeam - 1].right(), y, black);
		}
		for (int i = 0; i < 2; i++) {
			chara = sd.teams.at(4 * i).members.at(0);
			if (now.round < nRound) {
				recordNo = 0;
			}
			else {
				recordNo = now.round - nRound + 1;
			}
			for (int j = 0; recordNo <= now.round; j++, recordNo++) {
				DrawFormatStringToHandle(screen.center().x() + 12,
					chara.image.box.bottom() + space / 2 + j * (SfontSize + space),
					white, Sfont, "%2d", recordNo + 1);
			}
		}
	}
	chara = sd.teams.at(now.team).members.at(now.member);
	for (int arrow = 0, x = chara.image.box.right(), y = chara.image.box.top(); arrow < now.arrow; arrow++)
		DrawGraph(x - 10 * (arrow + 1), y, arrowImage, TRUE);
	DrawBox(screen.center().x() + 10, teamBox[0].bottom(),
		screen.right(), teamBox[0].bottom() + 2 * space + MfontSize, pressColor, TRUE);
	DrawStringToHandle(screen.center().x() + 120, teamBox[0].bottom() + space,
		(chara.name + ", throw darts!").c_str(), white, Mfont);
}

void ZeroOne::update() {
	cScene::update();
	cDarts::instance()->update();
	cTimer::instance()->update();
	bool isPaused = cTimer::instance()->isPaused();
	if (attempt < maxAttempt) {
		drawImage(sd.ctrl.forward.icon);
		if (ctrlRQ(sd.ctrl.forward)) {
			attempt++;
			now = record[attempt];
			return;
		}
	}
	int totalPoint = cDarts::instance()->totalPoint();
	if (cDarts::instance()->isThrowed()) {
		if (attempt < MAX_ATTEMPT - 1) {
			attempt++;
			maxAttempt = attempt;
		}
		if (totalPoint <= now.teamRemain[now.team]) { // if bill isn't busted
			now.teamRemain[now.team] -= totalPoint;
			now.teamRoundScore[now.team][now.round] += totalPoint;
			now.arrow--;
		}
		else { // if bill is busted
			now.teamRemain[now.team] += now.teamRoundScore[now.team][now.round];
			now.teamRoundScore[now.team][now.round] = -1;
			now.arrow = 0;
		}
		if (!now.teamRemain[now.team]) {
			now.isTeamFin[now.team] = true;
			now.arrow = 0;
			now.rank[now.team] = now.finRank;
			now.finRank++;
			if (now.finRank == nTeam) {
				now.isGameFin = true;
			}
		}
		for (int player = 0; player < nTeam; player++) {
			if (!now.isTeamFin[player]) {
				now.rank[player] = 0;
				for (int opponent = (player + 1) % nTeam; opponent != player;
					opponent = (opponent + 1) % nTeam) {
					if (now.teamRemain[player] > now.teamRemain[opponent] || now.isTeamFin[opponent] ||
						(now.teamRemain[player] == now.teamRemain[opponent] && player > opponent)) {
						now.rank[player]++;
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
					now.member++;
					now.round++;
					if (now.round >= MAX_ROUND) {
						now.round = MAX_ROUND - 1;
						for (int playerNo = 0; playerNo < nTeam; playerNo++) {
							for (int recordNo = 0; recordNo < MAX_ROUND - 1; recordNo++) {
								now.teamRoundScore[playerNo][recordNo] = now.teamRoundScore[playerNo][recordNo + 1];
							}
							now.teamRoundScore[playerNo][MAX_ROUND - 1] = 0;
						}
					}
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
		}
		else {
			mNextScene = GAME_START;
		}
	}
	else if (ctrlRQ(sd.ctrl.playerSelect)) mNextScene = PLAYER_SELECT;
	else if (ctrlRQ(sd.ctrl.gameSelect)) mNextScene = GAME_SELECT;
	else if (ctrlRQ(sd.ctrl.home)) mNextScene = HOME;
	else if (ctrlRQ(sd.ctrl.config)) {
		mNextScene = CONFIG;
		cTimer::instance()->pause();
	}
	else if (!isPaused && ctrlRQ(sd.ctrl.pause[FALSE])) cTimer::instance()->pause();
	else if (isPaused && ctrlRQ(sd.ctrl.pause[TRUE])) cTimer::instance()->resume();
}

void ZeroOne::fin() {
	sd.ctrl.skip.icon.box.setLowerRight(screen.right(), lowerFrame.top());
	if (nTeam > 4) {
		for (int player = 0; player < nTeam; player++) {
			sd.teams.at(player).members.at(0).image.box.setSize(100, 100);
		}
	}
}