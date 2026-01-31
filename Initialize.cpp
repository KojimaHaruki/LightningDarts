#include "Initialize.hpp"
#include "DxLib.h"
#include "resource.h"
#include "Screen.hpp"
#include "Font.hpp"
#include "Color.hpp"
#include "Sound.hpp"
#include "Darts.hpp"
#include "Keyboard.hpp"
#include "Game.hpp"
#include "Scene.hpp"
#include "Camera.hpp"

cInitialize::cInitialize() {
	cScreen::instance()->initSize();
	loadScreen();
	ChangeWindowMode(FALSE);
	SetDrawScreen(DX_SCREEN_BACK);
	SetMouseDispFlag(TRUE);
	
	// load images
	cScreen::instance()->loadImage();
	cpp = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG92), "PNG");
	dxlib = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG93), "PNG");
	
	// load fonts
	cFont::instance()->load();
	loadFont();

	// load colors
	cColor::instance()->load();
	loadColor();
}

void cInitialize::draw() {
	ClearDrawScreen();
	cScreen::instance()->draw();
	DrawStringToHandle(100, 50, "Powered by", white, XLfont);
	DrawGraph(200, 100 + XLfontSize, cpp, TRUE);
	DrawGraph(500, 100 + XLfontSize, dxlib, TRUE);
	DrawStringToHandle(screen.right() - 180, screen.bottom() - MfontSize - 10, 
		"now loading...", white, Mfont);
	ScreenFlip();
}

void cInitialize::update() {
	// load sounds
	cSound::instance()->init();

	// load images
	cKeyboard::instance()->loadKeyImage();
	cControl::instance()->init();
	cDarts::instance()->init();
	cGame::instance()->init();
	cPlayer::instance()->init();
	cCamera::instance()->init();

	// go to home scene
	cScene::instance()->setScene(cScene::HOME);
}