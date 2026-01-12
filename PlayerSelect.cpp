#include "PlayerSelect.hpp"
#include <string>
#include <random>
#include <algorithm>
#include "Mouse.hpp"

PlayerSelect::PlayerSelect(ShareData shareData) {
    sd = shareData;
    mNowScene = PLAYER_SELECT;
    players.clear();
    for (int team = 0, player = 0; team < sd.teams.size() && player < MAX_PLAYER_NUM; team++) {
        for (int member = 0; member < sd.teams.at(team).members.size() && player < MAX_PLAYER_NUM;
            member++, player++) {
            players.push_back(sd.teams.at(team).members.at(member));
        }
    }
    playersMem = players;
    setTeamType(sd.teamType);
    for (int group = 0, chara = 0; group < sd.groups.size() && chara < MAX_CHARA_NUM; group++) {
        for (int member = 0; member < sd.groups.at(group).members.size() && chara < MAX_CHARA_NUM;
            member++, chara++) {
            sd.groups.at(group).members.at(member).image.box.setSize(100, 100);
            sd.groups.at(group).members.at(member).image.box.setUpperLeft(
                sd.screen.left() + 100 * (chara % 7), sd.obj.upperFrame.box.bottom() + 100 * (chara / 7));
        }
    }
    for (int team = 0, player = 0; team < MAX_TEAM_NUM && player < MAX_PLAYER_NUM; team++) {
        for (int member = 0; member < DUO_MEMBER_NUM && player < MAX_PLAYER_NUM; member++, player++) {
            playerBox[player].setSize(100, 100);
            playerBox[player].setUpperLeft(
                sd.screen.right() - 200 + 100 * member, sd.obj.upperFrame.box.bottom() + 100 * team);
        }
    }
    for (int playModeNo = 0; playModeNo < TEAM_TYPE_NUM; playModeNo++) {
        teamTypeBox[playModeNo].setSize(
            100, sd.obj.lowerFrame.box.top() - sd.obj.upperFrame.box.bottom() - 400);
        teamTypeBox[playModeNo].setLowerLeft(100 * playModeNo + 400, sd.obj.lowerFrame.box.top());
    }
    shuffle.box = teamTypeBox[TeamType::DUO];
    shuffle.box.setUpperLeft(teamTypeBox[TeamType::DUO].upperRight());
    shuffle.color = sd.color.c;
    sd.ctrl.yes.icon.box.setSize(100, sd.obj.lowerFrame.box.top() - sd.obj.upperFrame.box.bottom() - 400);
    sd.ctrl.yes.icon.box.setLowerLeft(sd.screen.right() - 200, sd.obj.lowerFrame.box.top());
}

void PlayerSelect::reset() {
    Scene::reset();
    players.clear();
    playersMem = players;
    sd.teams.clear();
    sd.teamType = TeamType::SOLO;
}

