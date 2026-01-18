#include "PlayerSelect.hpp"
#include <string>
#include <random>
#include <algorithm>
#include "Mouse.hpp"
#include "Color.hpp"
#include "Game.hpp"
#include "Sound.hpp"

sPlayerSelect::sPlayerSelect(ShareData shareData) {
    sd = shareData;
    mNowScene = PLAYER_SELECT;
    for (int group = 0, chara = 0; group < sd.groups.size() && chara < MAX_CHARA_NUM; group++) {
        for (int member = 0; member < sd.groups.at(group).members.size() && chara < MAX_CHARA_NUM;
            member++, chara++) {
            sd.groups.at(group).members.at(member).image.box.setSize(100, 100);
            sd.groups.at(group).members.at(member).image.box.setUpperLeft(
                screen.left() + 100 * (chara % 7), upperFrame.bottom() + 100 * (chara / 7));
        }
    }
    for (int team = 0, player = 0;
        team < cTeam::MAX_DUO_TEAM_NUM && player < cTeam::MAX_SOLO_PLAYER_NUM; team++) {
        for (int member = 0;
            member < cTeam::DUO_MEMBER_NUM && player < cTeam::MAX_SOLO_PLAYER_NUM; member++, player++) {
            playerBox[player].setSize(100, 100);
            playerBox[player].setUpperLeft(
                screen.right() - 200 + 100 * member, upperFrame.bottom() + 100 * team);
        }
    }
    players.clear();
    for (int team = 0, player = 0; team < sd.teams.size() && player < cTeam::MAX_SOLO_PLAYER_NUM; team++) {
        for (int member = 0;
            member < sd.teams.at(team).members.size() && player < cTeam::MAX_SOLO_PLAYER_NUM;
            member++, player++) {
            players.push_back(sd.teams.at(team).members.at(member));
            players.at(player).image.box = playerBox[player];
        }
    }
    playersMem = players;
    for (int playModeNo = 0; playModeNo < cTeam::sType::NUM; playModeNo++) {
        teamTypeBox[playModeNo].setSize(
            100, lowerFrame.top() - upperFrame.bottom() - 400);
        teamTypeBox[playModeNo].setLowerLeft(100 * playModeNo + 400, lowerFrame.top());
    }
    shuffle.box = teamTypeBox[cTeam::sType::DUO];
    shuffle.box.setUpperLeft(teamTypeBox[cTeam::sType::DUO].upperRight());
    shuffle.color = cColor::instance()->cyan();
    sd.ctrl.yes.icon.box.setSize(100, lowerFrame.top() - upperFrame.bottom() - 400);
    sd.ctrl.yes.icon.box.setLowerLeft(screen.right() - 200, lowerFrame.top());
}

void sPlayerSelect::reset() {
    cScene::reset();
    players.clear();
    playersMem = players;
    sd.teams.clear();
    cTeam::instance()->init();
}

