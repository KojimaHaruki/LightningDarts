#pragma once
#include "Font.hpp"

int cFont::createFont(Data font) {
    return CreateFontToHandle(font.name.c_str(), font.size, font.thick, font.type, font.edge);
}

void cFont::load() {
    mXLFontHandle = createFont(mXLFont);
    mMFontHandle = createFont(mMFont);
    mSFontHandle = createFont(mSFont);
}