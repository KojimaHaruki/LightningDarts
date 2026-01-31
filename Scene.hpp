#pragma once
#include "Singleton.hpp"
#include "BaseScene.hpp"

class cScene : public Singleton<cScene> {
    cScene() {}
    friend class Singleton<cScene>;
    int mLastScene = INITIALIZE, mCurrentScene = INITIALIZE, mMaxScene = INITIALIZE;
    cBaseScene* mScene;

public:
    static constexpr int INITIALIZE = -1;
    static constexpr int QUIT = 0;
    static constexpr int CONFIG = 1;
    static constexpr int HOME = 2;
    static constexpr int GAME_SELECT = 3;
    static constexpr int PLAYER_SELECT = 4;
    static constexpr int CORK = 5;
    static constexpr int GAME_START = 6;
    static constexpr int GAME = 7;
    static constexpr int RESULT = 8;
    static constexpr int NUM = 9;

    // Setters
    bool setScene(int scene);

    // Getters
    int lastScene() { return mLastScene; }
    int currentScene() { return mCurrentScene; }
    int maxScene() { return mMaxScene; }

    // Functions
    void init();
    void draw() { mScene->draw(); }
    void update() { mScene->update(); }
};