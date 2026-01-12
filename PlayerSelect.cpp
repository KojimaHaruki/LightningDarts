#include "PlayerSelect.hpp"
#include <string>
#include "Mouse.hpp"

PlayerSelect::PlayerSelect(ShareData shareData) {
    sd = shareData; 
    mNowScene = PLAYER_SELECT;
    players.clear();
    players.reserve(MAX_PLAYER_NUM);
    nSelectedPlayer = 0;
    for (int team = 0; team < sd.teams.size() && nSelectedPlayer < MAX_PLAYER_NUM; team++) {
        for (int member = 0; member < sd.teams[team].members.size(); member++, nSelectedPlayer++) {
            players.push_back(sd.teams[team].members[member]);
        }
    }
    chTeamType(sd.teamType);
    for (int chara = 0, group = 0; chara < MAX_CHARA_NUM && group < sd.groups.size(); group++) {
        for (int member = 0; member < sd.groups[group].members.size(); member++, chara++) {
            sd.groups[group].members[member].image.box.setSize(100, 100);
            sd.groups[group].members[member].image.box.setUpperLeft(
                sd.screen.left() + 100 * (chara % 7), sd.obj.upperFrame.box.bottom() + 100 * (chara / 7));
        }
    }
    for (int team = 0, player = 0; team < MAX_TEAM_NUM && player < MAX_PLAYER_NUM; team++) {
        for (int member = 0; member < DUO_MEMBER_NUM; member++, player++) {
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
    sd.ctrl.yes.icon.box.setSize(100, sd.obj.lowerFrame.box.top() - sd.obj.upperFrame.box.bottom() - 400);
    sd.ctrl.yes.icon.box.setLowerLeft(sd.screen.right() - 200, sd.obj.lowerFrame.box.top());
}

void PlayerSelect::reset() {
    Scene::reset(); 
    players.clear();
    sd.teams.clear();
    sd.teamType = TeamType::SOLO;
}

void PlayerSelect::draw() {
    Scene::draw();
    for (int chara = 0, group = 0; chara < MAX_CHARA_NUM && group < sd.groups.size(); group++) {
        for (int member = 0; member < sd.groups[group].members.size(); member++, chara++) {
            int color = sd.color.w;
            if (players.size() < MAX_PLAYER_NUM) {
                switch (Mouse::getInstance()->getClickBoxState(sd.groups[group].members[member].image.box)) {
                case Key::RELEASED:
                    color = sd.color.touch;
                    break;
                case Key::RELEASEDtoPRESSED: case Key::PRESSED:
                    color = sd.color.press;
                    break;
                case Key::PRESSEDtoRELEASED:
                    while (players.size() < nSelectedPlayer) {
                        players.pop_back();
                    }
                    players.push_back(sd.groups[group].members[member]);
                    nSelectedPlayer++;
                    chTeamType(sd.teamType);
                    break;
                default:
                    break;
                }
            }
            drawImage(sd.groups[group].members[member].image);
            DrawStringToHandle(sd.groups[group].members[member].image.box.left(),
                sd.groups[group].members[member].image.box.top(),
                sd.groups[group].name.c_str(), color, sd.font.s.handle);
            DrawStringToHandle(
                sd.groups[group].members[member].image.box.left() + 
                5 * max(0, 10 - sd.groups[group].members[member].name.size()),
                sd.groups[group].members[member].image.box.bottom() - sd.font.s.size - 10,
                sd.groups[group].members[member].name.c_str(), color, sd.font.s.handle);
        }
    }
    for (int player = 0; player < MAX_PLAYER_NUM; player++) {
        div_t result = std::div(player, sd.teamType + 1);
        int team = result.quot, member = result.rem;
        drawBoxObj(playerBox[player], sd.color.team[team]);
        if (player < nSelectedPlayer) {
            drawImage(players[player].image);
            DrawStringToHandle(playerBox[player].left(), playerBox[player].top(),
                rankName[players[player].status.rank].c_str(), sd.color.w, sd.font.m.handle);
            DrawStringToHandle(
                playerBox[player].left() + 5 * max(0, 10 - players[player].name.size()),
                playerBox[player].bottom() - sd.font.s.size - 10,
                players[player].name.c_str(), sd.color.w, sd.font.s.handle);
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
        int color = sd.color.w;
        if (sd.teamType != teamType && nSelectedPlayer > teamType) {
            switch (Mouse::getInstance()->getClickBoxState(teamTypeBox[teamType])) {
            case Key::RELEASED:
                color = sd.color.touch;
                break;
            case Key::RELEASEDtoPRESSED:
            case Key::PRESSED:
                color = sd.color.press;
                break;
            case Key::PRESSEDtoRELEASED:
                chTeamType(teamType);
            default:
                break;
            }
        }
        DrawStringToHandle(
            teamTypeBox[teamType].left() + 5 + 4 * max(0, 8 - teamTypeName[teamType].size()),
            teamTypeBox[teamType].center().y() - sd.font.m.size / 2,
            teamTypeName[teamType].c_str(), color, sd.font.m.handle);
    }
    DrawStringToHandle(teamTypeBox[TeamType::DUO].right() + 5,
        teamTypeBox[TeamType::DUO].center().y() - sd.font.m.size - 4,
        "Shuffle", sd.color.w, sd.font.m.handle);
    DrawBox(sd.screen.right() - 200, sd.obj.upperFrame.box.bottom() + 400,
        sd.screen.right() - 100, sd.obj.lowerFrame.box.top(), sd.color.press, TRUE);
    DrawStringToHandle(sd.screen.right() - 195, sd.ctrl.yes.icon.box.center().y() - sd.font.m.size / 2,
        "OK!!", sd.color.w, sd.font.m.handle);
    for (int i = 0; i < 4; i++)
        DrawLine(sd.screen.left() + 100 * i, sd.obj.upperFrame.box.bottom(),
            sd.screen.left() + 100 * i, sd.obj.upperFrame.box.bottom() + 400, sd.color.k);
    for (int i = 4; i < 10; i++)
        DrawLine(sd.screen.left() + 100 * i, sd.obj.upperFrame.box.bottom(),
            sd.screen.left() + 100 * i, sd.obj.lowerFrame.box.top(), sd.color.k);
    DrawLine(700, 25, 700, 475, sd.color.k, 2);
    for (int i = 0; i <= 4; i++)
        DrawLine(sd.screen.left(), sd.obj.upperFrame.box.bottom() + 100 * i, 
            sd.screen.right(), sd.obj.upperFrame.box.bottom() + 100 * i, sd.color.k);
    DrawLine(teamTypeBox[0].left(), sd.obj.lowerFrame.box.top(), 
        sd.screen.right() - 100, sd.obj.lowerFrame.box.top(), sd.color.k);
    DrawStringToHandle(sd.screen.left(), sd.obj.lowerFrame.box.top() - sd.font.m.size - 15,
        "Player", sd.color.w, sd.font.m.handle);
    DrawGraph(sd.screen.right() - 140, sd.ctrl.yes.icon.box.center().y() - ICONSIZE_NORMAL.y() / 2,
        sd.ctrl.yes.key.image.handle, TRUE);
    DrawStringToHandle(sd.ctrl.mute[sd.sound].icon.box.right() + 5,
        sd.obj.upperFrame.box.center().y() - sd.font.m.size / 2,
        (gameName[sd.game] + " < Player Select").c_str(), sd.color.w, sd.font.m.handle);
}

void PlayerSelect::update() {
    Scene::update(); 
    if (nSelectedPlayer < players.size() && ctrlRQ(sd.ctrl.forward)) {
        nSelectedPlayer++;
        chTeamType(sd.teamType);
    }
    else if (ctrlRQ(sd.ctrl.skip)) {
        mNextScene = GAME_START;
        if (!players.size()) {
            players.push_back(sd.groups[0].members[0]);
            nSelectedPlayer = 1;
            chTeamType(TeamType::SOLO);
            return;
        }
        nSelectedPlayer = players.size();
        chTeamType(sd.teamType);
    }
    else if (nSelectedPlayer > 0 && ctrlRQ(sd.ctrl.yes)) mNextScene = GAME_START; 
    else if (ctrlRQ(sd.ctrl.back)) {
        if (nSelectedPlayer > 0) {
            nSelectedPlayer--;
            chTeamType(sd.teamType);
            return;
        }
        mNextScene = GAME_SELECT;
    }
}

void PlayerSelect::chTeamType(int teamType) {
    sd.teamType = teamType;
    sd.teams.clear();
    switch (teamType) {
    case TeamType::SOLO:
        for (int player = 0; player < nSelectedPlayer; player++) {
            sd.teams.push_back(Group());
            sd.teams[player].members.clear();
            players[player].image.box.setUpperLeft(
                sd.screen.right() - 200 + 100 * (player % DUO_MEMBER_NUM),
                sd.obj.upperFrame.box.bottom() + 100 * (player / DUO_MEMBER_NUM));
            players[player].status.rank = player;
            sd.teams[player].members.push_back(players[player]);
        }
        return;
    case TeamType::DUO:
        for (int player = 0, team = 0; player < nSelectedPlayer; team++) {
            sd.teams.push_back(Group());
            sd.teams[team].members.clear();
            for (int member = 0; member < DUO_MEMBER_NUM; member++, player++) {
                players[player].image.box.setUpperLeft(
                    sd.screen.right() - 200 + 100 * member, sd.obj.upperFrame.box.bottom() + 100 * team);
                players[player].status.rank = team + (nSelectedPlayer / 2) * member;
                sd.teams[team].members.push_back(players[player]);
            }
        }
        return;
    default:
        return;
    }
}

PlayerSelect::~PlayerSelect() {
}