#pragma once
#include "Singleton.hpp"
#include "DxLib.h"

struct sTime {
	int h = 0; // hour
	int m = 0; // min
	int s = 0; // second
	int ms = 0; // millisecond
	double t = 0; // total second
	int mt = 0; // total millisecond
};

class cTimer : public Singleton<cTimer> {
	cTimer() {}
	friend class Singleton<cTimer>;
	int startTime = 0;
	sTime mLapseTime;
	bool pause = true;
public:
	sTime time(int t_ms);
	sTime lapseTime() { return mLapseTime; }
	int lapseCount() { return mLapseTime.mt; }
	int drawLapseTime(int x, int y, unsigned int Color, int Mode = 0);
	int drawLapseTime(int x, int y, unsigned int Color, int FontHandle, int Mode = 0);
	void update();
	void reset()  { pause = true; mLapseTime = {}; }
	void start()  { pause = false; startTime = GetNowCount(); }
	void restart() { reset(); start(); }
	void stop()   { pause = true; }
	void resume() { pause = false; startTime = GetNowCount() - mLapseTime.mt; }
	bool isPaused() { return pause; }
	struct Mode {
		static constexpr int HMSmS = 0;
		static constexpr int MSmS = 1;
		static constexpr int SmS = 2;
		static constexpr int mS = 3;
		static constexpr int HMS = 4;
		static constexpr int MS = 5;
		static constexpr int S = 6;
		static constexpr int HM = 7;
	};
};
