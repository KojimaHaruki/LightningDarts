#pragma once
#include "Singleton.hpp"
#include <string>
#include "DxLib.h"

class cFont : public Singleton<cFont> {
    cFont() {}
    friend class Singleton<cFont>;
    int mXLFontHandle, mMFontHandle, mSFontHandle;
    struct Data {
        std::string name;
        int size = 0;
        int thick = 0;
        int type = 0;
        int edge = 0;
    };
    const Data mXLFont = { "Pristina", 60, 5, DX_FONTTYPE_ANTIALIASING_EDGE, 0 },
        mMFont = { "Times New Roman Bold", 18, 5, DX_FONTTYPE_ANTIALIASING_EDGE, 0 },
        mSFont = { "Times New Roman Bold", 14, 1, DX_FONTTYPE_ANTIALIASING_EDGE, 0 };
    // Functions
    int createFont(Data font) {
        return CreateFontToHandle(font.name.c_str(), font.size, font.thick, font.type, font.edge);
    }
public:
    void load() {
        mXLFontHandle = createFont(mXLFont);
        mMFontHandle = createFont(mMFont);
        mSFontHandle = createFont(mSFont);
    }
	int XLfont() { return mXLFontHandle; }
	int MFont() { return mMFontHandle; }
	int Sfont() { return mSFontHandle; }
    int XLfontSize() { return mXLFont.size; }
    int MfontSize() { return mMFont.size; }
    int SfontSize() { return mSFont.size; }
};