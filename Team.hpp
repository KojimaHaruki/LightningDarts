#pragma once
#include <string>
#include "Singleton.hpp"

class cTeam : public Singleton<cTeam> {
    cTeam() {}
    friend class Singleton<cTeam>;
public:
    struct sType {
        static constexpr int SOLO = 0;
        static constexpr int DUO = 1;
        static constexpr int NUM = 2;
    };
    // Functions
    void init() { mType = sType::SOLO; }
    bool setType(int type) {
        if (type < 0 || type >= sType::NUM) return false;
        mType = type; return true;
    }
    int type() { return mType; }
    std::string typeName() { return TYPE_NAME[mType]; }
    std::string typeName(int teamType) {
        return (teamType >= 0 && teamType < sType::NUM) ? TYPE_NAME[mType] : "None";
    }
private:
    int mType = sType::SOLO;
    const std::string TYPE_NAME[sType::NUM] = { "Solo", "Duo" };
};