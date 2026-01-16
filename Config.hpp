#pragma once
#include "Scene.hpp"
class Config : public cScene {
public:
    Config(int priorScene, ShareData sd);
    ~Config() {}
    void reset();
    void set();
    void update();
    void draw();
    void draw(sCtrl ctrl);
private:
    sCtrlKind ctrl;
};
