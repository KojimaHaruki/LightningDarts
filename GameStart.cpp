#include "GameStart.hpp"
#include <numbers>
#include "Darts.hpp"
#include "Game.hpp"
#include "Sound.hpp"
#include "Timer.hpp"
#include "Control.hpp"
#include "Scene.hpp"

cGameStart::cGameStart() {
	nTeam = cPlayer::inst()->nTeam();
	nowTime = time(NULL);
	startTime = nowTime + TIME_ENTRYtoSTART;
	double theta = std::numbers::pi, phi = std::numbers::pi / (double)cPlayer::MAX_SOLO_PLAYER_NUM;
	for (int team = 0; team < nTeam; team++, theta += 2.0 * std::numbers::pi / (double)nTeam) {
		if (cPlayer::inst()->nTeamMember(team) == 1) {
			cPlayer::inst()->teams().at(team).members.front().image.box().setC(
				screen.C().x() + 150.0 * cos(theta), screen.C().y() - 150.0 * sin(theta));
			continue;
		}
		cPlayer::inst()->teams().at(team).members.front().image.box().setC(
			screen.C().x() + 150.0 * cos(theta + phi),
			screen.C().y() - 150.0 * sin(theta + phi));
		cPlayer::inst()->teams().at(team).members.back().image.box().setC(
			screen.C().x() + 150.0 * cos(theta - phi),
			screen.C().y() - 150.0 * sin(theta - phi));
	}
}

void cGameStart::reset() {
	cBaseScene::reset();
	startTime = time(NULL) + TIME_ENTRYtoSTART;
}

void cGameStart::draw() {
	cBaseScene::draw();

	// draw icon
	cControl::inst()->icon(cControl::PLAYER_SELECT).draw();
	cControl::inst()->icon(cControl::GAME_SELECT).draw();
	cControl::inst()->icon(cControl::SKIP).draw();

	// draw scene name
	DrawStringToHandle(cControl::inst()->icon(cControl::MUTE).box().R() + 5,
		upperFrame.C().y() - MfontSize / 2,
		(cGame::inst()->modeName() + " < Game Start").c_str(), white, Mfont);

	// draw battle team
	cPlayer::sChara player = {};
	for (int team = 0; team < nTeam; team++) {
		for (int member = 0; member < cPlayer::inst()->nTeamMember(team); member++) {
			player = cPlayer::inst()->teamMember(team, member);
			player.image.draw();
			DrawStringToHandle(player.image.box().L(), player.image.box().T(),
				RANK_NAME[team].c_str(), rankColor[team], Lfont);
			DrawStringToHandle(player.image.box().L() + 5 * max(0, 10 - (int)player.name.size()),
				player.image.box().B() - SfontSize - 10, player.name.c_str(), white, Sfont);
		}
	}
	DrawStringToHandle(screen.C().x() - 10, screen.C().y() - MfontSize / 2, "VS", white, Lfont);
}

void cGameStart::update() {
	cBaseScene::update();
	nowTime = time(NULL);
	if (cControl::inst()->isRequested(cControl::BACK))
		cScene::inst()->setScene(cScene::CORK);
	else if (cControl::inst()->isRequested(cControl::SKIP) || nowTime >= startTime) {
		cScene::inst()->setScene(cScene::GAME); cTimer::inst()->restart();
	}
	else if (cControl::inst()->isRequested(cControl::CONFIG))
		cScene::inst()->setScene(cScene::CONFIG);
}
