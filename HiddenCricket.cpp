#include "HiddenCricket.hpp"
#include <algorithm>
#include <random>
#include "Darts.hpp"
#include "Game.hpp"
#include "Control.hpp"
#include "Scene.hpp"

cHiddenCricket::cHiddenCricket() : attempt(0), maxAttempt(0), selectPos(POS_NUM) {
	nTeam = cPlayer::inst()->nTeam();
	markPart[0].color = white;
	markPart[1].color = black;
	markPart[0].lineWidth = 3;
	markPart[1].lineWidth = 3;
	markPart[2].lineWidth = 2;
	attempt = 0;
	maxAttempt = 0;
	now = {};
	now.arrow = 3;
	for (int team = 0; team < nTeam; team++) now.rank[team] = team;
	std::vector<int> randomPoint;
	for (int point = 1; point <= 20; point++) randomPoint.push_back(point);
	std::shuffle(randomPoint.begin(), randomPoint.end(), std::mt19937{ std::random_device{}() });
	switch (cGame::inst()->mode()) {
	case cGame::sMode::HIDDEN_CRICKET:
		for (int pos = 0, point = randomPoint.at(0); pos < POS_NUM - 1; pos++, point = randomPoint.at(pos)) {
			now.posScore[pos] = -point - 1;
		}
		now.posScore[POS_NUM - 1] = -25;
		for (int point = 0; point <= 20; point++) {
			cDarts::inst()->setPointValidation(point, true);
		}
		break;
	case cGame::sMode::RANDOM_CRICKET:
		now.posScore[POS_NUM - 1] = 25;
		cDarts::inst()->setPointValidation(0, true); // BULL is valid
		for (int pos = 0, point = randomPoint.at(0); pos < POS_NUM - 1; pos++, point = randomPoint.at(pos)) {
			now.posScore[pos] = point;
			cDarts::inst()->setPointValidation(point, true);
		}
		break;
	case cGame::sMode::SELECT_A_CRICKET:
		selectPos = 0;
		for (int point = 0; point <= 20; point++) {
			cDarts::inst()->setPointValidation(point, false);
		}
		now.arrow = 1;
		break;
	default:
		cDarts::inst()->setPointValidation(0, true); // BULL is valid
		for (int point = 15; point <= 20; point++) {
			cDarts::inst()->setPointValidation(point, true);
		}
		break;
	}
	if (nTeam <= 4) {
		space = 8;
		for (int i = 0; i < 3; i++) {
			markPart[i].box.setS(20, 20);
		}
		for (int team = 0, x = screen.R() - 400, y = upperFrame.B() + space;
			team < nTeam; team++, x += 100, y = upperFrame.B() + space) {
			teamBox[team].setS(
				100, 100 + cPlayer::inst()->teamType() * 100 + POINT_NUM * (SfontSize + space));
			teamBox[team].setUL(x, y);
			for (int member = 0; member < cPlayer::inst()->nTeamMember(team); member++, y += 100) {
				cPlayer::inst()->teamMemberImageBox(team, member).setUL(x, y);
			}
		}
	}
	else {
		space = 2;
		for (int player = 0; player < nTeam; player++) {
			cPlayer::inst()->teamMemberImageBox(player, 0).setUL(
				screen.R() + 100 * (player - 4), upperFrame.B() + space);
			teamBox[player].setS(100,
				cPlayer::inst()->teamMemberImageBox(player, 0).S().y() + POINT_NUM * (SfontSize + space));
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

void cHiddenCricket::reset() {
	cBaseScene::reset();
	cDarts::inst()->reset();
	attempt = 0;
	maxAttempt = 0;
	now = {};
	now.arrow = 3;
	for (int team = 0; team < nTeam; team++) now.rank[team] = team;
	std::vector<int> randomPoint;
	for (int point = 1; point <= 20; point++) randomPoint.push_back(point);
	std::shuffle(randomPoint.begin(), randomPoint.end(), std::mt19937{ std::random_device{}() });
	switch (cGame::inst()->mode()) {
	case cGame::sMode::HIDDEN_CRICKET:
		for (int pos = 0, point = randomPoint.at(0); pos < POS_NUM - 1; pos++, point = randomPoint.at(pos)) {
			now.posScore[pos] = -point - 1;
		}
		now.posScore[POS_NUM - 1] = -25;
		for (int point = 0; point <= 20; point++) {
			cDarts::inst()->setPointValidation(point, true);
		}
		break;
	case cGame::sMode::RANDOM_CRICKET:
		now.posScore[POS_NUM - 1] = 25;
		cDarts::inst()->setPointValidation(0, true); // BULL is valid
		for (int pos = 0, point = randomPoint.at(0); pos < POS_NUM - 1; pos++, point = randomPoint.at(pos)) {
			now.posScore[pos] = point;
			cDarts::inst()->setPointValidation(point, true);
		}
		break;
	case cGame::sMode::SELECT_A_CRICKET:
		selectPos = 0;
		for (int point = 0; point <= 20; point++) {
			cDarts::inst()->setPointValidation(point, false);
		}
		now.arrow = 1;
		break;
	default:
		cDarts::inst()->setPointValidation(0, true); // BULL is valid
		for (int point = 15; point <= 20; point++) {
			cDarts::inst()->setPointValidation(point, true);
		}
		break;
	}
}

void cHiddenCricket::draw() {
	cBaseScene::draw();

	// draw icon
	if (attempt < maxAttempt) cControl::inst()->icon(cControl::FORWARD).draw();

	// draw darts board
	cDarts::inst()->draw();

	// draw round
	DrawFormatStringToHandle(screen.CX() - 80, upperFrame.B() + 10,
		white, Mfont, "Round%3d", now.round + 1);

	// draw score table
	cPlayer::sChara chara;
	if (nTeam <= 4) {
		DrawBox(screen.CX() + 10, teamBox[0].T(),
			teamBox[nTeam - 1].R(), teamBox[0].B(), tableColor, TRUE);
		DrawBox(teamBox[now.team].L(), teamBox[now.team].T(),
			teamBox[now.team].R(), teamBox[now.team].B(), white, TRUE);
		for (int team = 0; team < nTeam; team++) {
			markPart[2].color = teamColor[team];
			for (int member = 0; member < cPlayer::inst()->nTeamMember(team); member++) {
				chara = cPlayer::inst()->teamMember(team, member);
				chara.image.draw();
				DrawStringToHandle(chara.image.box().L() + 5 * max(0, 10 - (int)chara.name.size()),
					chara.image.box().B() - SfontSize - 6, chara.name.c_str(),
					white, Sfont);
			}
			DrawStringToHandle(teamBox[team].L(), teamBox[team].T(), rankName[now.rank[team]].c_str(),
				white, Mfont);
			for (int pos = 0; pos < POINT_NUM - 1; pos++) {
				markPart[2].box.setC(chara.image.box().CX(),
					chara.image.box().B() + (2 * pos + 1) * (SfontSize + space) / 2);
				markPart[0].box.setC(markPart[2].box.CX() - 1, markPart[2].box.C().y());
				markPart[1].box.setC(markPart[2].box.CX() + 1, markPart[2].box.C().y());
				for (int i = 0; i < 3; i++) {
					switch (now.teamPosScore[team][pos]) {
					case 3:
						DrawCircleAA(markPart[i].box.CX(), markPart[i].box.C().y(),
							(MfontSize) / 2.0 - 2.0, 100, markPart[i].color, FALSE, markPart[i].lineWidth);
					case 2:
						DrawLine(markPart[i].box.R(), markPart[i].box.T(),
							markPart[i].box.L(), markPart[i].box.B(),
							markPart[i].color, markPart[i].lineWidth);
					case 1:
						DrawLine(markPart[i].box.L(), markPart[i].box.T(),
							markPart[i].box.R(), markPart[i].box.B(),
							markPart[i].color, markPart[i].lineWidth);
					default:
						break;
					}
				}
			}
			DrawFormatStringToHandle(chara.image.box().CX() - 20,
				chara.image.box().B() + 7 * SfontSize + 15 * space / 2,
				white, Mfont, "%4d", now.teamBill[team]);
			DrawLine(teamBox[team].L(), teamBox[team].T(),
				teamBox[team].L(), teamBox[team].B(), black);
		}
		int y = cPlayer::inst()->teamMemberImageBox(0, cPlayer::inst()->teamType()).B();
		for (int pos = 0; pos < selectPos; pos++) {
			DrawStringToHandle(screen.CX() + 12,
				y + pos * SfontSize + (2 * pos + 1) * space / 2,
				cDarts::inst()->pointName(now.posScore[pos]).c_str(), white, Sfont);
		}
		for (int pointPos = 0, posY = 0; pointPos < POINT_NUM; pointPos++) {
			posY = y + pointPos * (SfontSize + space);
			DrawLine(screen.CX() + 10, posY, teamBox[nTeam - 1].R(), posY, black);
		}
		if (nTeam <= 2) {
			DrawStringToHandle(screen.CX() + 18, y + 7 * SfontSize + 15 * space / 2,
				"Pt", white, Sfont);
		}
		else {
			DrawStringToHandle(screen.CX() + 12, y + 7 * SfontSize + 15 * space / 2,
				"Bill", white, Sfont);
		}
	}
	else {
		DrawBox(screen.CX() + 10, teamBox[0].T(),
			teamBox[3].R(), teamBox[0].B(), tableColor, TRUE);
		DrawBox(screen.CX() + 10, teamBox[4].T(),
			teamBox[nTeam - 1].R(), teamBox[4].B(), tableColor, TRUE);
		DrawBox(teamBox[now.team].L(), teamBox[now.team].T(),
			teamBox[now.team].R(), teamBox[now.team].B(), white, TRUE);
		for (int team = 0; team < nTeam; team++) {
			markPart[2].color = teamColor[team];
			chara = cPlayer::inst()->teamMember(team, 0);
			DrawRotaGraph(chara.image.box().CX(), chara.image.box().C().y(),
				0.7, 0.0, chara.image.handle(), TRUE);
			DrawStringToHandle(chara.image.box().L(), chara.image.box().T(),
				rankName[now.rank[team]].c_str(), white, Sfont);
			DrawStringToHandle(chara.image.box().L() + 5 * max(0, 10 - (int)chara.name.size()),
				chara.image.box().B() - SfontSize - 6,
				chara.name.c_str(), white, Sfont);
			for (int posNo = 0; posNo < POINT_NUM - 1; posNo++) {
				markPart[2].box.setC(chara.image.box().CX(),
					chara.image.box().B() + (2 * posNo + 1) * (SfontSize + space) / 2);
				markPart[0].box.setC(markPart[2].box.CX() - 1, markPart[2].box.C().y());
				markPart[1].box.setC(markPart[2].box.CX() + 1, markPart[2].box.C().y());
				for (int i = 0; i < 3; i++) {
					switch (now.teamPosScore[team][posNo]) {
					case 3:
						DrawCircleAA(markPart[i].box.CX(), markPart[i].box.C().y(),
							(SfontSize) / 2.0 - 2.0, 100, markPart[i].color, FALSE, markPart[i].lineWidth);
					case 2:
						DrawLine(markPart[i].box.R(), markPart[i].box.T(),
							markPart[i].box.L(), markPart[i].box.B(),
							markPart[i].color, markPart[i].lineWidth);
					case 1:
						DrawLine(markPart[i].box.L(), markPart[i].box.T(),
							markPart[i].box.R(), markPart[i].box.B(),
							markPart[i].color, markPart[i].lineWidth);
					default:
						break;
					}
				}
			}
			DrawFormatStringToHandle(chara.image.box().CX() - 20,
				chara.image.box().B() + 7 * SfontSize + 15 * space / 2,
				white, Sfont, "%4d", now.teamBill[team]);
			DrawLine(teamBox[team].L(), teamBox[team].T(),
				teamBox[team].L(), teamBox[team].B(), black);
		}
		for (int pointPos = 0, y = 0; pointPos < POINT_NUM; pointPos++) {
			y = cPlayer::inst()->teamMemberImageBox(0, 0).B() + pointPos * (SfontSize + space);
			DrawLine(screen.CX() + 10, y, teamBox[3].R(), y, black);
			y = cPlayer::inst()->teamMemberImageBox(4, 0).B() + pointPos * (SfontSize + space);
			DrawLine(screen.CX() + 10, y, teamBox[nTeam - 1].R(), y, black);
		}
		for (int i = 0; i < 2; i++) {
			chara = cPlayer::inst()->teamMember(4 * i, 0);
			for (int pos = 0; pos < selectPos; pos++) {
				DrawStringToHandle(screen.CX() + 12,
					chara.image.box().B() + pos * SfontSize + (2 * pos + 1) * space / 2,
					cDarts::inst()->pointName(now.posScore[pos]).c_str(), white, Sfont);
			}
			DrawStringToHandle(screen.CX() + 12,
				chara.image.box().B() + 7 * SfontSize + 15 * space / 2,
				"Bill", white, Sfont);
		}
	}
	DrawBox(screen.CX() + 10, teamBox[0].B(),
		screen.R(), teamBox[0].B() + 2 * space + MfontSize, pressColor, TRUE);
	chara = cPlayer::inst()->teamMember(now.team, now.member);
	for (int arrow = 0, x = chara.image.box().R(), y = chara.image.box().T();
		arrow < now.arrow; arrow++)
		DrawGraph(x - 10 * (arrow + 1), y, cDarts::inst()->arrowImage(), TRUE);
	if (selectPos < POS_NUM) {
		DrawStringToHandle(screen.CX() + 120, teamBox[0].B() + space,
			(cPlayer::inst()->teamMemberName(now.team, now.member) + ", throw a dart!").c_str(),
			white, Mfont);
		return;
	}
	DrawStringToHandle(screen.CX() + 120, teamBox[0].B() + space,
		(chara.name + ", throw darts!").c_str(), white, Mfont);
}

void cHiddenCricket::update() {
	cBaseScene::update();
	cDarts::inst()->update();
	int point = cDarts::inst()->point(), power = cDarts::inst()->power(),
		totalPoint = cDarts::inst()->totalPoint();
	if (selectPos < POS_NUM) { // select-a-clicket
		if (now.posScore[selectPos] > 0) {
			cControl::inst()->icon(cControl::FORWARD).draw();
			if (cControl::inst()->isRequested(cControl::FORWARD)) {
				if (now.posScore[selectPos] == 25) {
					cDarts::inst()->setPointValidation(0, true);
				}
				else {
					cDarts::inst()->setPointValidation(now.posScore[selectPos], true);
				}
				selectPos++;
				return;
			}
		}
		if (cDarts::inst()->isThrowed()) {
			now.arrow--;
			if (point > 0) {
				if (!cDarts::inst()->isValidPoint(point)) {
					now.posScore[selectPos] = point;
					cDarts::inst()->setPointValidation(point, true);
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
			if (now.member >= cPlayer::inst()->nTeamMember(now.team)) {
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
		cControl::inst()->icon(cControl::FORWARD).draw();
		if (cControl::inst()->isRequested(cControl::FORWARD)) {
			attempt++;
			now = record[attempt];
			return;
		}
	}
	if (cDarts::inst()->isThrowed()) {
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
		if (cControl::inst()->isRequested(cControl::SKIP)) {
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
				if (now.member >= cPlayer::inst()->nTeamMember(now.team)) {
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
	if (cControl::inst()->isRequested(cControl::BACK)) {
		if (attempt > 0) {
			attempt--;
			now = record[attempt];
			return;
		}
		if (cGame::inst()->mode() == cGame::sMode::SELECT_A_CRICKET && selectPos > 0) {
			now.arrow = 1;
			selectPos--;
			if (now.posScore[selectPos] == 25) {
				cDarts::inst()->setPointValidation(0, false);
				return;
			}
			cDarts::inst()->setPointValidation(now.posScore[selectPos], false);
			return;
		}
		cScene::inst()->setScene(cScene::GAME_START);
	}
}

cHiddenCricket::~cHiddenCricket() {
	if (nTeam > 4) {
		for (int player = 0; player < nTeam; player++) {
			cPlayer::inst()->teamMemberImageBox(player, 0).setS(100, 100);
		}
	}
}