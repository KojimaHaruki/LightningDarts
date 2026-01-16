#include "PlayerSelect.hpp"
#include <string>
#include <random>
#include <algorithm>
#include "Mouse.hpp"
#include "Color.hpp"

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
                sd.screen.left() + 100 * (chara % 7), sd.obj.upperFrame.bottom() + 100 * (chara / 7));
        }
    }
    for (int team = 0, player = 0; team < MAX_TEAM_NUM && player < MAX_PLAYER_NUM; team++) {
        for (int member = 0; member < DUO_MEMBER_NUM && player < MAX_PLAYER_NUM; member++, player++) {
            playerBox[player].setSize(100, 100);
            playerBox[player].setUpperLeft(
                sd.screen.right() - 200 + 100 * member, sd.obj.upperFrame.bottom() + 100 * team);
        }
    }
    for (int playModeNo = 0; playModeNo < TEAM_TYPE_NUM; playModeNo++) {
        teamTypeBox[playModeNo].setSize(
            100, sd.obj.lowerFrame.top() - sd.obj.upperFrame.bottom() - 400);
        teamTypeBox[playModeNo].setLowerLeft(100 * playModeNo + 400, sd.obj.lowerFrame.top());
    }
    shuffle.box = teamTypeBox[sTeamType::DUO];
    shuffle.box.setUpperLeft(teamTypeBox[sTeamType::DUO].upperRight());
    shuffle.color = cColor::instance()->cyan();
    sd.ctrl.yes.icon.box.setSize(100, sd.obj.lowerFrame.top() - sd.obj.upperFrame.bottom() - 400);
    sd.ctrl.yes.icon.box.setLowerLeft(sd.screen.right() - 200, sd.obj.lowerFrame.top());
}

void PlayerSelect::reset() {
    cScene::reset();
    players.clear();
    playersMem = players;
    sd.teams.clear();
    sd.teamType = sTeamType::SOLO;
}

