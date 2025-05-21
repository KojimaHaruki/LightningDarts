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
	double theta = 0.0;
	if (sd.teamNum <= 4) {
		space = 4; 
		pointRecordNum = 9; 
		pointNum = pointRecordNum + 1;
		for (int team = 0, x = 0, y = sd.obj.upperFrame.box.bottom() + space; team < sd.teamNum; team++) {
			teamBox[team].setSize(100, 100 + sd.teamType * 100 + pointNum * (sd.font.normal.size + space));
			x = sd.screen.right() + 100 * (team - 4);
			teamBox[team].setUpperLeft(x, y);
			for (int member = 0, chara = 0; member <= sd.teamType; member++) {
				chara = sd.teamChara[team][member];
				sd.chara[chara].image.box.setUpperLeft(x, y + 100 * member);
			}
		}
	}
	else {
		space = 2; 
		pointRecordNum = 7; 
		pointNum = pointRecordNum + 1;
		for (int player = 0, chara = 0; player < 4; player++) {
			chara = sd.teamChara[player][0];
			sd.chara[chara].image.box.setSize(100, 70);
			sd.chara[chara].image.box.setUpperLeft(
				sd.screen.right() + 100 * (player - 4), sd.obj.upperFrame.box.bottom() + space);
			teamBox[player].setSize(
				100, sd.chara[chara].image.box.size().y() + pointNum * (sd.font.chara.size + space));
			teamBox[player].setUpperLeft(sd.chara[chara].image.box.upperLeft());
		}
		for (int playerNo = 4, charaNo = 0; playerNo < sd.teamNum; playerNo++) {
			charaNo = sd.teamChara[playerNo][0];
			sd.chara[charaNo].image.box.setSize(100, 70);
			sd.chara[charaNo].image.box.setUpperLeft(
				teamBox[playerNo - 4].left(), teamBox[playerNo - 4].bottom() + sd.font.normal.size + space);
			teamBox[playerNo].setSize(
				100, sd.chara[charaNo].image.box.size().y() + pointNum * (sd.font.chara.size + space));
			teamBox[playerNo].setUpperLeft(sd.chara[charaNo].image.box.upperLeft());
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
	for (int playerNo = 0; playerNo < sd.teamNum; playerNo++) {
		now.teamBill[playerNo] = initPoint[sd.game];
		now.rank[playerNo] = playerNo;
	}
	return;
}
void ZeroOne::draw() {
	Scene::draw();
	DrawFormatStringToHandle(sd.screen.center().x() - 80, sd.obj.upperFrame.box.bottom() + 10,
		sd.color.w, sd.font.normal.handle, "Turn%3d", now.turn + 1);
	int recordNo = 0;
	if (sd.teamNum <= 4) {
		DrawBox(sd.screen.center().x() + 10, teamBox[0].top(),
			teamBox[sd.teamNum - 1].right(), teamBox[0].bottom(), sd.obj.upperFrame.color, TRUE);
		DrawBox(teamBox[now.team].left(), teamBox[now.team].top(),
			teamBox[now.team].right(), teamBox[now.team].bottom(), sd.color.w, TRUE);
		for (int team = 0, chara = 0; team < sd.teamNum; team++) {
			for (int member = 0; member <= sd.teamType; member++) {
				chara = sd.teamChara[team][member];
				drawImage(sd.chara[sd.teamChara[team][member]].image);
				DrawStringToHandle(sd.chara[chara].image.box.left() + 5 * max(0, 10 - (int)sd.chara[chara].name.size()),
					sd.chara[chara].image.box.bottom() - sd.font.chara.size - 6, sd.chara[chara].name.c_str(), 
					sd.color.w, sd.font.chara.handle);
			}
			DrawStringToHandle(teamBox[team].left(), teamBox[team].top(), rankName[now.rank[team]].c_str(), 
				sd.color.w, sd.font.normal.handle);
			if (now.turn < pointRecordNum) {
				recordNo = 0;
			}
			else {
				recordNo = now.turn - pointRecordNum + 1;
			}
			for (int i = 0; recordNo <= now.turn; i++, recordNo++) {
				if (recordNo == now.turn && team > now.team) {
					break;
				}
				if (now.teamPoint[team][recordNo] == -1) {
					DrawStringToHandle(sd.chara[chara].image.box.center().x() - 25,
						sd.chara[chara].image.box.bottom() + space / 2 + i * (sd.font.normal.size + space),
						"BUST", sd.color.w, sd.font.normal.handle);
					continue;
				}
				DrawFormatStringToHandle(sd.chara[chara].image.box.center().x() - 25,
					sd.chara[chara].image.box.bottom() + space / 2 + i * (sd.font.normal.size + space),
					sd.color.w, sd.font.normal.handle, "%4d", now.teamPoint[team][recordNo]);
			}
			DrawFormatStringToHandle(sd.chara[chara].image.box.center().x() - 25,
				sd.chara[chara].image.box.bottom() + space / 2 + pointRecordNum * (sd.font.normal.size + space),
				sd.color.w, sd.font.normal.handle, "%4d", now.teamBill[team]);
			DrawLine(teamBox[team].left(), teamBox[team].top(),
				teamBox[team].left(), teamBox[team].bottom(), sd.color.k);
		}
		int y = sd.chara[sd.teamChara[0][sd.teamType]].image.box.bottom();
		if (now.turn < pointRecordNum) {
			recordNo = 0;
		}
		else {
			recordNo = now.turn - pointRecordNum + 1;
		}
		for (int i = 0; recordNo <= now.turn; i++, recordNo++) {
			DrawFormatStringToHandle(sd.screen.center().x() + 16, 
				y + space / 2 + i * (sd.font.normal.size + space),
				sd.color.w, sd.font.normal.handle, "%2d", recordNo + 1);
		}
		for (int recordNo = 0, posY = 0; recordNo < pointNum; recordNo++) {
			posY = y + recordNo * (sd.font.normal.size + space);
			DrawLine(sd.screen.center().x() + 10, posY, teamBox[sd.teamNum - 1].right(), posY, sd.color.k);
		}
	}
	else {
		DrawBox(sd.screen.center().x() + 10, teamBox[0].top(),
			teamBox[3].right(), teamBox[0].bottom(), sd.obj.upperFrame.color, TRUE);
		DrawBox(sd.screen.center().x() + 10, teamBox[4].top(),
			teamBox[sd.teamNum - 1].right(), teamBox[4].bottom(), sd.obj.lowerFrame.color, TRUE);
		DrawBox(teamBox[now.team].left(), teamBox[now.team].top(),
			teamBox[now.team].right(), teamBox[now.team].bottom(), sd.color.w, TRUE);
		for (int team = 0, charaNo = 0; team < sd.teamNum; team++) {
			charaNo = sd.teamChara[team][0];
			DrawRotaGraph(sd.chara[charaNo].image.box.center().x(), sd.chara[charaNo].image.box.center().y(),
				0.7, 0.0, sd.chara[charaNo].image.handle, TRUE);
			DrawStringToHandle(sd.chara[charaNo].image.box.left(), sd.chara[charaNo].image.box.top(),
				rankName[now.rank[team]].c_str(), sd.color.w, sd.font.chara.handle);
			DrawStringToHandle(sd.chara[charaNo].image.box.left() + 5 * max(0, 10 - (int)sd.chara[charaNo].name.size()),
				sd.chara[charaNo].image.box.bottom() - sd.font.chara.size - 6,
				sd.chara[charaNo].name.c_str(), sd.color.w, sd.font.chara.handle);
			if (now.turn < pointRecordNum) {
				recordNo = 0;
			}
			else {
				recordNo = now.turn - pointRecordNum + 1;
			}
			for (int i = 0; recordNo <= now.turn; i++, recordNo++) {
				if (recordNo == now.turn && team > now.team) {
					break;
				}
				if (now.teamPoint[team][recordNo] == -1) {
					DrawStringToHandle(sd.chara[charaNo].image.box.center().x() - 20,
						sd.chara[charaNo].image.box.bottom() + space / 2 + i * (sd.font.chara.size + space),
						"BUST", sd.color.w, sd.font.chara.handle);
					continue;
				}
				DrawFormatStringToHandle(sd.chara[charaNo].image.box.center().x() - 20,
					sd.chara[charaNo].image.box.bottom() + space / 2 + i * (sd.font.chara.size + space),
					sd.color.w, sd.font.chara.handle, "%4d", now.teamPoint[team][recordNo]);
			}
			DrawFormatStringToHandle(sd.chara[charaNo].image.box.center().x() - 20,
				sd.chara[charaNo].image.box.bottom() + space / 2 + pointRecordNum * (sd.font.chara.size + space),
				sd.color.w, sd.font.chara.handle, "%4d", now.teamBill[team]);
			DrawLine(teamBox[team].left(), teamBox[team].top(),
				teamBox[team].left(), teamBox[team].bottom(), sd.color.k);
		}
		for (int pointNo = 0, y = 0; pointNo < pointNum; pointNo++) {
			y = sd.chara[sd.teamChara[0][0]].image.box.bottom() + pointNo * (sd.font.chara.size + space);
			DrawLine(sd.screen.center().x() + 10, y, teamBox[3].right(), y, sd.color.k);
			y = sd.chara[sd.teamChara[4][0]].image.box.bottom() + pointNo * (sd.font.chara.size + space);
			DrawLine(sd.screen.center().x() + 10, y, teamBox[sd.teamNum - 1].right(), y, sd.color.k);
		}
		for (int i = 0, chara = 0; i < 2; i++) {
			chara = sd.teamChara[4 * i][0];
			if (now.turn < pointRecordNum) {
				recordNo = 0;
			}
			else {
				recordNo = now.turn - pointRecordNum + 1;
			}
			for (int j = 0; recordNo <= now.turn; j++, recordNo++) {
				DrawFormatStringToHandle(sd.screen.center().x() + 12,
					sd.chara[chara].image.box.bottom() + space / 2 + j * (sd.font.chara.size + space),
					sd.color.w, sd.font.chara.handle, "%2d", recordNo + 1);
			}
		}
	}
	int chara = sd.teamChara[now.team][now.turn % (sd.teamType + 1)];
	for (int arrow = 0, x = sd.chara[chara].image.box.right(), y = sd.chara[chara].image.box.top();
		arrow < now.arrow; arrow++)
		DrawGraph(x - 10 * (arrow + 1), y, sd.dartsArrow, TRUE);
	DrawBox(sd.screen.center().x() + 10, teamBox[0].bottom(), 
		sd.screen.right(), teamBox[0].bottom() + 2 * space + sd.font.normal.size, sd.color.press, TRUE);
	DrawStringToHandle(sd.screen.center().x() + 120, teamBox[0].bottom() + space,
		(sd.chara[chara].name + ", throw darts!").c_str(), sd.color.w, sd.font.normal.handle);
	return;
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
		if (Keyboard::getInstance()->getPressState(Darts::POINT_KEY[point]) == Key::PRESSEDtoRELEASED) {
			keyboardInput = true;
			break;
		}
	}
	if (darts.point >= 0 && Mouse::getInstance()->getClickState() == Key::PRESSEDtoRELEASED || keyboardInput) {
		if (attempt < MAX_ATTEMPT - 1) {
			attempt++;
			maxAttempt = attempt;
		}
		if (darts.totalPoint <= now.teamBill[now.team]) { // if bill isn't busted
			now.teamBill[now.team] -= darts.totalPoint;
			now.teamPoint[now.team][now.turn] += darts.totalPoint;
			now.arrow--;
		}
		else { // if bill is busted
			now.teamBill[now.team] += now.teamPoint[now.team][now.turn];
			now.teamPoint[now.team][now.turn] = -1;
			now.arrow = 0;
		}
		if (!now.teamBill[now.team]) {
			now.isPlayerFin[now.team] = true;
			now.arrow = 0;
			now.rank[now.team] = now.finRank;
			now.finRank++;
			if (now.finRank == sd.teamNum) {
				now.isGameFin = true;
			}
		}
		for (int player = 0; player < sd.teamNum; player++) {
			if (!now.isPlayerFin[player]) {
				now.rank[player] = 0;
				for (int opponent = (player + 1) % sd.teamNum; opponent != player;
					opponent = (opponent + 1) % sd.teamNum) {
					if (now.teamBill[player] > now.teamBill[opponent] || now.isPlayerFin[opponent] ||
						(now.teamBill[player] == now.teamBill[opponent] && player > opponent)) {
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
			for (int i = 0; i < sd.teamNum; i++) {
				now.team++;
				if (now.team >= sd.teamNum) {
					now.team = 0;
					now.turn++;
					if (now.turn >= MAX_TURN) {
						now.turn = MAX_TURN - 1;
						for (int playerNo = 0; playerNo < sd.teamNum; playerNo++) {
							for (int recordNo = 0; recordNo < MAX_TURN - 1; recordNo++) {
								now.teamPoint[playerNo][recordNo] = now.teamPoint[playerNo][recordNo + 1];
							}
							now.teamPoint[playerNo][MAX_TURN - 1] = 0;
						}
					}
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
	return;
}
void ZeroOne::fin() {
	sd.ctrl.skip.icon.box.setLowerRight(sd.screen.right(), sd.obj.lowerFrame.box.top());
	if (sd.teamNum > 4) {
		for (int player = 0; player < sd.teamNum; player++) {
			sd.chara[sd.teamChara[player][0]].image.box.setSize(100, 100);
		}
	}
	return;
}
ZeroOne::~ZeroOne() {
}