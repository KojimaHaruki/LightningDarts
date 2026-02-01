#include "Scene.hpp"
#include "Initialize.hpp"
#include "Home.hpp"
#include "GameSelect.hpp"
#include "PlayerSelect.hpp"
#include "Cork.hpp"
#include "GameStart.hpp"
#include "ZeroOne.hpp"
#include "StandardCricket.hpp"
#include "HiddenCricket.hpp"
#include "CountUp.hpp"
#include "CricketCountUp.hpp"
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
        if (cGame::inst()->category() == cGame::sCategory::ZERO_ONE) {
            mScene = new cZeroOne();
        }
        else if (cGame::inst()->mode() == cGame::sMode::STANDARD_CRICKET) {
            mScene = new cStandardCricket();
        }
        else if (cGame::inst()->mode() == cGame::sMode::COUNT_UP) {
            mScene = new cCountUp();
        }
        else if (cGame::inst()->mode() == cGame::sMode::CRICKET_COUNT_UP) {
            mScene = new cCricketCountUp();
        }
        else {
            mScene = new cHiddenCricket();
        }
        break;
    case cScene::QUIT: cCamera::inst()->fin(); break;
    default: return false;
    }
	mLastScene = mCurrentScene;
    mCurrentScene = scene;
    if (scene > mMaxScene) mMaxScene = scene;
	return true;
}