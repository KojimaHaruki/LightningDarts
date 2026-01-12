#include "ZeroOne.hpp"
#include <string>
#include <numbers>
#include "Mouse.hpp"

ZeroOne::ZeroOne(ShareData shareData) : attempt(0), maxAttempt(0) {
	mNowScene = ZERO_ONE;
	sd = shareData;
	sd.ctrl.skip.icon.box.setLowerRight(sd.screen.center().x() + 10, sd.obj.lowerFrame.box.top());
	reset();
	for (int point = 0; point < 21; point++) {
		isValidPoint[point] = true;
	}
	if (sd.teams.size() <= 4) {
		space = 4;
		nRound = 9;
		nScore = nRound + 1;
		for (int team = 0, x = sd.screen.right() - 400, y = sd.obj.upperFrame.box.bottom() + space;
			team < sd.teams.size(); team++, x += 100, y = sd.obj.upperFrame.box.bottom() + space) {
			teamBox[team].setSize(100, 100 + sd.teamType * 100 + nScore * (sd.font.m.size + space));
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
		for (int player = 0; player < sd.teams.size(); player++) {
			sd.teams.at(player).members.at(0).image.box.setSize(100, 70);
			teamBox[player].setSize(100,
				sd.teams.at(player).members.at(0).image.box.size().y() + nScore * (sd.font.s.size + space));
		}
		for (int player = 0; player < 4; player++) {
			sd.teams.at(player).members.at(0).image.box.setUpperLeft(
				sd.screen.right() + 100 * (player - 4), sd.obj.upperFrame.box.bottom() + space);
			teamBox[player].setUpperLeft(sd.teams.at(player).members.at(0).image.box.upperLeft());
			if (player + 4 < sd.teams.size()) {
				sd.teams.at(player + 4).members.at(0).image.box.setUpperLeft(
					teamBox[player].left(), teamBox[player].bottom() + sd.font.m.size + space);
				teamBox[player + 4].setUpperLeft(sd.teams.at(player + 4).members.at(0).image.box.upperLeft());
			}
		}
	}
	record[attempt] = now;
	darts.center.setXY(sd.screen.left() + 0.25 * sd.screen.width() + 5, sd.screen.center().y());
}

void ZeroOne::reset() {
	Scene::reset();
	sd.gameTime.reset();
	sd.gameTime.start();
	attempt = 0;
	maxAttempt = 0;
	now = {};
	now.arrow = 3;
	for (int playerNo = 0; playerNo < sd.teams.size(); playerNo++) {
		now.teamRemain[playerNo] = initPoint[sd.game];
		now.rank[playerNo] = playerNo;
	}
}

void ZeroOne::draw() {
	Scene::draw();
	DrawFormatStringToHandle(sd.screen.center().x() - 80, sd.obj.upperFrame.box.bottom() + 10,
		sd.color.w, sd.font.m.handle, "Turn%3d", now.round + 1);
	int recordNo = 0;
	Chara chara;
	if (sd.teams.size() <= 4) {
		DrawBox(sd.screen.center().x() + 10, teamBox[0].top(),
			teamBox[sd.teams.size() - 1].right(), teamBox[0].bottom(), sd.obj.upperFrame.color, TRUE);
		DrawBox(teamBox[now.team].left(), teamBox[now.team].top(),
			teamBox[now.team].right(), teamBox[now.team].bottom(), sd.color.w, TRUE);
		for (int team = 0; team < sd.teams.size(); team++) {
			for (int member = 0; member < sd.teams.at(team).members.size(); member++) {
				chara = sd.teams.at(team).members.at(member);
				drawImage(chara.image);
				DrawStringToHandle(chara.image.box.left() + 5 * max(0, 10 - chara.name.size()),
					chara.image.box.bottom() - sd.font.s.size - 6, chara.name.c_str(),
					sd.color.w, sd.font.s.handle);
			}
			DrawStringToHandle(teamBox[team].left(), teamBox[team].top(), rankName[now.rank[team]].c_str(),
				sd.color.w, sd.font.m.handle);
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
						chara.image.box.bottom() + space / 2 + i * (sd.font.m.size + space),
						"BUST", sd.color.w, sd.font.m.handle);
					continue;
				}
				DrawFormatStringToHandle(chara.image.box.center().x() - 25,
					chara.image.box.bottom() + space / 2 + i * (sd.font.m.size + space),
					sd.color.w, sd.font.m.handle, "%4d", now.teamRoundScore[team][recordNo]);
			}
			DrawFormatStringToHandle(chara.image.box.center().x() - 25,
				chara.image.box.bottom() + space / 2 + nRound * (sd.font.m.size + space),
				sd.color.w, sd.font.m.handle, "%4d", now.teamRemain[team]);
			DrawLine(teamBox[team].left(), teamBox[team].top(),
				teamBox[team].left(), teamBox[team].bottom(), sd.color.k);
		}
		int y = sd.teams.at(0).members.at(sd.teamType).image.box.bottom();
		if (now.round < nRound) {
			recordNo = 0;
		}
		else {
			recordNo = now.round - nRound + 1;
		}
		for (int i = 0; recordNo <= now.round; i++, recordNo++) {
			DrawFormatStringToHandle(sd.screen.center().x() + 16,
				y + space / 2 + i * (sd.font.m.size + space),
				sd.color.w, sd.font.m.handle, "%2d", recordNo + 1);
		}
		for (int recordNo = 0, posY = 0; recordNo < nScore; recordNo++) {
			posY = y + recordNo * (sd.font.m.size + space);
			DrawLine(sd.screen.center().x() + 10, posY, teamBox[sd.teams.size() - 1].right(), posY, sd.color.k);
		}
	}
	else {
		DrawBox(sd.screen.center().x() + 10, teamBox[0].top(),
			teamBox[3].right(), teamBox[0].bottom(), sd.obj.upperFrame.color, TRUE);
		DrawBox(sd.screen.center().x() + 10, teamBox[4].top(),
			teamBox[sd.teams.size() - 1].right(), teamBox[4].bottom(), sd.obj.lowerFrame.color, TRUE);
		DrawBox(teamBox[now.team].left(), teamBox[now.team].top(),
			teamBox[now.team].right(), teamBox[now.team].bottom(), sd.color.w, TRUE);
		for (int team = 0; team < sd.teams.size(); team++) {
			chara = sd.teams.at(team).members.at(0);
			DrawRotaGraph(chara.image.box.center().x(), chara.image.box.center().y(),
				0.7, 0.0, chara.image.handle, TRUE);
			DrawStringToHandle(chara.image.box.left(), chara.image.box.top(),
				rankName[now.rank[team]].c_str(), sd.color.w, sd.font.s.handle);
			DrawStringToHandle(chara.image.box.left() + 5 * max(0, 10 - chara.name.size()),
				chara.image.box.bottom() - sd.font.s.size - 6,
				chara.name.c_str(), sd.color.w, sd.font.s.handle);
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
						chara.image.box.bottom() + space / 2 + i * (sd.font.s.size + space),
						"BUST", sd.color.w, sd.font.s.handle);
					continue;
				}
				DrawFormatStringToHandle(chara.image.box.center().x() - 20,
					chara.image.box.bottom() + space / 2 + i * (sd.font.s.size + space),
					sd.color.w, sd.font.s.handle, "%4d", now.teamRoundScore[team][recordNo]);
			}
			DrawFormatStringToHandle(chara.image.box.center().x() - 20,
				chara.image.box.bottom() + space / 2 + nRound * (sd.font.s.size + space),
				sd.color.w, sd.font.s.handle, "%4d", now.teamRemain[team]);
			DrawLine(teamBox[team].left(), teamBox[team].top(),
				teamBox[team].left(), teamBox[team].bottom(), sd.color.k);
		}
		for (int pointNo = 0, y = 0; pointNo < nScore; pointNo++) {
			y = sd.teams.at(0).members.at(0).image.box.bottom() + pointNo * (sd.font.s.size + space);
			DrawLine(sd.screen.center().x() + 10, y, teamBox[3].right(), y, sd.color.k);
			y = sd.teams.at(4).members.at(0).image.box.bottom() + pointNo * (sd.font.s.size + space);
			DrawLine(sd.screen.center().x() + 10, y, teamBox[sd.teams.size() - 1].right(), y, sd.color.k);
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
				DrawFormatStringToHandle(sd.screen.center().x() + 12,
					chara.image.box.bottom() + space / 2 + j * (sd.font.s.size + space),
					sd.color.w, sd.font.s.handle, "%2d", recordNo + 1);
			}
		}
	}
	chara = sd.teams.at(now.team).members.at(now.member);
	for (int arrow = 0, x = chara.image.box.right(), y = chara.image.box.top();
		arrow < now.arrow; arrow++)
		DrawGraph(x - 10 * (arrow + 1), y, sd.dartsArrow, TRUE);
	DrawBox(sd.screen.center().x() + 10, teamBox[0].bottom(),
		sd.screen.right(), teamBox[0].bottom() + 2 * space + sd.font.m.size, sd.color.press, TRUE);
	DrawStringToHandle(sd.screen.center().x() + 120, teamBox[0].bottom() + space,
		(chara.name + ", throw darts!").c_str(), sd.color.w, sd.font.m.handle);
}

