#include "PlayerSelect.hpp"
#include <string>
#include "Mouse.hpp"
PlayerSelect::PlayerSelect(ShareData shareData) {
    sd = shareData; 
    mNowScene = PLAYER_SELECT;
    teamTypeMem = sd.teamType;
    playerChara[0] = 0;
    for (int player = 1; player < MAX_PLAYER_NUM; player++) playerChara[player] = -1;
    for (int team = 0, player = 0, chara = 0; team < MAX_PLAYER_NUM; team++) {
        for (int member = 0; member < DUO_MEMBER_NUM; member++) {
            chara = sd.teamChara[team][member];
            if (chara >= 0 && chara < sd.charaNum) {
                playerChara[player] = chara;
                player++;
            }
        }
    }
    for (int charaNo = 0; charaNo < sd.charaNum; charaNo++) {
        sd.chara[charaNo].image.box.setUpperLeft(
            sd.screen.left() + 100 * (charaNo % 7), sd.obj.upperFrame.box.bottom() + 100 * (charaNo / 7));
    }
    for (int playModeNo = 0; playModeNo < TeamType::NUM; playModeNo++) {
        teamTypeBox[playModeNo].setSize(
            100, sd.obj.lowerFrame.box.top() - sd.obj.upperFrame.box.bottom() - 400);
        teamTypeBox[playModeNo].setLowerLeft(100 * playModeNo + 400, sd.obj.lowerFrame.box.top());
    }
    sd.ctrl.yes.icon.box.setSize(100, sd.obj.lowerFrame.box.top() - sd.obj.upperFrame.box.bottom() - 400);
    sd.ctrl.yes.icon.box.setLowerLeft(sd.screen.right() - 200, sd.obj.lowerFrame.box.top());
}
void PlayerSelect::reset() {
    Scene::reset(); 
    sd.teamNum = 0; 
    sd.playerNum = 0;
    sd.teamType = TeamType::SOLO;
    for (int chara = 0; chara < MAX_CHARA_NUM; chara++) 
        sd.chara[chara].isPlayer = false;
    for (int team = 0; team < MAX_PLAYER_NUM; team++) {
        for (int member = 0; member < DUO_MEMBER_NUM; member++) {
            sd.teamChara[team][member] = -1;
        }
    }
}
void PlayerSelect::draw() {
    Scene::draw();
    for (int chara = 0; chara < sd.charaNum; chara++) {
        if (!sd.chara[chara].isPlayer && sd.playerNum < MAX_PLAYER_NUM) {
            switch (Mouse::getInstance()->getClickBoxState(sd.chara[chara].image.box)) {
            case Key::RELEASED: 
                drawBoxObj(sd.chara[chara].image.box, sd.color.touch); 
                break;
            case Key::RELEASEDtoPRESSED: case Key::PRESSED: 
                drawBoxObj(sd.chara[chara].image.box, sd.color.press); 
                break;
            case Key::PRESSEDtoRELEASED: 
                sd.chara[chara].isPlayer = true; 
                playerChara[sd.playerNum] = chara;
                sd.playerNum++;
                chTeamType(teamTypeMem);
                break;
            default: 
                break;
            }
        }
        drawImage(sd.chara[chara].image);
        DrawStringToHandle(sd.chara[chara].image.box.left() + 5 * max(0, 10 - (int)sd.chara[chara].name.size()),
            sd.chara[chara].image.box.bottom() - sd.font.chara.size - 10,
            sd.chara[chara].name.c_str(), sd.color.w, sd.font.chara.handle);
        DrawStringToHandle(sd.chara[chara].image.box.left(), sd.chara[chara].image.box.top(),
            sd.chara[chara].groupName.c_str(), sd.color.w, sd.font.chara.handle);
        if (sd.chara[chara].isPlayer) {
            DrawGraph(sd.chara[chara].image.box.left(), 
                sd.chara[chara].image.box.center().y() - sd.pic.selected.image.box.height() / 2,
                sd.pic.selected.image.handle, TRUE);
        }
    }
    switch (sd.teamType) {
    case TeamType::SOLO:
    case TeamType::TOURNAMENT:
        for (int i = 0, player = 0; i < 4; i++) {
            for (int j = 0, chara = 0; j < 2; j++, player++) {
                DrawBox(sd.screen.right() - 200 + 100 * j, sd.obj.upperFrame.box.bottom() + 100 * i,
                    sd.screen.right() - 100 + 100 * j, sd.obj.upperFrame.box.bottom() + 100 + 100 * i,
                    sd.color.team[player], TRUE);
                chara = sd.teamChara[player][0];
                DrawCircle(95 + 40 * player, sd.obj.lowerFrame.box.top() - sd.font.normal.size / 2 - 15,
                    sd.font.normal.size + 2, sd.color.team[player], FALSE);
                if (sd.chara[chara].isPlayer && chara >= 0 && chara < sd.charaNum && player < sd.playerNum) {
                    DrawGraph(sd.screen.right() - 200 + 100 * j, sd.obj.upperFrame.box.bottom() + 100 * i,
                        sd.chara[chara].image.handle, TRUE);
                    DrawStringToHandle(
                        sd.screen.right() - 200 + 100 * j + 5 * max(0, 10 - (int)sd.chara[chara].name.size()),
                        sd.obj.upperFrame.box.bottom() + 75 + 100 * i,
                        sd.chara[chara].name.c_str(), sd.color.w, sd.font.chara.handle);
                    DrawStringToHandle(sd.screen.right() - 200 + 100 * j, sd.obj.upperFrame.box.bottom() + 100 * i,
                        rankName[player].c_str(), sd.color.w, sd.font.normal.handle);
                    DrawCircle(95 + 40 * player, sd.obj.lowerFrame.box.top() - sd.font.normal.size / 2 - 15,
                        sd.font.normal.size + 2, sd.color.team[player], TRUE);
                }
            }
        }
        for (int i = 0; i < MAX_PLAYER_NUM; i++) {
            DrawCircle(95 + 40 * i, sd.obj.lowerFrame.box.top() - sd.font.normal.size / 2 - 15,
                sd.font.normal.size + 2, sd.color.team[i], FALSE);
        }
        break;
    case TeamType::DUO:
        for (int team = 0, player = 0; team < MAX_TEAM_NUM; team++) {
            DrawBox(sd.screen.right() - 200, sd.obj.upperFrame.box.bottom() + 100 * team,
                sd.screen.right(), sd.obj.upperFrame.box.bottom() + 100 + 100 * team,
                sd.color.team[team], TRUE);
            for (int member = 0, chara = 0; member < DUO_MEMBER_NUM; member++, player++) {
                chara = sd.teamChara[team][member];
                DrawCircle(95 + 40 * player, sd.obj.lowerFrame.box.top() - sd.font.normal.size / 2 - 15,
                    sd.font.normal.size + 2, sd.color.team[team], FALSE);
                if (sd.chara[chara].isPlayer && chara >= 0 && chara < sd.charaNum && player < sd.playerNum) {
                    DrawGraph(sd.screen.right() - 200 + 100 * member, sd.obj.upperFrame.box.bottom() + 100 * team,
                        sd.chara[chara].image.handle, TRUE);
                    DrawStringToHandle(
                        sd.screen.right() - 200 + 100 * member + 5 * max(0, 10 - (int)sd.chara[chara].name.size()),
                        sd.obj.upperFrame.box.bottom() + 75 + 100 * team,
                        sd.chara[chara].name.c_str(), sd.color.w, sd.font.chara.handle);
                    DrawStringToHandle(sd.screen.right() - 200 + 100 * member, sd.obj.upperFrame.box.bottom() + 100 * team,
                        rankName[team + sd.teamNum * member].c_str(), sd.color.w, sd.font.normal.handle);
                    DrawCircle(95 + 40 * player, sd.obj.lowerFrame.box.top() - sd.font.normal.size / 2 - 15,
                        sd.font.normal.size + 2, sd.color.team[team], TRUE);
                }
            }
        }
        break;
    default:
        break;
    }
    if (sd.teamType != TeamType::SOLO) {
        switch (Mouse::getInstance()->getClickBoxState(teamTypeBox[TeamType::SOLO])) {
        case Key::RELEASED: 
            drawBoxObj(teamTypeBox[TeamType::SOLO], sd.color.touch); 
            break;
        case Key::RELEASEDtoPRESSED: 
        case Key::PRESSED: 
            drawBoxObj(teamTypeBox[TeamType::SOLO], sd.color.press); 
            break;
        case Key::PRESSEDtoRELEASED:
            teamTypeMem = TeamType::SOLO; 
            chTeamType(TeamType::SOLO);
            break;
        default: break;
        }
    }
    if (sd.teamType != TeamType::DUO && sd.playerNum > 1 && !(sd.playerNum % 2)) {
        switch (Mouse::getInstance()->getClickBoxState(teamTypeBox[TeamType::DUO])) {
        case Key::RELEASED: 
            drawBoxObj(teamTypeBox[TeamType::DUO], sd.color.touch); 
            break;
        case Key::RELEASEDtoPRESSED: 
        case Key::PRESSED: 
            drawBoxObj(teamTypeBox[TeamType::DUO], sd.color.press); 
            break;
        case Key::PRESSEDtoRELEASED: 
            teamTypeMem = TeamType::DUO;
        default: 
            if (teamTypeMem == TeamType::DUO) { 
                chTeamType(TeamType::DUO);
            } 
            break;
        }
    }
    if (sd.teamType != TeamType::TOURNAMENT && sd.playerNum > 2) {
        switch (Mouse::getInstance()->getClickBoxState(teamTypeBox[TeamType::TOURNAMENT])) {
        case Key::RELEASED: 
            drawBoxObj(teamTypeBox[TeamType::TOURNAMENT], sd.color.touch); 
            break;
        case Key::RELEASEDtoPRESSED: 
        case Key::PRESSED: 
            drawBoxObj(teamTypeBox[TeamType::TOURNAMENT], sd.color.press); 
            break;
        case Key::PRESSEDtoRELEASED: 
            teamTypeMem = TeamType::TOURNAMENT; 
        default: 
            if (teamTypeMem == TeamType::TOURNAMENT) { 
                chTeamType(TeamType::TOURNAMENT);
            }
            break;
        }
    }
    drawBoxObj(teamTypeBox[sd.teamType], sd.color.gy, TRUE);
    for (int playMode = 0; playMode < 2; playMode++) {
        DrawStringToHandle(teamTypeBox[playMode].left() + 5 + 4 * max(0, 8 - (int)playModeName[playMode].size()),
            teamTypeBox[playMode].center().y() - sd.font.normal.size / 2,
            playModeName[playMode].c_str(), sd.color.w, sd.font.normal.handle);
    }
    DrawStringToHandle(teamTypeBox[TeamType::TOURNAMENT].left() + 5,
        teamTypeBox[TeamType::TOURNAMENT].center().y() - sd.font.normal.size - 4,
        "Tour-\n nament", sd.color.w, sd.font.normal.handle);
    DrawBox(sd.screen.right() - 200, sd.obj.upperFrame.box.bottom() + 400,
        sd.screen.right() - 100, sd.obj.lowerFrame.box.top(), sd.color.press, TRUE);
    DrawStringToHandle(sd.screen.right() - 195, sd.ctrl.yes.icon.box.center().y() - sd.font.normal.size / 2,
        "OK!!", sd.color.w, sd.font.normal.handle);
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
    DrawStringToHandle(sd.screen.left(), sd.obj.lowerFrame.box.top() - sd.font.normal.size - 15,
        "Player", sd.color.w, sd.font.normal.handle);
    for (int i = 0; i < MAX_PLAYER_NUM; i++) 
        DrawStringToHandle(90 + 40 * i, sd.obj.lowerFrame.box.top() - sd.font.normal.size - 15,
            std::to_string(i + 1).c_str(), sd.color.w, sd.font.normal.handle);
    DrawGraph(sd.screen.right() - 140, sd.ctrl.yes.icon.box.center().y() - ICONSIZE_NORMAL.y() / 2,
        sd.ctrl.yes.key.image.handle, TRUE);
    DrawStringToHandle(sd.ctrl.mute[sd.sound].icon.box.right() + 5,
        sd.obj.upperFrame.box.center().y() - sd.font.normal.size / 2,
        (gameName[sd.game] + " < Player Select").c_str(), sd.color.w, sd.font.normal.handle);
    return;
}
void PlayerSelect::update() {
    Scene::update(); 
    if (sd.playerNum < MAX_PLAYER_NUM) {
        int nextChara = playerChara[sd.playerNum];
        if (nextChara >= 0 && nextChara < sd.charaNum) {
            if (!sd.chara[nextChara].isPlayer) {
                drawImage(sd.ctrl.forward.icon);
                if (ctrlRQ(sd.ctrl.forward)) { 
                    sd.chara[nextChara].isPlayer = true; 
                    sd.playerNum++; 
                    chTeamType(teamTypeMem);
                    return; 
                }
            }
        }
    }
    if (ctrlRQ(sd.ctrl.skip)) {
        while (sd.playerNum < MAX_PLAYER_NUM) {
            int chara = playerChara[sd.playerNum];
            if (chara < 0 || chara >= sd.charaNum) { 
                break; 
            }
            if (sd.chara[chara].isPlayer) { 
                break; 
            }
            sd.chara[chara].isPlayer = true; 
            sd.playerNum++;
        }
        chTeamType(teamTypeMem);
        mNextScene = GAME_START;
    }
    else if (sd.playerNum > 0 && ctrlRQ(sd.ctrl.yes)) mNextScene = GAME_START; 
    else if (ctrlRQ(sd.ctrl.back)) {
        if (sd.playerNum > 0) { 
            sd.playerNum--; 
            sd.chara[playerChara[sd.playerNum]].isPlayer = false; 
            chTeamType(teamTypeMem);
            return;
        }
        mNextScene = GAME_SELECT;
    }
    return;
}
bool PlayerSelect::chTeamType(int teamType) {
    int tmpTeamType = teamType;
    switch (teamType) {
    case TeamType::DUO:
        if (sd.playerNum < 2 || sd.playerNum % 2 == 1) {
            tmpTeamType = TeamType::SOLO;
        }
        break;
    case TeamType::TOURNAMENT:
        if (sd.playerNum < 3) {
            tmpTeamType = TeamType::SOLO;
        }
        break;
    default: 
        break;
    }
    switch (tmpTeamType) {
    case TeamType::SOLO: 
    case TeamType::TOURNAMENT:
        sd.teamNum = sd.playerNum;
        for (int player = 0; player < MAX_PLAYER_NUM; player++) {
            for (int member = 0; member < DUO_MEMBER_NUM; member++) {
                sd.teamChara[player][member] = -1;
            }
        }
        for (int player = 0; player < MAX_PLAYER_NUM; player++) {
            sd.teamChara[player][0] = playerChara[player];
        }
        break;
    case TeamType::DUO:
        sd.teamNum = sd.playerNum / 2;
        for (int player = 0; player < MAX_PLAYER_NUM; player++) {
            for (int member = 0; member < DUO_MEMBER_NUM; member++) {
                sd.teamChara[player][member] = -1;
            }
        }
        for (int team = 0; team < MAX_TEAM_NUM; team++) {
            for (int member = 0; member < DUO_MEMBER_NUM; member++) {
                sd.teamChara[team][member] = playerChara[DUO_MEMBER_NUM * team + member];
            }
        }
        break;
    default:
        return false;
    }
    sd.teamType = tmpTeamType;
    return true;
}
PlayerSelect::~PlayerSelect() {
}