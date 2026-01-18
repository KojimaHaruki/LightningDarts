#pragma once
#include "Coordinate.hpp"

class cBox {
public:
    cBox() : mX1(0), mY1(0), mX2(0), mY2(0), mLineWidth(1), mColor(0U), mFill(true) {}
    ~cBox() {}

    // setter
    void setSize(int width, int height) { mX2 = mX1 + width; mY2 = mY1 + height; }
    void setSize(cCoordinate2d<int> size) { setSize(size.x(), size.y()); }
    void setWidth(int width) { mX2 = mX1 + width; }
    void setHeight(int height) { mY2 = mY1 + height; }
    void setBox(int x1, int y1, int x2, int y2) { mX1 = x1; mY1 = y1; mX2 = x2; mY2 = y2; }
    void setCenter(int x, int y);
    void setCenter(cCoordinate2d<int> C) { setCenter(C.x(), C.y()); }
    void setCenterX(int x);
    void setCenterY(int y);
    void setUpperLeft(int x, int y);
    void setUpperLeft(cCoordinate2d<int> P) { setUpperLeft(P.x(), P.y()); }
    void setLowerLeft(int x, int y);
    void setLowerLeft(cCoordinate2d<int> P) { setLowerLeft(P.x(), P.y()); }
    void setLowerRight(int x, int y);
    void setLowerRight(cCoordinate2d<int> P) { setLowerRight(P.x(), P.y()); }
    void setUpperRight(int x, int y);
    void setUpperRight(cCoordinate2d<int> P) { setUpperRight(P.x(), P.y()); }
    void setX(int x) { mX2 = x + width(); mX1 = x; }
    void setY(int y) { mY2 = y + height(); mY1 = y; }
    void setLeft(int x) { mX2 = x + width(); mX1 = x; }
    void setRight(int x) { mX1 = x - width(); mX2 = x; }
    void setTop(int y) { mY2 = y + height(); mY1 = y; }
    void setBottom(int y) { mY1 = y - height(); mY2 = y; }
    void setLineWidth(int width) { mLineWidth = (width > 0) ? width : 1; }
    void setColor(unsigned int color) { mColor = color; }
    void setFill(bool fill) { mFill = fill; }
    void fill() { mFill = true; }
    void unfill() { mFill = false; }

    // getter
    cCoordinate2d<int> size() { cCoordinate2d<int> S(width(), height()); return S; }
    int width() { return mX2 - mX1; }
    int height() { return mY2 - mY1; }
    cCoordinate2d<int> center();
    int centerX() { return (mX1 + mX2) / 2; }
    int centerY() { return (mY1 + mY2) / 2; }
    cCoordinate2d<int> upperLeft();
    cCoordinate2d<int> lowerLeft();
    cCoordinate2d<int> lowerRight();
    cCoordinate2d<int> upperRight();
    int x() { return mX1; }
    int y() { return mY1; }
    int left() { return mX1; }
    int right() { return mX2; }
    int top() { return mY1; }
    int bottom() { return mY2; }
    int lineWidth() { return mLineWidth; }
    unsigned int color() { return mColor; }

    // functions
    bool isInBox(cCoordinate2d<int> P);
    bool isOnBoxEdge(cCoordinate2d<int> P);
    int draw();
    
private:
    int mX1 = 0, mY1 = 0, mX2 = 0, mY2 = 0, mLineWidth = 1;
    unsigned int mColor = 0U;
    bool mFill = true;
};