void PlayerSelect::draw() {
    Scene::draw();
    unsigned int color = sd.color.w;
    for (int group = 0, chara = 0; group < sd.groups.size() && chara < MAX_CHARA_NUM; group++) {
        for (int member = 0; member < sd.groups.at(group).members.size() && chara < MAX_CHARA_NUM;
            member++, chara++, color = sd.color.w) {
            if (players.size() < MAX_PLAYER_NUM) {
                switch (Mouse::instance()->getClickBoxState(sd.groups.at(group).members.at(member).image.box)) {
                case Key::RELEASED:
                    color = sd.color.touch;
                    break;
                case Key::RELEASEDtoPRESSED: case Key::PRESSED:
                    color = sd.color.press;
                    break;
                case Key::PRESSEDtoRELEASED:
                    color = sd.color.execute;
                    players.push_back(sd.groups.at(group).members.at(member));
                    setTeamType(sd.teamType);
                    playersMem = players;
                default:
                    break;
                }
            }
            drawImage(sd.groups.at(group).members.at(member).image);
            DrawStringToHandle(sd.groups.at(group).members.at(member).image.box.left(),
                sd.groups.at(group).members.at(member).image.box.top(),
                sd.groups.at(group).name.c_str(), color, sd.font.s.handle);
            DrawStringToHandle(
                sd.groups.at(group).members.at(member).image.box.left() +
                2 + 5 * max(0, 10 - (int)sd.groups.at(group).members.at(member).name.size()),
                sd.groups.at(group).members.at(member).image.box.bottom() - sd.font.s.size - 10,
                sd.groups.at(group).members.at(member).name.c_str(), color, sd.font.s.handle);
        }
    }
    for (int player = 0; player < MAX_PLAYER_NUM; player++) {
        div_t result = std::div(player, sd.teamType + 1);
        int team = result.quot, member = result.rem;
        drawBoxObj(playerBox[player], sd.color.team[team]);
        if (player < players.size()) {
            drawImage(players.at(player).image);
            DrawStringToHandle(playerBox[player].left(), playerBox[player].top(),
                rankName[players.at(player).status.rank].c_str(), sd.color.w, sd.font.m.handle);
            DrawStringToHandle(
                playerBox[player].left() + 2 + 5 * max(0, 10 - (int)players.at(player).name.size()),
                playerBox[player].bottom() - sd.font.s.size - 10,
                players.at(player).name.c_str(), sd.color.w, sd.font.s.handle);
            DrawCircle(95 + 40 * player, sd.obj.lowerFrame.box.top() - sd.font.m.size / 2 - 15,
                sd.font.m.size + 2, sd.color.team[team], TRUE);
        }
        DrawCircle(95 + 40 * player, sd.obj.lowerFrame.box.top() - sd.font.m.size / 2 - 15,
            sd.font.m.size + 2, sd.color.team[team], FALSE);
        DrawStringToHandle(90 + 40 * player, sd.obj.lowerFrame.box.top() - sd.font.m.size - 15,
            std::to_string(player + 1).c_str(), sd.color.w, sd.font.m.handle);
    }
    drawBoxObj(teamTypeBox[sd.teamType], sd.color.gy, TRUE);
    for (int teamType = 0; teamType < TEAM_TYPE_NUM; teamType++) {
        color = sd.color.w;
        if (sd.teamType != teamType && players.size() > teamType) {
            switch (Mouse::instance()->getClickBoxState(teamTypeBox[teamType])) {
            case Key::RELEASED:
                color = sd.color.touch;
                break;
            case Key::RELEASEDtoPRESSED: case Key::PRESSED:
                color = sd.color.press;
                break;
            case Key::PRESSEDtoRELEASED:
                color = sd.color.execute;
                setTeamType(teamType);
            default:
                break;
            }
        }
        DrawStringToHandle(
            teamTypeBox[teamType].left() + 5 + 4 * max(0, 8 - (int)teamTypeName[teamType].size()),
            teamTypeBox[teamType].center().y() - sd.font.m.size / 2,
            teamTypeName[teamType].c_str(), color, sd.font.m.handle);
    }
    // Shuffle button
    drawBoxObj(shuffle);
    color = sd.color.w;
    switch (Mouse::instance()->getClickBoxState(shuffle.box)) {
    case Key::RELEASED:
        color = sd.color.touch;
        break;
    case Key::RELEASEDtoPRESSED: case Key::PRESSED:
        color = sd.color.press;
        break;
    case Key::PRESSEDtoRELEASED:
        if (players.size() > 0) {
            color = sd.color.execute;
            std::mt19937_64 get_random_mt(std::random_device{}());
            std::shuffle(players.begin(), players.end(), get_random_mt);
            setTeamType(sd.teamType);
        }
    default:
        break;
    }
    DrawStringToHandle(shuffle.box.left() + 9, shuffle.box.center().y() - sd.font.m.size / 2,
        "Shuffle", color, sd.font.m.handle);
    // OK button
    drawBoxObj(sd.ctrl.yes.icon.box, sd.color.m);
    color = sd.color.w;
    switch (Mouse::instance()->getClickBoxState(sd.ctrl.yes.icon.box)) {
    case Key::RELEASED:
        color = sd.color.touch;
        break;
    case Key::RELEASEDtoPRESSED: case Key::PRESSED:
        color = sd.color.press;
        break;
    case Key::PRESSEDtoRELEASED:
        if (players.size() > 0) {
            color = sd.color.execute;
        }
    default:
        break;
    }
    DrawStringToHandle(sd.ctrl.yes.icon.box.left() + 5,
        sd.ctrl.yes.icon.box.center().y() - sd.font.m.size / 2, "OK!!", color, sd.font.m.handle);
    DrawGraph(sd.screen.right() - 140, sd.ctrl.yes.icon.box.center().y() - ICONSIZE_NORMAL.y() / 2,
        sd.ctrl.yes.key.image.handle, TRUE);
    int x = sd.screen.left(), y1 = sd.obj.upperFrame.box.bottom(), y2 = y1 + 400;
    for (int i = 0; i < 4; i++, x += 100)
        DrawLine(x, y1, x, y2, sd.color.k);
    y2 = sd.obj.lowerFrame.box.top();
    for (int i = 4; i < 10; i++, x += 100)
        DrawLine(x, y1, x, y2, sd.color.k);
    DrawLine(playerBox[0].left(), y1, playerBox[0].left(), y2, sd.color.k, 2);
    DrawLine(teamTypeBox[0].left(), y2, sd.screen.right() - 100, y2, sd.color.k);
    int x1 = sd.screen.left(), x2 = sd.screen.right();
    for (int i = 0, y = sd.obj.upperFrame.box.bottom(); i <= 4; i++, y += 100)
        DrawLine(x1, y, x2, y, sd.color.k);
    DrawStringToHandle(sd.screen.left(), sd.obj.lowerFrame.box.top() - sd.font.m.size - 15,
        "Player", sd.color.w, sd.font.m.handle);
    DrawStringToHandle(sd.ctrl.mute[0].icon.box.right() + 5,
        sd.obj.upperFrame.box.center().y() - sd.font.m.size / 2,
        (gameName[sd.game] + " < Player Select").c_str(), sd.color.w, sd.font.m.handle);
}

