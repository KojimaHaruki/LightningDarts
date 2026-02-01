#pragma once
#include <string>
#include "Box.hpp"

class cImage {
public:
    cImage();
    ~cImage();

    // setter
    void setTrans(bool trans) { mTrans = trans; }
    
    // getter
    cBox &box() { return mBox; }
    int handle() { return mHandle; }

    // fuctions
    void load(std::string path);
    void load(int resource, std::string type);
    bool reload();
    int draw();

private:
    std::string mPath = {}, mName = {}, mType = {};
    bool isResource = false;
    int mResource = 0;
    cBox mBox;
    int mHandle = -1;
    bool mTrans = true;
};