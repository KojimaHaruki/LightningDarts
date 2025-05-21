#include "GameStart.hpp"
#include <string>
#include <numbers>
GameStart::GameStart(ShareData shareData) {
	sd = shareData;
	mNowScene = GAME_START; 
	nowTime = time(NULL);
	startTime = nowTime + timeFromEntryToStart;
	double theta = 0.0, phi = std::numbers::pi / (double)MAX_PLAYER_NUM;
	switch (sd.teamType) {
	case TeamType::SOLO:
		for (int player = 0; player < sd.teamNum; player++) {
			theta = 2.0 * player * std::numbers::pi / (double)sd.teamNum + std::numbers::pi;
			sd.chara[sd.teamChara[player][0]].image.box.setCenter(
				sd.screen.center().x() + 150.0 * cos(theta), sd.screen.center().y() - 150.0 * sin(theta));
		}
		break;
	case TeamType::DUO:
		for (int team = 0; team < sd.teamNum; team++) {
			theta = 2.0 * team * std::numbers::pi / (double)sd.teamNum + std::numbers::pi;
			sd.chara[sd.teamChara[team][0]].image.box.setCenter(
				sd.screen.center().x() + 150.0 * cos(theta + phi),
				sd.screen.center().y() - 150.0 * sin(theta + phi));
			sd.chara[sd.teamChara[team][1]].image.box.setCenter(
				sd.screen.center().x() + 150.0 * cos(theta - phi),
				sd.screen.center().y() - 150.0 * sin(theta - phi));
		}
		break;
	default:
		break;
	}
	
}
void GameStart::reset() {
	Scene::reset(); startTime = time(NULL) + timeFromEntryToStart;
}
void GameStart::draw() {
	Scene::draw();
	for (int team = 0, chara = 0; team < sd.teamNum; team++) {
		for (int member = 0; member <= sd.teamType; member++) {
			chara = sd.teamChara[team][member];
			drawImage(sd.chara[chara].image);
			DrawStringToHandle(sd.chara[chara].image.box.left(), sd.chara[chara].image.box.top(),
				rankName[team + sd.teamNum * member].c_str(), sd.color.w, sd.font.normal.handle);
			DrawStringToHandle(sd.chara[chara].image.box.left() + 5 * max(0, 10 - (int)sd.chara[chara].name.size()),
				sd.chara[chara].image.box.bottom() - sd.font.chara.size - 10,
				sd.chara[chara].name.c_str(), sd.color.w, sd.font.chara.handle);
		}
	}
	DrawStringToHandle(sd.screen.center().x() - 10, sd.screen.center().y() - sd.font.normal.size / 2, "VS",
		sd.color.w, sd.font.normal.handle);
	DrawStringToHandle(sd.ctrl.mute[sd.sound].icon.box.right() + 5,
		sd.obj.upperFrame.box.center().y() - sd.font.normal.size / 2,
		(gameName[sd.game] + " < " + std::to_string(sd.teamNum) + " player / " 
			+ playModeName[sd.teamType] + " < Game Start").c_str(), sd.color.w, sd.font.normal.handle);
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
		else {
			mNextScene = CRICKET;
		}
	}
	return;
}
GameStart::~GameStart() {
}

