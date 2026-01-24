#include "Sound.hpp"
#include <filesystem>
#include <random>
#include <algorithm>
#include <iostream>
#include "resource.h"
namespace fs = std::filesystem;

std::string cSound::name(int SoundNo) {
    if (SoundNo < 0 || SoundNo >= sKind::NUM) return ""; 
    return mName[SoundNo];
}

void cSound::load() {
    InitSoundMem();
    std::error_code err;
    std::vector<std::string> artistName;
    for (fs::directory_iterator iter(BGMFolderPath + "/"), end;
        iter != end && !err && artistName.size() < MAX_ARTIST_NUM; iter.increment(err)) {
        const fs::directory_entry entry = *iter;
        if (!entry.path().has_extension()) { // if found path is folder,
            artistName.push_back(entry.path().filename().string()); // get group name
        }
    }
    artistName.shrink_to_fit();
    mBGMs.clear();
    for (int artist = 0; artist < artistName.size(); artist++) {
        for (fs::directory_iterator iter(BGMFolderPath + "/" + artistName[artist] + "/"), end;
            iter != end && !err && mBGMs.size() < MAX_BGM_NUM; iter.increment(err)) {
            const fs::directory_entry entry = *iter;
            std::string extension = entry.path().extension().string();
            if (extension == ".wav" || extension == ".mp3" || extension == ".ogg") {
                std::string name = artistName[artist] + " - " + entry.path().filename().string();
                name.erase(name.length() - extension.length(), extension.length());
                mBGMs.push_back(BGM(entry.path().string(), name));
            }
        }
        mBGMs.shrink_to_fit();
    }
    mRandomBGMs = mBGMs;
    std::mt19937_64 get_random_mt(std::random_device{}());
    std::shuffle(mRandomBGMs.begin(), mRandomBGMs.end(), get_random_mt);
    if (err) {
        std::cout << err.value() << std::endl;
        std::cout << err.message() << std::endl;
    }
    // load sound effect
    for (int i = 0; i < SE_NUM; i++) {
        mSE[i] = LoadSoundMemByResource(MAKEINTRESOURCE(IDR_WAVE1 + i), "MP3");
    }
    for (int i = 0; i < COMBO_SE_NUM; i++) {
        mComboSE[i] = LoadSoundMemByResource(MAKEINTRESOURCE(IDR_WAVE5 + i), "MP3");
    }
}

void cSound::initSoundVol() {
    for (int i = 0; i < sKind::NUM; i++) mVol[i] = MAX_VOL;
    setVol(sKind::TOTAL, MAX_VOL);
}

void cSound::init() {
    load();
    StopSound();
    initSoundVol();
    playBGM(0);
}

void cSound::mute() {
    StopSoundMem(mPlayingBGMHandle);
    mIsBGMPlayed = FALSE; // check mute mode
}

void cSound::unmute() {
    ChangeVolumeSoundMem(mBGMVol, mPlayingBGMHandle);
    // play BGM
    if (mBGMPlayMode == sPlayMode::LOOP)
        PlaySoundMem(mPlayingBGMHandle, DX_PLAYTYPE_BACK | DX_PLAYTYPE_LOOP, 0);
    else
        PlaySoundMem(mPlayingBGMHandle, DX_PLAYTYPE_BACK, 0);
    mIsBGMPlayed = TRUE; // check mute mode
}

bool cSound::setVol(int SoundNo, int Vol) {
    if (SoundNo < 0 || SoundNo >= sKind::NUM) return false; // if sound doesn't exist, exit
    if (Vol > MAX_VOL) { // if sound's volume is above maximum volume,
        mVol[SoundNo] = MAX_VOL; // set maximum volume
    }
    else if (Vol < MIN_VOL) { // if sound's volume is below minimum volume,
        mVol[SoundNo] = MIN_VOL; // set minimum volume
    }
    else mVol[SoundNo] = Vol; // othewise set sound's volume
    if (SoundNo != sKind::SE) { // if sound is BGMs or total sounds,
        mBGMVol = (int)(0.0064 * mVol[sKind::TOTAL] * mVol[sKind::BGM]); // set BGMs' volume
        ChangeVolumeSoundMem(mBGMVol, mPlayingBGMHandle); // change volume of BGM which is now playing
    }
    if (SoundNo != sKind::BGM) { // if sound is SEs or total sounds,
        mSEVol = (int)(0.0064 * mVol[sKind::TOTAL] * mVol[sKind::SE]); // set SEs' volume
        // change SEs'volume
        for (int i = 0; i < SE_NUM; i++) { ChangeVolumeSoundMem(mSEVol, mSE[i]); }
        for (int i = 0; i < COMBO_SE_NUM; i++) { ChangeVolumeSoundMem(mSEVol, mComboSE[i]); }
    } return true;
}

bool cSound::playBGM(int BGMNo) {
    if (BGMNo < 0 || BGMNo >= mBGMs.size()) // if BGM doesn't exist,
        return false; // exit
    int nextBGMHandle = -1;
    if (mBGMPlayMode == sPlayMode::RANDAM) {
        nextBGMHandle = LoadSoundMem(mRandomBGMs[BGMNo].path.c_str());
        mPlayingBGMName = mRandomBGMs[BGMNo].name;
    }
    else {
        nextBGMHandle = LoadSoundMem(mBGMs[BGMNo].path.c_str());
        mPlayingBGMName = mBGMs[BGMNo].name;
    }
    if (mPlayingBGMHandle != -1) StopSoundMem(mPlayingBGMHandle);  // stop BGM which is now playing
	DeleteSoundMem(mPlayingBGMHandle); // delete BGM which is now playing
    ChangeVolumeSoundMem(mBGMVol, nextBGMHandle); // set new BGM's volume
    mPlayingBGMHandle = nextBGMHandle; // set new BGM's handle
    if (mIsBGMPlayed) unmute(); // unmute sounds
    mPlayingBGMNo = BGMNo; // set BGM
    return true;
}

bool cSound::playNextBGM() {
    int nextBGMNo = mPlayingBGMNo + 1;
    if (mBGMPlayMode == sPlayMode::RANDAM && nextBGMNo >= mBGMs.size()) {
        nextBGMNo = 0;
        std::mt19937_64 get_random_mt(std::random_device{}());
        std::shuffle(mRandomBGMs.begin(), mRandomBGMs.end(), get_random_mt);
    }
    return playBGM(nextBGMNo);
}

bool cSound::playLastBGM() {
    int lastBGMNo = (mPlayingBGMNo - 1 + mBGMs.size()) % mBGMs.size();
    return playBGM(lastBGMNo);
}

void cSound::update() {
    if (mIsBGMPlayed && !CheckSoundMem(mPlayingBGMHandle)) {
        int nextBGMNo = mPlayingBGMNo;
        switch (mBGMPlayMode) {
        case sPlayMode::RANDAM: case sPlayMode::ASCEND: 
            playNextBGM(); 
            break;
        case sPlayMode::DESCEND: 
            playLastBGM(); 
            break;
        default:
            playBGM(nextBGMNo);
            break;
        }
    }
}