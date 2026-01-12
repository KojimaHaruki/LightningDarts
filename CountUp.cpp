#include "CountUp.hpp"
#include <string>
#include <numbers>
#include "Mouse.hpp"

CountUp::CountUp(ShareData shareData) : attempt(0), maxAttempt(0) {
	mNowScene = COUNT_UP;
	sd = shareData;
	sd.ctrl.skip.icon.box.setLowerRight(sd.screen.center().x() + 10, sd.obj.lowerFrame.box.top());
	reset();
	if (sd.teams.size() <= 4) {
		space = 4;
		for (int team = 0, x = sd.screen.right() - 400, y = sd.obj.upperFrame.box.bottom() + space;
			team < sd.teams.size(); team++, x += 100, y = sd.obj.upperFrame.box.bottom() + space) {
			teamBox[team].setSize(100, 100 + sd.teamType * 100 + SCORE_NUM * (sd.font.m.size + space));
			teamBox[team].setUpperLeft(x, y);
			for (int member = 0; member < sd.teams.at(team).members.size(); member++, y += 100) {
				sd.teams.at(team).members.at(member).image.box.setUpperLeft(x, y);
			}
		}
	}
	else {
		space = 2;
		for (int player = 0; player < sd.teams.size(); player++) {
			sd.teams.at(player).members.at(0).image.box.setSize(100, 70);
			teamBox[player].setSize(100,
				sd.teams.at(player).members.at(0).image.box.size().y() + SCORE_NUM * (sd.font.s.size + space));
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

void CountUp::reset() {
	Scene::reset();
	sd.gameTime.reset();
	sd.gameTime.start();
	attempt = 0;
	maxAttempt = 0;
	now = {};
	now.arrow = 3;
	switch (sd.game) {
	case Game::CRICKET_COUNT_UP:
		for (int i = 0; i < 21; i++) {
			isValidPoint[i] = false;
		}
		isValidPoint[20] = true; // 20 is initially valid in Cricket Countup
		return;
	default:
		for (int point = 0; point < 21; point++) {
			isValidPoint[point] = true;
		}
		return;
	}
}

void CountUp::draw() {
	Scene::draw();
	DrawFormatStringToHandle(sd.screen.center().x() - 90, sd.obj.upperFrame.box.bottom() + 10,
		sd.color.w, sd.font.m.handle, "Round%2d", now.round + 1);
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
				DrawStringToHandle(chara.image.box.left() + 5 * max(0, 10 - (int)chara.name.size()),
					chara.image.box.bottom() - sd.font.s.size - 6, chara.name.c_str(),
					sd.color.w, sd.font.s.handle);
			}
			DrawStringToHandle(teamBox[team].left(), teamBox[team].top(), rankName[now.rank[team]].c_str(),
				sd.color.w, sd.font.m.handle);
			for (int round = 0; round <= now.round; round++) {
				if (round == now.round && team > now.team) {
					break;
				}
				DrawFormatStringToHandle(chara.image.box.center().x() - 25,
					chara.image.box.bottom() + space / 2 + round * (sd.font.m.size + space),
					sd.color.w, sd.font.m.handle, "%4d", now.teamRoundScore[team][round]);
			}
			DrawFormatStringToHandle(chara.image.box.center().x() - 25,
				chara.image.box.bottom() + space / 2 + ROUND_NUM * (sd.font.m.size + space),
				sd.color.w, sd.font.m.handle, "%4d", now.teamScore[team]);
			DrawLine(teamBox[team].left(), teamBox[team].top(),
				teamBox[team].left(), teamBox[team].bottom(), sd.color.k);
		}
		int y = sd.teams.at(0).members.at(sd.teamType).image.box.bottom();
		for (int round = 0; round < ROUND_NUM; round++) {
			DrawFormatStringToHandle(sd.screen.center().x() + 16,
				y + space / 2 + round * (sd.font.m.size + space),
				sd.color.w, sd.font.m.handle, "%2d", round + 1);
		}
		for (int recordNo = 0, posY = 0; recordNo < SCORE_NUM; recordNo++) {
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
			for (int round = 0; round <= now.round; round++) {
				if (round == now.round && team > now.team) {
					break;
				}
				DrawFormatStringToHandle(chara.image.box.center().x() - 20,
					chara.image.box.bottom() + space / 2 + round * (sd.font.s.size + space),
					sd.color.w, sd.font.s.handle, "%4d", now.teamRoundScore[team][round]);
			}
			DrawFormatStringToHandle(chara.image.box.center().x() - 20,
				chara.image.box.bottom() + space / 2 + ROUND_NUM * (sd.font.s.size + space),
				sd.color.w, sd.font.s.handle, "%4d", now.teamScore[team]);
			DrawLine(teamBox[team].left(), teamBox[team].top(),
				teamBox[team].left(), teamBox[team].bottom(), sd.color.k);
		}
		for (int pointNo = 0, y = 0; pointNo < SCORE_NUM; pointNo++) {
			y = sd.teams.at(0).members.at(0).image.box.bottom() + pointNo * (sd.font.s.size + space);
			DrawLine(sd.screen.center().x() + 10, y, teamBox[3].right(), y, sd.color.k);
			y = sd.teams.at(4).members.at(0).image.box.bottom() + pointNo * (sd.font.s.size + space);
			DrawLine(sd.screen.center().x() + 10, y, teamBox[sd.teams.size() - 1].right(), y, sd.color.k);
		}
		for (int i = 0; i < 2; i++) {
			for (int j = 0, recordNo = 0; recordNo <= now.round; j++, recordNo++) {
				DrawFormatStringToHandle(sd.screen.center().x() + 12,
					sd.teams.at(4 * i).members.at(0).image.box.bottom() + space / 2 + j * (sd.font.s.size + space),
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

void CountUp::update() {
	Scene::update();
	if (attempt < maxAttempt) {
		drawImage(sd.ctrl.forward.icon);
		if (ctrlRQ(sd.ctrl.forward)) {
			attempt++;
			now = record[attempt];
			return;
		}
	}
	if (!now.isGameFin) {
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
			if (isValidPoint[0] && darts.point == 25) {
				now.teamScore[now.team] += darts.totalPoint;
				now.teamRoundScore[now.team][now.round] += darts.totalPoint;
			}
			else if (isValidPoint[darts.point]) {
				now.teamScore[now.team] += darts.totalPoint;
				now.teamRoundScore[now.team][now.round] += darts.totalPoint;
			}
			now.arrow--;
			for (int player = 0; player < sd.teams.size(); player++) {
				now.rank[player] = 0;
				for (int opponent = (player + 1) % sd.teams.size(); opponent != player;
					opponent = (opponent + 1) % sd.teams.size()) {
					if (now.teamScore[player] < now.teamScore[opponent]) {
						now.rank[player]++;
					}
				}
			}
			record[attempt] = now;
		}
		if (ctrlRQ(sd.ctrl.skip)) {
			if (attempt < MAX_ATTEMPT - 1) {
				attempt++;
				maxAttempt = attempt;
			}
			now.arrow = 0;
		}
		if (now.arrow < 1) {
			now.team++;
			now.arrow = 3;
			if (now.team >= sd.teams.size()) {
				if (now.round == ROUND_NUM - 1) {
					now.team = sd.teams.size() - 1;
					now.isGameFin = true;
					now.arrow = 0;
				}
				else {
					now.team = 0;
					now.member++;
					now.round++;
					if (sd.game == Game::CRICKET_COUNT_UP) {
						isValidPoint[CRICKET_NUMBER_SCORE[now.round - 1]] = false;
						if (now.round < CRICKET_NUMBER_NUM) {
							isValidPoint[CRICKET_NUMBER_SCORE[now.round]] = true;
						}
						else {
							for (int pointNo = 0; pointNo < CRICKET_NUMBER_NUM; pointNo++) {
								isValidPoint[CRICKET_NUMBER_SCORE[pointNo]] = true;
							}
						}
					}
				}
			}
			if (now.member >= sd.teams.at(now.team).members.size()) {
				now.member = 0;
			}
			record[attempt] = now;
			return;
		}
	}
	if (ctrlRQ(sd.ctrl.back)) {
		if (attempt > 0) {
			attempt--;
			now = record[attempt];
			if (now.team == sd.teams.size() - 1 && now.arrow == 1 && now.round < CRICKET_NUMBER_NUM) {
				if (now.round < CRICKET_NUMBER_NUM - 1) {
					isValidPoint[CRICKET_NUMBER_SCORE[now.round + 1]] = false;
				}
				else {
					for (int pointNo = 0; pointNo < CRICKET_NUMBER_NUM; pointNo++) {
						isValidPoint[CRICKET_NUMBER_SCORE[pointNo]] = false;
					}
				}
				isValidPoint[CRICKET_NUMBER_SCORE[now.round]] = true;
			}
		}
		else {
			mNextScene = GAME_START;
		}
	}
}

void CountUp::fin() {
	sd.ctrl.skip.icon.box.setLowerRight(sd.screen.right(), sd.obj.lowerFrame.box.top());
	if (sd.teams.size() > 4) {
		for (int player = 0; player < sd.teams.size(); player++) {
			sd.teams.at(player).members.at(0).image.box.setSize(100, 100);
		}
	}
}

CountUp::~CountUp() {
}