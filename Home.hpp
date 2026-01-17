#pragma once
#include "Scene.hpp"
class Home : public cScene {
public:
    Home();
    Home(ShareData shareData);
    ~Home() {}
    void draw();
    void update();
};