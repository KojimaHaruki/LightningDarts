#pragma once
#include <string>
#include "DxLib.h"
#include "Box.hpp"

class cImage {
public:
    cImage();
    ~cImage();

    // setter
    void setBox(cBox box) { mBox = box; }
    void setTrans(bool trans) { mTrans = trans; }
    
    // getter
    cBox box() { return mBox; }

    // fuctions
    void load(std::string path);
    void load(std::string name, std::string type);
    void load(int resource, std::string type);
    bool reload();
    int draw();

private:
    std::string mPath = {}, mName = {}, mType = {};
    bool isResource = false;
    cBox mBox;
    int mHandle = -1;
    bool mTrans = true;
};