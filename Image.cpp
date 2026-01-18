#include "Image.hpp"
#include "DxLib.h"

cImage::cImage() : mPath(), mName(), mType(), isResource(false), mHandle(0), mTrans(true) {
}

void cImage::load(std::string path) { 
    mPath = path; isResource = false; mHandle = LoadGraph(mPath.c_str()); 
}

void cImage::load(int resource, std::string type) {
    mResource = resource; isResource = true; mType = type;
    mHandle = LoadGraphToResource(MAKEINTRESOURCE(resource), type.c_str());
}

bool cImage::reload() {
    if (isResource) mHandle = LoadGraphToResource(MAKEINTRESOURCE(mResource), mType.c_str());
    else if (mPath.size() > 0) mHandle = LoadGraph(mPath.c_str());
    return mHandle != -1;
}

int cImage::draw() {
    int transFlag = mTrans ? TRUE : FALSE;
    return DrawGraph(mBox.left(), mBox.top(), mHandle, transFlag);
}

cImage::~cImage() {
}