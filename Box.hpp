#pragma once
#include "Coordinate.hpp"

class cBox {
    int mX1 = 0, mY1 = 0, mX2 = 0, mY2 = 0;
    int mLineWidth = 1;
    unsigned int mColor = 0U;
    bool mFill = true, mDraw = true;
public:
    cBox() : mX1(0), mY1(0), mX2(0), mY2(0), mLineWidth(1), mColor(0U), mFill(true), mDraw(true) {}
    ~cBox() {}

    // Setters
    void setWidth(int w) { mX2 = mX1 + w; }
    void setHeight(int h) { mY2 = mY1 + h; }
    void setSize(int w, int h) { setWidth(w); setHeight(h); }
    void setSize(cCoordinate2d<int> size) { setWidth(size.x()); setHeight(size.y()); }
    void setCenterX(int x);
    void setCenterY(int y);
    void setCenter(int x, int y) { setCenterX(x); setCenterY(y); }
    void setCenter(cCoordinate2d<int> C) { setCenterX(C.x()); setCenterY(C.y()); }
    void setX1(int x1) { mX1 = x1; }
    void setX2(int x2) { mX2 = x2; }
    void setX(int x1, int x2) { mX1 = x1; mX2 = x2; }
    void setY1(int y1) { mY1 = y1; }
    void setY2(int y2) { mY2 = y2; }
    void setY(int y1, int y2) { mY1 = y1; mY2 = y2; }
    void setXY1(int x1, int y1) { mX1 = x1; mY1 = y1; }
    void setXY2(int x2, int y2) { mX2 = x2; mY2 = y2; }
    void setXY(int x1, int y1, int x2, int y2) { mX1 = x1; mY1 = y1; mX2 = x2; mY2 = y2; }
    void setLeft(int x) { setX(x, x + width()); }
    void setRight(int x) { setX(x - width(), x); }
    void setTop(int y) { setY(y, y + height()); }
    void setBottom(int y) { setY(y - height(), y); }
    void setUpperLeft(int x, int y) { setLeft(x); setTop(y); }
    void setUpperLeft(cCoordinate2d<int> P) { setLeft(P.x()); setTop(P.y()); }
    void setLowerLeft(int x, int y) { setLeft(x); setBottom(y); }
    void setLowerLeft(cCoordinate2d<int> P) { setLeft(P.x()); setBottom(P.y()); }
    void setLowerRight(int x, int y) { setRight(x); setBottom(y); }
    void setLowerRight(cCoordinate2d<int> P) { setRight(P.x()); setBottom(P.y()); }
    void setUpperRight(int x, int y) { setRight(x); setTop(y); }
    void setUpperRight(cCoordinate2d<int> P) { setRight(P.x()); setTop(P.y()); }
    void setLineWidth(int width) { mLineWidth = (width > 0) ? width : 1; }
    void setColor(unsigned int color) { mColor = color; }
    void setFill(bool fill) { mFill = fill; }
	void setDraw(bool draw) { mDraw = draw; }

    // getter
    cCoordinate2d<int> size() { cCoordinate2d<int> S(width(), height()); return S; }
    int width() { return mX2 - mX1; }
    int height() { return mY2 - mY1; }
    int centerX() { return (mX1 + mX2) / 2; }
    int centerY() { return (mY1 + mY2) / 2; }
    cCoordinate2d<int> center(){ cCoordinate2d<int> C(centerX(), centerY()); return C; }
    int left() { return mX1; }
    int right() { return mX2; }
    int top() { return mY1; }
    int bottom() { return mY2; }
    cCoordinate2d<int> upperLeft() { cCoordinate2d<int> P(mX1, mY1); return P; }
    cCoordinate2d<int> lowerLeft() { cCoordinate2d<int> P(mX1, mY2); return P; }
    cCoordinate2d<int> lowerRight() { cCoordinate2d<int> P(mX2, mY2); return P; }
    cCoordinate2d<int> upperRight() { cCoordinate2d<int> P(mX2, mY1); return P; }
    int lineWidth() { return mLineWidth; }
    unsigned int color() { return mColor; }

    // Functions
    bool isInBox(cCoordinate2d<int> P);
    bool isOnBoxEdge(cCoordinate2d<int> P);
    int draw();
    int fill();
    int unfill();
};
