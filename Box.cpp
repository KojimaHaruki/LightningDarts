#include "Box.hpp"
Box::Box() {
}
void Box::setBox(int x1, int y1, int x2, int y2) {
    mBox.mUpperLeft._Val[0] = x1; mBox.mUpperLeft._Val[1] = y1;
    mBox.mUpperRight._Val[0] = x2; mBox.mUpperRight._Val[1] = y1;
    mBox.mLowerLeft._Val[0] = x1; mBox.mLowerLeft._Val[1] = y2;
    mBox.mLowerRight._Val[0] = x2; mBox.mLowerRight._Val[1] = y2;
    mBox.mSize._Val[0] = x2 - x1; mBox.mSize._Val[1] = y2 - y1;
    mBox.mCenter._Val[0] = (x1 + x2) / 2; mBox.mCenter._Val[1] = (y1 + y2) / 2;
    return;
}
void Box::setCenter(int x, int y) {
    mBox.mCenter._Val[0] = x; mBox.mCenter._Val[1] = y;
    mBox.mUpperLeft._Val[0] = x - real(mBox.mSize) / 2; mBox.mUpperLeft._Val[1] = y - imag(mBox.mSize) / 2;
    mBox.mLowerLeft._Val[0] = x - real(mBox.mSize) / 2; mBox.mLowerLeft._Val[1] = y + imag(mBox.mSize) / 2;
    mBox.mUpperRight._Val[0] = x + real(mBox.mSize) / 2; mBox.mUpperRight._Val[1] = y - imag(mBox.mSize) / 2;
    mBox.mLowerRight._Val[0] = x + real(mBox.mSize) / 2; mBox.mLowerRight._Val[1] = y + imag(mBox.mSize) / 2;
    return;
}
void Box::setUpperLeft(int x, int y) {
    mBox.mUpperLeft._Val[0] = x; mBox.mUpperLeft._Val[1] = y;
    mBox.mLowerLeft._Val[0] = x; mBox.mLowerLeft._Val[1] = y + imag(mBox.mSize);
    mBox.mUpperRight._Val[0] = x + real(mBox.mSize); mBox.mUpperRight._Val[1] = y;
    mBox.mLowerRight._Val[0] = x + real(mBox.mSize); mBox.mLowerRight._Val[1] = y + imag(mBox.mSize);
    mBox.mCenter._Val[0] = x + real(mBox.mSize) / 2; mBox.mCenter._Val[1] = y + imag(mBox.mSize) / 2;
    return;
}
void Box::setLowerLeft(int x, int y) {
    mBox.mLowerLeft._Val[0] = x; mBox.mLowerLeft._Val[1] = y;
    mBox.mUpperLeft._Val[0] = x; mBox.mUpperLeft._Val[1] = y - imag(mBox.mSize);
    mBox.mLowerRight._Val[0] = x + real(mBox.mSize); mBox.mLowerRight._Val[1] = y;
    mBox.mUpperRight._Val[0] = x + real(mBox.mSize); mBox.mUpperRight._Val[1] = y - imag(mBox.mSize);
    mBox.mCenter._Val[0] = x + real(mBox.mSize) / 2; mBox.mCenter._Val[1] = y - imag(mBox.mSize) / 2;
    return;
}
void Box::setLowerRight(int x, int y) {
    mBox.mLowerRight._Val[0] = x; mBox.mLowerRight._Val[1] = y;
    mBox.mUpperRight._Val[0] = x; mBox.mUpperRight._Val[1] = y - imag(mBox.mSize);
    mBox.mLowerLeft._Val[0] = x - real(mBox.mSize); mBox.mLowerLeft._Val[1] = y;
    mBox.mUpperLeft._Val[0] = x - real(mBox.mSize); mBox.mUpperLeft._Val[1] = y - imag(mBox.mSize);
    mBox.mCenter._Val[0] = x - real(mBox.mSize) / 2; mBox.mCenter._Val[1] = y - imag(mBox.mSize) / 2;
    return;
}
void Box::setUpperRight(int x, int y) {
    mBox.mUpperRight._Val[0] = x; mBox.mUpperRight._Val[1] = y;
    mBox.mLowerRight._Val[0] = x; mBox.mLowerRight._Val[1] = y + imag(mBox.mSize);
    mBox.mUpperLeft._Val[0] = x - real(mBox.mSize); mBox.mUpperLeft._Val[1] = y;
    mBox.mLowerLeft._Val[0] = x - real(mBox.mSize); mBox.mLowerLeft._Val[1] = y + imag(mBox.mSize);
    mBox.mCenter._Val[0] = x - real(mBox.mSize) / 2; mBox.mCenter._Val[1] = y + imag(mBox.mSize) / 2;
    return;
}
bool Box::isInBox(std::complex<int> P) {
    return real(P) > real(mBox.mUpperLeft) && imag(P) > imag(mBox.mUpperLeft) &&
        real(P) < real(mBox.mLowerRight) && imag(P) < imag(mBox.mLowerRight);
}
Box::~Box() {
}