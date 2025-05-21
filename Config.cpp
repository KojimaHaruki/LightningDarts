#include "Config.hpp"
#include "Mouse.hpp"
Config::Config(int priorScene, ShareData shareData) {
    sd = shareData;
    mNowScene = priorScene;
    isConfig = true; 
    set();
}
void Config::draw(Ctrl ctrl) {
    drawImage(ctrl.icon);
    DrawStringToHandle(ctrl.icon.box.right() + 5, ctrl.icon.box.center().y() - sd.font.normal.size / 2, 
        ctrl.name.c_str(), sd.color.w, sd.font.normal.handle);
    drawImage(ctrl.key.image);
    return;
}
void Config::reset() {
    Scene::reset(); initCtrlKey(); initScreenSize(); initSoundVol(); return;
}
void Config::set() {
    ctrl = sd.ctrl;
    ctrl.bgm.icon.box.setUpperLeft(10, 210);
    ctrl.bgm.key.image.box.setUpperLeft(200, 210);
    int iconX[2] = { sd.screen.center().x() + 5, sd.screen.center().x() + 5 + sd.screen.width() / 4 };
    int keyX[2] = {};
    for (int i = 0; i < 2; i++) keyX[i] = iconX[i] + 150;
    ctrl.home.icon.box.setUpperLeft(iconX[0], sd.obj.upperFrame.box.bottom() + sd.font.title.size + 5);
    ctrl.home.key.image.box.setUpperLeft(keyX[0], ctrl.home.icon.box.top());
    ctrl.skill.icon.box.setUpperLeft(iconX[0], ctrl.home.icon.box.bottom() + 5);
    ctrl.skill.key.image.box.setUpperLeft(keyX[0], ctrl.skill.icon.box.top());
    ctrl.config.icon.box.setUpperLeft(iconX[0], ctrl.skill.icon.box.bottom() + 5);
    ctrl.config.key.image.box.setUpperLeft(keyX[0], ctrl.config.icon.box.top());
    ctrl.quit.icon.box.setUpperLeft(iconX[1], ctrl.config.icon.box.top());
    ctrl.quit.key.image.box.setUpperLeft(keyX[1], ctrl.config.icon.box.top());
    ctrl.init.icon.box.setUpperLeft(iconX[0], ctrl.config.icon.box.bottom() + 5);
    ctrl.init.key.image.box.setUpperLeft(keyX[0], ctrl.init.icon.box.top());
    ctrl.skip.icon.box.setUpperLeft(iconX[1], ctrl.init.icon.box.top());
    ctrl.skip.key.image.box.setUpperLeft(keyX[1], ctrl.init.icon.box.top());
    for (int i = 0; i < 2; i++) { 
        ctrl.mute[i].icon.box.setUpperLeft(iconX[1], ctrl.home.icon.box.top()); 
        ctrl.mute[i].key.image.box.setUpperLeft(keyX[1], ctrl.home.icon.box.top());
        ctrl.pause[i].icon.box.setUpperLeft(iconX[1], ctrl.skill.icon.box.top());
        ctrl.pause[i].key.image.box.setUpperLeft(keyX[1] - 30, ctrl.skill.icon.box.top());
        ctrl.window[i].icon.box.setUpperLeft(iconX[0], ctrl.init.icon.box.bottom() + 5);
        ctrl.window[i].key.image.box.setUpperLeft(keyX[1], ctrl.window[i].icon.box.top());
    }
    ctrl.gameSelect.icon.box.setUpperLeft(iconX[0], ctrl.window[0].icon.box.bottom() + 5);
    ctrl.gameSelect.key.image.box.setUpperLeft(keyX[1], ctrl.gameSelect.icon.box.top());
    ctrl.playerSelect.icon.box.setUpperLeft(iconX[0], ctrl.gameSelect.icon.box.bottom() + 5);
    ctrl.playerSelect.key.image.box.setUpperLeft(keyX[1], ctrl.playerSelect.icon.box.top());
    ctrl.reset.icon.box.setUpperLeft(iconX[0], ctrl.playerSelect.icon.box.bottom() + 5);
    ctrl.reset.key.image.box.setUpperLeft(keyX[1], ctrl.reset.icon.box.top());
    ctrl.back.icon.box.setUpperLeft(iconX[0], ctrl.reset.icon.box.bottom() + 5);
    ctrl.back.key.image.box.setUpperLeft(keyX[0], ctrl.back.icon.box.top());
    ctrl.forward.icon.box.setUpperLeft(iconX[1], ctrl.back.icon.box.top());
    ctrl.forward.key.image.box.setUpperLeft(keyX[1], ctrl.back.icon.box.top());
    return;
}
void Config::draw() {
    Scene::draw();
    // control setting
    draw(ctrl.home); 
    draw(ctrl.mute[sd.sound]); 
    draw(ctrl.playerSelect); 
    draw(ctrl.gameSelect);
    draw(ctrl.skill); 
    draw(ctrl.pause[sd.gameTime.isPaused()]); 
    draw(ctrl.config); 
    draw(ctrl.window[sd.window]); 
    draw(ctrl.quit);
    draw(ctrl.init); 
    draw(ctrl.reset); 
    draw(ctrl.bgm); 
    draw(ctrl.skip); 
    draw(ctrl.back); 
    draw(ctrl.forward);
    // music setting 
    DrawStringToHandle(5, 25, "Sound", sd.color.w, sd.font.title.handle);
    DrawStringToHandle(sd.screen.center().x(), 25, "System", sd.color.w, sd.font.title.handle);
    DrawStringToHandle(10, 180, ("Play mode: " + BGMModeName[sd.bgmMode]).c_str() , sd.color.w, sd.font.normal.handle);
    DrawGraph(355, 175, sd.ctrl.down.icon.handle, TRUE);
    DrawGraph(380, 175, sd.ctrl.up.icon.handle, TRUE);
    for (int i = 0; i < Sound::NUM; i++) {
        DrawStringToHandle(10, 90 + 30 * i, SoundName[i].c_str(), sd.color.w, sd.font.normal.handle);
        DrawBox(100, 95 + 30 * i, 100 + sd.soundVol[i], 105 + 30 * i, sd.color.w, TRUE);
        DrawBox(100 + sd.soundVol[i], 95 + 30 * i, 300, 105 + 30 * i, sd.color.k, TRUE);
        DrawFormatStringToHandle(315, 90 + 30 * i, sd.color.w, sd.font.normal.handle, "%3d", sd.soundVol[i]);
        DrawGraph(355, 85 + 30 * i, sd.ctrl.down.icon.handle, TRUE);
        DrawGraph(380, 85 + 30 * i, sd.ctrl.up.icon.handle, TRUE);
        switch (Mouse::getInstance()->getClickBoxState(95, 90 + 30 * i, 305, 110 + 30 * i)) {
        case Key::RELEASED:
            DrawBox(95 + sd.soundVol[i], 90 + 30 * i, 105 + sd.soundVol[i], 110 + 30 * i, sd.color.touch, TRUE);
            break;
        case Key::RELEASEDtoPRESSED: case Key::PRESSED:
            DrawBox(95 + sd.soundVol[i], 90 + 30 * i, 105 + sd.soundVol[i], 110 + 30 * i, sd.color.press, TRUE);
            changeSoundVol(i, Mouse::getInstance()->x() - 100);
            break;
        default: 
            DrawBox(95 + sd.soundVol[i], 90 + 30 * i, 105 + sd.soundVol[i], 110 + 30 * i, sd.color.gy, TRUE);
            break;
        }
        if (Mouse::getInstance()->getClickBoxCount(355, 85 + 30 * i, 380, 110 + 30 * i) > 10) {
            changeSoundVol(i, sd.soundVol[i] - 1);
        }
        else if (Mouse::getInstance()->getClickBoxCount(380, 85 + 30 * i, 405, 110 + 30 * i) > 10) {
            changeSoundVol(i, sd.soundVol[i] + 1);
        }
        else if (Mouse::getInstance()->getClickBoxCount(355, 85 + 30 * i, 380, 110 + 30 * i) == -1) {
            changeSoundVol(i, sd.soundVol[i] - 4);
        }
        else if (Mouse::getInstance()->getClickBoxCount(380, 85 + 30 * i, 405, 110 + 30 * i) == -1) {
            changeSoundVol(i, sd.soundVol[i] + 4);
        }
    }
    DrawStringToHandle(sd.ctrl.mute[sd.sound].icon.box.upperRight().x(),
        sd.obj.upperFrame.box.center().y() - sd.font.normal.size / 2, "Config", sd.color.w, sd.font.normal.handle);
    return;
}
void Config::update() {
    Scene::update();
    if (isBoxClicked(355, 175, 380, 200)) {
        PlaySoundMem(sd.se[1], DX_PLAYTYPE_BACK); sd.bgmMode = (sd.bgmMode - 1 + BGMMode::NUM) % BGMMode::NUM;
    }
    else if (isBoxClicked(380, 175, 405, 200)) {
        PlaySoundMem(sd.se[1], DX_PLAYTYPE_BACK); sd.bgmMode = (sd.bgmMode + 1) % BGMMode::NUM;
    }
    else if (ctrlRQ(sd.ctrl.home)) mNextScene = HOME;
    else if (ctrlRQ(sd.ctrl.back)) mNextScene = mNowScene;
    else if (ctrlRQ(sd.ctrl.window[sd.window])) set();
    else if (ctrlRQ(sd.ctrl.reset)) reset();
    return;
}
Config::~Config() {

}