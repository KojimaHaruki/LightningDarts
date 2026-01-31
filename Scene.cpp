#include "Scene.hpp"
#include "Initialize.hpp"
#include "Home.hpp"
#include "GameSelect.hpp"
#include "PlayerSelect.hpp"
#include "Cork.hpp"
#include "GameStart.hpp"
#include "ZeroOne.hpp"
#include "StandardCricket.hpp"
#include "CountUp.hpp"
#include "Config.hpp"
#include "Game.hpp"
#include "Camera.hpp"

void cScene::init() {
    mLastScene = INITIALIZE; mCurrentScene = INITIALIZE; mMaxScene = INITIALIZE; 
    mScene = new cInitialize();
}

bool cScene::setScene(int scene) {
	if (scene < 0 && scene >= NUM && scene == mCurrentScene) return false;
    delete mScene;
    switch (scene) {
    case cScene::CONFIG:        mScene = new cConfig();       break;
    case cScene::HOME:          mScene = new cHome();         break;
    case cScene::GAME_SELECT:   mScene = new cGameSelect();   break;
    case cScene::PLAYER_SELECT: mScene = new cPlayerSelect(); break;
    case cScene::CORK:          mScene = new cCork();         break;
    case cScene::GAME_START:    mScene = new cGameStart();    break;
    case cScene::GAME:      
        switch (cGame::instance()->category()) {
            case cGame::sCategory::ZERO_ONE:
				mScene = new cZeroOne();         break;
            case cGame::sCategory::CRICKET:
                mScene = new cStandardCricket(); break;
            case cGame::sCategory::COUNT_UP:
				mScene = new cCountUp();         break;
        default:
            break;
        }
        break;
    case cScene::QUIT: cCamera::instance()->fin(); break;
    default: return false;
    }
	mLastScene = mCurrentScene;
    mCurrentScene = scene;
    if (scene > mMaxScene) mMaxScene = scene;
	return true;
}