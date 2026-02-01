#include "PlayerSelect.hpp"
#include <string>
#include <random>
#include <algorithm>
#include "Mouse.hpp"
#include "Game.hpp"
#include "Control.hpp"
#include "Keyboard.hpp"
#include "Scene.hpp"

cPlayerSelect::cPlayerSelect() {
    for (int group = 0, chara = 0;
        group < cPlayer::inst()->nGroup() && chara < cPlayer::MAX_CHARA_NUM; group++) {
        for (int member = 0;
            member < cPlayer::inst()->nGroupMember(group) && chara < cPlayer::MAX_CHARA_NUM;
            member++, chara++) {
            cPlayer::inst()->groupMemberImageBox(group, member).setS(100, 100);
            cPlayer::inst()->groupMemberImageBox(group, member).setUL(
                screen.L() + 100 * (chara % 7), upperFrame.B() + 100 * (chara / 7));
        }
    }
    for (int team = 0, player = 0;
        team < cPlayer::MAX_DUO_TEAM_NUM && player < cPlayer::MAX_SOLO_PLAYER_NUM; team++) {
        for (int member = 0;
            member < cPlayer::DUO_MEMBER_NUM && player < cPlayer::MAX_SOLO_PLAYER_NUM; member++, player++) {
            playerBox[player].setS(100, 100);
            playerBox[player].setUL(
                screen.R() - 200 + 100 * member, upperFrame.B() + 100 * team);
        }
    }
    players.clear();
    for (int team = 0, player = 0; team < cPlayer::inst()->nTeam() && player < cPlayer::MAX_SOLO_PLAYER_NUM; team++) {
        for (int member = 0;
            member < cPlayer::inst()->nTeamMember(team) && player < cPlayer::MAX_SOLO_PLAYER_NUM;
            member++, player++) {
            players.push_back(cPlayer::inst()->teamMember(team, member));
            players.at(player).image.box() = playerBox[player];
        }
    }
    playersMem = players;
    for (int teamType = 0; teamType < cPlayer::sTeamType::NUM; teamType++) {
        teamTypeBox[teamType].setS(100, lowerFrame.T() - upperFrame.B() - 400);
        teamTypeBox[teamType].setLL(100 * teamType + 400, lowerFrame.T());
        if (teamType == cPlayer::inst()->teamType()) {
            teamTypeBox[teamType].setColor(white); continue;
        }
        teamTypeBox[teamType].setColor(gray);
    }
    setTeamType(cPlayer::inst()->teamType());
    shuffle = teamTypeBox[cPlayer::sTeamType::DUO];
    shuffle.setUL(teamTypeBox[cPlayer::sTeamType::DUO].UR());
    shuffle.setColor(cyan);
    cControl::inst()->iconBox(cControl::YES).setS(
        100, lowerFrame.T() - upperFrame.B() - 400);
    cControl::inst()->iconBox(cControl::YES).setLL(screen.R() - 200, lowerFrame.T());
    cControl::inst()->iconBox(cControl::YES).setColor(magenta);
    cKeyboard::inst()->keyBox(cControl::inst()->keyCode(cControl::YES)).setR(
        cControl::inst()->icon(cControl::YES).box().R() - 10);
    cKeyboard::inst()->keyBox(cControl::inst()->keyCode(cControl::YES)).setCY(
        cControl::inst()->icon(cControl::YES).box().CY());
}

void cPlayerSelect::reset() {
    cBaseScene::reset();
    players.clear();
    playersMem = players;
    cPlayer::inst()->initTeam();
}

