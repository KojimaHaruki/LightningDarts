#include "Image.hpp"

cImage::cImage() : mPath(), mName(), mType(), isResource(false), mHandle(-1), mTrans(true) {
}

void cImage::load(std::string path) { 
    mPath = path; isResource = false; mHandle = LoadGraph(mPath.c_str()); 
}

void cImage::load(std::string name, std::string type) {
    mName = name; mType = type; mPath = name + "." + type; isResource = false;
    mHandle = LoadGraph(mPath.c_str());
}

void cImage::load(int resource, std::string type) {
    mName = MAKEINTRESOURCE(resource); isResource = true; mType = type;
    mHandle = LoadGraphToResource(MAKEINTRESOURCE(resource), type.c_str());
}

bool cImage::reload() {
    if (isResource) mHandle = LoadGraphToResource(mName.c_str(), mType.c_str());
    else if (mPath.size() > 0) mHandle = LoadGraph(mPath.c_str());
    return mHandle != -1;
}

int cImage::draw() {
    int transFlag = mTrans ? TRUE : FALSE;
    return DrawGraph(mBox.left(), mBox.top(), mHandle, transFlag);
}

cImage::~cImage() { 
    DeleteGraph(mHandle); 
}