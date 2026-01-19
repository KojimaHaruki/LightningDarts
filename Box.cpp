#pragma once
#include "Box.hpp"
#include "DxLib.h"

void cBox::setCenterX(int x) { 
    int w = width(); mX1 = x - w / 2; mX2 = x + w / 2;
}

void cBox::setCenterY(int y) { 
    int h = height(); mY1 = y - h / 2; mY2 = y + h / 2;
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