void cPlayerSelect::draw() {
    cBaseScene::draw();

    // icons
    if (players.size() < playersMem.size()) cControl::inst()->icon(cControl::FORWARD).draw();
    cControl::inst()->icon(cControl::GAME_SELECT).draw();
    cControl::inst()->icon(cControl::SKIP).draw();

    // characters
    unsigned int color = white;
    for (int group = 0, chara = 0; group < cPlayer::inst()->nGroup() && chara < cPlayer::MAX_CHARA_NUM; group++) {
        for (int member = 0; member < cPlayer::inst()->nGroupMember(group) && chara < cPlayer::MAX_CHARA_NUM;
            member++, chara++, color = white) {
            if (players.size() < cPlayer::MAX_SOLO_PLAYER_NUM) {
                switch (cMouse::inst()->LclickBoxState(
                    cPlayer::inst()->groupMemberImageBox(group, member))) {
                case sKeyState::RELEASE:
                    color = touchColor; break;
                case sKeyState::RELEASEtoPRESS: case sKeyState::PRESS:
                    color = pressColor; break;
                case sKeyState::PRESStoRELEASE:
                    color = executeColor; players.push_back(
                        cPlayer::inst()->groupMember(group, member));
                    setTeamType(cPlayer::inst()->teamType()); playersMem = players; break;
                default:
                    break;
                }
            }
            cPlayer::inst()->groupMemberImage(group, member).draw();
            DrawStringToHandle(cPlayer::inst()->groupMemberImageBox(group, member).L(),
                cPlayer::inst()->groupMemberImageBox(group, member).T(),
                cPlayer::inst()->groups().at(group).name.c_str(), color, Mfont);
            DrawStringToHandle(
                cPlayer::inst()->groupMemberImageBox(group, member).L() +
                2 + 5 * max(0, 10 - (int)cPlayer::inst()->groupMemberName(group, member).size()),
                cPlayer::inst()->groupMemberImageBox(group, member).B() - SfontSize - 10,
                cPlayer::inst()->groupMemberName(group, member).c_str(), color, Sfont);
        }
    }

    // players
    DrawStringToHandle(screen.L(), lowerFrame.T() - MfontSize - 15, "Player", white, Mfont);
    for (int player = 0; player < cPlayer::MAX_SOLO_PLAYER_NUM; player++) {
        div_t result = std::div(player, cPlayer::inst()->teamType() + 1);
        int team = result.quot, member = result.rem;
        playerBox[player].draw();
        if (player < players.size()) {
            players.at(player).image.draw();
            DrawStringToHandle(playerBox[player].L(), playerBox[player].T(),
                PLAYER_NAME[players.at(player).status.rank].c_str(), white, Mfont);
            DrawStringToHandle(
                playerBox[player].L() + 2 + 5 * max(0, 10 - (int)players.at(player).name.size()),
                playerBox[player].B() - SfontSize - 10,
                players.at(player).name.c_str(), white, Sfont);
            DrawCircle(95 + 40 * player, lowerFrame.T() - MfontSize / 2 - 15,
                MfontSize + 2, teamColor[team], TRUE);
        }
        DrawCircle(95 + 40 * player, lowerFrame.T() - MfontSize / 2 - 15,
            MfontSize + 2, teamColor[team], FALSE);
        DrawStringToHandle(90 + 40 * player, lowerFrame.T() - MfontSize - 15,
            std::to_string(player + 1).c_str(), white, Mfont);
    }

    // team types
    for (int teamType = 0; teamType < cPlayer::sTeamType::NUM; teamType++) {
        teamTypeBox[teamType].draw();
        color = white;
        if (teamType != cPlayer::inst()->teamType() && players.size() > teamType) {
            switch (cMouse::inst()->LclickBoxState(teamTypeBox[teamType])) {
            case sKeyState::RELEASE:
                color = touchColor; break;
            case sKeyState::RELEASEtoPRESS: case sKeyState::PRESS:
                color = pressColor; break;
            case sKeyState::PRESStoRELEASE:
                color = executeColor; setTeamType(teamType); break;
            default: break;
            }
        }
        DrawStringToHandle(
            teamTypeBox[teamType].L() + 5 + 4 * max(0, 8 - (int)cPlayer::inst()->teamTypeName(teamType).size()),
            teamTypeBox[teamType].C().y() - MfontSize / 2,
            cPlayer::inst()->teamTypeName(teamType).c_str(), color, Mfont);
    }

    // shuffle button
    shuffle.draw();
    color = white;
    switch (cMouse::inst()->LclickBoxState(shuffle)) {
    case sKeyState::RELEASE:
        color = touchColor; break;
    case sKeyState::RELEASEtoPRESS: case sKeyState::PRESS:
        color = pressColor; break;
    case sKeyState::PRESStoRELEASE:
        if (players.size() > 0) {
            color = executeColor;
            std::mt19937_64 get_random_mt(std::random_device{}());
            std::shuffle(players.begin(), players.end(), get_random_mt);
            setTeamType(cPlayer::inst()->teamType());
        }
        break;
    default: break;
    }
    DrawStringToHandle(shuffle.L() + 9, shuffle.CY() - MfontSize / 2, "Shuffle", color, Mfont);

    // OK button
    cControl::inst()->icon(cControl::YES).box().draw();
    color = white;
    switch (cMouse::inst()->LclickBoxState(cControl::inst()->icon(cControl::YES).box())) {
    case sKeyState::RELEASE:
        color = touchColor; break;
    case sKeyState::RELEASEtoPRESS: case sKeyState::PRESS:
        color = pressColor; break;
    case sKeyState::PRESStoRELEASE:
        if (players.size() > 0) { color = executeColor; } break;
    default: break;
    }
    DrawStringToHandle(cControl::inst()->icon(cControl::YES).box().L() + 5,
        cControl::inst()->icon(cControl::YES).box().CY() - MfontSize / 2, "OK!!", color, Mfont);
    cKeyboard::inst()->keyImage(cControl::inst()->keyCode(cControl::YES)).draw();
    int x = screen.L(), y1 = upperFrame.B(), y2 = y1 + 400;
    for (int i = 0; i < 4; i++, x += 100) DrawLine(x, y1, x, y2, black);
    y2 = lowerFrame.T();
    for (int i = 4; i < 10; i++, x += 100) DrawLine(x, y1, x, y2, black);
    DrawLine(playerBox[0].L(), y1, playerBox[0].L(), y2, black, 2);
    DrawLine(teamTypeBox[0].L(), y2, screen.R() - 100, y2, black);
    int x1 = screen.L(), x2 = screen.R();
    for (int i = 0, y = upperFrame.B(); i <= 4; i++, y += 100)
        DrawLine(x1, y, x2, y, black);

    // scene title
    DrawStringToHandle(cControl::inst()->icon(cControl::MUTE).box().R() + 5,
        upperFrame.C().y() - MfontSize / 2,
        (cGame::inst()->modeName() + " < Player Select").c_str(), white, Mfont);
}

