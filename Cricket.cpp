#include "Cricket.hpp"
#include <string>
#include <numbers>
#include "Mouse.hpp"
Cricket::Cricket(ShareData shareData) : attempt(0), maxAttempt(0), selectPos(POS_NUM) {
	sd = shareData;
	mNowScene = CRICKET;
	markPart[0].color = sd.color.w; 
	markPart[1].color = sd.color.k; 
	markPart[0].lineWidth = 3; 
	markPart[1].lineWidth = 3; 
	markPart[2].lineWidth = 2;
	reset();
	if (sd.teamNum <= 4) {
		space = 8;
		for (int i = 0; i < 3; i++) {
			markPart[i].box.setSize(20, 20);
		}
		for (int team = 0, x = 0, y = sd.obj.upperFrame.box.bottom() + space; team < sd.teamNum; team++) {
			teamBox[team].setSize(100, 100 + sd.teamType * 100 + POINT_NUM * (sd.font.chara.size + space));
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
		for (int i = 0; i < 3; i++) {
			markPart[i].box.setSize(sd.font.chara.size, sd.font.chara.size);
		}
		for (int team = 0, chara = 0; team < 4; team++) {
			chara = sd.teamChara[team][0];
			sd.chara[chara].image.box.setSize(100, 70);
			sd.chara[chara].image.box.setUpperLeft(
				sd.screen.right() + 100 * (team - 4), sd.obj.upperFrame.box.bottom() + space);
			teamBox[team].setSize(100, sd.chara[chara].image.box.size().y() + POINT_NUM * (sd.font.chara.size + space));
			teamBox[team].setUpperLeft(sd.chara[chara].image.box.upperLeft());
		}
		for (int team = 4, chara = 0; team < sd.teamNum; team++) {
			chara = sd.teamChara[team][0];
			sd.chara[chara].image.box.setSize(100, 70);
			sd.chara[chara].image.box.setUpperLeft(
				teamBox[team - 4].left(), teamBox[team - 4].bottom() + sd.font.normal.size + space);
			teamBox[team].setSize(
				100, sd.chara[chara].image.box.size().y() + POINT_NUM * (sd.font.chara.size + space));
			teamBox[team].setUpperLeft(sd.chara[chara].image.box.upperLeft());
		}
	}
	record[attempt] = now;
	darts.center.setXY(sd.screen.left() + 0.25 * sd.screen.width() + 5, sd.screen.center().y());
}
void Cricket::reset() {
	Scene::reset(); 
	sd.gameTime.reset(); 
	sd.gameTime.start();
	attempt = 0;
	maxAttempt = 0;
	now = {}; 
	now.arrow = 3;
	for (int team = 0; team < sd.teamNum; team++) now.rank[team] = team;
	bool isPointUsed[21] = {};
	switch (sd.game) {
	case Game::HIDDEN_CRICKET:
		for (int pos = 0, randPointNo = 0; pos < POS_NUM; pos++) {
			randPointNo = GetRand(20 - pos);
			for (int point = 0, pointNo = 0; point < 21; point++) {
				if (!isPointUsed[point]) {
					if (pointNo == randPointNo) {
						if (point == 20) {
							now.posPoint[pos] = -25;
						}
						else {
							now.posPoint[pos] = -point - 1;
						}
						isPointUsed[point] = true;
						break;
					}
					pointNo++;
				}
			}
		}
		for (int point = 0; point <= 20; point++) {
			isValidPoint[point] = true;
		}
		break;
	case Game::RANDAM_CRICKET:
		isValidPoint[0] = true; // BULL is valid
		for (int pos = 0, randPointNo = 0; pos < BULL; pos++) {
			randPointNo = GetRand(19 - pos);
			for (int point = 1, pointNo = 0; point <= 20; point++) {
				if (!isValidPoint[point]) {
					if (pointNo == randPointNo) {
						now.posPoint[pos] = point;
						isValidPoint[point] = true;
						break;
					}
					pointNo++;
				}
			}
		}
		break;
	case Game::SELECT_A_CRICKET:
		for (int pos = 0; pos < POS_NUM; pos++) {
			now.posPoint[pos] = 0;
		}
		selectPos = 0;
		for (int point = 0; point <= 20; point++) {
			isValidPoint[point] = false;
		}
		break;
	default:
		isValidPoint[0] = true; // BULL is valid
		for (int point = 15; point <= 20; point++) {
			isValidPoint[point] = true;
		}
		break;
	}
	return;
}
void Cricket::draw() {
	Scene::draw();
	DrawFormatStringToHandle(sd.screen.center().x() - 80, sd.obj.upperFrame.box.bottom() + 10,
		sd.color.w, sd.font.normal.handle, "Turn%3d", now.turn + 1);
	if (sd.teamNum <= 4) {
		DrawBox(sd.screen.center().x() + 10, teamBox[0].top(),
			teamBox[sd.teamNum - 1].right(), teamBox[0].bottom(), sd.obj.upperFrame.color, TRUE);
		DrawBox(teamBox[now.team].left(), teamBox[now.team].top(),
			teamBox[now.team].right(), teamBox[now.team].bottom(), sd.color.w, TRUE);
		for (int team = 0, chara = 0; team < sd.teamNum; team++) {
			markPart[2].color = sd.color.team[team];
			for (int member = 0; member <= sd.teamType; member++) {
				chara = sd.teamChara[team][member];
				drawImage(sd.chara[chara].image);
				DrawStringToHandle(sd.chara[chara].image.box.left() + 5 * max(0, 10 - (int)sd.chara[chara].name.size()),
					sd.chara[chara].image.box.bottom() - sd.font.chara.size - 6, sd.chara[chara].name.c_str(), 
					sd.color.w, sd.font.chara.handle);
			}
			DrawStringToHandle(teamBox[team].left(), teamBox[team].top(), rankName[now.rank[team]].c_str(), 
				sd.color.w, sd.font.normal.handle);
			for (int pos = 0; pos < POINT_NUM - 1; pos++) {
				markPart[2].box.setCenter(sd.chara[chara].image.box.center().x(),
					sd.chara[chara].image.box.bottom() + (2 * pos + 1) * (sd.font.chara.size + space) / 2);
				markPart[0].box.setCenter(markPart[2].box.center().x() - 1, markPart[2].box.center().y());
				markPart[1].box.setCenter(markPart[2].box.center().x() + 1, markPart[2].box.center().y());
				for (int i = 0; i < 3; i++) {
					switch (now.teamPoint[team][pos]) {
					case 3: DrawCircleAA(markPart[i].box.center().x(), markPart[i].box.center().y(),
						(sd.font.normal.size) / 2.0 - 2.0, 100, markPart[i].color, FALSE, markPart[i].lineWidth);
					case 2: DrawLine(markPart[i].box.upperRight().x(), markPart[i].box.upperRight().y(),
						markPart[i].box.lowerLeft().x(), markPart[i].box.lowerLeft().y(),
						markPart[i].color, markPart[i].lineWidth);
					case 1: DrawLine(markPart[i].box.upperLeft().x(), markPart[i].box.upperLeft().y(),
						markPart[i].box.lowerRight().x(), markPart[i].box.lowerRight().y(),
						markPart[i].color, markPart[i].lineWidth);
					default:
						break;
					}
				}
			}
			DrawFormatStringToHandle(sd.chara[chara].image.box.center().x() - 20,
				sd.chara[chara].image.box.bottom() + 7 * sd.font.chara.size + 15 * space / 2,
				sd.color.w, sd.font.normal.handle, "%4d", now.teamBill[team]);
			DrawLine(teamBox[team].left(), teamBox[team].top(),
				teamBox[team].left(), teamBox[team].bottom(), sd.color.k);
		}
		int y = sd.chara[sd.teamChara[0][sd.teamType]].image.box.bottom();
		for (int pos = 0; pos < selectPos; pos++) {
			if (now.posPoint[pos] == 25) { // bull
				DrawStringToHandle(sd.screen.center().x() + 12,
					y + pos * sd.font.chara.size + (2 * pos + 1) * space / 2,
					darts.pointName[21].c_str(), sd.color.w, sd.font.chara.handle);
			}
			else if (now.posPoint[pos] > 0) { // except bull
				DrawStringToHandle(sd.screen.center().x() + 12,
					y + pos * sd.font.chara.size + (2 * pos + 1) * space / 2,
					darts.pointName[now.posPoint[pos]].c_str(), sd.color.w, sd.font.chara.handle);
			}
		}
		for (int pointPos = 0, posY = 0; pointPos < POINT_NUM; pointPos++) {
			posY = y + pointPos * (sd.font.chara.size + space);
			DrawLine(sd.screen.center().x() + 10, posY, teamBox[sd.teamNum - 1].right(), posY, sd.color.k);
		}
		if (sd.teamNum <= 2) {
			DrawStringToHandle(sd.screen.center().x() + 18, y + 7 * sd.font.chara.size + 15 * space / 2,
				"Pt", sd.color.w, sd.font.chara.handle);
		}
		else {
			DrawStringToHandle(sd.screen.center().x() + 12, y + 7 * sd.font.chara.size + 15 * space / 2,
				"Bill", sd.color.w, sd.font.chara.handle);
		}
	}
	else {
		DrawBox(sd.screen.center().x() + 10, teamBox[0].top(),
			teamBox[3].right(), teamBox[0].bottom(), sd.obj.upperFrame.color, TRUE);
		DrawBox(sd.screen.center().x() + 10, teamBox[4].top(),
			teamBox[sd.teamNum - 1].right(), teamBox[4].bottom(), sd.obj.lowerFrame.color, TRUE);
		DrawBox(teamBox[now.team].left(), teamBox[now.team].top(),
			teamBox[now.team].right(), teamBox[now.team].bottom(), sd.color.w, TRUE);
		for (int team = 0, chara = 0; team < sd.teamNum; team++) {
			markPart[2].color = sd.color.team[team];
			chara = sd.teamChara[team][0];
			DrawRotaGraph(sd.chara[chara].image.box.center().x(), sd.chara[chara].image.box.center().y(),
				0.7, 0.0, sd.chara[chara].image.handle, TRUE);
			DrawStringToHandle(sd.chara[chara].image.box.left(), sd.chara[chara].image.box.top(),
				rankName[now.rank[team]].c_str(), sd.color.w, sd.font.chara.handle);
			DrawStringToHandle(sd.chara[chara].image.box.left() + 5 * max(0, 10 - (int)sd.chara[chara].name.size()),
				sd.chara[chara].image.box.bottom() - sd.font.chara.size - 6,
				sd.chara[chara].name.c_str(), sd.color.w, sd.font.chara.handle);
			for (int posNo = 0; posNo < POINT_NUM - 1; posNo++) {
				markPart[2].box.setCenter(sd.chara[chara].image.box.center().x(),
					sd.chara[chara].image.box.bottom() + (2 * posNo + 1) * (sd.font.chara.size + space) / 2);
				markPart[0].box.setCenter(markPart[2].box.center().x() - 1, markPart[2].box.center().y());
				markPart[1].box.setCenter(markPart[2].box.center().x() + 1, markPart[2].box.center().y());
				for (int i = 0; i < 3; i++) {
					switch (now.teamPoint[team][posNo]) {
					case 3: DrawCircleAA(markPart[i].box.center().x(), markPart[i].box.center().y(),
						(sd.font.chara.size) / 2.0 - 2.0, 100, markPart[i].color, FALSE, markPart[i].lineWidth);
					case 2: DrawLine(markPart[i].box.upperRight().x(), markPart[i].box.upperRight().y(),
						markPart[i].box.lowerLeft().x(), markPart[i].box.lowerLeft().y(), 
						markPart[i].color, markPart[i].lineWidth);
					case 1: DrawLine(markPart[i].box.upperLeft().x(), markPart[i].box.upperLeft().y(),
						markPart[i].box.lowerRight().x(), markPart[i].box.lowerRight().y(),
						markPart[i].color, markPart[i].lineWidth);
					default:
						break;
					}
				}
			}
			DrawFormatStringToHandle(sd.chara[chara].image.box.center().x() - 20,
				sd.chara[chara].image.box.bottom() + 7 * sd.font.chara.size + 15 * space / 2,
				sd.color.w, sd.font.chara.handle, "%4d", now.teamBill[team]);
			DrawLine(teamBox[team].left(), teamBox[team].top(),
				teamBox[team].left(), teamBox[team].bottom(), sd.color.k);
		}
		for (int pointPos = 0, y = 0; pointPos < POINT_NUM; pointPos++) {
			y = sd.chara[sd.teamChara[0][0]].image.box.bottom() + pointPos * (sd.font.chara.size + space);
			DrawLine(sd.screen.center().x() + 10, y, teamBox[3].right(), y, sd.color.k);
			y = sd.chara[sd.teamChara[4][0]].image.box.bottom() + pointPos * (sd.font.chara.size + space);
			DrawLine(sd.screen.center().x() + 10, y, teamBox[sd.teamNum - 1].right(), y, sd.color.k);
		}
		for (int i = 0, chara = 0; i < 2; i++) {
			chara = sd.teamChara[4 * i][0];
			for (int pos = 0; pos < selectPos; pos++) {
				if (now.posPoint[pos] == 25) { // bull
					DrawStringToHandle(sd.screen.center().x() + 12,
						sd.chara[chara].image.box.bottom() + pos * sd.font.chara.size + (2 * pos + 1) * space / 2,
						darts.pointName[21].c_str(), sd.color.w, sd.font.chara.handle);
				}
				else if (now.posPoint[pos] > 0) { // except bull
					DrawStringToHandle(sd.screen.center().x() + 12,
						sd.chara[chara].image.box.bottom() + pos * sd.font.chara.size + (2 * pos + 1) * space / 2,
						darts.pointName[now.posPoint[pos]].c_str(), sd.color.w, sd.font.chara.handle);
				}
			}
			DrawStringToHandle(sd.screen.center().x() + 12,
				sd.chara[chara].image.box.bottom() + 7 * sd.font.chara.size + 15 * space / 2,
				"Bill", sd.color.w, sd.font.chara.handle);
		}
	}
	int chara = sd.teamChara[now.team][now.turn % (sd.teamType + 1)];
	for (int arrow = 0, x = sd.chara[chara].image.box.right(), y = sd.chara[chara].image.box.top(); 
		arrow < now.arrow; arrow++)
		DrawGraph(x - 10 * (arrow + 1), y, sd.dartsArrow, TRUE);
	DrawBox(sd.screen.center().x() + 10, teamBox[0].bottom(),
		sd.screen.right(), teamBox[0].bottom() + 2 * space + sd.font.normal.size, sd.color.press, TRUE);
	if (selectPos < POS_NUM) {
		DrawStringToHandle(sd.screen.center().x() + 120, teamBox[0].bottom() + space,
			(sd.chara[sd.teamChara[selectPos % sd.teamNum][selectPos / sd.teamNum % (sd.teamType + 1)]].name
				+ ", select a valid point!").c_str(), 
			sd.color.w, sd.font.normal.handle);
		return;
	}
	DrawStringToHandle(sd.screen.center().x() + 120, teamBox[0].bottom() + space,
		(sd.chara[chara].name + ", throw darts!").c_str(), sd.color.w, sd.font.normal.handle);
	return;
}
void Cricket::update() {
	Scene::update(); 
	if (selectPos < POS_NUM) { // select-a-clicket
		if (now.posPoint[selectPos] > 0) {
			drawImage(sd.ctrl.forward.icon);
			if (ctrlRQ(sd.ctrl.forward)) {
				if (now.posPoint[selectPos] == 25) {
					isValidPoint[0] = true;
				}
				else {
					isValidPoint[now.posPoint[selectPos]] = true;
				}
				selectPos++;
				return;
			}
		}
		if (Mouse::getInstance()->getClickState() == Key::PRESSEDtoRELEASED || 
			Keyboard::getInstance()->getPressState(KEY_INPUT_F12) == Key::PRESSEDtoRELEASED) {
			if (darts.point == 25) {
				if (!isValidPoint[0]) {
					now.posPoint[selectPos] = darts.point;
					isValidPoint[0] = true;
					selectPos++;
				}
			}
			else if (darts.point > 0) {
				if (!isValidPoint[darts.point]) {
					now.posPoint[selectPos] = darts.point;
					isValidPoint[darts.point] = true;
					selectPos++;
				}
			}
			if (selectPos == POS_NUM) {
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
		now.arrow--;
		int cricketPos = -1;
		for (int posNo = 0; posNo < POS_NUM; posNo++) {
			if (darts.point == abs(now.posPoint[posNo])) {
				cricketPos = posNo;
				break;
			}
		}
		if (cricketPos >= 0 && cricketPos < POS_NUM) { 
			if (now.posPoint[cricketPos] < 0) { // hidden cricket
				now.posPoint[cricketPos] *= -1;
				darts.power++;
			}
			now.teamPoint[now.team][cricketPos] += darts.power;
			if (now.teamPoint[now.team][cricketPos] > 3) {
				int damage = POS_POINT[cricketPos] * (now.teamPoint[now.team][cricketPos] - 3);
				now.teamPoint[now.team][cricketPos] = 3;
				if (sd.teamNum <= 2) {
					int opponent = (now.team + 1) % sd.teamNum;
					if (now.teamPoint[opponent][cricketPos] < 3) {
						now.teamBill[now.team] += damage;
					}
					if (now.teamBill[now.team] > now.teamBill[opponent]) {
						now.rank[now.team] = 0;
						now.rank[opponent] = 1;
					}
				}
				else {
					for (int opponent = (now.team + 1) % sd.teamNum; opponent != now.team; 
						opponent = (opponent + 1) % sd.teamNum) {
						if (now.teamPoint[opponent][cricketPos] < 3) {
							now.teamBill[opponent] += damage;
						}
					}
				}
			}
			now.isTeamFin[now.team] = true;
			for (int posNo = 0; posNo < POS_NUM; posNo++) {
				if (now.teamPoint[now.team][posNo] < 3) {
					now.isTeamFin[now.team] = false;
					break; 
				}
			}
			if (sd.teamNum <= 2) {
				for (int opponent = (now.team + 1) % sd.teamNum; opponent != now.team;
					opponent = (opponent + 1) % sd.teamNum) {
					if (!now.isTeamFin[opponent] && now.teamBill[now.team] < now.teamBill[opponent]) {
						now.isTeamFin[now.team] = false;
					}
				}
			}
			else {
				for (int opponent = (now.team + 1) % sd.teamNum; opponent != now.team; 
					opponent = (opponent + 1) % sd.teamNum) {
					if (!now.isTeamFin[opponent] && now.teamBill[now.team] > now.teamBill[opponent]) {
						now.isTeamFin[now.team] = false;
					}
				}
			}
		}
		else if (darts.point == 25) {
			isValidPoint[0] = false;
		}
		else if (darts.point > 0) {
			isValidPoint[darts.point] = false;
		}
		if (now.isTeamFin[now.team]) {
			now.arrow = 0;
			now.rank[now.team] = now.finRank;
			now.finRank++;
			if (now.finRank == sd.teamNum) {
				now.isGameFin = true;
			}
		}
		if (sd.teamNum > 2) {
			for (int team = 0; team < sd.teamNum; team++) {
				if (!now.isTeamFin[team]) {
					now.rank[team] = 0;
					for (int opponent = (team + 1) % sd.teamNum; opponent != team; 
						opponent = (opponent + 1) % sd.teamNum){
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
			for (int i = 0; i < sd.teamNum; i++) {
				now.team++;
				if (now.team >= sd.teamNum) {
					now.team = 0;
					now.turn++;
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
		else if (sd.game == Game::SELECT_A_CRICKET && selectPos > 0) {
			selectPos--;
			if (now.posPoint[selectPos] == 25) {
				isValidPoint[0] = false;
			}
			else {
				isValidPoint[now.posPoint[selectPos]] = false;
			}
		}
		else {
			mNextScene = GAME_START;
		}
	}
	return;
}
void Cricket::fin() {
	sd.ctrl.skip.icon.box.setLowerRight(sd.screen.right(), sd.obj.lowerFrame.box.top());
	if (sd.teamNum > 4) {
		for (int playerNo = 0, charaNo = 0; playerNo < sd.teamNum; playerNo++) {
			charaNo = sd.teamChara[playerNo][0];
			sd.chara[charaNo].image.box.setSize(100, 100);
		}
	}
	return;
}
Cricket::~Cricket() {
}