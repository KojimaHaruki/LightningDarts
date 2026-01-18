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
    static constexpr int SOLO_MEMBER_NUM = 1;
    static constexpr int DUO_MEMBER_NUM = 2;
    static constexpr int MAX_SOLO_PLAYER_NUM = 8;
    static constexpr int MAX_DUO_TEAM_NUM = 4;
    // Functions
    void init() { mType = sType::SOLO; }
    bool setType(int type) {
        if (type < 0 || type >= sType::NUM) return false;
        mType = type; return true;
    }
    int type() { return mType; }
    std::string typeName() { return TYPE_NAME[mType]; }
    std::string typeName(int teamType) {
        return (teamType >= 0 && teamType < sType::NUM) ? TYPE_NAME[teamType] : "None";
    }
private:
    int mType = sType::SOLO;
    const std::string TYPE_NAME[sType::NUM] = { "Solo", "Duo" };
};