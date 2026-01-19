#pragma once
#include "BaseScene.hpp"

class cHome : public cBaseScene {
public:
    cHome();
    ~cHome() {}
    void draw();
    void update();
private:
    const std::string wday[7] = { "Sun","Mon","Tue","Wed","Thu", "Fri","Sat" };
};