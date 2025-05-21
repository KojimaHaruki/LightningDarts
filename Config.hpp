#pragma once
#include "Scene.hpp"
class Config : public Scene {
public:
    Config(int priorScene, ShareData sd);
    ~Config();
    void reset();
    void set();
    void update();
    void draw();
    void draw(Ctrl ctrl);
private:
    CtrlKind ctrl;
};
