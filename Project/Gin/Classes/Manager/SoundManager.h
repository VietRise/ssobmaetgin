#ifndef __SOUND_MANAGER_H__
#define __SOUND_MANAGER_H__

#define USE_AUDIO_ENGINE 0

#include "cocos2d.h"
#include "Define/GameDefine.h"

#if USE_AUDIO_ENGINE
#else

#include "SimpleAudioEngine.h"

#endif

enum class SoundEvent
{
    CLICK_NORMAL_BUTTON,
    DEAL_CARD,
    TAKE_OFF,
    DOUBLE_MONEY,
    REWARD_MONEY,

    TL_THROW_CARD,
    TL_DRAW_CARD,
    TL_HITPOT_1,
    TL_HITPOT_2,
    TL_SHOW_FINAL_RESULT,
    TL_TAKE_CARD,
    TL_WIN,
    TL_WIN_GAME,
    TL_WIN_JACKPOT,
    TL_WIN_MULTIPLY,
    VIDEO_REWARD_LOTTERY,
    TL_GIVE_CARD,
    TL_DROP_CARD,
    TL_FIGHT,
    TL_CHALLENGE,
    TL_FOLD,
    TL_BURNED,
    TL_SCORE,
	TL_READY,
	TL_BONUS_WIN,
	TL_GIN,
	TL_TURN,
	TL_CLICK_CARD,
	TL_COMPLETE,
	TL_SECRET_MELD,
    DAILY_BONUS,
    CLAIM_MONEY,
    UNLOCK_TABLE,
    TL_UNDERCUT_25,
    TL_GIN_25,
    TL_BIGGIN_50,
    TL_MULTIPLY,
};

class SoundManager : public cocos2d::Node
{
private:
#if USE_AUDIO_ENGINE
#else
    CocosDenshion::SimpleAudioEngine* _simpleAudioEngine;
#endif
    bool                              _loopBG;
    std::vector<std::string>          _listBackgroundMusics;
    float                             _deltaTime;
    int                               _currentIndexBGMusic;
    std::map<SoundEvent, std::string> _maps;

    void initMaps();

public:
    ~SoundManager();
    SoundManager();

    virtual bool init() override;
    CREATE_FUNC(SoundManager);

    static SoundManager* getInstance();
    static void destroyInstance();

    bool getLoopBG();
    void setLoopBG(bool loopBG);

    void playBackgroundMusicAuto();
    void stopBackgroundMusicAuto();
    virtual void update(float dt) override;

    void preloadBackgroundMusic(const char* filePath);
    void playBackgroundMusic(const char* filePath, bool loop);
    void stopBackgroundMusic();
    void pauseBackgroundMusic();
    void resumeBackgroundMusic();
    void rewindBackgroundMusic();
    bool isPlayingBackgroundMusic();
    float getBackgroundMusicVolume();
    void setBackgroundMusicVolume(float volume);

    void preloadEffect(const char* filePath);
    void unloadEffect(const char* filePath);
    int playEffect(const char* filePath, bool loop = false, float gain = 1.0f);
    void pauseEffect(unsigned int soundId);
    void pauseAllEffects();
    void resumeEffect(int soundId);
    void resumeAllEffects();
    void stopEffect(unsigned int soundId);
    void stopAllEffects();
    float getEffectsVolume();
    void setEffectsVolume(float volume);
    const std::map<SoundEvent, std::string>& getMaps();

    static int playSoundEffect(SoundEvent soundEvent, bool loop = false, float gain = 1.0f);
    static void stopSoundEffect(int soundId);
    static int playSoundEffectRandom(const std::vector<SoundEvent>& listRandom);

    bool isMusicActive();
};

#endif // ! __SOUND_MANAGER_H__
