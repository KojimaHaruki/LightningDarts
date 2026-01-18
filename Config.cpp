#include "Config.hpp"
#include "Mouse.hpp"
#include "Sound.hpp"
#include "Timer.hpp"

Config::Config(int priorScene, ShareData shareData) {
    sd = shareData;
    mNowScene = priorScene;
    set();
}

void Config::draw(sCtrl ctrl) {
    drawImage(ctrl.icon);
    DrawStringToHandle(ctrl.icon.box.right() + 5, ctrl.icon.box.center().y() - MfontSize / 2,
        ctrl.name.c_str(), white, Mfont);
    drawImage(ctrl.key.image);
    return;
}

void Config::reset() {
    cScene::reset();
    initCtrlKey();
    initScreenSize();
    cSound::instance()->initSoundVol(); return;
}

void Config::set() {
    ctrl = sd.ctrl;
    ctrl.bgm.icon.box.setUpperLeft(10, 210);
    ctrl.bgm.key.image.box.setUpperLeft(200, 210);
    int iconX[2] = { screen.center().x(), screen.center().x() + 20 + screen.width() / 4 };
    int keyX[2] = { iconX[0] + 190, iconX[1] + 150 };
    ctrl.home.icon.box.setUpperLeft(iconX[0], upperFrame.bottom() + XLfontSize + 30);
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
        ctrl.pause[i].key.image.box.setUpperLeft(keyX[1], ctrl.skill.icon.box.top());
        ctrl.window[i].icon.box.setUpperLeft(iconX[0], ctrl.init.icon.box.bottom() + 5);
        ctrl.window[i].key.image.box.setUpperLeft(keyX[0], ctrl.window[i].icon.box.top());
    }
    ctrl.gameSelect.icon.box.setUpperLeft(iconX[1], ctrl.window[0].icon.box.top());
    ctrl.gameSelect.key.image.box.setUpperLeft(keyX[1], ctrl.gameSelect.icon.box.top());
    ctrl.playerSelect.icon.box.setUpperLeft(iconX[0], ctrl.gameSelect.icon.box.bottom() + 5);
    ctrl.playerSelect.key.image.box.setUpperLeft(keyX[0], ctrl.playerSelect.icon.box.top());
    ctrl.reset.icon.box.setUpperLeft(iconX[1], ctrl.playerSelect.icon.box.top());
    ctrl.reset.key.image.box.setUpperLeft(keyX[1], ctrl.reset.icon.box.top());
    ctrl.back.icon.box.setUpperLeft(iconX[0], ctrl.reset.icon.box.bottom() + 5);
    ctrl.back.key.image.box.setUpperLeft(keyX[0], ctrl.back.icon.box.top());
    ctrl.forward.icon.box.setUpperLeft(iconX[1], ctrl.back.icon.box.top());
    ctrl.forward.key.image.box.setUpperLeft(keyX[1], ctrl.back.icon.box.top());
    return;
}

