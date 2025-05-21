#pragma once
#include "Coordinate.hpp"
struct PixelBox {
    Coordinate2d<int> mSize, mCenter, mUpperLeft, mLowerLeft, mLowerRight, mUpperRight;
};
class Box {
private:
    PixelBox mBox;
public:
    Box();
    ~Box();
    inline void setSize(int x, int y) { mBox.mSize.setXY(x, y); return; }
    inline void setSize(Coordinate2d<int> xy) { mBox.mSize = xy; return; }
    void setBox(int x1, int y1, int x2, int y2);
    void setCenter(int x, int y);
    void setCenter(Coordinate2d<int> xy) { setCenter(xy.x(), xy.y()); return; }
    void setUpperLeft(int x, int y);
    void setUpperLeft(Coordinate2d<int> xy) { setUpperLeft(xy.x(), xy.y()); return; }
    void setLowerLeft(int x, int y);
    void setLowerLeft(Coordinate2d<int> xy) { setLowerLeft(xy.x(), xy.y()); return; }
    void setLowerRight(int x, int y);
    void setLowerRight(Coordinate2d<int> xy) { setLowerRight(xy.x(), xy.y()); return; }
    void setUpperRight(int x, int y);
    void setUpperRight(Coordinate2d<int> xy) { setUpperRight(xy.x(), xy.y()); return; }
    inline PixelBox box() { return mBox; }
    inline Coordinate2d<int> size() { return mBox.mSize; }
    inline Coordinate2d<int> center() { return mBox.mCenter; }
    inline Coordinate2d<int> upperLeft() { return mBox.mUpperLeft; }
    inline Coordinate2d<int> lowerLeft() { return mBox.mLowerLeft; }
    inline Coordinate2d<int> lowerRight() { return mBox.mLowerRight; }
    inline Coordinate2d<int> upperRight() { return mBox.mUpperRight; }
    inline int width() { return mBox.mSize.x(); }
    inline int height() { return mBox.mSize.y(); }
    inline int top() { return mBox.mUpperLeft.y(); }
    inline int bottom() { return mBox.mLowerLeft.y(); }
    inline int left() { return mBox.mUpperLeft.x(); }
    inline int right() { return mBox.mUpperRight.x(); }
    bool isInBox(Coordinate2d<int> P);
};
