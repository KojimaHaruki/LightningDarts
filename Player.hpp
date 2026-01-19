#pragma once
#include "Singleton.hpp"
#include <string>
#include <vector>
#include "Image.hpp"

class cPlayer : public Singleton<cPlayer> {
    cPlayer() {}
    friend class Singleton<cPlayer>;
public:
	// character
    static constexpr int MAX_CHARA_NUM = 28;
    static constexpr int MAX_GROUP_NUM = 10;
    struct sCharaStatus {
        double winRate = 0;
        int rank = 0;
    };
    struct sChara {
        std::string name = {};
        cImage image;
        std::string group = {};
        sCharaStatus status;
    };
    struct sGroup {
        std::string name = {};
        std::vector<sChara> members = {};
    };

	// team type
    struct sTeamType {
        static constexpr int SOLO = 0;
        static constexpr int DUO = 1;
        static constexpr int NUM = 2;
    };
    static constexpr int SOLO_MEMBER_NUM = 1;
    static constexpr int DUO_MEMBER_NUM = 2;
    static constexpr int MAX_SOLO_PLAYER_NUM = 8;
    static constexpr int MAX_DUO_TEAM_NUM = 4;

    // Getters
    std::vector<sGroup>& groups() { return mGroup; }
    std::vector<sChara>& groupMembers(int group) { return mGroup.at(group).members; }
    sChara& groupMember(int group, int member) { return mGroup.at(group).members.at(member); }
    cImage& groupMemberImage(int group, int member) { return mGroup.at(group).members.at(member).image; }
    cBox& groupMemberImageBox(int group, int member) { 
        return mGroup.at(group).members.at(member).image.box(); 
    }
    std::string groupMemberName(int group, int member) {
        return mGroup.at(group).members.at(member).name;
    }
	int nGroup() { return mGroup.size(); }
    int nGroupMember(int group) { return mGroup.at(group).members.size(); }
    std::vector<sGroup>& teams() { return mTeam; }
    std::vector<sChara>& teamMembers(int team) { return mTeam.at(team).members; }
    sChara& teamMember(int team, int member) { return mTeam.at(team).members.at(member); }
    cImage& teamMemberImage(int team, int member) { return mTeam.at(team).members.at(member).image; }
    cBox& teamMemberImageBox(int team, int member) { return mTeam.at(team).members.at(member).image.box(); }
    std::string& teamMemberName(int team, int member) { return mTeam.at(team).members.at(member).name; }
    int nTeam() { return mTeam.size(); }
    int nTeamMember(int team) { return mTeam.at(team).members.size(); }
    int teamType() { return mTeamType; }
    std::string teamTypeName() { return TEAM_TYPE_NAME[mTeamType]; }
    std::string teamTypeName(int teamType);

    // Functions
    void loadImage();
    void loadCharaImage();
    void reloadCharaImage();
    void loadPlayerImage();
    void init() { initGroup(); initTeam(); }
    void initGroup() { mGroup.clear(); mGroup.reserve(MAX_GROUP_NUM);}
    void initTeam() { mTeam.clear(); mTeam.reserve(MAX_SOLO_PLAYER_NUM); mTeamType = sTeamType::SOLO; }
    bool setTeamType(int type) {
        if (type < 0 || type >= sTeamType::NUM) return false;
        mTeamType = type; return true;
    }

private:
    // Character
    const std::string playerFolderPath = "C:/Users/miniy/OneDrive/‰æ‘œ/Darts Player";
    std::vector<sGroup> mGroup, mTeam;

	// Team type
    int mTeamType = sTeamType::SOLO;
    const std::string TEAM_TYPE_NAME[sTeamType::NUM] = { "Solo", "Duo" };
};