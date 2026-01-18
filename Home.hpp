#pragma once
#include "Scene.hpp"

class Home : public cScene {
public:
    Home();
    Home(ShareData shareData);
    ~Home() {}
    void draw();
    void update();
private:
    const std::string wday[7] = { "Sun","Mon","Tue","Wed","Thu", "Fri","Sat" };
};