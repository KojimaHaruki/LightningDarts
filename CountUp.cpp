#include "CountUp.hpp"
#include "Darts.hpp"
#include "Game.hpp"
#include "Timer.hpp"
#include "Control.hpp"

cCountUp::cCountUp() : attempt(0), maxAttempt(0) {
	mNowScene = COUNT_UP;
	nTeam = cPlayer::instance()->nTeam();
	attempt = 0;
	maxAttempt = 0;
	now = {};
	now.arrow = 3;
	switch (cGame::instance()->mode()) {
	case cGame::sMode::CRICKET_COUNT_UP:
		for (int point = 0; point < cDarts::sPoint::NUM; point++) {
			cDarts::instance()->setPointValidation(point, false);
		}
		cDarts::instance()->setPointValidation(20, true); // 20 is initially valid in Cricket Countup
		break;
	default:
		for (int point = 0; point < cDarts::sPoint::NUM; point++) {
			cDarts::instance()->setPointValidation(point, true);
		}
		break;
	}
	if (nTeam <= cPlayer::MAX_DUO_TEAM_NUM) {
		space = 4;
		for (int team = 0, x = screen.right() - 400, y = upperFrame.bottom() + space;
			team < nTeam; team++, x += 100, y = upperFrame.bottom() + space) {
			teamBox[team].setSize(
				100, 100 + cPlayer::instance()->teamType() * 100 + SCORE_NUM * (MfontSize + space));
			teamBox[team].setUpperLeft(x, y);
			for (int member = 0; member < cPlayer::instance()->nTeamMember(team); member++, y += 100) {
				cPlayer::instance()->teamMemberImageBox(team, member).setUpperLeft(x, y);
			}
			if (team == now.team) {
				teamBox[team].setColor(white); continue;
			}
			teamBox[team].setColor(tableColor);
		}
	}
	else {
		space = 2;
		for (int team = 0; team < nTeam; team++) {
			cPlayer::instance()->teamMemberImageBox(team, 0).setSize(100, 70);
			teamBox[team].setSize(100,
				cPlayer::instance()->teamMemberImageBox(team, 0).size().y() + SCORE_NUM * (SfontSize + space));
			if (team == now.team) {
				teamBox[team].setColor(white); continue;
			}
			teamBox[team].setColor(tableColor);
		}
		for (int player = 0; player < 4; player++) {
			cPlayer::instance()->teamMemberImageBox(player, 0).setUpperLeft(
				screen.right() + 100 * (player - 4), upperFrame.bottom() + space);
			teamBox[player].setUpperLeft(cPlayer::instance()->teamMemberImageBox(player, 0).upperLeft());
			if (player + 4 < nTeam) {
				cPlayer::instance()->teamMemberImageBox(player + 4, 0).setUpperLeft(
					teamBox[player].left(), teamBox[player].bottom() + MfontSize + space);
				teamBox[player + 4].setUpperLeft(cPlayer::instance()->teamMemberImageBox(player + 4, 0).upperLeft());
			}
		}
	}
	record[attempt] = now;
}

void cCountUp::reset() {
	cBaseScene::reset();
	cTimer::instance()->restart();
	attempt = 0;
	maxAttempt = 0;
	now = {};
	now.arrow = 3;
	switch (cGame::instance()->mode()) {
	case cGame::sMode::CRICKET_COUNT_UP:
		for (int point = 0; point < cDarts::sPoint::NUM; point++) {
			cDarts::instance()->setPointValidation(point, false);
		}
		cDarts::instance()->setPointValidation(20, true); // 20 is initially valid in Cricket Countup
		return;
	default:
		for (int point = 0; point < cDarts::sPoint::NUM; point++) {
			cDarts::instance()->setPointValidation(point, true);
		}
		return;
	}
}

