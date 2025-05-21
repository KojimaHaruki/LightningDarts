#include "Box.hpp"
Box::Box() {
}
void Box::setBox(int x1, int y1, int x2, int y2) {
    mBox.mUpperLeft.setXY(x1, y1); mBox.mUpperRight.setXY(x2, y1);
    mBox.mLowerLeft.setXY(x1, y2); mBox.mLowerRight.setXY(x2, y2);
    mBox.mSize.setXY(x2 - x1, y2 - y1);
    mBox.mCenter.setXY((x1 + x2) / 2, (y1 + y2) / 2);
    return;
}
void Box::setCenter(int x, int y) {
    mBox.mCenter.setXY(x, y);
    mBox.mUpperLeft.setXY(x - mBox.mSize.x() / 2, y - mBox.mSize.y() / 2);
    mBox.mLowerLeft.setXY(x - mBox.mSize.x() / 2, y + mBox.mSize.y() / 2);
    mBox.mUpperRight.setXY(x + mBox.mSize.x() / 2, y - mBox.mSize.y() / 2);
    mBox.mLowerRight.setXY(x + mBox.mSize.x() / 2, y + mBox.mSize.y() / 2);
    return;
}
void Box::setUpperLeft(int x, int y) {
    mBox.mUpperLeft.setXY(x, y);
    mBox.mLowerLeft.setXY(x, y + mBox.mSize.y());
    mBox.mUpperRight.setXY(x + mBox.mSize.x(), y);
    mBox.mLowerRight.setXY(x + mBox.mSize.x(), y + mBox.mSize.y());
    mBox.mCenter.setXY(x + mBox.mSize.x() / 2, y + mBox.mSize.y() / 2);
    return;
}
void Box::setLowerLeft(int x, int y) {
    mBox.mLowerLeft.setXY(x, y);
    mBox.mUpperLeft.setXY(x, y - mBox.mSize.y());
    mBox.mLowerRight.setXY(x + mBox.mSize.x(), y);
    mBox.mUpperRight.setXY(x + mBox.mSize.x(), y - mBox.mSize.y());
    mBox.mCenter.setXY(x + mBox.mSize.x() / 2, y - mBox.mSize.y() / 2);
    return;
}
void Box::setLowerRight(int x, int y) {
    mBox.mLowerRight.setXY(x, y);
    mBox.mUpperRight.setXY(x, y - mBox.mSize.y());
    mBox.mLowerLeft.setXY(x - mBox.mSize.x(), y);
    mBox.mUpperLeft.setXY(x - mBox.mSize.x(), y - mBox.mSize.y());
    mBox.mCenter.setXY(x - mBox.mSize.x() / 2, y - mBox.mSize.y() / 2);
    return;
}
void Box::setUpperRight(int x, int y) {
    mBox.mUpperRight.setXY(x, y);
    mBox.mLowerRight.setXY(x, y + mBox.mSize.y());
    mBox.mUpperLeft.setXY(x - mBox.mSize.x(), y);
    mBox.mLowerLeft.setXY(x - mBox.mSize.x(), y + mBox.mSize.y());
    mBox.mCenter.setXY(x - mBox.mSize.x() / 2, y + mBox.mSize.y() / 2);
    return;
}
bool Box::isInBox(Coordinate2d<int> P) {
    return P.x() > mBox.mUpperLeft.x() && P.y() > mBox.mUpperLeft.y() &&
        P.x() < mBox.mLowerRight.x() && P.y() < mBox.mLowerRight.y();
}
Box::~Box() {
}