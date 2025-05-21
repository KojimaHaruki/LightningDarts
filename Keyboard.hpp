#pragma once

#include "Singleton.hpp"
#include "Key.hpp"

class Keyboard : public Singleton<Keyboard> {
	Keyboard();
	friend Singleton< Keyboard >;
public:
	bool update();
	int getPressCount(int keyCode);
	int getPressState(int keyCode);
	static const int KEY_NUM = 256;
private:
	Key key[KEY_NUM];
	inline bool isValidKey(int keyCode) { return keyCode >= 0 && keyCode < KEY_NUM; }
};