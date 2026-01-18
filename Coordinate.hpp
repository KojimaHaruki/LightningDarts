#pragma once
#define _USE_MATH_DEFINES
#include <cmath>

template<typename T> class Coordinate2d {
private:
    T mX, mY;
public:
	Coordinate2d() : mX(), mY() {};
    Coordinate2d(T x, T y) { mX = x; mY = y; };
	~Coordinate2d () {};
    void setXY(T x, T y) { mX = x; mY = y; return; }
    void setPolar(T r, T theta) { mX = r * cos(theta); mY = r * sin(theta); return; }
    void setX(T x) { mX = x; return; }
    void setY(T y) { mY = y; return; }
    T x() { return mX; }
    T y() { return mY; }
    T r() { return sqrt(mX * mX + mY * mY); }
    T theta() { return fmod(atan2(mX, mY) + M_PI, M_PI); }
};