void cPlayerSelect::update() {
    cBaseScene::update();
    if (players.size() < playersMem.size() && cControl::inst()->isRequested(cControl::FORWARD)) {
        players.push_back(playersMem.at(players.size()));
    }
    else if (cControl::inst()->isRequested(cControl::SKIP)) {
        cScene::inst()->setScene(cScene::CORK);
        while (players.size() < playersMem.size()) {
            players.push_back(playersMem.at(players.size()));
        }
        if (!players.size()) {
            players.push_back(cPlayer::inst()->groups()[0].members[0]);
            setTeamType(cPlayer::sTeamType::SOLO);
            return;
        }
        setTeamType(cPlayer::inst()->teamType());
    }
    else if (players.size() > 0 && cControl::inst()->isRequested(cControl::YES))
        cScene::inst()->setScene(cScene::CORK);
    else if (cControl::inst()->isRequested(cControl::BACK)) {
        if (players.size() > 0) {
            players.pop_back();
            setTeamType(cPlayer::inst()->teamType());
            return;
        }
        cScene::inst()->setScene(cScene::GAME_SELECT);
    }
    else if (cControl::inst()->isRequested(cControl::CONFIG))
        cScene::inst()->setScene(cScene::CONFIG);
    else if (cControl::inst()->isRequested(cControl::ANOTHER_WINDOW + GetWindowModeFlag())) {
        for (int player = 0; player < players.size(); player++) {
            players.at(player).image.reload();
        }
    }
}

void cPlayerSelect::setTeamType(int teamType) {
    cPlayer::inst()->setTeamType((players.size() < 2) ? cPlayer::sTeamType::SOLO : teamType);
    cPlayer::inst()->teams().clear();
    if (cPlayer::inst()->teamType() == cPlayer::sTeamType::SOLO) {
        for (int team = 0; team < players.size(); team++) {
            cPlayer::inst()->teams().push_back(cPlayer::sGroup());
            players.at(team).image.box().setUL(
                screen.R() - 200 + 100 * (team % cPlayer::DUO_MEMBER_NUM),
                upperFrame.B() + 100 * (team / cPlayer::DUO_MEMBER_NUM));
            players.at(team).status.rank = team;
            cPlayer::inst()->teamMembers(team).push_back(players.at(team));
            cPlayer::inst()->teams().at(team).name = players.at(team).name;
        }
        for (int player = 0; player < cPlayer::MAX_SOLO_PLAYER_NUM; player++) {
            playerBox[player].setColor(teamColor[player]);
        }
        teamTypeBox[cPlayer::sTeamType::SOLO].setColor(white);
        teamTypeBox[cPlayer::sTeamType::DUO].setColor(gray);
        return;
    }
    for (int player = 0, team = 0; player < players.size(); team++) {
        cPlayer::inst()->teams().push_back(cPlayer::sGroup());
        for (int member = 0;
            member < cPlayer::DUO_MEMBER_NUM && player < players.size(); member++, player++) {
            players.at(player).image.box().setUL(
                screen.R() - 200 + 100 * member, upperFrame.B() + 100 * team);
            players.at(player).status.rank = team + (static_cast<int>(players.size()) / 2) * member;
            cPlayer::inst()->teamMembers(team).push_back(players.at(player));
        }
        cPlayer::inst()->teams().at(team).name = cPlayer::inst()->teamMember(team, 0).name;
        if (cPlayer::inst()->nTeamMember(team) == cPlayer::DUO_MEMBER_NUM) {
            cPlayer::inst()->teams().at(team).name += 
                " & " + cPlayer::inst()->teamMember(team, 1).name;
        }
    }
    for (int player = 0; player < cPlayer::MAX_SOLO_PLAYER_NUM; player++) {
        playerBox[player].setColor(teamColor[player / cPlayer::DUO_MEMBER_NUM]);
    }
    teamTypeBox[cPlayer::sTeamType::SOLO].setColor(gray);
    teamTypeBox[cPlayer::sTeamType::DUO].setColor(white);
}