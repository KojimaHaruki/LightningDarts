#pragma once
#define _USE_MATH_DEFINES
#include "math.h"

template<typename T> class cCoordinate2d {
private:
    T mX, mY;
public:
	cCoordinate2d() : mX(), mY() {};
    cCoordinate2d(T x, T y) { mX = x; mY = y; };
	~cCoordinate2d () {};
    void setXY(T x, T y) { mX = x; mY = y; return; }
    void setPolar(T r, T theta) { mX = r * cos(theta); mY = r * sin(theta); return; }
    void setX(T x) { mX = x; return; }
    void setY(T y) { mY = y; return; }
    T x() { return mX; }
    T y() { return mY; }
    T r() { return sqrt(mX * mX + mY * mY); }
    T theta() { return fmod(atan2(mX, mY) + M_PI, M_PI); }
};
