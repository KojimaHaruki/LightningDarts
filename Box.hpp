#pragma once
#include "Coordinate.hpp"
#include "DxLib.h"

class cBox {
    int mX1 = 0, mY1 = 0, mX2 = 0, mY2 = 0;
    int mLineW = 1;
    unsigned int mColor = 0U;
    int mFillFlag = TRUE, mDrawFlag = TRUE;
public:
    cBox();
    ~cBox() {}

    // Setters
    void setW(int w) { mX2 = mX1 + w; } // width
    void setH(int h) { mY2 = mY1 + h; } // height
    void setS(int w, int h) { setW(w); setH(h); } // size
    void setS(cCoordinate2d<int> size) { setW(size.x()); setH(size.y()); } // size
    void setCX(int x); // x-coordinate of center
    void setCY(int y); // y-coordinate of center
    void setC(int x, int y) { setCX(x); setCY(y); } // center
    void setC(cCoordinate2d<int> C) { setCX(C.x()); setCY(C.y()); } // center
    void setX1(int x1) { mX1 = x1; } // left
    void setX2(int x2) { mX2 = x2; } // right
    void setX(int x1, int x2) { mX1 = x1; mX2 = x2; } // lateral
    void setY1(int y1) { mY1 = y1; } // upper
    void setY2(int y2) { mY2 = y2; } // lower
    void setY(int y1, int y2) { mY1 = y1; mY2 = y2; } // longitudinal
    void setXY1(int x1, int y1) { mX1 = x1; mY1 = y1; } // upper left
    void setXY2(int x2, int y2) { mX2 = x2; mY2 = y2; } // lower right
    void setXY(int x1, int y1, int x2, int y2) { mX1 = x1; mY1 = y1; mX2 = x2; mY2 = y2; } // box
    void setL(int x) { setX(x, x + W()); } // left
    void setR(int x) { setX(x - W(), x); } // right
    void setT(int y) { setY(y, y + H()); } // top
    void setB(int y) { setY(y - H(), y); } // bottom
    void setUL(int x, int y) { setL(x); setT(y); } // upper left
    void setUL(cCoordinate2d<int> P) { setL(P.x()); setT(P.y()); } // upper left
    void setLL(int x, int y) { setL(x); setB(y); } // lower left
    void setLL(cCoordinate2d<int> P) { setL(P.x()); setB(P.y()); } // lower left
    void setLR(int x, int y) { setR(x); setB(y); } // lower right
    void setLR(cCoordinate2d<int> P) { setR(P.x()); setB(P.y()); } // lower right
    void setUR(int x, int y) { setR(x); setT(y); } // upper right
    void setUR(cCoordinate2d<int> P) { setR(P.x()); setT(P.y()); } // upper right
    void setLineW(int width) { mLineW = (width > 0) ? width : 1; } // line width
    void setColor(unsigned int color) { mColor = color; }
    void setFillFlag(bool fill) { mFillFlag = fill; }
    void setDrawFlag(bool draw) { mDrawFlag = draw; }

    // getter
    cCoordinate2d<int> S() { cCoordinate2d<int> mS(W(), H()); return mS; } // size
    int W() { return mX2 - mX1; } // width
    int H() { return mY2 - mY1; } // height
    int CX() { return (mX1 + mX2) / 2; } // x-coordinate of center
    int CY() { return (mY1 + mY2) / 2; } // y-coordinate of center
    cCoordinate2d<int> C() { cCoordinate2d<int> mC(CX(), CY()); return mC; } // center
    int L() { return mX1; } // left
    int R() { return mX2; } // right
    int T() { return mY1; } // top
    int B() { return mY2; } // bottom
    cCoordinate2d<int> UL() { cCoordinate2d<int> P(mX1, mY1); return P; } // upper left
    cCoordinate2d<int> LL() { cCoordinate2d<int> P(mX1, mY2); return P; } // lower left
    cCoordinate2d<int> LR() { cCoordinate2d<int> P(mX2, mY2); return P; } // lower right
    cCoordinate2d<int> UR() { cCoordinate2d<int> P(mX2, mY1); return P; } // upper right
    int lineW() { return mLineW; }
    unsigned int color() { return mColor; }
    int fillFlag() { return mFillFlag; }

    // Functions
    bool isInBox(cCoordinate2d<int> P);
    bool isOnBoxEdge(cCoordinate2d<int> P);
    int draw();
    int fill();
    int unfill();
};