void PlayerSelect::draw() {
    cScene::draw();
    unsigned int color = white;
    for (int group = 0, chara = 0; group < sd.groups.size() && chara < MAX_CHARA_NUM; group++) {
        for (int member = 0; member < sd.groups.at(group).members.size() && chara < MAX_CHARA_NUM;
            member++, chara++, color = white) {
            if (players.size() < MAX_PLAYER_NUM) {
                switch (Mouse::instance()->getClickBoxState(sd.groups.at(group).members.at(member).image.box)) {
                case Key::RELEASED:
                    color = touchColor;
                    break;
                case Key::RELEASEDtoPRESSED: case Key::PRESSED:
                    color = pressColor;
                    break;
                case Key::PRESSEDtoRELEASED:
                    color = executeColor;
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
                sd.groups.at(group).name.c_str(), color, Sfont);
            DrawStringToHandle(
                sd.groups.at(group).members.at(member).image.box.left() +
                2 + 5 * max(0, 10 - (int)sd.groups.at(group).members.at(member).name.size()),
                sd.groups.at(group).members.at(member).image.box.bottom() - SfontSize - 10,
                sd.groups.at(group).members.at(member).name.c_str(), color, Sfont);
        }
    }
    for (int player = 0; player < MAX_PLAYER_NUM; player++) {
        div_t result = std::div(player, sd.teamType + 1);
        int team = result.quot, member = result.rem;
        drawBoxObj(playerBox[player], teamColor[team]);
        if (player < players.size()) {
            drawImage(players.at(player).image);
            DrawStringToHandle(playerBox[player].left(), playerBox[player].top(),
                rankName[players.at(player).status.rank].c_str(), white, Mfont);
            DrawStringToHandle(
                playerBox[player].left() + 2 + 5 * max(0, 10 - (int)players.at(player).name.size()),
                playerBox[player].bottom() - SfontSize - 10,
                players.at(player).name.c_str(), white, Sfont);
            DrawCircle(95 + 40 * player, sd.obj.lowerFrame.top() - MfontSize / 2 - 15,
                MfontSize + 2, teamColor[team], TRUE);
        }
        DrawCircle(95 + 40 * player, sd.obj.lowerFrame.top() - MfontSize / 2 - 15,
            MfontSize + 2, teamColor[team], FALSE);
        DrawStringToHandle(90 + 40 * player, sd.obj.lowerFrame.top() - MfontSize - 15,
            std::to_string(player + 1).c_str(), white, Mfont);
    }
    drawBoxObj(teamTypeBox[sd.teamType], gray, TRUE);
    for (int teamType = 0; teamType < TEAM_TYPE_NUM; teamType++) {
        color = white;
        if (sd.teamType != teamType && players.size() > teamType) {
            switch (Mouse::instance()->getClickBoxState(teamTypeBox[teamType])) {
            case Key::RELEASED:
                color = touchColor;
                break;
            case Key::RELEASEDtoPRESSED: case Key::PRESSED:
                color = pressColor;
                break;
            case Key::PRESSEDtoRELEASED:
                color = executeColor;
                setTeamType(teamType);
            default:
                break;
            }
        }
        DrawStringToHandle(
            teamTypeBox[teamType].left() + 5 + 4 * max(0, 8 - (int)teamTypeName[teamType].size()),
            teamTypeBox[teamType].center().y() - MfontSize / 2,
            teamTypeName[teamType].c_str(), color, Mfont);
    }
    // Shuffle button
    drawBoxObj(shuffle);
    color = white;
    switch (Mouse::instance()->getClickBoxState(shuffle.box)) {
    case Key::RELEASED:
        color = touchColor;
        break;
    case Key::RELEASEDtoPRESSED: case Key::PRESSED:
        color = pressColor;
        break;
    case Key::PRESSEDtoRELEASED:
        if (players.size() > 0) {
            color = executeColor;
            std::mt19937_64 get_random_mt(std::random_device{}());
            std::shuffle(players.begin(), players.end(), get_random_mt);
            setTeamType(sd.teamType);
        }
    default:
        break;
    }
    DrawStringToHandle(shuffle.box.left() + 9, shuffle.box.center().y() - MfontSize / 2,
        "Shuffle", color, Mfont);
    // OK button
    drawBoxObj(sd.ctrl.yes.icon.box, magenta);
    color = white;
    switch (Mouse::instance()->getClickBoxState(sd.ctrl.yes.icon.box)) {
    case Key::RELEASED:
        color = touchColor;
        break;
    case Key::RELEASEDtoPRESSED: case Key::PRESSED:
        color = pressColor;
        break;
    case Key::PRESSEDtoRELEASED:
        if (players.size() > 0) {
            color = executeColor;
        }
    default:
        break;
    }
    DrawStringToHandle(sd.ctrl.yes.icon.box.left() + 5,
        sd.ctrl.yes.icon.box.center().y() - MfontSize / 2, "OK!!", color, Mfont);
    DrawGraph(sd.screen.right() - 140, sd.ctrl.yes.icon.box.center().y() - ICONSIZE_NORMAL.y() / 2,
        sd.ctrl.yes.key.image.handle, TRUE);
    int x = sd.screen.left(), y1 = sd.obj.upperFrame.bottom(), y2 = y1 + 400;
    for (int i = 0; i < 4; i++, x += 100) DrawLine(x, y1, x, y2, black);
    y2 = sd.obj.lowerFrame.top();
    for (int i = 4; i < 10; i++, x += 100) DrawLine(x, y1, x, y2, black);
    DrawLine(playerBox[0].left(), y1, playerBox[0].left(), y2, black, 2);
    DrawLine(teamTypeBox[0].left(), y2, sd.screen.right() - 100, y2, black);
    int x1 = sd.screen.left(), x2 = sd.screen.right();
    for (int i = 0, y = sd.obj.upperFrame.bottom(); i <= 4; i++, y += 100)
        DrawLine(x1, y, x2, y, black);
    DrawStringToHandle(sd.screen.left(), sd.obj.lowerFrame.top() - MfontSize - 15,
        "Player", white, Mfont);
    DrawStringToHandle(sd.ctrl.mute[0].icon.box.right() + 5,
        sd.obj.upperFrame.center().y() - MfontSize / 2,
        (gameName[mGame] + " < Player Select").c_str(), white, Mfont);
}

void PlayerSelect::update() {
    cScene::update();
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
            setTeamType(sTeamType::SOLO);
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
    else if (ctrlRQ(sd.ctrl.gameSelect)) mNextScene = GAME_SELECT;
    else if (ctrlRQ(sd.ctrl.home)) mNextScene = HOME;
    else if (ctrlRQ(sd.ctrl.config)) mNextScene = CONFIG;
}

void PlayerSelect::setTeamType(int teamType) {
    if (players.size() < 1) return;
    sd.teamType = (players.size() < 2) ? sTeamType::SOLO : teamType;
    sd.teams.clear();
    switch (sd.teamType) {
    case sTeamType::SOLO:
        for (int player = 0; player < players.size(); player++) {
            sd.teams.push_back(sGroup());
            players.at(player).image.box.setUpperLeft(
                sd.screen.right() - 200 + 100 * (player % DUO_MEMBER_NUM),
                sd.obj.upperFrame.bottom() + 100 * (player / DUO_MEMBER_NUM));
            players.at(player).status.rank = player;
            sd.teams.at(player).members.push_back(players.at(player));
        }
        return;
    case sTeamType::DUO:
        for (int player = 0, team = 0; player < players.size(); team++) {
            sd.teams.push_back(sGroup());
            for (int member = 0; member < DUO_MEMBER_NUM && player < players.size(); member++, player++) {
                players.at(player).image.box.setUpperLeft(
                    sd.screen.right() - 200 + 100 * member, sd.obj.upperFrame.bottom() + 100 * team);
                players.at(player).status.rank = team + (players.size() / 2) * member;
                sd.teams.at(team).members.push_back(players.at(player));
            }
        }
    default:
        return;
    }
}