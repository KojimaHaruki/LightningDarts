#include "PlayerSelect.hpp"
#include <string>
#include <random>
#include <algorithm>
#include "Mouse.hpp"
#include "Color.hpp"
#include "Game.hpp"
#include "Sound.hpp"
#include "Control.hpp"
#include "Keyboard.hpp"

cPlayerSelect::cPlayerSelect() {
    mNowScene = PLAYER_SELECT;
    for (int group = 0, chara = 0;
        group < cPlayer::instance()->nGroup() && chara < cPlayer::MAX_CHARA_NUM; group++) {
        for (int member = 0;
            member < cPlayer::instance()->nGroupMember(group) && chara < cPlayer::MAX_CHARA_NUM;
            member++, chara++) {
            cPlayer::instance()->groupMemberImageBox(group, member).setSize(100, 100);
            cPlayer::instance()->groupMemberImageBox(group, member).setUpperLeft(
                screen.left() + 100 * (chara % 7), upperFrame.bottom() + 100 * (chara / 7));
        }
    }
    for (int team = 0, player = 0;
        team < cPlayer::MAX_DUO_TEAM_NUM && player < cPlayer::MAX_SOLO_PLAYER_NUM; team++) {
        for (int member = 0;
            member < cPlayer::DUO_MEMBER_NUM && player < cPlayer::MAX_SOLO_PLAYER_NUM; member++, player++) {
            playerBox[player].setSize(100, 100);
            playerBox[player].setUpperLeft(
                screen.right() - 200 + 100 * member, upperFrame.bottom() + 100 * team);
        }
    }
    players.clear();
    for (int team = 0, player = 0; team < cPlayer::instance()->nTeam() && player < cPlayer::MAX_SOLO_PLAYER_NUM; team++) {
        for (int member = 0;
            member < cPlayer::instance()->nTeamMember(team) && player < cPlayer::MAX_SOLO_PLAYER_NUM;
            member++, player++) {
            players.push_back(cPlayer::instance()->teamMember(team, member));
            players.at(player).image.box() = playerBox[player];
        }
    }
    playersMem = players;
    for (int teamType = 0; teamType < cPlayer::sTeamType::NUM; teamType++) {
        teamTypeBox[teamType].setSize(100, lowerFrame.top() - upperFrame.bottom() - 400);
        teamTypeBox[teamType].setLowerLeft(100 * teamType + 400, lowerFrame.top());
        if (teamType == cPlayer::instance()->teamType()) {
            teamTypeBox[teamType].setColor(white); continue;
        }
        teamTypeBox[teamType].setColor(gray);
    }
    setTeamType(cPlayer::instance()->teamType());
    shuffle = teamTypeBox[cPlayer::sTeamType::DUO];
    shuffle.setUpperLeft(teamTypeBox[cPlayer::sTeamType::DUO].upperRight());
    shuffle.setColor(cyan);
    cControl::instance()->iconBox(cControl::YES).setSize(
        100, lowerFrame.top() - upperFrame.bottom() - 400);
    cControl::instance()->iconBox(cControl::YES).setLowerLeft(screen.right() - 200, lowerFrame.top());
    cControl::instance()->iconBox(cControl::YES).setColor(magenta);
    cKeyboard::instance()->keyBox(cControl::instance()->keyCode(cControl::YES)).setRight(
        cControl::instance()->icon(cControl::YES).box().right() - 10);
    cKeyboard::instance()->keyBox(cControl::instance()->keyCode(cControl::YES)).setCenterY(
        cControl::instance()->icon(cControl::YES).box().centerY());
}

void cPlayerSelect::reset() {
    cBaseScene::reset();
    players.clear();
    playersMem = players;
    cPlayer::instance()->init();
}

