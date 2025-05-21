#pragma once
#include "DxLib.h"
struct Time {
	int h = 0; // hour
	int m = 0; // min
	int s = 0; // second
	int ms = 0; // millisecond
	double t = 0; // total second
	int mt = 0; // total millisecond
};
class Timer {
public:
	Timer();
	~Timer();
	Time time(int t_ms);
	inline Time GetLapseTime() { return lapseTime; }
	inline int GetLapseCount() { return lapseTime.mt; }
	int drawLapseTime(int x, int y, unsigned int Color, int Mode = 0);
	int drawLapseTime(int x, int y, unsigned int Color, int FontHandle, int Mode = 0);
	void update();
	inline void reset()  { pause = TRUE; lapseTime = {}; return; }
	inline void start()  { pause = FALSE; startTime = GetNowCount(); return; }
	inline void stop()   { pause = TRUE; return; }
	inline void resume() { pause = FALSE; startTime = GetNowCount() - lapseTime.mt; return; }
	inline int isPaused() { return pause; }
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
private:
	int startTime;
	Time lapseTime;
	int pause;
};



