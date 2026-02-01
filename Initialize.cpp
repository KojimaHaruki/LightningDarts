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
	cScreen::inst()->initSize();
	loadScreen();
	ChangeWindowMode(FALSE);
	SetDrawScreen(DX_SCREEN_BACK);
	SetMouseDispFlag(TRUE);
	
	// load images
	cScreen::inst()->loadImage();
	cpp = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG92), "PNG");
	dxlib = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG93), "PNG");
	
	// load fonts
	cFont::inst()->load();
	loadFont();

	// load colors
	cColor::inst()->load();
	loadColor();
}

void cInitialize::draw() {
	ClearDrawScreen();
	cScreen::inst()->draw();
	DrawStringToHandle(100, 50, "Powered by", white, XLfont);
	DrawGraph(200, 100 + XLfontSize, cpp, TRUE);
	DrawGraph(500, 100 + XLfontSize, dxlib, TRUE);
	DrawStringToHandle(screen.R() - 180, screen.B() - MfontSize - 10, 
		"now loading...", white, Mfont);
	ScreenFlip();
}

void cInitialize::update() {
	// load sounds
	cSound::inst()->init();

	// load images
	cKeyboard::inst()->loadKeyImage();
	cControl::inst()->init();
	cDarts::inst()->init();
	cGame::inst()->init();
	cPlayer::inst()->init();
	cCamera::inst()->init();

	// go to home scene
	cScene::inst()->setScene(cScene::HOME);
}