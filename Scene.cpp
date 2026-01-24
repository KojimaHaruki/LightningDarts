#include "Scene.hpp"
#include "Initialize.hpp"
#include "Home.hpp"
#include "GameSelect.hpp"
#include "PlayerSelect.hpp"
#include "GameStart.hpp"
#include "ZeroOne.hpp"
#include "StandardCricket.hpp"
#include "CountUp.hpp"
#include "Config.hpp"

void cScene::init() {
	mLastScene = INITIALIZE; mCurrentScene = INITIALIZE; mScene = new cInitialize();
}

bool cScene::setScene(int scene) {
	if (scene < 0 && scene >= NUM && scene == mCurrentScene) return false;
	mScene->fin();
    switch (scene) {
    case cScene::CONFIG:        mScene = new cConfig();                 break;
    case cScene::HOME:          mScene = new cHome();                   break;
    case cScene::GAME_SELECT:   mScene = new cGameSelect();             break;
    case cScene::PLAYER_SELECT: mScene = new cPlayerSelect();           break;
    case cScene::GAME_START:    mScene = new cGameStart();              break;
    case cScene::ZERO_ONE:      mScene = new cZeroOne();                break;
    case cScene::STANDARD_CRICKET: mScene = new cStandardCricket();     break;
    case cScene::COUNT_UP:      mScene = new cCountUp();                break;
    case cScene::QUIT:          delete mScene; return 0;
    default: return -1;
    }
	mLastScene = mCurrentScene;
    mCurrentScene = scene;
	return true;
}