void sPlayerSelect::draw() {
    cScene::draw();
    drawImage(sd.ctrl.home.icon); drawImage(sd.ctrl.back.icon);
    drawImage(sd.ctrl.mute[cSound::instance()->isBGMPlayed()].icon); drawImage(sd.ctrl.gameSelect.icon);
    drawImage(sd.ctrl.config.icon); drawImage(sd.ctrl.window[sd.window].icon); drawImage(sd.ctrl.quit.icon);
    drawImage(sd.ctrl.init.icon); drawImage(sd.ctrl.reset.icon); drawImage(sd.ctrl.bgm.icon);
    drawImage(sd.ctrl.skip.icon);
    unsigned int color = white;
    for (int group = 0, chara = 0; group < sd.groups.size() && chara < MAX_CHARA_NUM; group++) {
        for (int member = 0; member < sd.groups.at(group).members.size() && chara < MAX_CHARA_NUM;
            member++, chara++, color = white) {
            if (players.size() < cTeam::MAX_SOLO_PLAYER_NUM) {
                switch (cMouse::instance()->clickBoxState(sd.groups.at(group).members.at(member).image.box)) {
                case Key::RELEASED:
                    color = touchColor; break;
                case Key::RELEASEDtoPRESSED: case Key::PRESSED:
                    color = pressColor; break;
                case Key::PRESSEDtoRELEASED:
                    color = executeColor; players.push_back(sd.groups.at(group).members.at(member));
                    setTeamType(cTeam::instance()->type()); playersMem = players; break;
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
    for (int player = 0; player < cTeam::MAX_SOLO_PLAYER_NUM; player++) {
        div_t result = std::div(player, cTeam::instance()->type() + 1);
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
            DrawCircle(95 + 40 * player, lowerFrame.top() - MfontSize / 2 - 15,
                MfontSize + 2, teamColor[team], TRUE);
        }
        DrawCircle(95 + 40 * player, lowerFrame.top() - MfontSize / 2 - 15,
            MfontSize + 2, teamColor[team], FALSE);
        DrawStringToHandle(90 + 40 * player, lowerFrame.top() - MfontSize - 15,
            std::to_string(player + 1).c_str(), white, Mfont);
    }
    drawBoxObj(teamTypeBox[cTeam::instance()->type()], gray, TRUE);
    for (int teamType = 0; teamType < cTeam::sType::NUM; teamType++) {
        color = white;
        if (cTeam::instance()->type() != teamType && players.size() > teamType) {
            switch (cMouse::instance()->clickBoxState(teamTypeBox[teamType])) {
            case Key::RELEASED:
                color = touchColor; break;
            case Key::RELEASEDtoPRESSED: case Key::PRESSED:
                color = pressColor; break;
            case Key::PRESSEDtoRELEASED:
                color = executeColor; setTeamType(teamType); break;
            default: break;
            }
        }
        DrawStringToHandle(
            teamTypeBox[teamType].left() + 5 + 4 * max(0, 8 - (int)cTeam::instance()->typeName().size()),
            teamTypeBox[teamType].center().y() - MfontSize / 2,
            cTeam::instance()->typeName().c_str(), color, Mfont);
    }
    // Shuffle button
    drawBoxObj(shuffle);
    color = white;
    switch (cMouse::instance()->clickBoxState(shuffle.box)) {
    case Key::RELEASED:
        color = touchColor; break;
    case Key::RELEASEDtoPRESSED: case Key::PRESSED:
        color = pressColor; break;
    case Key::PRESSEDtoRELEASED:
        if (players.size() > 0) {
            color = executeColor;
            std::mt19937_64 get_random_mt(std::random_device{}());
            std::shuffle(players.begin(), players.end(), get_random_mt);
            setTeamType(cTeam::instance()->type());
        }
        break;
    default: break;
    }
    DrawStringToHandle(shuffle.box.left() + 9, shuffle.box.center().y() - MfontSize / 2,
        "Shuffle", color, Mfont);
    // OK button
    drawBoxObj(sd.ctrl.yes.icon.box, magenta);
    color = white;
    switch (cMouse::instance()->clickBoxState(sd.ctrl.yes.icon.box)) {
    case Key::RELEASED:
        color = touchColor; break;
    case Key::RELEASEDtoPRESSED: case Key::PRESSED:
        color = pressColor; break;
    case Key::PRESSEDtoRELEASED:
        if (players.size() > 0) { color = executeColor; } break;
    default: break;
    }
    DrawStringToHandle(sd.ctrl.yes.icon.box.left() + 5,
        sd.ctrl.yes.icon.box.center().y() - MfontSize / 2, "OK!!", color, Mfont);
    DrawGraph(screen.right() - 140, sd.ctrl.yes.icon.box.center().y() - DEFAULT_ICON_HEIGHT / 2,
        sd.ctrl.yes.key.image.handle, TRUE);
    int x = screen.left(), y1 = upperFrame.bottom(), y2 = y1 + 400;
    for (int i = 0; i < 4; i++, x += 100) DrawLine(x, y1, x, y2, black);
    y2 = lowerFrame.top();
    for (int i = 4; i < 10; i++, x += 100) DrawLine(x, y1, x, y2, black);
    DrawLine(playerBox[0].left(), y1, playerBox[0].left(), y2, black, 2);
    DrawLine(teamTypeBox[0].left(), y2, screen.right() - 100, y2, black);
    int x1 = screen.left(), x2 = screen.right();
    for (int i = 0, y = upperFrame.bottom(); i <= 4; i++, y += 100)
        DrawLine(x1, y, x2, y, black);
    DrawStringToHandle(screen.left(), lowerFrame.top() - MfontSize - 15,
        "Player", white, Mfont);
    DrawStringToHandle(sd.ctrl.mute[0].icon.box.right() + 5,
        upperFrame.center().y() - MfontSize / 2,
        (cGame::instance()->modeName() + " < Player Select").c_str(), white, Mfont);
}

void sPlayerSelect::update() {
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
            setTeamType(cTeam::sType::SOLO);
            return;
        }
        setTeamType(cTeam::instance()->type());
    }
    else if (nPlayer > 0 && ctrlRQ(sd.ctrl.yes)) mNextScene = GAME_START;
    else if (ctrlRQ(sd.ctrl.back)) {
        if (nPlayer > 0) {
            players.pop_back();
            setTeamType(cTeam::instance()->type());
            return;
        }
        mNextScene = GAME_SELECT;
    }
    else if (ctrlRQ(sd.ctrl.gameSelect)) mNextScene = GAME_SELECT;
    else if (ctrlRQ(sd.ctrl.home)) mNextScene = HOME;
    else if (ctrlRQ(sd.ctrl.config)) mNextScene = CONFIG;
}

void sPlayerSelect::setTeamType(int teamType) {
    cTeam::instance()->setType((players.size() < 2) ? cTeam::sType::SOLO : teamType);
    sd.teams.clear();
    switch (cTeam::instance()->type()) {
    case cTeam::sType::SOLO:
        for (int player = 0; player < players.size(); player++) {
            sd.teams.push_back(sGroup());
            players.at(player).image.box.setUpperLeft(
                screen.right() - 200 + 100 * (player % cTeam::DUO_MEMBER_NUM),
                upperFrame.bottom() + 100 * (player / cTeam::DUO_MEMBER_NUM));
            players.at(player).status.rank = player;
            sd.teams.at(player).members.push_back(players.at(player));
        }
        return;
    case cTeam::sType::DUO:
        for (int player = 0, team = 0; player < players.size(); team++) {
            sd.teams.push_back(sGroup());
            for (int member = 0;
                member < cTeam::DUO_MEMBER_NUM && player < players.size(); member++, player++) {
                players.at(player).image.box.setUpperLeft(
                    screen.right() - 200 + 100 * member, upperFrame.bottom() + 100 * team);
                players.at(player).status.rank = team + (players.size() / 2) * member;
                sd.teams.at(team).members.push_back(players.at(player));
            }
        }
        return;
    default:
        return;
    }
}