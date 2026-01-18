#define _USE_MATH_DEFINES
#include "GameStart.hpp"
#include <numbers>
#include "Darts.hpp"
#include "Game.hpp"
#include "Sound.hpp"
#include "Timer.hpp"

cGameStart::cGameStart(ShareData shareData) {
	sd = shareData;
	mNowScene = GAME_START;
	nowTime = time(NULL);
	startTime = nowTime + timeFromEntryToStart;
	double theta = M_PI, phi = M_PI / (double)cTeam::MAX_SOLO_PLAYER_NUM;
	for (int team = 0; team < sd.teams.size(); team++, theta += 2.0 * M_PI / (double)sd.teams.size()) {
		if (sd.teams.at(team).members.size() == 1) {
			sd.teams.at(team).members[0].image.box().setCenter(
				screen.center().x() + 150.0 * cos(theta), screen.center().y() - 150.0 * sin(theta));
			continue;
		}
		sd.teams.at(team).members[0].image.box().setCenter(
			screen.center().x() + 150.0 * cos(theta + phi),
			screen.center().y() - 150.0 * sin(theta + phi));
		sd.teams.at(team).members[1].image.box().setCenter(
			screen.center().x() + 150.0 * cos(theta - phi),
			screen.center().y() - 150.0 * sin(theta - phi));
	}
}

void cGameStart::reset() {
	cScene::reset();
	startTime = time(NULL) + timeFromEntryToStart;
}

void cGameStart::draw() {
	cScene::draw();

	// draw icon
	sd.ctrl.playerSelect.icon.draw();
	sd.ctrl.gameSelect.icon.draw();
	sd.ctrl.skip.icon.draw();

	// draw scene name
	DrawStringToHandle(sd.ctrl.mute[0].icon.box().right() + 5, upperFrame.center().y() - MfontSize / 2,
		(cGame::instance()->modeName() + " / " + cTeam::instance()->typeName() + " < Game Start").c_str(),
		white, Mfont);

	// draw battle team
	for (int team = 0; team < sd.teams.size(); team++) {
		for (int member = 0; member < sd.teams.at(team).members.size(); member++) {
			sChara player = sd.teams.at(team).members.at(member);
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
	cScene::update();
	nowTime = time(NULL);
	if (ctrlRQ(sd.ctrl.back)) mNextScene = PLAYER_SELECT;
	else if (ctrlRQ(sd.ctrl.skip) || nowTime >= startTime) {
		switch (cGame::instance()->category()) {
		case cGame::sCategory::ZERO_ONE:
			mNextScene = ZERO_ONE; break;
		case cGame::sCategory::CRICKET:
			mNextScene = STANDARD_CRICKET; break;
		case cGame::sCategory::COUNT_UP:
			mNextScene = COUNT_UP; break;
		default:
			mNextScene = GAME_SELECT; return;
		}
		cTimer::instance()->restart();
	}
	else if (ctrlRQ(sd.ctrl.playerSelect)) mNextScene = PLAYER_SELECT;
	else if (ctrlRQ(sd.ctrl.gameSelect)) mNextScene = GAME_SELECT;
	else if (ctrlRQ(sd.ctrl.home)) mNextScene = HOME;
	else if (ctrlRQ(sd.ctrl.config)) mNextScene = CONFIG;
}