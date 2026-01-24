#pragma once
#include "BaseScene.hpp"

class cInitialize : public cBaseScene {
public:
    cInitialize();
    ~cInitialize() {}
    void draw();
    void update();
private:
    int cpp = 0, dxlib = 0;
};