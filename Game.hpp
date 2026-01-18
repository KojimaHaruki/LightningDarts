#pragma once
#include <string>
#include "Singleton.hpp"

class cGame : public Singleton<cGame> {
    cGame() {}
    friend class Singleton<cGame>;
public:
    struct sMode {
        static constexpr int ZERO_ONE_301 = 0;
        static constexpr int ZERO_ONE_501 = 1;
        static constexpr int ZERO_ONE_701 = 2;
        static constexpr int ZERO_ONE_901 = 3;
        static constexpr int ZERO_ONE_1101 = 4;
        static constexpr int ZERO_ONE_1501 = 5;
        static constexpr int ZERO_ONE_NUM = 6;
        static constexpr int STANDARD_CRICKET = 6;
        static constexpr int RANDOM_CRICKET = 7;
        static constexpr int HIDDEN_CRICKET = 8;
        static constexpr int SELECT_A_CRICKET = 9;
        static constexpr int ALL_NUMBERS_CRICKET = 10;
        static constexpr int CRICKET_NUM = 5;
        static constexpr int COUNT_UP = 11;
        static constexpr int CRICKET_COUNT_UP = 12;
        static constexpr int COUNT_UP_NUM = 2;
        static constexpr int NUM = 13;
    };
    struct sCategory {
        static constexpr int ZERO_ONE = 0;
        static constexpr int CRICKET = 1;
        static constexpr int COUNT_UP = 2;
        static constexpr int NUM = 3;
    };
    static constexpr int CATEGORY_MODE_NUM[sCategory::NUM] = { 
        sMode::ZERO_ONE_NUM, sMode::CRICKET_NUM, sMode::COUNT_UP_NUM
    };
    // Functions
    void init() { mMode = sMode::STANDARD_CRICKET; mCategory = sCategory::CRICKET; }
    bool setMode(int mode) {
        switch (mode) {
        case sMode::ZERO_ONE_301: case sMode::ZERO_ONE_501: case sMode::ZERO_ONE_701: 
        case sMode::ZERO_ONE_901: case sMode::ZERO_ONE_1101: case sMode::ZERO_ONE_1501:
            mMode = mode; mCategory = sCategory::ZERO_ONE; return true;
        case sMode::STANDARD_CRICKET: case sMode::RANDOM_CRICKET: case sMode::HIDDEN_CRICKET:
        case sMode::SELECT_A_CRICKET: case sMode::ALL_NUMBERS_CRICKET:
            mMode = mode; mCategory = sCategory::CRICKET; return true;
        case sMode::COUNT_UP: case sMode::CRICKET_COUNT_UP:
            mMode = mode; mCategory = sCategory::COUNT_UP; return true;
        default:
            return false;
        }
        return true;
    }
    int mode() { return mMode; }
    int category() { return mCategory; }
    std::string modeName() { return MODE_NAME[mMode]; }
    std::string modeName(int mode) { return (mode >= 0 && mode < sMode::NUM) ? MODE_NAME[mode] : "None"; }
    std::string categoryName() { return CATEGORY_NAME[mCategory]; }
    std::string categoryName(int category) { 
        return (category >= 0 && category < sCategory::NUM) ? CATEGORY_NAME[category] : "None"; 
    }
private:
    int mMode = sMode::STANDARD_CRICKET, mCategory = sCategory::CRICKET;
    const std::string CATEGORY_NAME[sCategory::NUM] = { "01", "Cricket", "Count-up" };
    const std::string MODE_NAME[sMode::NUM] = {
        "301", "501", "701", "901", "1101", "1501", "Standard Cricket", "Random Cricket", "Hidden Cricket",
        "Select-a-Cricket", "All numbers Cricket", "Count-up", "Cricket Count-up"
    };
};