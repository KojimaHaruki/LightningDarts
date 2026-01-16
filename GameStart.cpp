#define _USE_MATH_DEFINES
#include "GameStart.hpp"
#include <string>
#include <numbers>
#include "Darts.hpp"

cGameStart::cGameStart(ShareData shareData) {
	sd = shareData;
	mNowScene = GAME_START;
	nowTime = time(NULL);
	startTime = nowTime + timeFromEntryToStart;
	double theta = M_PI, phi = M_PI / (double)MAX_PLAYER_NUM;
	for (int team = 0; team < sd.teams.size(); team++, theta += 2.0 * M_PI / (double)sd.teams.size()) {
		if (sd.teams[team].members.size() == 1) {
			sd.teams[team].members[0].image.box.setCenter(
				sd.screen.center().x() + 150.0 * cos(theta), sd.screen.center().y() - 150.0 * sin(theta));
			continue;
		}
		sd.teams[team].members[0].image.box.setCenter(
			sd.screen.center().x() + 150.0 * cos(theta + phi),
			sd.screen.center().y() - 150.0 * sin(theta + phi));
		sd.teams[team].members[1].image.box.setCenter(
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
	for (int team = 0; team < sd.teams.size(); team++) {
		for (int member = 0; member < sd.teams[team].members.size(); member++) {
			sChara player = sd.teams[team].members[member];
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
		(gameName[mGame] + " / " + teamTypeName[sd.teamType] + " < Game Start").c_str(),
		white, Mfont);
	return;
}

void cGameStart::update() {
	cScene::update(); 
	nowTime = time(NULL);
	if (ctrlRQ(sd.ctrl.back)) mNextScene = PLAYER_SELECT;
	else if (ctrlRQ(sd.ctrl.skip) || nowTime >= startTime) {
		switch (mGame) {
		case cDarts::sGame::ZERO_ONE_301: case cDarts::sGame::ZERO_ONE_501:
		case cDarts::sGame::ZERO_ONE_701: case cDarts::sGame::ZERO_ONE_901:
		case cDarts::sGame::ZERO_ONE_1101: case cDarts::sGame::ZERO_ONE_1501:
			mNextScene = ZERO_ONE;
			break;
		case cDarts::sGame::STANDARD_CRICKET: case cDarts::sGame::RANDAM_CRICKET:
		case cDarts::sGame::HIDDEN_CRICKET: case cDarts::sGame::SELECT_A_CRICKET:
		case cDarts::sGame::ALL_NUMBER_CRICKET:
			mNextScene = STANDARD_CRICKET;
			break;
		case cDarts::sGame::CRICKET_COUNT_UP: case cDarts::sGame::COUNT_UP:
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

