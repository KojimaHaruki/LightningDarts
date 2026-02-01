#pragma once
#include "Singleton.hpp"
#include "Key.hpp"
#include "Image.hpp"

class cKeyboard : public Singleton<cKeyboard> {
	cKeyboard() {}
	friend Singleton< cKeyboard >;

public:
	static constexpr int KEY_NUM = 256;
	static constexpr int VALID_KEY_NUM = 67;
	static constexpr int TYPE_KEY_STATE = sKeyState::RELEASEtoPRESS;

	bool isValidKey(int key) { return 0 <= key && key < KEY_NUM; }

	void loadKeyImage();
	void reloadKeyImage();
	cImage& keyImage(int key);
	cBox& keyBox(int key);
	
	void update();

	int keyCount(int key);
	int keyState(int key);
	bool isKeyTyped(int key);

private:
	static constexpr int KeyNo[VALID_KEY_NUM] = {
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 28, 29, 30, 31,
		32, 33, 34, 35, 36, 37, 38, 42, 44, 45, 46, 47, 48, 49, 50, 54, 56, 57, 58, 59, 60, 61, 62, 63, 64, 
		65, 66, 67, 68, 87, 88, 157, 184, 197, 200, 203, 205, 208, 210, 211 };
	
	struct sKey {
		int count = 0; // positive: pressed, negative: released
		cImage image;
	};
	sKey keys[KEY_NUM];
};