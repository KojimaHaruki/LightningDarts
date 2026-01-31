#include "GameStart.hpp"
#include <numbers>
#include "Darts.hpp"
#include "Game.hpp"
#include "Sound.hpp"
#include "Timer.hpp"
#include "Control.hpp"
#include "Scene.hpp"

cGameStart::cGameStart() {
	nTeam = cPlayer::instance()->nTeam();
	nowTime = time(NULL);
	startTime = nowTime + TIME_ENTRYtoSTART;
	double theta = std::numbers::pi, phi = std::numbers::pi / (double)cPlayer::MAX_SOLO_PLAYER_NUM;
	for (int team = 0; team < nTeam; team++, theta += 2.0 * std::numbers::pi / (double)nTeam) {
		if (cPlayer::instance()->nTeamMember(team) == 1) {
			cPlayer::instance()->teams().at(team).members.front().image.box().setCenter(
				screen.center().x() + 150.0 * cos(theta), screen.center().y() - 150.0 * sin(theta));
			continue;
		}
		cPlayer::instance()->teams().at(team).members.front().image.box().setCenter(
			screen.center().x() + 150.0 * cos(theta + phi),
			screen.center().y() - 150.0 * sin(theta + phi));
		cPlayer::instance()->teams().at(team).members.back().image.box().setCenter(
			screen.center().x() + 150.0 * cos(theta - phi),
			screen.center().y() - 150.0 * sin(theta - phi));
	}
}

void cGameStart::reset() {
	cBaseScene::reset();
	startTime = time(NULL) + TIME_ENTRYtoSTART;
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
	cPlayer::sChara player = {};
	for (int team = 0; team < nTeam; team++) {
		for (int member = 0; member < cPlayer::instance()->nTeamMember(team); member++) {
			player = cPlayer::instance()->teamMember(team, member);
			player.image.draw();
			DrawStringToHandle(player.image.box().left(), player.image.box().top(),
				RANK_NAME[team].c_str(), rankColor[team], Lfont);
			DrawStringToHandle(player.image.box().left() + 5 * max(0, 10 - (int)player.name.size()),
				player.image.box().bottom() - SfontSize - 10, player.name.c_str(), white, Sfont);
		}
	}
	DrawStringToHandle(screen.center().x() - 10, screen.center().y() - MfontSize / 2, "VS", white, Lfont);
}

void cGameStart::update() {
	cBaseScene::update();
	nowTime = time(NULL);
	if (cControl::instance()->isRequested(cControl::BACK))
		cScene::instance()->setScene(cScene::CORK);
	else if (cControl::instance()->isRequested(cControl::SKIP) || nowTime >= startTime) {
		cScene::instance()->setScene(cScene::GAME); cTimer::instance()->restart();
	}
	else if (cControl::instance()->isRequested(cControl::CONFIG))
		cScene::instance()->setScene(cScene::CONFIG);
}