void cCountUp::draw() {
	cBaseScene::draw();

	// draw icon
	if (attempt < maxAttempt) cControl::instance()->icon(cControl::FORWARD).draw();

	// draw darts board
	cDarts::instance()->draw();

	// draw round
	DrawFormatStringToHandle(screen.center().x() - 80, upperFrame.bottom() + 10,
		white, Mfont, "Round%3d", now.round + 1);

	// draw score table
	cPlayer::sChara chara;
	if (nTeam <= 4) {
		DrawBox(screen.center().x() + 10, teamBox[0].top(),
			teamBox[nTeam - 1].right(), teamBox[0].bottom(), tableColor, TRUE);
		teamBox[now.team].draw();
		for (int team = 0; team < nTeam; team++) {
			for (int member = 0; member < cPlayer::instance()->nTeamMember(team); member++) {
				chara = cPlayer::instance()->teamMember(team, member);
				chara.image.draw();
				DrawStringToHandle(chara.image.box().left() + 5 * max(0, 10 - (int)chara.name.size()),
					chara.image.box().bottom() - SfontSize - 6, chara.name.c_str(),
					white, Sfont);
			}
			DrawStringToHandle(teamBox[team].left(), teamBox[team].top(), rankName[now.rank[team]].c_str(),
				white, Mfont);
			for (int round = 0; round <= now.round; round++) {
				if (round == now.round && team > now.team) {
					break;
				}
				DrawFormatStringToHandle(chara.image.box().center().x() - 25,
					chara.image.box().bottom() + space / 2 + round * (MfontSize + space),
					white, Mfont, "%4d", now.teamRoundScore[team][round]);
			}
			DrawFormatStringToHandle(chara.image.box().center().x() - 25,
				chara.image.box().bottom() + space / 2 + ROUND_NUM * (MfontSize + space),
				white, Mfont, "%4d", now.teamScore[team]);
			DrawLine(teamBox[team].left(), teamBox[team].top(),
				teamBox[team].left(), teamBox[team].bottom(), black);
		}
		int y = cPlayer::instance()->teamMemberImageBox(0, cPlayer::instance()->teamType()).bottom();
		for (int round = 0; round < ROUND_NUM; round++) {
			DrawFormatStringToHandle(screen.center().x() + 16,
				y + space / 2 + round * (MfontSize + space),
				white, Mfont, "%2d", round + 1);
		}
		for (int recordNo = 0, posY = 0; recordNo < SCORE_NUM; recordNo++) {
			posY = y + recordNo * (MfontSize + space);
			DrawLine(screen.center().x() + 10, posY, teamBox[nTeam - 1].right(), posY, black);
		}
	}
	else {
		DrawBox(screen.center().x() + 10, teamBox[0].top(),
			teamBox[3].right(), teamBox[0].bottom(), tableColor, TRUE);
		DrawBox(screen.center().x() + 10, teamBox[4].top(),
			teamBox[nTeam - 1].right(), teamBox[4].bottom(), tableColor, TRUE);
		teamBox[now.team].draw();
		for (int team = 0; team < nTeam; team++) {
			chara = cPlayer::instance()->teamMember(team, 0);
			DrawRotaGraph(chara.image.box().center().x(), chara.image.box().center().y(),
				0.7, 0.0, chara.image.handle(), TRUE);
			DrawStringToHandle(chara.image.box().left(), chara.image.box().top(),
				rankName[now.rank[team]].c_str(), white, Sfont);
			DrawStringToHandle(chara.image.box().left() + 5 * max(0, 10 - chara.name.size()),
				chara.image.box().bottom() - SfontSize - 6,
				chara.name.c_str(), white, Sfont);
			for (int round = 0; round <= now.round; round++) {
				if (round == now.round && team > now.team) {
					break;
				}
				DrawFormatStringToHandle(chara.image.box().center().x() - 20,
					chara.image.box().bottom() + space / 2 + round * (SfontSize + space),
					white, Sfont, "%4d", now.teamRoundScore[team][round]);
			}
			DrawFormatStringToHandle(chara.image.box().center().x() - 20,
				chara.image.box().bottom() + space / 2 + ROUND_NUM * (SfontSize + space),
				white, Sfont, "%4d", now.teamScore[team]);
			DrawLine(teamBox[team].left(), teamBox[team].top(),
				teamBox[team].left(), teamBox[team].bottom(), black);
		}
		for (int pointNo = 0, y = 0; pointNo < SCORE_NUM; pointNo++) {
			y = cPlayer::instance()->teamMemberImageBox(0, 0).bottom() + pointNo * (SfontSize + space);
			DrawLine(screen.center().x() + 10, y, teamBox[3].right(), y, black);
			y = cPlayer::instance()->teamMemberImageBox(4, 0).bottom() + pointNo * (SfontSize + space);
			DrawLine(screen.center().x() + 10, y, teamBox[nTeam - 1].right(), y, black);
		}
		for (int i = 0; i < 2; i++) {
			for (int j = 0, recordNo = 0; recordNo <= now.round; j++, recordNo++) {
				DrawFormatStringToHandle(screen.center().x() + 12,
					cPlayer::instance()->teamMemberImageBox(4 * i, 0).bottom() + space / 2 + j * (SfontSize + space),
					white, Sfont, "%2d", recordNo + 1);
			}
		}
	}
	chara = cPlayer::instance()->teamMember(now.team, now.member);
	for (int arrow = 0, x = chara.image.box().right(), y = chara.image.box().top();
		arrow < now.arrow; arrow++)
		DrawGraph(x - 10 * (arrow + 1), y, cDarts::instance()->arrowImage(), TRUE);
	DrawBox(screen.center().x() + 10, teamBox[0].bottom(),
		screen.right(), teamBox[0].bottom() + 2 * space + MfontSize, pressColor, TRUE);
	DrawStringToHandle(screen.center().x() + 120, teamBox[0].bottom() + space,
		(chara.name + ", throw darts!").c_str(), white, Mfont);
}

