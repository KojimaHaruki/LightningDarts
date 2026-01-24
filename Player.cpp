#include "Player.hpp"
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

bool cPlayer::setTeamType(int type) {
    if (type < 0 || type >= sTeamType::NUM) return false;
    mTeamType = type; return true;
}

std::string cPlayer::teamTypeName(int teamType) {
    return (teamType >= 0 && teamType < sTeamType::NUM) ? TEAM_TYPE_NAME[teamType] : "None";
}

void cPlayer::loadImage() {
    if (mGroup.size() > 0) reloadCharaImage();
	else loadGroupImage();
    if (mTeam.size() > 0) loadTeamImage();
}

void cPlayer::loadGroupImage() {
    std::error_code err;
    cPlayer::instance()->groups().clear();
    cPlayer::instance()->groups().push_back(sGroup("Guest"));
    for (fs::directory_iterator iter(playerFolderPath + "/"), end;
        iter != end && !err && cPlayer::instance()->groups().size() < MAX_GROUP_NUM; iter.increment(err)) {
        const fs::directory_entry entry = *iter;
        // if found path is valid folder,
        if (!entry.path().has_extension() && entry.path().filename().string() != "Guest") {
            cPlayer::instance()->groups().push_back(sGroup(entry.path().filename().string())); // get group name
        }
    }
    for (int group = 0, chara = 0; group < cPlayer::instance()->groups().size(); group++) {
        for (fs::directory_iterator iter(playerFolderPath + "/" + cPlayer::instance()->groups()[group].name + "/"), end;
            iter != end && !err && chara < MAX_CHARA_NUM; iter.increment(err), chara++) {
            const fs::directory_entry entry = *iter;
            std::string extension = entry.path().extension().string();
            if (extension == ".jpg" || extension == ".png") { // if found file is image,
                std::string name = entry.path().filename().string(),
                    path = entry.path().string();
                name.erase(name.length() - extension.length(), extension.length());
                cPlayer::instance()->groups().at(group).members.push_back(cPlayer::sChara());
                cPlayer::instance()->groups().at(group).members.back().name = name;
                cPlayer::instance()->groups().at(group).members.back().image.load(path);
                cPlayer::instance()->groups().at(group).members.back().group = cPlayer::instance()->groups().at(group).name;
            }
        }
    }
    cPlayer::instance()->groups().shrink_to_fit();
    if (err) {
        std::cout << err.value() << std::endl;
        std::cout << err.message() << std::endl;
    }
}

void cPlayer::loadTeamImage() {
    for (int team = 0; team < mTeam.size(); team++) {
        for (int member = 0; member < mTeam.at(team).members.size(); member++) {
            mTeam.at(team).members.at(member).image.reload();
        }
    }
}

void cPlayer::reloadCharaImage() {
    for (int group = 0; group < mGroup.size(); group++) {
        for (int member = 0; member < mGroup.at(group).members.size(); member++) {
            mGroup.at(group).members.at(member).image.reload();
        }
    }
}

void cPlayer::initGroup() { 
    mGroup.clear(); mGroup.reserve(MAX_GROUP_NUM); 
}

void cPlayer::initTeam() { 
    mTeam.clear(); mTeam.reserve(MAX_SOLO_PLAYER_NUM); mTeamType = sTeamType::SOLO; 
}

void cPlayer::init() { 
    initGroup(); initTeam(); loadGroupImage();
}