#define _USE_MATH_DEFINES
#include "GameStart.hpp"
#include <string>
#include <numbers>
#include "Darts.hpp"
#include "Game.hpp"
#include "Team.hpp"
#include "Sound.hpp"

cGameStart::cGameStart(ShareData shareData) {
	sd = shareData;
	mNowScene = GAME_START;
	nowTime = time(NULL);
	startTime = nowTime + timeFromEntryToStart;
	double theta = M_PI, phi = M_PI / (double)MAX_PLAYER_NUM;
	for (int team = 0; team < sd.teams.size(); team++, theta += 2.0 * M_PI / (double)sd.teams.size()) {
		if (sd.teams.at(team).members.size() == 1) {
			sd.teams.at(team).members[0].image.box.setCenter(
				sd.screen.center().x() + 150.0 * cos(theta), sd.screen.center().y() - 150.0 * sin(theta));
			continue;
		}
		sd.teams.at(team).members[0].image.box.setCenter(
			sd.screen.center().x() + 150.0 * cos(theta + phi),
			sd.screen.center().y() - 150.0 * sin(theta + phi));
		sd.teams.at(team).members[1].image.box.setCenter(
			sd.screen.center().x() + 150.0 * cos(theta - phi),
			sd.screen.center().y() - 150.0 * sin(theta - phi));
	}
}

void cGameStart::reset() {
	cScene::reset();
	startTime = time(NULL) + timeFromEntryToStart;
}

void cGameStart::draw() {
	cScene::draw();
	drawImage(sd.ctrl.pause[cDarts::instance()->timer().isPaused()].icon);
	drawImage(sd.ctrl.skill.icon);
	drawImage(sd.ctrl.playerSelect.icon);
	drawImage(sd.ctrl.gameSelect.icon);
	drawImage(sd.ctrl.skip.icon);
	drawImage(sd.ctrl.home.icon);
	drawImage(sd.ctrl.back.icon);
	drawImage(sd.ctrl.mute[cSound::instance()->isBGMPlayed()].icon);
	drawImage(sd.ctrl.config.icon);
	drawImage(sd.ctrl.window[sd.window].icon);
	drawImage(sd.ctrl.quit.icon);
	drawImage(sd.ctrl.init.icon);
	drawImage(sd.ctrl.reset.icon);
	drawImage(sd.ctrl.bgm.icon);
	for (int team = 0; team < sd.teams.size(); team++) {
		for (int member = 0; member < sd.teams.at(team).members.size(); member++) {
			sChara player = sd.teams.at(team).members.at(member);
			drawImage(player.image);
			DrawStringToHandle(player.image.box.left(), player.image.box.top(),
				rankName[player.status.rank].c_str(), white, Mfont);
			DrawStringToHandle(player.image.box.left() + 5 * max(0, 10 - player.name.size()),
				player.image.box.bottom() - SfontSize - 10, player.name.c_str(), white, Sfont);
		}
	}
	DrawStringToHandle(sd.screen.center().x() - 10, sd.screen.center().y() - MfontSize / 2, "VS",
		white, Mfont);
	DrawStringToHandle(sd.ctrl.mute[0].icon.box.right() + 5,
		sd.obj.upperFrame.center().y() - MfontSize / 2,
		(cGame::instance()->modeName() + " / " + cTeam::instance()->typeName() + " < Game Start").c_str(),
		white, Mfont);
	return;
}

void cGameStart::update() {
	cScene::update();
	nowTime = time(NULL);
	if (ctrlRQ(sd.ctrl.back)) mNextScene = PLAYER_SELECT;
	else if (ctrlRQ(sd.ctrl.skip) || nowTime >= startTime) {
		switch (mGameMode) {
		case cGame::sMode::ZERO_ONE_301: case cGame::sMode::ZERO_ONE_501:
		case cGame::sMode::ZERO_ONE_701: case cGame::sMode::ZERO_ONE_901:
		case cGame::sMode::ZERO_ONE_1101: case cGame::sMode::ZERO_ONE_1501:
			mNextScene = ZERO_ONE;
			break;
		case cGame::sMode::STANDARD_CRICKET: case cGame::sMode::RANDOM_CRICKET:
		case cGame::sMode::HIDDEN_CRICKET: case cGame::sMode::SELECT_A_CRICKET:
		case cGame::sMode::ALL_NUMBERS_CRICKET:
			mNextScene = STANDARD_CRICKET;
			break;
		case cGame::sMode::CRICKET_COUNT_UP: case cGame::sMode::COUNT_UP:
			mNextScene = COUNT_UP;
			break;
		default:
			mNextScene = GAME_SELECT;
			return;
		}
		// set timer
		cDarts::instance()->timer().reset();
		cDarts::instance()->timer().start();
	}
	else if (ctrlRQ(sd.ctrl.playerSelect)) mNextScene = PLAYER_SELECT;
	else if (ctrlRQ(sd.ctrl.gameSelect)) mNextScene = GAME_SELECT;
	else if (ctrlRQ(sd.ctrl.home)) mNextScene = HOME;
	else if (ctrlRQ(sd.ctrl.config)) mNextScene = CONFIG;
}

