#pragma once
#include "BaseScene.hpp"
#include "Control.hpp"

class cConfig : public cBaseScene {
public:
    cConfig();
    ~cConfig() {}
    void reset();
    void set();
    void update();
    void draw();
    void draw(int ctrl);
private:
    cImage mIcon[cControl::NUM];
};
