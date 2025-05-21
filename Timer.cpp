#include "Timer.hpp"
Timer::Timer() : startTime(0), lapseTime(), pause(TRUE) {}
void Timer::update() {
	if (pause) return; 
	lapseTime.mt = GetNowCount() - startTime; 
	lapseTime = time(lapseTime.mt); 
	return;
}
Time Timer::time(int t_ms) {
	Time time; div_t res_h, res_m, res_s;
	res_h = div(t_ms, 3600000); time.h = res_h.quot;
	res_m = div(res_h.rem, 60000); time.m = res_m.quot;
	res_s = div(res_m.rem, 1000); time.s = res_s.quot;
	time.ms = res_s.rem; time.mt = t_ms; time.t = 0.001 * t_ms;
	return time;
}
int Timer::drawLapseTime(int x, int y, unsigned int color, int mode) {
	switch (mode) {
	case Mode::HMSmS: 
		DrawFormatString(x, y, color, "%02d:%02d:%02d.%03d", lapseTime.h, lapseTime.m, lapseTime.s, lapseTime.ms); break;
	case Mode::MSmS: 
		DrawFormatString(x, y, color, "%02d:%02d.%03d", 60 * lapseTime.h + lapseTime.m, lapseTime.s, lapseTime.ms); break;
	case Mode::SmS: 
		DrawFormatString(x, y, color, "%6.3lf", lapseTime.t); break;
	case Mode::mS: 
		DrawFormatString(x, y, color, "%d ms", lapseTime.mt); break;
	case Mode::HMS: 
		DrawFormatString(x, y, color, "%02d:%02d:%02d", lapseTime.h, lapseTime.m, lapseTime.s); break;
	case Mode::MS: 
		DrawFormatString(x, y, color, "%02d:%02d", 60 * lapseTime.h + lapseTime.m, lapseTime.s); break;
	case Mode::S: 
		DrawFormatString(x, y, color, "%d s", (int)lapseTime.t); break;
	case Mode::HM: 
		DrawFormatString(x, y, color, "%02d:%02d", lapseTime.h, lapseTime.m); break;
	default: return -1;
	} return 0;
}
int Timer::drawLapseTime(int x, int y, unsigned int color, int fontHandle, int mode) {
	switch (mode) {
	case Mode::HMSmS:
		DrawFormatStringToHandle(x, y, color, fontHandle, 
			"%02d:%02d:%02d.%03d", lapseTime.h, lapseTime.m, lapseTime.s, lapseTime.ms); break;
	case Mode::MSmS:
		DrawFormatStringToHandle(x, y, color, fontHandle, 
			"%02d:%02d.%03d", 60 * lapseTime.h + lapseTime.m, lapseTime.s, lapseTime.ms); break;
	case Mode::SmS:
		DrawFormatStringToHandle(x, y, color, fontHandle, "%6.3lf", lapseTime.t); break;
	case Mode::mS:
		DrawFormatStringToHandle(x, y, color, fontHandle, "%d ms", lapseTime.mt); break;
	case Mode::HMS:
		DrawFormatStringToHandle(x, y, color, fontHandle, 
			"%02d:%02d:%02d", lapseTime.h, lapseTime.m, lapseTime.s); break;
	case Mode::MS:
		DrawFormatStringToHandle(x, y, color, fontHandle, 
			"%02d:%02d", 60 * lapseTime.h + lapseTime.m, lapseTime.s); break;
	case Mode::S:
		DrawFormatStringToHandle(x, y, color, fontHandle, "%d s", (int)lapseTime.t); break;
	case Mode::HM:
		DrawFormatStringToHandle(x, y, color, fontHandle, "%02d:%02d", lapseTime.h, lapseTime.m); break;
	default: return -1;
	} return 0;
}
Timer::~Timer() {}