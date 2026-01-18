#pragma once
#include <string>
#include <vector>
#include "Singleton.hpp"
#include "DxLib.h"

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
        static constexpr int LOOP = 0;
        static constexpr int ASCEND = 1;
        static constexpr int DESCEND = 2;
        static constexpr int RANDAM = 3;
        static constexpr int NUM = 4;
    };
    // SE
    static constexpr int SE_NUM = 4;
    static constexpr int COMBO_SE_NUM = 7;
	// Functions
    std::string name(int SoundNo);
    void load();
    void initSoundVol();
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
	int playSE(int SENo) { return PlaySoundMem(mSE[SENo], DX_PLAYTYPE_BACK); }
	int playComboSE(int ComboSENo) { return PlaySoundMem(mComboSE[ComboSENo], DX_PLAYTYPE_BACK); }
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
    const std::string BGMFolderPath = "./BGM";
    struct BGM {
        std::string path = {};
        std::string name = {};
    };
    std::vector<BGM> mBGMs, mRandomBGMs;
    int mPlayingBGMNo = 0;
    int mPlayingBGMHandle = 0;
	std::string mPlayingBGMName = {};
    int mSE[SE_NUM] = {};
    int mComboSE[COMBO_SE_NUM] = {};
};