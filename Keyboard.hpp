#pragma once

#include "Singleton.hpp"
#include "Key.hpp"
#include "Image.hpp"

class cKeyboard : public Singleton<cKeyboard> {
	cKeyboard() {}
	friend Singleton< cKeyboard >;
public:
	void loadKeyImage();
	void reloadKeyImage();
	cImage keyImage(int keyCode);
	cBox& keyBox(int keyCode);
	bool update();
	int pressKeyCount(int keyCode);
	int pressKeyState(int keyCode);
	static constexpr int KEY_NUM = 256;
	static constexpr int VALID_KEY_NUM = 67;
private:
	int mPressKeyCount[KEY_NUM] = {}; // positive: pressed, negative: released
	int mPressKeyState[KEY_NUM] = {};
	static constexpr int KeyNo[VALID_KEY_NUM] = {
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 28, 29, 30, 31,
		32, 33, 34, 35, 36, 37, 38, 42, 44, 45, 46, 47, 48, 49, 50, 54, 56, 57, 58, 59, 60, 61, 62, 63, 64, 
		65, 66, 67, 68, 87, 88, 157, 184, 197, 200, 203, 205, 208, 210, 211 };
	cImage mKeyImage[KEY_NUM] = {};
};