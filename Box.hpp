#pragma once
#include <complex>
struct PixelBox {
    std::complex<int> mSize, mCenter, mUpperLeft, mLowerLeft, mLowerRight, mUpperRight;
};
class Box {
private:
    PixelBox mBox;
public:
    Box();
    ~Box();
    const void setSize(int x, int y) { mBox.mSize._Val[0] = x; mBox.mSize._Val[1] = y; return; }
    const void setSize(std::complex<int> xy) { mBox.mSize = xy; return; }
    void setBox(int x1, int y1, int x2, int y2);
    void setCenter(int x, int y);
    void setCenter(std::complex<int> xy) { setCenter(real(xy), imag(xy)); return; }
    void setUpperLeft(int x, int y);
    void setUpperLeft(std::complex<int> xy) { setUpperLeft(real(xy), imag(xy)); return; }
    void setLowerLeft(int x, int y);
    void setLowerLeft(std::complex<int> xy) { setLowerLeft(real(xy), imag(xy)); return; }
    void setLowerRight(int x, int y);
    void setLowerRight(std::complex<int> xy) { setLowerRight(real(xy), imag(xy)); return; }
    void setUpperRight(int x, int y);
    void setUpperRight(std::complex<int> xy) { setUpperRight(real(xy), imag(xy)); return; }
    const PixelBox box() { return mBox; }
    const std::complex<int> size() { return mBox.mSize; }
    const std::complex<int> center() { return mBox.mCenter; }
    const std::complex<int> upperLeft() { return mBox.mUpperLeft; }
    const std::complex<int> lowerLeft() { return mBox.mLowerLeft; }
    const std::complex<int> lowerRight() { return mBox.mLowerRight; }
    const std::complex<int> upperRight() { return mBox.mUpperRight; }
    const int width() { return real(mBox.mSize); }
    const int height() { return imag(mBox.mSize); }
    const int top() { return imag(mBox.mUpperLeft); }
    const int bottom() { return imag(mBox.mLowerLeft); }
    const int left() { return real(mBox.mUpperLeft); }
    const int right() { return real(mBox.mUpperRight); }
    bool isInBox(std::complex<int> P);
};
