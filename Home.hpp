#pragma once
#include "Scene.hpp"
class Home : public Scene {
public:
    Home();
    Home(ShareData shareData);
    ~Home();
    void draw();
    void update();
};