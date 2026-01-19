#pragma once
#include "Singleton.hpp"
#include <string>

class cPlayer : public Singleton<cPlayer> {
    cPlayer() {}
    friend class Singleton<cPlayer>;
public:
    struct sTeamType {
        static constexpr int SOLO = 0;
        static constexpr int DUO = 1;
        static constexpr int NUM = 2;
    };
    static constexpr int SOLO_MEMBER_NUM = 1;
    static constexpr int DUO_MEMBER_NUM = 2;
    static constexpr int MAX_SOLO_PLAYER_NUM = 8;
    static constexpr int MAX_DUO_TEAM_NUM = 4;
    // Functions
    void init() { mTeamType = sTeamType::SOLO; }
    bool setTeamType(int type) {
        if (type < 0 || type >= sTeamType::NUM) return false;
        mTeamType = type; return true;
    }
    int teamType() { return mTeamType; }
    std::string teamTypeName() { return TEAM_TYPE_NAME[mTeamType]; }
    std::string teamTypeName(int teamType) {
        return (teamType >= 0 && teamType < sTeamType::NUM) ? TEAM_TYPE_NAME[teamType] : "None";
    }
private:
    int mTeamType = sTeamType::SOLO;
    const std::string TEAM_TYPE_NAME[sTeamType::NUM] = { "Solo", "Duo" };
};