#pragma once
#include "Scene.hpp"
#include "Control.hpp"

class Config : public cScene {
public:
    Config(int priorScene, ShareData sd);
    ~Config() {}
    void reset();
    void set();
    void update();
    void draw();
    void draw(int ctrl);
private:
    cImage mIcon[cControl::NUM];
};