void PlayerSelect::update() {
    Scene::update();
    int nPlayer = players.size();
    if (nPlayer < playersMem.size()) {
        drawImage(sd.ctrl.forward.icon);
        if (ctrlRQ(sd.ctrl.forward)) {
            players.push_back(playersMem.at(nPlayer));
            return;
        }
    }
    if (ctrlRQ(sd.ctrl.skip)) {
        mNextScene = GAME_START;
        while (players.size() < playersMem.size()) {
            players.push_back(playersMem.at(players.size()));
        }
        if (!players.size()) {
            players.push_back(sd.groups[0].members[0]);
            setTeamType(TeamType::SOLO);
            return;
        }
        setTeamType(sd.teamType);
    }
    else if (nPlayer > 0 && ctrlRQ(sd.ctrl.yes)) mNextScene = GAME_START;
    else if (ctrlRQ(sd.ctrl.back)) {
        if (nPlayer > 0) {
            players.pop_back();
            setTeamType(sd.teamType);
            return;
        }
        mNextScene = GAME_SELECT;
    }
}

void PlayerSelect::setTeamType(int teamType) {
    if (players.size() < 1) return;
    sd.teamType = (players.size() < 2) ? TeamType::SOLO : teamType;
    sd.teams.clear();
    switch (sd.teamType) {
    case TeamType::SOLO:
        for (int player = 0; player < players.size(); player++) {
            sd.teams.push_back(Group());
            players.at(player).image.box.setUpperLeft(
                sd.screen.right() - 200 + 100 * (player % DUO_MEMBER_NUM),
                sd.obj.upperFrame.box.bottom() + 100 * (player / DUO_MEMBER_NUM));
            players.at(player).status.rank = player;
            sd.teams.at(player).members.push_back(players.at(player));
        }
        return;
    case TeamType::DUO:
        for (int player = 0, team = 0; player < players.size(); team++) {
            sd.teams.push_back(Group());
            for (int member = 0; member < DUO_MEMBER_NUM && player < players.size(); member++, player++) {
                players.at(player).image.box.setUpperLeft(
                    sd.screen.right() - 200 + 100 * member, sd.obj.upperFrame.box.bottom() + 100 * team);
                players.at(player).status.rank = team + (players.size() / 2) * member;
                sd.teams.at(team).members.push_back(players.at(player));
            }
        }
    default:
        return;
    }
}

PlayerSelect::~PlayerSelect() {
}