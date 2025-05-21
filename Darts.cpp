#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "winmm.lib")   // MSVC —p
#include "Darts.h"
#include "resource.h"
#include "colorlib.h"
#include <filesystem>
namespace fs = std::filesystem;
namespace cl = colorlib;
// Initialize various Value
Darts::Darts() : scene(ConfigID::Home), PlayingBGMNo(0), WindowMode(FALSE),
PauseMode(PauseID::Resume), MuteMode(MuteModeID::Unmute), player(), playerNum(0) {
    SetWindowIconID(IDI_ICON1); 
    SetWindowText("Lightning Darts");
    SetLogDrawOutFlag(FALSE); 
    if (DxLib_Init()) exit(-1);
    SRand(GetNowCount()); // initialize random number by the lapse time in Windows
    ctrl.home.key.code    = KEY_INPUT_H;      ctrl.back.key.code  = KEY_INPUT_BACK;
    ctrl.forward.key.code = KEY_INPUT_RETURN; ctrl.quit.key.code  = KEY_INPUT_ESCAPE;
    ctrl.config.key.code  = KEY_INPUT_C;      ctrl.skill.key.code = KEY_INPUT_E;
    ctrl.skip.key.code    = KEY_INPUT_S;      ctrl.init.key.code  = KEY_INPUT_I;
    ctrl.reGame.key.code  = KEY_INPUT_G;      ctrl.chBGM.key.code = KEY_INPUT_B;
    ctrl.left.key.code    = KEY_INPUT_LEFT;   ctrl.right.key.code = KEY_INPUT_RIGHT;
    ctrl.up.key.code      = KEY_INPUT_UP;     ctrl.down.key.code  = KEY_INPUT_DOWN;
    ctrl.start.key.code   = KEY_INPUT_SPACE;  ctrl.yes.key.code   = KEY_INPUT_Y;
    ctrl.no.key.code      = KEY_INPUT_N;      ctrl.reset.key.code = KEY_INPUT_R;
    for (int i = 0; i < 2; i++) {
        ctrl.mute[i].key.code = KEY_INPUT_M;
        ctrl.chWindow[i].key.code = KEY_INPUT_F11;
        ctrl.pause[i].key.code = KEY_INPUT_P;
    }
    ChangeWindow(WindowMode);
    // load color
    color.w = cl::srgb("w"); color.y = cl::srgb("y"); color.k = cl::srgb("k");
    color.OK = cl::srgb("Strawberry"); color.Reset = cl::srgb("AshGrey");
    color.player[0] = cl::srgb("b"); color.player[1] = cl::srgb("r"); color.player[2] = cl::srgb("g");
    color.player[3] = color.y; color.player[4] = cl::srgb("c"); color.player[5] = cl::srgb("m");
    color.player[6] = cl::srgb("Violet"); color.player[7] = cl::srgb("Marigold");
    ICONSIZE_NORMAL.setXY(25, 25);
    obj.upperFrame.box.setSize(screen.width(), ICONSIZE_NORMAL.y());
    obj.upperFrame.box.setUpperLeft(screen.upperLeft());
    obj.upperFrame.color = cl::srgb("CocoaBrown");
    obj.lowerFrame.box.setSize(screen.width(), ICONSIZE_NORMAL.y());
    obj.lowerFrame.box.setLowerLeft(screen.lowerLeft());
    obj.lowerFrame.color = cl::srgb("CocoaBrown");
    ctrl.home.name = "Home"; ctrl.home.icon.box.setSize(30, 25); ctrl.home.icon.box.setUpperLeft(screen.upperLeft());
    ctrl.back.name = "Back"; ctrl.back.icon.box.setSize(ICONSIZE_NORMAL); 
    ctrl.back.icon.box.setUpperLeft(ctrl.home.icon.box.upperRight());
    ctrl.forward.name = "Forward"; ctrl.forward.icon.box.setSize(ICONSIZE_NORMAL); 
    ctrl.forward.icon.box.setUpperLeft(ctrl.back.icon.box.upperRight());
    ctrl.mute[0].name = "Unmute"; ctrl.mute[1].name = "Mute";
    for (int i = 0; i < 2; i++) {
        ctrl.mute[i].icon.box.setSize(ICONSIZE_NORMAL);
        ctrl.mute[i].icon.box.setUpperLeft(ctrl.forward.icon.box.upperRight());
    }
    ctrl.quit.name = "Quit"; ctrl.quit.icon.box.setSize(ICONSIZE_NORMAL); 
    ctrl.quit.icon.box.setUpperRight(screen.upperRight());
    ctrl.chWindow[0].name = "Another window"; ctrl.chWindow[1].name = "Maximize window";
    for (int i = 0; i < 2; i++) {
        ctrl.chWindow[i].icon.box.setSize(ICONSIZE_NORMAL);
        ctrl.chWindow[i].icon.box.setUpperRight(ctrl.quit.icon.box.upperLeft());
    }
    ctrl.config.name = "Config"; ctrl.config.icon.box.setSize(ICONSIZE_NORMAL);
    ctrl.config.icon.box.setUpperRight(ctrl.chWindow[0].icon.box.upperLeft());
    ctrl.pause[0].name = "Pause"; ctrl.pause[1].name = "Resume";
    for (int i = 0; i < 2; i++) {
        ctrl.pause[i].icon.box.setSize(ICONSIZE_NORMAL);
        ctrl.pause[i].icon.box.setUpperRight(ctrl.config.icon.box.upperLeft());
    }
    ctrl.skill.name = "Skill"; ctrl.skill.icon.box.setSize(ICONSIZE_NORMAL); 
    ctrl.skill.icon.box.setUpperRight(ctrl.pause[0].icon.box.upperLeft());
    ctrl.skip.name = "Skip"; ctrl.skip.icon.box.setSize(76, 25); 
    ctrl.skip.icon.box.setLowerRight(obj.lowerFrame.box.upperRight());
    ctrl.init.name = "Initialize"; ctrl.init.icon.box.setSize(ICONSIZE_NORMAL); 
    ctrl.init.icon.box.setLowerLeft(screen.lowerLeft());
    ctrl.reGame.name = "Restart from game select"; ctrl.reGame.icon.box.setSize(ICONSIZE_NORMAL); 
    ctrl.reGame.icon.box.setLowerLeft(ctrl.init.icon.box.lowerRight());
    ctrl.chBGM.name = "Change BGM"; ctrl.chBGM.icon.box.setSize(ICONSIZE_NORMAL); 
    ctrl.chBGM.icon.box.setLowerLeft(ctrl.reGame.icon.box.lowerRight());
    thunder.image.pos.setXY(-50, -200);

    // load BGM
    fs::directory_iterator iter(bgmFolderPath), end;
    std::error_code err;
    int folderPathLength = bgmFolderPath.size(), bgmPathLength = 0;
    for (int i = 0; iter != end && !err && i < MAX_BGM_NUM; iter.increment(err), i++) {
        const fs::directory_entry entry = *iter;
        bgm[i].name = entry.path().string();
        bgm[i].handle = LoadSoundMem(bgm[i].name.c_str());
        bgmPathLength = bgm[i].name.size();
        if (bgm[i].handle == -1 || bgmPathLength - folderPathLength - 5 < 0) { i--; continue; }
        bgm[i].name.erase(bgmPathLength - 4, 4);
        bgm[i].name.erase(0, folderPathLength + 1);
    }
    if (err) {
        std::cout << err.value() << std::endl;
        std::cout << err.message() << std::endl;
    }
    // load sound effect
    for (int i = 0; i < SEID::Num; i++) SE[i] = LoadSoundMemByResource(MAKEINTRESOURCE(IDR_WAVE10 + i), "MP3");
    for (int i = 0; i < SEID::ComboNum; i++) ComboSE[i] = LoadSoundMemByResource(MAKEINTRESOURCE(IDR_WAVE14 + i), "MP3");
    InitScene();
    //FILE* icdf = fopen(DataFileName, "rb"); // open data file
    //if (icdf == NULL) { // if file doesn't exist, 
    //    InitScene();
    //}
    //else { // if file exists,
    //    fread(&cd, sizeof(cd), 1, icdf); fclose(icdf); // read & close file
    //    ChangeSoundVol(0, cd.SoundVol[0]); // set sound's volume
    //}
    
    updateScene(Scene::Home); // set home scene
}
void Darts::ChangeWindow(int WindowModeFlag) {
    screen.setSize(900, 500); screen.setUpperLeft(0, 0);
    if (!WindowModeFlag) SetGraphMode(screen.width(), screen.height(), 32); // change screen size
    ChangeWindowMode(WindowModeFlag); 
    SetDrawScreen(DX_SCREEN_BACK);
    SetMouseDispFlag(TRUE);
    // load image
    for (int i = 0; i < KEY_NUM; i++) key[KeyNo[i]].image.handle = LoadGraphToResource(MAKEINTRESOURCE(KeyNo[i]), "PNG");
    ctrl.home.key.image.handle = key[ctrl.home.key.code].image.handle;
    ctrl.back.key.image.handle = key[ctrl.back.key.code].image.handle;
    ctrl.forward.key.image.handle = key[ctrl.forward.key.code].image.handle;
    ctrl.quit.key.image.handle = key[ctrl.quit.key.code].image.handle;
    ctrl.config.key.image.handle = key[ctrl.config.key.code].image.handle;
    ctrl.skill.key.image.handle = key[ctrl.skill.key.code].image.handle;
    ctrl.skip.key.image.handle = key[ctrl.skip.key.code].image.handle;
    ctrl.init.key.image.handle = key[ctrl.init.key.code].image.handle;
    ctrl.reGame.key.image.handle = key[ctrl.reGame.key.code].image.handle;
    ctrl.chBGM.key.image.handle = key[ctrl.chBGM.key.code].image.handle;
    ctrl.left.key.image.handle = key[ctrl.left.key.code].image.handle;
    ctrl.right.key.image.handle = key[ctrl.right.key.code].image.handle;
    ctrl.up.key.image.handle = key[ctrl.up.key.code].image.handle;
    ctrl.down.key.image.handle = key[ctrl.down.key.code].image.handle;
    ctrl.start.key.image.handle = key[ctrl.start.key.code].image.handle;
    ctrl.no.key.image.handle = key[ctrl.no.key.code].image.handle;
    ctrl.yes.key.image.handle = key[ctrl.yes.key.code].image.handle;
    ctrl.reset.key.image.handle = key[ctrl.reset.key.code].image.handle;
    for (int i = 0; i < 2; i++) {
        ctrl.mute[i].key.image.handle = key[ctrl.mute[i].key.code].image.handle;
        ctrl.chWindow[i].key.image.handle = key[ctrl.chWindow[i].key.code].image.handle;
        ctrl.pause[i].key.image.handle = key[ctrl.pause[i].key.code].image.handle;
    }
    
    std::error_code err;
    int folderPathLength = 0, charaPathLength = 0; 
    charaNum = 0;
    std::string folderPath;
    for (int grade = 0; grade < GRADE_NUM; grade++) {
        gradeCharaNum[grade] = 0;
        folderPath = playerFolderPath + gradeName[grade];
        std::filesystem::directory_iterator iter(folderPath), end;
        folderPathLength = folderPath.size();
        for (; iter != end && !err && charaNum < MAX_CHARA_NUM; iter.increment(err)) {
            const std::filesystem::directory_entry entry = *iter;
            chara[charaNum].name = entry.path().string();
            chara[charaNum].image.handle = LoadGraph(chara[charaNum].name.c_str());
            charaPathLength = chara[charaNum].name.size();
            if (chara[charaNum].image.handle == -1 || charaPathLength - folderPathLength - 1 < 0) { continue; }
            chara[charaNum].grade = grade;
            chara[charaNum].name.erase(charaPathLength - 4, 4);
            chara[charaNum].name.erase(0, folderPathLength + 1);
            charaNum++, gradeCharaNum[grade]++;
        }
    }
    if (err) {
        std::cout << err.value() << std::endl;
        std::cout << err.message() << std::endl;
    }
    ctrl.left.icon.handle = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG68), "PNG");
    ctrl.right.icon.handle = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG69), "PNG");
    ctrl.skill.icon.handle = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG70), "PNG");
    ctrl.home.icon.handle = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG71), "PNG");
    ctrl.reGame.icon.handle = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG72), "PNG");
    ctrl.init.icon.handle = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG73), "PNG");
    ctrl.skip.icon.handle = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG74), "PNG");
    ctrl.quit.icon.handle = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG75), "PNG");
    ctrl.config.icon.handle = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG76), "PNG");
    ctrl.chBGM.icon.handle = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG77), "PNG");
    for (int i = 0; i < 2; i++) {
        ctrl.chWindow[i].icon.handle = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG78 + i), "PNG");
        ctrl.mute[i].icon.handle = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG80 + i), "PNG");
        ctrl.pause[i].icon.handle = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG82 + i), "PNG");
    }
    ctrl.back.icon.handle = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG84), "PNG");
    ctrl.forward.icon.handle = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG85), "PNG");
    ctrl.down.icon.handle = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG86), "PNG");
    ctrl.up.icon.handle = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG87), "PNG");
    darts.image.handle = LoadGraphToResource(MAKEINTRESOURCE(IDB_JPG1), "JPG");
    selected.image.handle = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG88), "PNG");
    thunder.image.handle = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG89), "PNG");
    // load font
    font.title.handle = 
        CreateFontToHandle(font.title.name.c_str(), font.title.size, font.title.thick, font.title.type);
    font.normal.handle = 
        CreateFontToHandle(font.normal.name.c_str(), font.normal.size, font.normal.thick, font.normal.type);
    font.chara.handle =
        CreateFontToHandle(font.chara.name.c_str(), font.chara.size, font.chara.thick, font.chara.type);
    WindowMode = GetWindowModeFlag(); return;
}
void Darts::Config() {
    DrawBox(screen.left(), obj.upperFrame.box.bottom(),
        screen.center().x(), obj.lowerFrame.box.top(), color.player[0], TRUE);
    DrawBox(screen.center().x(), obj.upperFrame.box.bottom(),
        screen.right(), obj.lowerFrame.box.top(), color.player[1], TRUE);
    DrawStringToHandle(5, 25, "Music", color.w, font.title.handle);
    DrawStringToHandle(screen.center().x() + 5, 25, "System", color.w, font.title.handle);
    DrawStringToHandle(5, 180, "Play Style", color.w, font.normal.handle);
    DrawStringToHandle(115, 180, (": " + BGMModeName[cd.BGMMode]).c_str(), color.w, font.normal.handle);
    DrawStringToHandle(0, 210, "Scene Theme", color.w, font.normal.handle);
    DrawGraph(355, 175, ctrl.down.icon.handle, TRUE); 
    DrawGraph(380, 175, ctrl.up.icon.handle, TRUE);
    DrawGraph(610, 425, key[ctrl.forward.key.code].image.handle, TRUE);
    DrawStringToHandle(450, 430, "Forward", color.w, font.normal.handle);
    DrawGraph(420, 425, ctrl.forward.icon.handle, TRUE);
    for (int i = 0; i < SoundID::NUM; i++) {
        DrawStringToHandle(0, 90 + 30 * i, SoundName[i].c_str(), color.w, font.normal.handle);
        DrawBox(100, 95 + 30 * i, 100 + cd.SoundVol[i], 105 + 30 * i, color.w, TRUE);
        DrawBox(100 + cd.SoundVol[i], 95 + 30 * i, 300, 105 + 30 * i, color.k, TRUE);
        DrawBox(95 + cd.SoundVol[i], 90 + 30 * i, 105 + cd.SoundVol[i], 110 + 30 * i, color.player[2], TRUE);
        DrawFormatStringToHandle(315, 90 + 30 * i, color.w, font.normal.handle, "%3d", cd.SoundVol[i]);
        DrawStringToHandle(5, 240 + 30 * i, SceneName[i].c_str(), color.w, font.normal.handle);
        DrawStringToHandle(115, 240 + 30 * i, (": " + bgm[i].name).c_str(), color.w, font.normal.handle);
        for (int k = 0; k < 2; k++) {
            DrawGraph(355, 85 + 30 * i + 150 * k, ctrl.down.icon.handle, TRUE);
            DrawGraph(380, 85 + 30 * i + 150 * k, ctrl.up.icon.handle, TRUE);
        }
        if (Mouse::getInstance()->getClickBoxCount(355, 85 + 30 * i, 380, 110 + 30 * i) > 10) 
            ChangeSoundVol(i, cd.SoundVol[i] - 1);
        else if (Mouse::getInstance()->getClickBoxCount(380, 85 + 30 * i, 405, 110 + 30 * i) > 10)
            ChangeSoundVol(i, cd.SoundVol[i] + 1);
        else if (Mouse::getInstance()->getClickBoxCount(355, 85 + 30 * i, 380, 110 + 30 * i) == 1) 
            ChangeSoundVol(i, cd.SoundVol[i] - 4);
        else if (Mouse::getInstance()->getClickBoxCount(380, 85 + 30 * i, 405, 110 + 30 * i) == 1) 
            ChangeSoundVol(i, cd.SoundVol[i] + 4);
        if (Mouse::getInstance()->getClickBoxState(99, 88 + 30 * i, 302, 112 + 30 * i) == Key::PRESSED) {
            ChangeSoundVol(i, Mouse::getInstance()->x() - 100);
        }
    }
    int iconX = screen.center().x() + 5, keyX = iconX + 315;
    DrawGraph(iconX, 100, ctrl.home.icon.handle, TRUE); DrawGraph(keyX, 100, ctrl.home.key.image.handle, TRUE);
    DrawStringToHandle(iconX + ctrl.home.icon.box.width() + 5, 100 + (ctrl.home.icon.box.height() - font.normal.size) / 2,
        ctrl.home.name.c_str(), color.w, font.normal.handle);

    DrawGraph(iconX, 130, ctrl.reGame.icon.handle, TRUE); DrawGraph(keyX, 130, ctrl.reGame.key.image.handle, TRUE);
    DrawStringToHandle(iconX + ctrl.reGame.icon.box.width() + 5, 130 + (ctrl.reGame.icon.box.height() - font.normal.size) / 2,
        ctrl.reGame.name.c_str(), color.w, font.normal.handle);

    DrawGraph(iconX, 160, ctrl.init.icon.handle, TRUE); DrawGraph(keyX, 160, ctrl.home.key.image.handle, TRUE);
    DrawStringToHandle(iconX + ctrl.init.icon.box.width() + 5, 160 + (ctrl.init.icon.box.height() - font.normal.size) / 2,
        ctrl.init.name.c_str(), color.w, font.normal.handle);

    DrawGraph(iconX, 190, ctrl.skip.icon.handle, TRUE); DrawGraph(keyX, 190, ctrl.skip.key.image.handle, TRUE);
    DrawStringToHandle(iconX + ctrl.skip.icon.box.width() + 5, 190 + (ctrl.skip.icon.box.height() - font.normal.size) / 2,
        ctrl.skip.name.c_str(), color.w, font.normal.handle);

    DrawGraph(iconX, 220, ctrl.quit.icon.handle, TRUE); DrawGraph(keyX, 220, ctrl.quit.key.image.handle, TRUE);
    DrawStringToHandle(iconX + ctrl.quit.icon.box.width() + 5, 220 + (ctrl.quit.icon.box.height() - font.normal.size) / 2,
        ctrl.quit.name.c_str(), color.w, font.normal.handle);

    DrawGraph(iconX, 250, ctrl.config.icon.handle, TRUE); DrawGraph(keyX, 250, ctrl.config.key.image.handle, TRUE);
    DrawStringToHandle(iconX + ctrl.config.icon.box.width() + 5, 250 + (ctrl.config.icon.box.height() - font.normal.size) / 2,
        ctrl.config.name.c_str(), color.w, font.normal.handle);

    DrawGraph(iconX, 280, ctrl.chBGM.icon.handle, TRUE); DrawGraph(keyX, 280, ctrl.chBGM.key.image.handle, TRUE);
    DrawStringToHandle(iconX + ctrl.chBGM.icon.box.width() + 5, 280 + (ctrl.chBGM.icon.box.height() - font.normal.size) / 2,
        ctrl.chBGM.name.c_str(), color.w, font.normal.handle);

    DrawGraph(iconX, 310, ctrl.chWindow[WindowMode].icon.handle, TRUE); 
    DrawGraph(keyX, 310, ctrl.chWindow[WindowMode].key.image.handle, TRUE);
    DrawStringToHandle(iconX + ctrl.chWindow[WindowMode].icon.box.width() + 5, 
        310 + (ctrl.chWindow[WindowMode].icon.box.height() - font.normal.size) / 2,
        ctrl.chWindow[WindowMode].name.c_str(), color.w, font.normal.handle);

    DrawGraph(iconX, 340, ctrl.mute[MuteMode].icon.handle, TRUE);
    DrawGraph(keyX, 340, ctrl.mute[MuteMode].key.image.handle, TRUE);
    DrawStringToHandle(iconX + ctrl.mute[MuteMode].icon.box.width() + 5, 
        340 + (ctrl.mute[MuteMode].icon.box.height() - font.normal.size) / 2,
        ctrl.mute[MuteMode].name.c_str(), color.w, font.normal.handle);
    
    DrawGraph(iconX, 370, ctrl.pause[PauseMode].icon.handle, TRUE);
    DrawGraph(keyX, 370, ctrl.pause[PauseMode].key.image.handle, TRUE);
    DrawStringToHandle(iconX + ctrl.pause[PauseMode].icon.box.width() + 5, 
        370 + (ctrl.pause[PauseMode].icon.box.height() - font.normal.size) / 2,
        ctrl.pause[PauseMode].name.c_str(), color.w, font.normal.handle);

    if (SkillMode) DrawStringToHandle(350, 430, "On", color.w, font.normal.handle);
    else DrawStringToHandle(350, 430, "Off", color.w, font.normal.handle);
    if (isCtrled(ctrl.back)) updateScene(scene + Scene::Num);
    else if (isBoxClicked(355, 175, 405, 200)) {
        PlaySoundMem(SE[1], DX_PLAYTYPE_BACK);
        if (Mouse::getInstance()->x() < 380) { cd.BGMMode += BGMModeID::Num - 1; }
        else { cd.BGMMode++; } 
        cd.BGMMode %= BGMModeID::Num;
        if (cd.BGMMode == BGMModeID::Loop) { PlaySoundMem(bgm[PlayingBGMNo].handle, DX_PLAYTYPE_BACK | DX_PLAYTYPE_LOOP); }
        else { PlaySoundMem(bgm[PlayingBGMNo].handle, DX_PLAYTYPE_BACK); }
    }
    else if (isCtrlIconClicked(ctrl.quit)) scene = Scene::Quit;
    return;
}
bool Darts::ChangeSound(int MuteModeNo) {
    if (MuteModeNo < 0 || MuteModeNo >= MuteModeID::Num) return false; // if mute mode doesn't exist, exit
    if (MuteModeNo == MuteModeID::Mute) StopSoundMem(bgm[PlayingBGMNo].handle); // if mute is selected, stop BGM
    else { // if unmute is selected
        ChangeVolumeSoundMem(BGMVol, bgm[PlayingBGMNo].handle); // set BGM volume
        // play BGM
        if (cd.BGMMode == BGMModeID::Loop) { 
            PlaySoundMem(bgm[PlayingBGMNo].handle, DX_PLAYTYPE_BACK | DX_PLAYTYPE_LOOP, 0); 
        }
        else { PlaySoundMem(bgm[PlayingBGMNo].handle, DX_PLAYTYPE_BACK, 0); }
    } MuteMode = CheckSoundMem(bgm[PlayingBGMNo].handle); // check mute mode
    return true;
}
bool Darts::ChangeSoundVol(int SoundNo, int Vol) {
    if (SoundNo < 0 || SoundNo >= SoundID::NUM) return false; // if sound doesn't exist, exit
    if (Vol > SoundVol::Max) { // if sound's volume is above maximum volume,
        cd.SoundVol[SoundNo] = SoundVol::Max; // set maximum volume
    }
    else if (Vol < SoundVol::Min) { // if sound's volume is below minimum volume,
        cd.SoundVol[SoundNo] = SoundVol::Min; // set minimum volume
    }
    else cd.SoundVol[SoundNo] = Vol; // othewise set sound's volume
    if (SoundNo != SoundID::SE) { // if sound is BGMs or total sounds,
        BGMVol = (int)(0.0064 * cd.SoundVol[SoundID::Total] * cd.SoundVol[SoundID::BGM]); // set BGMs' volume
        ChangeVolumeSoundMem(BGMVol, bgm[PlayingBGMNo].handle); // change volume of BGM which is now playing
    }
    if (SoundNo != SoundID::BGM) { // if sound is SEs or total sounds,
        SEVol = (int)(0.0064 * cd.SoundVol[SoundID::Total] * cd.SoundVol[SoundID::SE]); // set SEs' volume
        // change SEs'volume
        for (int i = 0; i < SEID::Num; i++) { ChangeVolumeSoundMem(SEVol, SE[i]); }
        for (int i = 0; i < SEID::ComboNum; i++) { ChangeVolumeSoundMem(SEVol, ComboSE[i]); }
    } return true;
}
bool Darts::PlayBGM(int BGMNo) {
    if (BGMNo < 0 || BGMNo >= MAX_BGM_NUM) return false; // if BGM doesn't exist, exit
    // if BGM exists,
    StopSoundMem(bgm[PlayingBGMNo].handle);  // stop BGM which is now playing
    ChangeVolumeSoundMem(BGMVol, bgm[BGMNo].handle); // set BGM's volume
    if (MuteMode == MuteModeID::Unmute) { // if sounds aren't muted, 
        // play BGM
        if (cd.BGMMode == BGMModeID::Loop) { PlaySoundMem(bgm[BGMNo].handle, DX_PLAYTYPE_BACK | DX_PLAYTYPE_LOOP); }
        else { PlaySoundMem(bgm[BGMNo].handle, DX_PLAYTYPE_BACK); }
    }
    PlayingBGMNo = BGMNo; // set BGM
    return true;
}
void Darts::Home() {
    DrawStringToHandle(screen.center().x() - 3 * font.title.size, screen.center().y() - font.title.size / 2,
        "Lightning Darts", color.y, font.title.handle);
    DrawGraph(screen.center().x() - 56, 11 * screen.center().y() / 8 - 12, ctrl.start.key.image.handle, TRUE);
    int y = 11 * screen.center().y() / 8 - font.normal.size / 2;
    DrawStringToHandle(11 * screen.center().x() / 16, y, "Press", color.w, font.normal.handle);
    DrawStringToHandle(screen.center().x() - 30, y, "space", color.w, font.normal.handle);
    DrawStringToHandle(19 * screen.center().x() / 16, y, "to start ...", color.w, font.normal.handle);
    if (isCtrlKeyTyped(ctrl.start)) { updateScene(Scene::GameSelect); return; }
    if (isCtrled(ctrl.back)) { scene = Scene::Quit; return; }
    if (isCtrled(ctrl.forward)) { updateScene(Scene::GameSelect); return; } return;
}
void Darts::GameSelect() {
    Box box; box.setSize(230, font.normal.size + 12);
    for (int i = 0; i < GAME_KIND_NUM; i++) {
        DrawStringToHandle(screen.left() + 5 + screen.size().x() * i / 3, obj.upperFrame.box.bottom(),
            gameKindName[i].c_str(), color.w, font.title.handle);
        for (int j = 0; j < GAME_NUM; j++) {
            box.setUpperLeft(screen.left() + 20 + screen.size().x() * i / 3,
                obj.upperFrame.box.bottom() + font.title.size + 4 + screen.size().y() * j / 10);
            switch (Mouse::getInstance()->getClickBoxState(box)) {
            case Key::RELEASED: 
                DrawBox(box.upperLeft().x(), box.upperLeft().y(), box.lowerRight().x(), box.lowerRight().y(), 
                    color.y, TRUE); break;
            case Key::PRESSED:
                DrawBox(box.upperLeft().x(), box.upperLeft().y(), box.lowerRight().x(), box.lowerRight().y(),
                    color.OK, TRUE); break;
            case Key::PRESSEDtoRELEASED:
                cd.gameKind = i; cd.game = j; updateScene(Scene::CharSelect); break;
            default: break;
            }
            DrawStringToHandle(screen.left() + 20 + screen.size().x() * i / 3,
                obj.upperFrame.box.bottom() + font.title.size + 10 + screen.size().y() * j / 10,
                gameName[i][j].c_str(), color.w, font.normal.handle);
        }
    }
    if (isCtrled(ctrl.back)) { updateScene(Scene::Home); return; }
    if (cd.gameKind >= 0 && cd.gameKind < GAME_KIND_NUM && cd.game >= 0 && cd.game < GAME_NUM) {
        if (isCtrled(ctrl.forward) || isCtrled(ctrl.skip)) { updateScene(Scene::CharSelect); return; }
    } return;
}
void Darts::CharSelect() {
    if (isCtrled(ctrl.back)) { 
        if (playerNum > 0) { playerNum--; chara[player[playerNum]].isPlayer = false; }
        else { updateScene(Scene::GameSelect); return; }
    }
    int charaNo = 0;
    for (int grade = 0; grade < GRADE_NUM; grade++) {
        for (int i = 0; i < gradeCharaNum[grade]; i++, charaNo++) {
            DrawGraph(screen.left() + 100 * i, obj.upperFrame.box.bottom() + 100 * grade, 
                chara[charaNo].image.handle, TRUE);
            DrawStringToHandle(100 * i + 5 * max(0, 10 - (int)chara[charaNo].name.size()), 
                obj.upperFrame.box.bottom() + 75 + 100 * grade,
                chara[charaNo].name.c_str(), color.w, font.chara.handle);
            if (chara[charaNo].isPlayer) {
                DrawGraph(screen.left() + 100 * i, obj.upperFrame.box.bottom() + 100 * grade, selected.image.handle, TRUE);
            }
            else if (playerNum < MAX_PLAYER_NUM && 
                isBoxClicked(screen.left() + 100 * i, obj.upperFrame.box.bottom() + 100 * grade,
                screen.left() + 100 * i + 100, obj.upperFrame.box.bottom() + 100 * grade + 100)) {
                chara[charaNo].isPlayer = true; player[playerNum] = charaNo; playerNum++;
            }
        }
        DrawStringToHandle(screen.left(), obj.upperFrame.box.bottom() + 100 * grade,
            gradeName[grade].c_str(), color.w, font.normal.handle);
    }
    int playerNo = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++, playerNo++) {
            DrawBox(screen.right() - 200 + 100 * j, obj.upperFrame.box.bottom() + 100 * i,
                screen.right() - 100 + 100 * j, obj.upperFrame.box.bottom() + 100 + 100 * i, 
                color.player[playerNo], TRUE);
            if (playerNo < playerNum) {
                DrawGraph(screen.right() - 200 + 100 * j, obj.upperFrame.box.bottom() + 100 * i,
                    chara[player[playerNo]].image.handle, TRUE);
                DrawFormatStringToHandle(screen.right() - 200 + 100 * j, obj.upperFrame.box.bottom() + 100 * i,
                    color.w, font.normal.handle, "%dP", playerNo + 1);
                DrawStringToHandle(
                    screen.right() - 200 + 100 * j + 5 * max(0, 10 - (int)chara[player[playerNo]].name.size()),
                    obj.upperFrame.box.bottom() + 75 + 100 * i,
                    chara[player[playerNo]].name.c_str(), color.w, font.chara.handle);
            }
        }
    }
    DrawBox(screen.right() - 200, obj.upperFrame.box.bottom() + 400,
        screen.right() - 100, obj.lowerFrame.box.top(), color.OK, TRUE);
    DrawBox(screen.right() - 100, obj.upperFrame.box.bottom() + 400,
        screen.right(), obj.lowerFrame.box.top(), color.Reset, TRUE);
    for (int i = 0; i < 6; i++) 
        DrawLine(screen.left() + 100 * i, obj.upperFrame.box.bottom(), 
            screen.left() + 100 * i, obj.upperFrame.box.bottom() + 400, color.k);
    for (int i = 6; i < 10; i++)
        DrawLine(screen.left() + 100 * i, obj.upperFrame.box.bottom(),
            screen.left() + 100 * i, obj.lowerFrame.box.top(), color.k);
    DrawLine(700, 25, 700, 475, color.k, 2);
    for (int i = 0; i <= GRADE_NUM; i++) 
        DrawLine(screen.left(), obj.upperFrame.box.bottom() + 100 * i, screen.right(),
            obj.upperFrame.box.bottom() + 100 * i, color.k);
    DrawStringToHandle(screen.left(), obj.lowerFrame.box.top() - font.normal.size - 15, 
        "Player", color.w, font.normal.handle);
    for (int i = 0; i < playerNum; i++) {
        DrawCircle(95 + 40 * i, obj.lowerFrame.box.top() - font.normal.size / 2 - 15,
            font.normal.size + 2, color.player[i], TRUE);
    }
    for (int i = 0; i < MAX_PLAYER_NUM; i++) {
        DrawCircle(95 + 40 * i, obj.lowerFrame.box.top() - font.normal.size / 2 - 15, 
            font.normal.size + 2, color.player[i], FALSE);
        DrawStringToHandle(90 + 40 * i, obj.lowerFrame.box.top() - font.normal.size - 15,
            std::to_string(i + 1).c_str(), color.w, font.normal.handle);
    }
    for (int i = 0; i < 2; i++) {
        DrawStringToHandle(400 + 100 * i + 4 * max(0, 8 - (int)playStyleName[i].size()), 
            obj.lowerFrame.box.top() - font.normal.size - 15, playStyleName[i].c_str(), color.w, font.normal.handle);
    }   
    DrawStringToHandle(610, obj.lowerFrame.box.top() - 2 * font.normal.size - 10,
        "Tour-\n nament", color.w, font.normal.handle);
    DrawStringToHandle(screen.right() - 190, obj.lowerFrame.box.top() - font.normal.size - 15,
        "OK!!", color.w, font.normal.handle);
    DrawGraph(screen.right() - 130, obj.lowerFrame.box.top() - ICONSIZE_NORMAL.y() - 12,
        ctrl.yes.key.image.handle, TRUE);
    DrawStringToHandle(screen.right() - 90, obj.lowerFrame.box.top() - font.normal.size - 15,
        "Reset", color.w, font.normal.handle);
    DrawGraph(screen.right() - 30, obj.lowerFrame.box.top() - ICONSIZE_NORMAL.y() - 12,
        ctrl.reset.key.image.handle, TRUE);
    return;
}
void Darts::GameStart() {
    // DrawBox(0, 25, 640, 455, color.player[CharNo], TRUE); DrawGraph(260, 40, CharImage[CharNo], TRUE);
    DrawCtrlIcon(ctrl.forward);
    // DrawStringToHandle(100, 300, ("V.S.   " + CharName[CharNo]).c_str(), color.w, font.title);
    t.Update();
    DrawFormatStringToHandle(100, 400, color.w, font.normal.handle,
        "Game start in %d s without control", StartCount / 1000 - t.GetLapseTime().s);
    if (isCtrled(ctrl.back)) updateScene(Scene::CharSelect);
    if (isCtrled(ctrl.forward) || isCtrled(ctrl.skip) || t.GetLapseCount() >= StartCount) {
        scene = Scene::Game; InitScene(); PauseMode = 0; t.Start();// Forward, Skip or 3s passed
    } return;
}
void Darts::InitScene() {
    switch (scene) {
    case Scene::Home: case Scene::GameSelect: cd.game = 0; cd.gameKind = 0;
    case Scene::CharSelect: 
        //CharLv = 0; CharNo = Player::Initial;
    case Scene::GameStart: case Scene::Game: SkillMode = true; Turn = 1; 
    case Scene::Pause: case Scene::Result: if (scene != Scene::Home) break;
    default:
        cd.BGMMode = 0; for (int i = 0; i < SoundID::NUM; i++) { cd.SoundVol[i] = SoundVol::Max; }
        ChangeSoundVol(SoundID::Total, cd.SoundVol[SoundID::Total]);
        break;
    } updateScene(scene); return;
}
void Darts::Game() {
    //DrawBox(0, 25, 640, 455, color.player[CharNo], TRUE); DrawGraph(-70, 60, CharImage[CharNo], TRUE);
    DrawBox(110, 30, 160, 135, color.Reset, TRUE); DrawBox(160, 30, 635, 450, color.Reset, TRUE);
    DrawStringToHandle(193, 140, "CT", color.w, font.normal.handle);
    for (int i = 0; i < MaxCT - 1; i++) {
        if (i < CT) { DrawBox(195, 425 - 15 * i, 209, 439 - 15 * i, color.player[7], TRUE); continue; }
        DrawBox(195, 425 - 15 * i, 209, 439 - 15 * i, color.player[7], TRUE);
    }
    t.DrawLapseTimeToHandle(540, 40, color.w, font.normal.handle);
}
int Darts::DrawScene() {
    DrawPicture(darts); DrawPicture(thunder);
    switch (scene) {
    case Scene::Home: Home(); break;
    case Scene::GameSelect: GameSelect(); break;
    case Scene::CharSelect: CharSelect(); break;
    case Scene::GameStart: GameStart(); break;
    case Scene::Game: case Scene::Pause: case Scene::Result: Game(); break;
    default: Config(); break;
    } 
    DrawStringToHandle(ctrl.chBGM.icon.box.right(), screen.bottom() - font.normal.size - 4,
        bgm[PlayingBGMNo].name.c_str(), color.w, font.normal.handle);
    DrawStringToHandle(screen.right() - 430, screen.bottom() - font.normal.size - 4,
        "Lightning Darts C 2025 Haruki Kojima", color.w, font.normal.handle);
    DrawCircle(screen.right() - 240, screen.bottom() - font.normal.size / 2 - 4,
        font.normal.size / 2 + 1, color.k, FALSE, 3);
    DrawCircle(screen.right() - 240, screen.bottom() - font.normal.size / 2 - 4,
        font.normal.size / 2 + 1, color.w, FALSE, 1);
    switch (scene) {
    case Scene::Home:      DrawStringToHandle(115, 5, "Home", color.w, font.normal.handle); break;
    case Scene::GameSelect: DrawStringToHandle(115, 5, "Game Select", color.w, font.normal.handle); break;
    case Scene::CharSelect:
        DrawStringToHandle(115, 5, (gameName[cd.gameKind][cd.game] + " >> Charcter Select").c_str(), 
            color.w, font.normal.handle);
        break;
    case Scene::GameStart:
        DrawStringToHandle(115, 5, (gameName[cd.gameKind][cd.game] + " >> Charcter Select >> Game Start").c_str(),
            color.w, font.normal.handle); break;
    case Scene::Game: case Scene::Pause: case Scene::Result:
        DrawStringToHandle(115, 5, gameName[cd.gameKind][cd.game].c_str(), color.w, font.normal.handle); break;
    default: 
        DrawStringToHandle(115, 5, "Config", color.w, font.normal.handle); break;                                                                 break;
    } return scene;
}
void Darts::updateScene(int TargetScene) {
    switch (TargetScene) {
    case Scene::Home: if (scene) PlayBGM(0); break;
    case Scene::GameSelect: break;
    case Scene::CharSelect: break;
    case Scene::GameStart: t.Start();  break;
    case Scene::Game: if (scene < 0) { t.Resume(); break; } Shuttle++;
        // if (!mainMode && Shuttle >= MaxShuttle) { updateScene(Scene::Result); return; }
        break;
    case Scene::Pause: PauseMode = 1; break;
    case Scene::Result: 
        /*if (cd.MaxCharLv[mode][CharNo] >= 9) cd.MaxCharLv[mode][CharNo] = 9;
        else if (mode == Mode::Shuttle) {
            for (int i = 0; i < 10; i++) {
                if (Phase < 2 * i + 3) { if (cd.MaxCharLv[mode][CharNo] < i) { cd.MaxCharLv[mode][CharNo] = i; } break; }
            }
        }
        else {
            for (int i = 0; i < 10; i++) {
                if (Phase < 2 * i + 8 - CharNo) { 
                    if (cd.MaxCharLv[mode][CharNo] < i) { cd.MaxCharLv[mode][CharNo] = i; } break; 
                }
            }
        }*/ if (Turn < 0) { PlayBGM(7); break; } HP = 0; PlayBGM(8); t.Stop(); break;
    default: if (scene >= 0) PlayBGM(7); break;
    } scene = TargetScene; return;
}
void Darts::BackSystem() {
    Keyboard::getInstance()->update();
    Mouse::getInstance()->update();
    switch (scene) {
    case Scene::Game:
        t.Update();
        if (isCtrled(ctrl.skill)) { SkillMode = !SkillMode; }
        // if ((mainMode == Mode::Battle && HP <= 0)) { updateScene(Scene::Result); return; }
    case Scene::Pause:
        if (isCtrled(ctrl.pause[PauseMode])) {
            if (!PauseMode) { updateScene(Scene::Pause); t.Stop(); return; } PauseMode = 0; t.Resume(); scene = Scene::Game; return;
        }
    case Scene::Result:
        if (isCtrled(ctrl.back)) { updateScene(Scene::GameStart); return; }
    case ConfigID::ModeSelect: case ConfigID::CharSelect: case ConfigID::GameStart: case ConfigID::Game:
    case ConfigID::Pause: case ConfigID::Result: case Scene::CharSelect: case Scene::GameStart:
        if (isCtrled(ctrl.reGame)) { updateScene(Scene::GameSelect); return; }
    default:
        if (isCtrled(ctrl.quit)) scene = Scene::Quit;
        else if (isCtrled(ctrl.home)) updateScene(Scene::Home);
        else if (isCtrled(ctrl.mute[MuteMode])) ChangeSound((MuteMode + 1) % 2);
        else if (isCtrled(ctrl.config)) {
            if (scene == Scene::Game) { PauseMode = 1; scene = Scene::Pause; t.Stop(); }
            if (scene >= 0) { updateScene(scene - Scene::Num); }
            else { updateScene(scene + Scene::Num); }
        }
        else if (isCtrled(ctrl.chWindow[WindowMode])) ChangeWindow((WindowMode + 1) % 2);
        else if (isCtrled(ctrl.init)) InitScene();
        else if (isCtrled(ctrl.chBGM)) PlayBGM((PlayingBGMNo + 1) % MAX_BGM_NUM);
        else if (MuteMode && !CheckSoundMem(bgm[PlayingBGMNo].handle)) {
            switch (cd.BGMMode) {
            case BGMModeID::Ascending: PlayBGM((PlayingBGMNo + 1) % MAX_BGM_NUM); return;
            case BGMModeID::Descending: PlayBGM((PlayingBGMNo + MAX_BGM_NUM - 1) % MAX_BGM_NUM); return;
            case BGMModeID::Random: PlayBGM(GetRand(MAX_BGM_NUM - 1));
            default: return;
            }
        }
    } return;
}
Darts::~Darts() {
    FILE* ocdf = fopen(DataFileName, "wb+"); fwrite(&cd, sizeof(cd), 1, ocdf); fclose(ocdf);
    PlaySound(NULL, 0, 0); InitSoundMem(); DxLib_End();
}

