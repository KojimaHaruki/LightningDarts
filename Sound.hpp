#pragma once
#include <string>
#include <vector>
#include "Singleton.hpp"
#include "DxLib.h"
#include "Darts.hpp"

class cSound : public Singleton<cSound> {
    cSound() {}
	friend class Singleton<cSound>;
public:
    // Sound
    struct sKind {
        static constexpr int TOTAL = 0;
        static constexpr int BGM = 1;
        static constexpr int SE = 2;
        static constexpr int NUM = 3;
	};
    static constexpr int MIN_VOL = 0;
    static constexpr int MAX_VOL = 200;
    // BGM
    static constexpr int MAX_BGM_NUM = 100;
    static constexpr int MAX_ARTIST_NUM = 50;
    struct sPlayMode {
        static constexpr int LOOP = 0, ASCEND = 1, DESCEND = 2, RANDAM = 3, NUM = 4;
    };
    // SE
    struct sCartrigeKind {
        static constexpr int bigSR = 0, SR = 1, SG = 2, single = 3, multiple = 4, NUM = 5;
    };

    struct sComboKind {
        static constexpr int A = 0, B = 1, C = 2, D = 3, E = 4, F = 5, G = 6, NUM = 7;
    };
    
	// Functions
    std::string name(int SoundNo);
    void load();
    void initSoundVol();
	void init();
    void mute();
	void unmute();
	int isBGMPlayed() { return mIsBGMPlayed; }
	std::string playingBGMName() { return mPlayingBGMName; }
	int bgmPlayMode() { return mBGMPlayMode; }
	void setBgmPlayMode(int PlayMode) { mBGMPlayMode = PlayMode; }
    std::string bgmPlayModeName() { return mBGMPlayModeName[mBGMPlayMode]; }
	int vol(int SoundNo) { return mVol[SoundNo]; }
    bool setVol(int SoundNo, int Vol);
    bool playBGM(int BGMNo);
	bool playNextBGM();
    bool playLastBGM();
	void update();
    int playShotSE(int shot);
    int playCartrigeSE(int cartrige);
    int playSelectSE() { return PlaySoundMem(mSelectSE, DX_PLAYTYPE_BACK); }
private:
    const std::string mName[sKind::NUM] = { "Total", "BGM", "SE" };
    int mIsBGMPlayed = TRUE;
    int mBGMVol = 0;
    int mSEVol = 0;
    int mVol[sKind::NUM] = {}; // SoundVol[SoundNo] SoundNo 0: Total, 1: BGM, 2: SE
    int mBGMPlayMode = sPlayMode::RANDAM; // BGM 0: Loop, 1: Ascending order, 2: Decending order, 3: Random
    const std::string mBGMPlayModeName[sPlayMode::NUM] = {
        "Loop", "Ascending order", "Descending order", "Random"
    };
    const std::string BGMFolderPath = "C:/Users/miniy/Music/BGM";
    struct BGM {
        std::string path = {};
        std::string name = {};
    };
    std::vector<BGM> mBGMs, mRandomBGMs;
    int mPlayingBGMNo = 0;
    int mPlayingBGMHandle = 0;
	std::string mPlayingBGMName = {};
    int mShotSE[cDarts::sRadialPos::NUM] = {}, mCartrigeSE[sCartrigeKind::NUM] = {}, mSelectSE;
    //int mComboSE[sComboKind::NUM] = {};
};