void cCountUp::update() {
	cBaseScene::update();
	cDarts::instance()->update();
	cTimer::instance()->update();
	int point = cDarts::instance()->point(), totalPoint = cDarts::instance()->totalPoint();
	if (attempt < maxAttempt) {
		cControl::instance()->icon(cControl::FORWARD).draw();
		if (cControl::instance()->isRequested(cControl::FORWARD)) {
			attempt++;
			now = record[attempt];
			if (cGame::instance()->mode() == cGame::sMode::CRICKET_COUNT_UP) {
				if (now.round < CRICKET_NUMBER_NUM) {
					for (int point = 0; point < CRICKET_NUMBER_NUM; point++) {
						cDarts::instance()->setPointValidation(CRICKET_NUMBER_SCORE[point], false);
					}
					cDarts::instance()->setPointValidation(CRICKET_NUMBER_SCORE[now.round], true);
					return;
				}
				for (int point = 0; point < CRICKET_NUMBER_NUM; point++) {
					cDarts::instance()->setPointValidation(CRICKET_NUMBER_SCORE[point], true);
				}
			}
		}
	}
	if (!now.isGameFin) {
		if (cDarts::instance()->isThrowed()) {
			if (attempt < MAX_ATTEMPT - 1) {
				attempt++;
				maxAttempt = attempt;
			}
			if (cDarts::instance()->isValidPoint(0) && point == 25) {
				now.teamScore[now.team] += totalPoint;
				now.teamRoundScore[now.team][now.round] += totalPoint;
			}
			else if (cDarts::instance()->isValidPoint(point)) {
				now.teamScore[now.team] += totalPoint;
				now.teamRoundScore[now.team][now.round] += totalPoint;
			}
			now.arrow--;
			for (int player = 0; player < nTeam; player++) {
				now.rank[player] = 0;
				for (int opponent = (player + 1) % nTeam; opponent != player;
					opponent = (opponent + 1) % nTeam) {
					if (now.teamScore[player] < now.teamScore[opponent]) {
						now.rank[player]++;
					}
				}
			}
			record[attempt] = now;
		}
		if (cControl::instance()->isRequested(cControl::SKIP)) {
			if (attempt < MAX_ATTEMPT - 1) {
				attempt++;
				maxAttempt = attempt;
			}
			now.arrow = 0;
		}
		if (now.arrow < 1) {
			teamBox[now.team].setColor(tableColor);
			now.team++;
			now.arrow = 3;
			if (now.team >= nTeam) {
				if (now.round == ROUND_NUM - 1) {
					now.team = nTeam - 1;
					now.isGameFin = true;
					now.arrow = 0;
				}
				else {
					now.team = 0;
					now.member++;
					now.round++;
					if (cGame::instance()->mode() == cGame::sMode::CRICKET_COUNT_UP) {
						if (now.round < CRICKET_NUMBER_NUM) {
							for (int point = 0; point < CRICKET_NUMBER_NUM; point++) {
								cDarts::instance()->setPointValidation(CRICKET_NUMBER_SCORE[point], false);
							}
							cDarts::instance()->setPointValidation(CRICKET_NUMBER_SCORE[now.round], true);
						}
						else {
							for (int point = 0; point < CRICKET_NUMBER_NUM; point++) {
								cDarts::instance()->setPointValidation(CRICKET_NUMBER_SCORE[point], true);
							}
						}
					}
				}
			}
			if (now.member >= cPlayer::instance()->nTeamMember(now.team)) {
				now.member = 0;
			}
			teamBox[now.team].setColor(white);
			record[attempt] = now;
			return;
		}
	}
	if (cControl::instance()->isRequested(cControl::BACK)) {
		if (attempt > 0) {
			attempt--;
			now = record[attempt];
			if (cGame::instance()->mode() == cGame::sMode::CRICKET_COUNT_UP) {
				if (now.team == nTeam - 1 && now.arrow == 1 && now.round < CRICKET_NUMBER_NUM) {
					for (int pointNo = 0; pointNo < CRICKET_NUMBER_NUM; pointNo++) {
						cDarts::instance()->setPointValidation(CRICKET_NUMBER_SCORE[pointNo], false);
					}
					cDarts::instance()->setPointValidation(CRICKET_NUMBER_SCORE[now.round], true);
				}
			}
		}
		else {
			mNextScene = GAME_START;
		}
	}
	else if (cControl::instance()->isRequested(cControl::PLAYER_SELECT)) mNextScene = PLAYER_SELECT;
	else if (cControl::instance()->isRequested(cControl::GAME_SELECT)) mNextScene = GAME_SELECT;
	else if (cControl::instance()->isRequested(cControl::HOME)) mNextScene = HOME;
	else if (cControl::instance()->isRequested(cControl::CONFIG)) {
		mNextScene = CONFIG;
		cTimer::instance()->pause();
	}
}

void cCountUp::fin() {
	if (nTeam > 4) {
		for (int player = 0; player < nTeam; player++) {
			cPlayer::instance()->teamMember(player, 0).image.box().setHeight(100);
		}
	}
}