void Config::draw() {
    cScene::draw();

    // draw icon
    if (cTimer::instance()->isPaused()) drawImage(sd.ctrl.pause[TRUE].icon);
    else drawImage(sd.ctrl.pause[FALSE].icon);
    drawImage(sd.ctrl.skill.icon);
    if (mNowScene >= PLAYER_SELECT) drawImage(sd.ctrl.playerSelect.icon);
    if (mNowScene >= GAME_SELECT) drawImage(sd.ctrl.gameSelect.icon);

    // control setting
    draw(ctrl.home);
    draw(ctrl.mute[cSound::instance()->isBGMPlayed()]);
    draw(ctrl.playerSelect);
    draw(ctrl.gameSelect);
    draw(ctrl.skill);
    if (cTimer::instance()->isPaused()) draw(ctrl.pause[TRUE]); else draw(ctrl.pause[FALSE]);
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
    DrawStringToHandle(5, upperFrame.bottom() + 10, "Sound", white, XLfont);
    DrawStringToHandle(screen.center().x(), upperFrame.bottom() + 10, "System", white, XLfont);
    DrawStringToHandle(10, 180, ("Play mode: " + cSound::instance()->bgmPlayModeName()).c_str(),
        white, Mfont);
    DrawGraph(355, 175, sd.ctrl.down.icon.handle, TRUE);
    DrawGraph(380, 175, sd.ctrl.up.icon.handle, TRUE);
    for (int i = 0; i < cSound::sKind::NUM; i++) {
        DrawStringToHandle(10, 90 + 30 * i,
            cSound::instance()->name(i).c_str(), white, Mfont);
        int vol = cSound::instance()->vol(i);
        DrawBox(100, 95 + 30 * i, 100 + vol, 105 + 30 * i, white, TRUE);
        DrawBox(100 + vol, 95 + 30 * i, 300, 105 + 30 * i, black, TRUE);
        DrawFormatStringToHandle(315, 90 + 30 * i, white, Mfont, "%3d", vol);
        DrawGraph(355, 85 + 30 * i, sd.ctrl.down.icon.handle, TRUE);
        DrawGraph(380, 85 + 30 * i, sd.ctrl.up.icon.handle, TRUE);
        switch (cMouse::instance()->clickBoxState(95, 90 + 30 * i, 305, 110 + 30 * i)) {
        case Key::RELEASED:
            DrawBox(95 + vol, 90 + 30 * i, 105 + vol, 110 + 30 * i, touchColor, TRUE);
            break;
        case Key::RELEASEDtoPRESSED: case Key::PRESSED:
            DrawBox(95 + vol, 90 + 30 * i, 105 + vol, 110 + 30 * i, pressColor, TRUE);
            cSound::instance()->setVol(i, cMouse::instance()->x() - 100);
            break;
        default:
            DrawBox(95 + vol, 90 + 30 * i, 105 + vol, 110 + 30 * i, gray, TRUE);
            break;
        }
        if (cMouse::instance()->clickBoxCount(355, 85 + 30 * i, 380, 110 + 30 * i) > 10) {
            cSound::instance()->setVol(i, vol - 1);
        }
        else if (cMouse::instance()->clickBoxCount(380, 85 + 30 * i, 405, 110 + 30 * i) > 10) {
            cSound::instance()->setVol(i, vol + 1);
        }
        else if (cMouse::instance()->clickBoxCount(355, 85 + 30 * i, 380, 110 + 30 * i) == -1) {
            cSound::instance()->setVol(i, vol - 4);
        }
        else if (cMouse::instance()->clickBoxCount(380, 85 + 30 * i, 405, 110 + 30 * i) == -1) {
            cSound::instance()->setVol(i, vol + 4);
        }
    }
    DrawStringToHandle(sd.ctrl.mute[cSound::instance()->isBGMPlayed()].icon.box.upperRight().x(),
        upperFrame.center().y() - MfontSize / 2, "Config", white, Mfont);
}

void Config::update() {
    cScene::update();
    if (isBoxClicked(355, 175, 380, 200)) {
        cSound::instance()->playSE(1);
        cSound::instance()->setBgmPlayMode(
            (cSound::instance()->bgmPlayMode() - 1 + cSound::sPlayMode::NUM) % cSound::sPlayMode::NUM);
    }
    else if (isBoxClicked(380, 175, 405, 200)) {
        cSound::instance()->playSE(1);
        cSound::instance()->setBgmPlayMode(
            (cSound::instance()->bgmPlayMode() + 1) % cSound::sPlayMode::NUM);
    }
    else if (ctrlRQ(sd.ctrl.home)) mNextScene = HOME;
    else if (ctrlRQ(sd.ctrl.back)) mNextScene = mNowScene;
    else if (ctrlRQ(sd.ctrl.window[sd.window])) set();
    else if (ctrlRQ(sd.ctrl.reset)) reset();
    else if (mNowScene >= PLAYER_SELECT && ctrlRQ(sd.ctrl.playerSelect)) mNextScene = PLAYER_SELECT;
    else if (mNowScene >= GAME_SELECT && ctrlRQ(sd.ctrl.gameSelect)) mNextScene = GAME_SELECT;
    else if (mNowScene >= HOME && ctrlRQ(sd.ctrl.home)) mNextScene = HOME;
}