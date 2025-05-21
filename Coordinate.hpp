#pragma once
#include <cmath>
template<typename T> struct XY {
    T x, y;
};
template<typename T> struct Polar {
    T r, theta;
};
template<typename T> class Coordinate2d {
private:
    XY<T> mXY;
    Polar<T> mPolar;
public:
	Coordinate2d() : mXY(), mPolar() {};
    Coordinate2d(T x, T y) : mPolar() { mXY.x = x; mXY.y; };
	~Coordinate2d () {};
    inline void setXY(T x, T y) { mXY.x = x; mXY.y = y; return; }
    inline void setPolar(T r, T theta) { mXY.x = r * cos(theta); mXY.y = r * sin(theta); return; }
    inline void setX(T x) { mXY.x = x; return; }
    inline void setY(T y) { mXY.y = y; return; }
    inline XY<T> xy() { return mXY; }
    inline T x() { return mXY.x; }
    inline T y() { return mXY.y; }
    Polar<T> polar() { 
        mPolar.r = sqrt(mXY.x * mXY.x + mXY.y * mXY.y); mPolar.theta = atan2(mXY.y, mXY.x); return mPolar; 
    }
};
