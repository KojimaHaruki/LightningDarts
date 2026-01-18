#include "Timer.hpp"

sTime cTimer::time(int t_ms) {
	sTime time; div_t res_h, res_m, res_s;
	res_h = div(t_ms, 3600000); time.h = res_h.quot;
	res_m = div(res_h.rem, 60000); time.m = res_m.quot;
	res_s = div(res_m.rem, 1000); time.s = res_s.quot;
	time.ms = res_s.rem; time.mt = t_ms; time.t = 0.001 * t_ms;
	return time;
}

int cTimer::drawLapseTime(int x, int y, unsigned int color, int fontHandle, int mode) {
	switch (mode) {
	case Mode::HMSmS:
		DrawFormatStringToHandle(x, y, color, fontHandle, 
			"%02d:%02d:%02d.%03d", mLapseTime.h, mLapseTime.m, mLapseTime.s, mLapseTime.ms); break;
	case Mode::MSmS:
		DrawFormatStringToHandle(x, y, color, fontHandle, 
			"%02d:%02d.%03d", 60 * mLapseTime.h + mLapseTime.m, mLapseTime.s, mLapseTime.ms); break;
	case Mode::SmS:
		DrawFormatStringToHandle(x, y, color, fontHandle, "%6.3lf", mLapseTime.t); break;
	case Mode::mS:
		DrawFormatStringToHandle(x, y, color, fontHandle, "%d ms", mLapseTime.mt); break;
	case Mode::HMS:
		DrawFormatStringToHandle(x, y, color, fontHandle, 
			"%02d:%02d:%02d", mLapseTime.h, mLapseTime.m, mLapseTime.s); break;
	case Mode::MS:
		DrawFormatStringToHandle(x, y, color, fontHandle, 
			"%02d:%02d", 60 * mLapseTime.h + mLapseTime.m, mLapseTime.s); break;
	case Mode::S:
		DrawFormatStringToHandle(x, y, color, fontHandle, "%d s", (int)mLapseTime.t); break;
	case Mode::HM:
		DrawFormatStringToHandle(x, y, color, fontHandle, "%02d:%02d", mLapseTime.h, mLapseTime.m); break;
	default: return -1;
	} return 0;
}