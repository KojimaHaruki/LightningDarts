#define _USE_MATH_DEFINES
#include "GameStart.hpp"
#include <string>
#include <numbers>

GameStart::GameStart(ShareData shareData) {
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

void GameStart::reset() {
	Scene::reset(); startTime = time(NULL) + timeFromEntryToStart;
}

void GameStart::draw() {
	Scene::draw();
	for (int team = 0; team < sd.teams.size(); team++) {
		for (int member = 0; member < sd.teams[team].members.size(); member++) {
			Chara player = sd.teams[team].members[member];
			drawImage(player.image);
			DrawStringToHandle(player.image.box.left(), player.image.box.top(),
				rankName[player.status.rank].c_str(), sd.color.w, sd.font.m.handle);
			DrawStringToHandle(player.image.box.left() + 5 * max(0, 10 - player.name.size()), 
				player.image.box.bottom() - sd.font.s.size - 10,
				player.name.c_str(), sd.color.w, sd.font.s.handle);
		}
	}
	DrawStringToHandle(sd.screen.center().x() - 10, sd.screen.center().y() - sd.font.m.size / 2, "VS",
		sd.color.w, sd.font.m.handle);
	DrawStringToHandle(sd.ctrl.mute[0].icon.box.right() + 5,
		sd.obj.upperFrame.box.center().y() - sd.font.m.size / 2,
		(gameName[sd.game] + " / " + teamTypeName[sd.teamType] + " < Game Start").c_str(), 
		sd.color.w, sd.font.m.handle);
	return;
}

void GameStart::update() {
	Scene::update(); nowTime = time(NULL);
	if (ctrlRQ(sd.ctrl.back)) mNextScene = PLAYER_SELECT;
	else if (ctrlRQ(sd.ctrl.skip) || nowTime >= startTime) { 
		// set timer
		sd.gameTime.reset();
		sd.gameTime.start();
		if (sd.game < Game::ZERO_ONE_NUM) {
			mNextScene = ZERO_ONE;
		}
		else if (sd.game < Game::CRICKET_COUNT_UP) {
			mNextScene = CRICKET;
		}
		else {
			mNextScene = COUNT_UP;
		}
	}
	return;
}

GameStart::~GameStart() {
}