void cPlayerSelect::draw() {
    cBaseScene::draw();

    // icons
    cControl::instance()->icon(cControl::GAME_SELECT).draw();
    cControl::instance()->icon(cControl::SKIP).draw();

    // characters
    unsigned int color = white;
    for (int group = 0, chara = 0; group < cPlayer::instance()->nGroup() && chara < cPlayer::MAX_CHARA_NUM; group++) {
        for (int member = 0; member < cPlayer::instance()->nGroupMember(group) && chara < cPlayer::MAX_CHARA_NUM;
            member++, chara++, color = white) {
            if (players.size() < cPlayer::MAX_SOLO_PLAYER_NUM) {
                switch (cMouse::instance()->clickBoxState(
                    cPlayer::instance()->groupMemberImageBox(group, member))) {
                case sKey::RELEASED:
                    color = touchColor; break;
                case sKey::RELEASEDtoPRESSED: case sKey::PRESSED:
                    color = pressColor; break;
                case sKey::PRESSEDtoRELEASED:
                    color = executeColor; players.push_back(
                        cPlayer::instance()->groupMember(group, member));
                    setTeamType(cPlayer::instance()->teamType()); playersMem = players; break;
                default:
                    break;
                }
            }
            cPlayer::instance()->groupMemberImage(group, member).draw();
            DrawStringToHandle(cPlayer::instance()->groupMemberImageBox(group, member).left(),
                cPlayer::instance()->groupMemberImageBox(group, member).top(),
                cPlayer::instance()->groups().at(group).name.c_str(), color, Mfont);
            DrawStringToHandle(
                cPlayer::instance()->groupMemberImageBox(group, member).left() +
                2 + 5 * max(0, 10 - (int)cPlayer::instance()->groupMemberName(group, member).size()),
                cPlayer::instance()->groupMemberImageBox(group, member).bottom() - SfontSize - 10,
                cPlayer::instance()->groupMemberName(group, member).c_str(), color, Sfont);
        }
    }

    // players
    DrawStringToHandle(screen.left(), lowerFrame.top() - MfontSize - 15, "Player", white, Mfont);
    for (int player = 0; player < cPlayer::MAX_SOLO_PLAYER_NUM; player++) {
        div_t result = std::div(player, cPlayer::instance()->teamType() + 1);
        int team = result.quot, member = result.rem;
        playerBox[player].draw();
        if (player < players.size()) {
            players.at(player).image.draw();
            DrawStringToHandle(playerBox[player].left(), playerBox[player].top(),
                PLAYER_NAME[players.at(player).status.rank].c_str(), white, Mfont);
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

    // team types
    for (int teamType = 0; teamType < cPlayer::sTeamType::NUM; teamType++) {
        teamTypeBox[teamType].draw();
        color = white;
        if (teamType != cPlayer::instance()->teamType() && players.size() > teamType) {
            switch (cMouse::instance()->clickBoxState(teamTypeBox[teamType])) {
            case sKey::RELEASED:
                color = touchColor; break;
            case sKey::RELEASEDtoPRESSED: case sKey::PRESSED:
                color = pressColor; break;
            case sKey::PRESSEDtoRELEASED:
                color = executeColor; setTeamType(teamType); break;
            default: break;
            }
        }
        DrawStringToHandle(
            teamTypeBox[teamType].left() + 5 + 4 * max(0, 8 - (int)cPlayer::instance()->teamTypeName(teamType).size()),
            teamTypeBox[teamType].center().y() - MfontSize / 2,
            cPlayer::instance()->teamTypeName(teamType).c_str(), color, Mfont);
    }

    // shuffle button
    shuffle.draw();
    color = white;
    switch (cMouse::instance()->clickBoxState(shuffle)) {
    case sKey::RELEASED:
        color = touchColor; break;
    case sKey::RELEASEDtoPRESSED: case sKey::PRESSED:
        color = pressColor; break;
    case sKey::PRESSEDtoRELEASED:
        if (players.size() > 0) {
            color = executeColor;
            std::mt19937_64 get_random_mt(std::random_device{}());
            std::shuffle(players.begin(), players.end(), get_random_mt);
            setTeamType(cPlayer::instance()->teamType());
        }
        break;
    default: break;
    }
    DrawStringToHandle(shuffle.left() + 9, shuffle.centerY() - MfontSize / 2, "Shuffle", color, Mfont);

    // OK button
    cControl::instance()->icon(cControl::YES).box().draw();
    color = white;
    switch (cMouse::instance()->clickBoxState(cControl::instance()->icon(cControl::YES).box())) {
    case sKey::RELEASED:
        color = touchColor; break;
    case sKey::RELEASEDtoPRESSED: case sKey::PRESSED:
        color = pressColor; break;
    case sKey::PRESSEDtoRELEASED:
        if (players.size() > 0) { color = executeColor; } break;
    default: break;
    }
    DrawStringToHandle(cControl::instance()->icon(cControl::YES).box().left() + 5,
        cControl::instance()->icon(cControl::YES).box().centerY() - MfontSize / 2, "OK!!", color, Mfont);
    cKeyboard::instance()->keyImage(cControl::instance()->keyCode(cControl::YES)).draw();
    int x = screen.left(), y1 = upperFrame.bottom(), y2 = y1 + 400;
    for (int i = 0; i < 4; i++, x += 100) DrawLine(x, y1, x, y2, black);
    y2 = lowerFrame.top();
    for (int i = 4; i < 10; i++, x += 100) DrawLine(x, y1, x, y2, black);
    DrawLine(playerBox[0].left(), y1, playerBox[0].left(), y2, black, 2);
    DrawLine(teamTypeBox[0].left(), y2, screen.right() - 100, y2, black);
    int x1 = screen.left(), x2 = screen.right();
    for (int i = 0, y = upperFrame.bottom(); i <= 4; i++, y += 100)
        DrawLine(x1, y, x2, y, black);

    // scene title
    DrawStringToHandle(cControl::instance()->icon(cControl::MUTE).box().right() + 5,
        upperFrame.center().y() - MfontSize / 2,
        (cGame::instance()->modeName() + " < Player Select").c_str(), white, Mfont);
}

void cPlayerSelect::update() {
    cBaseScene::update();
    if (players.size() < playersMem.size()) {
        cControl::instance()->icon(cControl::FORWARD).draw();
        if (cControl::instance()->isRequested(cControl::FORWARD)) {
            players.push_back(playersMem.at(players.size()));
            return;
        }
    }
    if (cControl::instance()->isRequested(cControl::SKIP)) {
        mNextScene = GAME_START;
        while (players.size() < playersMem.size()) {
            players.push_back(playersMem.at(players.size()));
        }
        if (!players.size()) {
            players.push_back(cPlayer::instance()->groups()[0].members[0]);
            setTeamType(cPlayer::sTeamType::SOLO);
            return;
        }
        setTeamType(cPlayer::instance()->teamType());
    }
    else if (players.size() > 0 && cControl::instance()->isRequested(cControl::YES)) mNextScene = GAME_START;
    else if (cControl::instance()->isRequested(cControl::BACK)) {
        if (players.size() > 0) {
            players.pop_back();
            setTeamType(cPlayer::instance()->teamType());
            return;
        }
        mNextScene = GAME_SELECT;
    }
    else if (cControl::instance()->isRequested(cControl::GAME_SELECT)) mNextScene = GAME_SELECT;
    else if (cControl::instance()->isRequested(cControl::HOME)) mNextScene = HOME;
    else if (cControl::instance()->isRequested(cControl::CONFIG)) mNextScene = CONFIG;
    else if (cControl::instance()->isRequested(cControl::ANOTHER_WINDOW + GetWindowModeFlag())) {
        for (int player = 0; player < players.size(); player++) {
            players.at(player).image.reload();
        }
    }
}

void cPlayerSelect::setTeamType(int teamType) {
    cPlayer::instance()->setTeamType((players.size() < 2) ? cPlayer::sTeamType::SOLO : teamType);
    cPlayer::instance()->teams().clear();
    if (cPlayer::instance()->teamType() == cPlayer::sTeamType::SOLO) {
        for (int team = 0; team < players.size(); team++) {
            cPlayer::instance()->teams().push_back(cPlayer::sGroup());
            players.at(team).image.box().setUpperLeft(
                screen.right() - 200 + 100 * (team % cPlayer::DUO_MEMBER_NUM),
                upperFrame.bottom() + 100 * (team / cPlayer::DUO_MEMBER_NUM));
            players.at(team).status.rank = team;
            cPlayer::instance()->teamMembers(team).push_back(players.at(team));
            cPlayer::instance()->teams().at(team).name = players.at(team).name;
        }
        for (int player = 0; player < cPlayer::MAX_SOLO_PLAYER_NUM; player++) {
            playerBox[player].setColor(teamColor[player]);
        }
        teamTypeBox[cPlayer::sTeamType::SOLO].setColor(white);
        teamTypeBox[cPlayer::sTeamType::DUO].setColor(gray);
        return;
    }
    for (int player = 0, team = 0; player < players.size(); team++) {
        cPlayer::instance()->teams().push_back(cPlayer::sGroup());
        for (int member = 0;
            member < cPlayer::DUO_MEMBER_NUM && player < players.size(); member++, player++) {
            players.at(player).image.box().setUpperLeft(
                screen.right() - 200 + 100 * member, upperFrame.bottom() + 100 * team);
            players.at(player).status.rank = team + (players.size() / 2) * member;
            cPlayer::instance()->teamMembers(team).push_back(players.at(player));
        }
        cPlayer::instance()->teams().at(team).name = cPlayer::instance()->teamMember(team, 0).name;
        if (cPlayer::instance()->nTeamMember(team) == cPlayer::DUO_MEMBER_NUM) {
            cPlayer::instance()->teams().at(team).name += 
                " & " + cPlayer::instance()->teamMember(team, 1).name;
        }
    }
    for (int player = 0; player < cPlayer::MAX_SOLO_PLAYER_NUM; player++) {
        playerBox[player].setColor(teamColor[player / cPlayer::DUO_MEMBER_NUM]);
    }
    teamTypeBox[cPlayer::sTeamType::SOLO].setColor(gray);
    teamTypeBox[cPlayer::sTeamType::DUO].setColor(white);
}