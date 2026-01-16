#include "Color.hpp"
#include "colorlib.hpp"
namespace cl = colorlib;

void cColor::load() {
    mWhite = cl::srgb("w"); mBlack = cl::srgb("k"); mGray = cl::srgb("AshGrey");
	mBrown = cl::srgb("CocoaBrown"); mRed = cl::srgb("r"); mGreen = cl::srgb("g"); mBlue = cl::srgb("b");
    mMagenta = cl::srgb("m"); mCyan = cl::srgb("c"); mYellow = cl::srgb("y");
	mTableColor = mBrown; mTouchColor = mYellow; mPressColor = cl::srgb("Strawberry");
    mExecuteColor = cl::srgb("Violet");
    mTeamColor[0] = mBlue; mTeamColor[1] = mRed; mTeamColor[2] = mGreen; mTeamColor[3] = mYellow; mTeamColor[4] = mCyan; mTeamColor[5] = mMagenta;
    mTeamColor[6] = mExecuteColor; mTeamColor[7] = cl::srgb("Marigold");
    mRankColor[0] = cl::srgb("GoldenYellow"); mRankColor[1] = cl::srgb("SilverGray");
    mRankColor[2] = cl::srgb("Bronze"); mRankColor[3] = cl::srgb("TurquoiseBlue");
    mRankColor[4] = cl::srgb("CobaltBlue"); mRankColor[5] = cl::srgb("SaxBlue");
    mRankColor[6] = cl::srgb("NavyBlue"); mRankColor[7] = cl::srgb("IronBlue");
}