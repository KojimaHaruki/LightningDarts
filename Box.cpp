#pragma once
#include "Box.hpp"
#include "DxLib.h"

void cBox::setCenter(int x, int y) {
    int w = width(), h = height(); setBox(x - w / 2, y - h / 2, x + w / 2, y + h / 2);
}

void cBox::setCenterX(int x) { 
    int w = width(); mX1 = x - w / 2; mX2 = x + w / 2;
}

void cBox::setCenterY(int y) { 
    int h = height(); mY1 = y - h / 2; mY2 = y + h / 2;
}

void cBox::setUpperLeft(int x, int y) {
    int w = width(), h = height(); setBox(x, y, x + w, y + h);
}

void cBox::setLowerLeft(int x, int y) {
    int w = width(), h = height(); setBox(x, y - h, x + w, y);
}

void cBox::setLowerRight(int x, int y) {
    int w = width(), h = height(); setBox(x - w, y - h, x, y);
}

void cBox::setUpperRight(int x, int y) {
    int w = width(), h = height(); setBox(x - w, y, x, y + h);
}

cCoordinate2d<int> cBox::center() { 
    cCoordinate2d<int> C(centerX(), centerY()); return C; 
}

cCoordinate2d<int> cBox::upperLeft() { 
    cCoordinate2d<int> P(mX1, mY1); return P; 
}

cCoordinate2d<int> cBox::lowerLeft() { 
    cCoordinate2d<int> P(mX1, mY1 + height()); return P; 
}

cCoordinate2d<int> cBox::lowerRight() { 
    cCoordinate2d<int> P(mX1 + width(), mY1 + height()); return P; 
}

cCoordinate2d<int> cBox::upperRight() { 
    cCoordinate2d<int> P(mX1 + width(), mY1); return P; 
}

bool cBox::isInBox(cCoordinate2d<int> P) { 
    return P.x() > mX1 && P.x() < mX2 && P.y() > mY1 && P.y() < mY2; 
}

bool cBox::isOnBoxEdge(cCoordinate2d<int> P) {
    if (mLineWidth == 1) {
        if (P.x() >= mX1 && P.x() <= mX2) {
            if (P.y() == mY1 || P.y() == mY2) {
                return true;
            }
        }
        if (P.y() >= mY1 && P.y() <= mY2) {
            if (P.x() == mX1 || P.x() == mX2) {
                return true;
            }
        }
        return false;
    }
    if (P.x() >= mX1 - mLineWidth / 2 && P.x() <= mX2 + mLineWidth / 2) {
        if (P.y() >= mY1 - mLineWidth / 2 && P.y() <= mY1 + mLineWidth / 2) {
            return true;
        }
        if (P.y() >= mY2 - mLineWidth / 2 && P.y() <= mY2 + mLineWidth / 2) {
            return true;
        }
    }
    if (P.y() >= mY1 - mLineWidth / 2 && P.y() <= mY2 + mLineWidth / 2) {
        if (P.x() >= mX1 - mLineWidth / 2 && P.x() <= mX1 + mLineWidth / 2) {
            return true;
        }
        if (P.x() >= mX2 - mLineWidth / 2 && P.x() <= mX2 + mLineWidth / 2) {
            return true;
        }
    }
    return false;
}

int cBox::draw() {
    int fillFlag = mFill ? TRUE : FALSE;
    return DrawBox(mX1, mY1, mX2, mY2, mColor, fillFlag, mLineWidth);
}