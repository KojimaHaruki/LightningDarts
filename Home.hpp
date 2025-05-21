#pragma once
#include "Scene.hpp"
class Home : public Scene {
public:
    Home();
    Home(ShareData shareData);
    ~Home();
    void draw();
    void update();
private:
    const std::string wday[7] = { "Sun","Mon","Tue","Wed","Thu", "Fri","Sat" };
    time_t nowTime;
    tm nowLocalTime;
    errno_t timeError;
};