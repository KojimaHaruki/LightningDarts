#include "CountUp.hpp"
#include "Darts.hpp"
#include "Game.hpp"
#include "Control.hpp"
#include "Scene.hpp"

cCountUp::cCountUp() : attempt(0), maxAttempt(0) {
	now = {};
	now.arrow = 3;
	for (int point = 0; point < cDarts::sPoint::NUM; point++) 
		cDarts::inst()->setPointValidation(point, true);
	if (nTeam <= cPlayer::MAX_DUO_TEAM_NUM) {
		space = 4;
		for (int team = 0, x = screen.R() - 400, y = upperFrame.B() + space;
			team < nTeam; team++, x += 100, y = upperFrame.B() + space) {
			teamBox[team].setS(
				100, 100 + cPlayer::inst()->teamType() * 100 + SCORE_NUM * (MfontSize + space));
			teamBox[team].setUL(x, y);
			for (int member = 0; member < cPlayer::inst()->nTeamMember(team); member++, y += 100) {
				cPlayer::inst()->teamMemberImageBox(team, member).setUL(x, y);
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
			cPlayer::inst()->teamMemberImageBox(team, 0).setS(100, 70);
			teamBox[team].setS(100,
				cPlayer::inst()->teamMemberImageBox(team, 0).S().y() + SCORE_NUM * (SfontSize + space));
			if (team == now.team) {
				teamBox[team].setColor(white); continue;
			}
			teamBox[team].setColor(tableColor);
		}
		for (int player = 0; player < 4; player++) {
			cPlayer::inst()->teamMemberImageBox(player, 0).setUL(
				screen.R() + 100 * (player - 4), upperFrame.B() + space);
			teamBox[player].setUL(cPlayer::inst()->teamMemberImageBox(player, 0).UL());
			if (player + 4 < nTeam) {
				cPlayer::inst()->teamMemberImageBox(player + 4, 0).setUL(
					teamBox[player].L(), teamBox[player].B() + MfontSize + space);
				teamBox[player + 4].setUL(cPlayer::inst()->teamMemberImageBox(player + 4, 0).UL());
			}
		}
	}
	record[attempt] = now;
}

void cCountUp::reset() {
	cBaseScene::reset();
	cDarts::inst()->reset();
	attempt = 0;
	maxAttempt = 0;
	now = {};
	now.arrow = 3;
	for (int point = 0; point < cDarts::sPoint::NUM; point++) 
		cDarts::inst()->setPointValidation(point, true);
}

void cCountUp::draw() {
	cBaseScene::draw();

	// draw icon
	if (attempt < maxAttempt) cControl::inst()->icon(cControl::FORWARD).draw();

	// draw darts board
	cDarts::inst()->draw();

	// draw round
	DrawFormatStringToHandle(screen.C().x() - 80, upperFrame.B() + 10,
		white, Mfont, "Round%3d", now.round + 1);

	// draw score table
	cPlayer::sChara chara;
	if (nTeam <= 4) {
		DrawBox(screen.C().x() + 10, teamBox[0].T(),
			teamBox[nTeam - 1].R(), teamBox[0].B(), tableColor, TRUE);
		teamBox[now.team].draw();
		for (int team = 0; team < nTeam; team++) {
			for (int member = 0; member < cPlayer::inst()->nTeamMember(team); member++) {
				chara = cPlayer::inst()->teamMember(team, member);
				chara.image.draw();
				DrawStringToHandle(chara.image.box().L() + 5 * max(0, 10 - (int)chara.name.size()),
					chara.image.box().B() - SfontSize - 6, chara.name.c_str(),
					white, Sfont);
			}
			DrawStringToHandle(teamBox[team].L(), teamBox[team].T(), rankName[now.rank[team]].c_str(),
				white, Mfont);
			for (int round = 0; round <= now.round; round++) {
				if (round == now.round && team > now.team) {
					break;
				}
				DrawFormatStringToHandle(chara.image.box().C().x() - 25,
					chara.image.box().B() + space / 2 + round * (MfontSize + space),
					white, Mfont, "%4d", now.teamRoundScore[team][round]);
			}
			DrawFormatStringToHandle(chara.image.box().C().x() - 25,
				chara.image.box().B() + space / 2 + ROUND_NUM * (MfontSize + space),
				white, Mfont, "%4d", now.teamScore[team]);
			DrawLine(teamBox[team].L(), teamBox[team].T(),
				teamBox[team].L(), teamBox[team].B(), black);
		}
		int y = cPlayer::inst()->teamMemberImageBox(0, cPlayer::inst()->teamType()).B();
		for (int round = 0; round < ROUND_NUM; round++) {
			DrawFormatStringToHandle(screen.C().x() + 16,
				y + space / 2 + round * (MfontSize + space),
				white, Mfont, "%2d", round + 1);
		}
		for (int recordNo = 0, posY = 0; recordNo < SCORE_NUM; recordNo++) {
			posY = y + recordNo * (MfontSize + space);
			DrawLine(screen.C().x() + 10, posY, teamBox[nTeam - 1].R(), posY, black);
		}
	}
	else {
		DrawBox(screen.C().x() + 10, teamBox[0].T(),
			teamBox[3].R(), teamBox[0].B(), tableColor, TRUE);
		DrawBox(screen.C().x() + 10, teamBox[4].T(),
			teamBox[nTeam - 1].R(), teamBox[4].B(), tableColor, TRUE);
		teamBox[now.team].draw();
		for (int team = 0; team < nTeam; team++) {
			chara = cPlayer::inst()->teamMember(team, 0);
			DrawRotaGraph(chara.image.box().C().x(), chara.image.box().C().y(),
				0.7, 0.0, chara.image.handle(), TRUE);
			DrawStringToHandle(chara.image.box().L(), chara.image.box().T(),
				rankName[now.rank[team]].c_str(), white, Sfont);
			DrawStringToHandle(chara.image.box().L() + 5 * max(0, 10 - (int)chara.name.size()),
				chara.image.box().B() - SfontSize - 6,
				chara.name.c_str(), white, Sfont);
			for (int round = 0; round <= now.round; round++) {
				if (round == now.round && team > now.team) {
					break;
				}
				DrawFormatStringToHandle(chara.image.box().C().x() - 20,
					chara.image.box().B() + space / 2 + round * (SfontSize + space),
					white, Sfont, "%4d", now.teamRoundScore[team][round]);
			}
			DrawFormatStringToHandle(chara.image.box().C().x() - 20,
				chara.image.box().B() + space / 2 + ROUND_NUM * (SfontSize + space),
				white, Sfont, "%4d", now.teamScore[team]);
			DrawLine(teamBox[team].L(), teamBox[team].T(),
				teamBox[team].L(), teamBox[team].B(), black);
		}
		for (int pointNo = 0, y = 0; pointNo < SCORE_NUM; pointNo++) {
			y = cPlayer::inst()->teamMemberImageBox(0, 0).B() + pointNo * (SfontSize + space);
			DrawLine(screen.C().x() + 10, y, teamBox[3].R(), y, black);
			y = cPlayer::inst()->teamMemberImageBox(4, 0).B() + pointNo * (SfontSize + space);
			DrawLine(screen.C().x() + 10, y, teamBox[nTeam - 1].R(), y, black);
		}
		for (int i = 0; i < 2; i++) {
			for (int j = 0, recordNo = 0; recordNo <= now.round; j++, recordNo++) {
				DrawFormatStringToHandle(screen.C().x() + 12,
					cPlayer::inst()->teamMemberImageBox(4 * i, 0).B() + space / 2 + j * (SfontSize + space),
					white, Sfont, "%2d", recordNo + 1);
			}
		}
	}
	chara = cPlayer::inst()->teamMember(now.team, now.member);
	for (int arrow = 0, x = chara.image.box().R(), y = chara.image.box().T();
		arrow < now.arrow; arrow++)
		DrawGraph(x - 10 * (arrow + 1), y, cDarts::inst()->arrowImage(), TRUE);
	DrawBox(screen.C().x() + 10, teamBox[0].B(),
		screen.R(), teamBox[0].B() + 2 * space + MfontSize, pressColor, TRUE);
	DrawStringToHandle(screen.C().x() + 120, teamBox[0].B() + space,
		(chara.name + ", throw darts!").c_str(), white, Mfont);
}

void cCountUp::update() {
	cBaseScene::update();
	cDarts::inst()->update();
	int point = cDarts::inst()->point(), totalPoint = cDarts::inst()->totalPoint();
	if (attempt < maxAttempt) {
		cControl::inst()->icon(cControl::FORWARD).draw();
		if (cControl::inst()->isRequested(cControl::FORWARD)) {
			attempt++;
			now = record[attempt];
		}
	}
	if (!now.isGameFin) {
		if (cDarts::inst()->isThrowed()) {
			if (attempt < MAX_ATTEMPT - 1) {
				attempt++;
				maxAttempt = attempt;
			}
			if (cDarts::inst()->isValidPoint(0) && point == 25) {
				now.teamScore[now.team] += totalPoint;
				now.teamRoundScore[now.team][now.round] += totalPoint;
			}
			else if (cDarts::inst()->isValidPoint(point)) {
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
		if (cControl::inst()->isRequested(cControl::SKIP)) {
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
				}
			}
			if (now.member >= cPlayer::inst()->nTeamMember(now.team)) {
				now.member = 0;
			}
			teamBox[now.team].setColor(white);
			record[attempt] = now;
			return;
		}
	}
	if (cControl::inst()->isRequested(cControl::BACK)) {
		if (attempt > 0) {
			attempt--;
			now = record[attempt];
			return;
		}
		cScene::inst()->setScene(cScene::GAME_START);
	}
}

cCountUp::~cCountUp() {
	if (nTeam > 4) {
		for (int player = 0; player < nTeam; player++) {
			cPlayer::inst()->teamMember(player, 0).image.box().setH(100);
		}
	}
}