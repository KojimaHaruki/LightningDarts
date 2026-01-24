#define _USE_MATH_DEFINES
#include "GameStart.hpp"
#include <numbers>
#include "Darts.hpp"
#include "Game.hpp"
#include "Sound.hpp"
#include "Timer.hpp"
#include "Control.hpp"
#include "Scene.hpp"

cGameStart::cGameStart() {
	nowTime = time(NULL);
	startTime = nowTime + timeFromEntryToStart;
	double theta = M_PI, phi = M_PI / (double)cPlayer::MAX_SOLO_PLAYER_NUM;
	for (int team = 0; team < cPlayer::instance()->nTeam(); 
		team++, theta += 2.0 * M_PI / (double)cPlayer::instance()->nTeam()) {
		if (cPlayer::instance()->nTeamMember(team) == 1) {
			cPlayer::instance()->teams().at(team).members[0].image.box().setCenter(
				screen.center().x() + 150.0 * cos(theta), screen.center().y() - 150.0 * sin(theta));
			continue;
		}
		cPlayer::instance()->teams().at(team).members[0].image.box().setCenter(
			screen.center().x() + 150.0 * cos(theta + phi),
			screen.center().y() - 150.0 * sin(theta + phi));
		cPlayer::instance()->teams().at(team).members[1].image.box().setCenter(
			screen.center().x() + 150.0 * cos(theta - phi),
			screen.center().y() - 150.0 * sin(theta - phi));
	}
}

void cGameStart::reset() {
	cBaseScene::reset();
	startTime = time(NULL) + timeFromEntryToStart;
}

void cGameStart::draw() {
	cBaseScene::draw();

	// draw icon
	cControl::instance()->icon(cControl::PLAYER_SELECT).draw();
	cControl::instance()->icon(cControl::GAME_SELECT).draw();
	cControl::instance()->icon(cControl::SKIP).draw();

	// draw scene name
	DrawStringToHandle(cControl::instance()->icon(cControl::MUTE).box().right() + 5,
		upperFrame.center().y() - MfontSize / 2,
		(cGame::instance()->modeName() + " < Game Start").c_str(), white, Mfont);

	// draw battle team
	for (int team = 0; team < cPlayer::instance()->nTeam(); team++) {
		for (int member = 0; member < cPlayer::instance()->nTeamMember(team); member++) {
			cPlayer::sChara player = cPlayer::instance()->teamMember(team, member);
			player.image.draw();
			DrawStringToHandle(player.image.box().left(), player.image.box().top(),
				PLAYER_NAME[player.status.rank].c_str(), white, Mfont);
			DrawStringToHandle(player.image.box().left() + 5 * max(0, 10 - player.name.size()),
				player.image.box().bottom() - SfontSize - 10, player.name.c_str(), white, Sfont);
		}
	}
	DrawStringToHandle(screen.center().x() - 10, screen.center().y() - MfontSize / 2, "VS", white, Mfont);
}

void cGameStart::update() {
	cBaseScene::update();
	nowTime = time(NULL);
	if (cControl::instance()->isRequested(cControl::BACK))
		cScene::instance()->setScene(cScene::PLAYER_SELECT);
	else if (cControl::instance()->isRequested(cControl::SKIP) || nowTime >= startTime) {
		switch (cGame::instance()->category()) {
		case cGame::sCategory::ZERO_ONE:
			cScene::instance()->setScene(cScene::ZERO_ONE); break;
		case cGame::sCategory::CRICKET:
			cScene::instance()->setScene(cScene::STANDARD_CRICKET); break;
		case cGame::sCategory::COUNT_UP:
			cScene::instance()->setScene(cScene::COUNT_UP); break;
		default:
			cScene::instance()->setScene(cScene::GAME_SELECT); return;
		}
		cTimer::instance()->restart();
	}
	else if (cControl::instance()->isRequested(cControl::CONFIG))
		cScene::instance()->setScene(cScene::CONFIG);
}