void ZeroOne::update() {
	Scene::update();
	if (attempt < maxAttempt) {
		drawImage(sd.ctrl.forward.icon);
		if (ctrlRQ(sd.ctrl.forward)) {
			attempt++;
			now = record[attempt];
			return;
		}
	}
	bool keyboardInput = false;
	for (int point = 0; point < Darts::POINT_NUM; point++) {
		if (Keyboard::instance()->getPressState(Darts::POINT_KEY[point]) == Key::PRESSEDtoRELEASED) {
			keyboardInput = true;
			break;
		}
	}
	if (darts.point >= 0 && Mouse::instance()->getClickState() == Key::PRESSEDtoRELEASED || keyboardInput) {
		if (attempt < MAX_ATTEMPT - 1) {
			attempt++;
			maxAttempt = attempt;
		}
		if (darts.totalPoint <= now.teamRemain[now.team]) { // if bill isn't busted
			now.teamRemain[now.team] -= darts.totalPoint;
			now.teamRoundScore[now.team][now.round] += darts.totalPoint;
			now.arrow--;
		}
		else { // if bill is busted
			now.teamRemain[now.team] += now.teamRoundScore[now.team][now.round];
			now.teamRoundScore[now.team][now.round] = -1;
			now.arrow = 0;
		}
		if (!now.teamRemain[now.team]) {
			now.isPlayerFin[now.team] = true;
			now.arrow = 0;
			now.rank[now.team] = now.finRank;
			now.finRank++;
			if (now.finRank == sd.teams.size()) {
				now.isGameFin = true;
			}
		}
		for (int player = 0; player < sd.teams.size(); player++) {
			if (!now.isPlayerFin[player]) {
				now.rank[player] = 0;
				for (int opponent = (player + 1) % sd.teams.size(); opponent != player;
					opponent = (opponent + 1) % sd.teams.size()) {
					if (now.teamRemain[player] > now.teamRemain[opponent] || now.isPlayerFin[opponent] ||
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
			for (int i = 0; i < sd.teams.size(); i++) {
				now.team++;
				if (now.team >= sd.teams.size()) {
					now.team = 0;
					now.member++;
					now.round++;
					if (now.round >= MAX_ROUND) {
						now.round = MAX_ROUND - 1;
						for (int playerNo = 0; playerNo < sd.teams.size(); playerNo++) {
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
				if (!now.isPlayerFin[now.team]) {
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
}

void ZeroOne::fin() {
	sd.ctrl.skip.icon.box.setLowerRight(sd.screen.right(), sd.obj.lowerFrame.box.top());
	if (sd.teams.size() > 4) {
		for (int player = 0; player < sd.teams.size(); player++) {
			sd.teams.at(player).members.at(0).image.box.setSize(100, 100);
		}
	}
}

ZeroOne::~ZeroOne() {
}