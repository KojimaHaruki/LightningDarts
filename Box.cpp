#include "Box.hpp"

cBox::cBox() : mX1(0), mY1(0), mX2(0), mY2(0), mLineW(1), mColor(0U), mFillFlag(TRUE), mDrawFlag(TRUE) {
}

void cBox::setCX(int x) {
    int w = W(); mX1 = x - w / 2; mX2 = x + w / 2;
}

void cBox::setCY(int y) {
    int h = H(); mY1 = y - h / 2; mY2 = y + h / 2;
}

bool cBox::isInBox(cCoordinate2d<int> P) {
    return P.x() > mX1 && P.x() < mX2 && P.y() > mY1 && P.y() < mY2;
}

bool cBox::isOnBoxEdge(cCoordinate2d<int> P) {
    if (mLineW == 1) {
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
    if (P.x() >= mX1 - mLineW / 2 && P.x() <= mX2 + mLineW / 2) {
        if (P.y() >= mY1 - mLineW / 2 && P.y() <= mY1 + mLineW / 2) {
            return true;
        }
        if (P.y() >= mY2 - mLineW / 2 && P.y() <= mY2 + mLineW / 2) {
            return true;
        }
    }
    if (P.y() >= mY1 - mLineW / 2 && P.y() <= mY2 + mLineW / 2) {
        if (P.x() >= mX1 - mLineW / 2 && P.x() <= mX1 + mLineW / 2) {
            return true;
        }
        if (P.x() >= mX2 - mLineW / 2 && P.x() <= mX2 + mLineW / 2) {
            return true;
        }
    }
    return false;
}

int cBox::draw() {
    if (!mDrawFlag) return -1;
    return DrawBox(mX1, mY1, mX2, mY2, mColor, mFillFlag, mLineW);
}

int cBox::fill() {
    if (!mDrawFlag) return -1;
    return DrawBox(mX1, mY1, mX2, mY2, mColor, TRUE, mLineW);
}

int cBox::unfill() {
    if (!mDrawFlag) return -1;
    return DrawBox(mX1, mY1, mX2, mY2, mColor, FALSE, mLineW);
}