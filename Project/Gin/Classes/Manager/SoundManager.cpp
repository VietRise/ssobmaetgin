#include "SoundManager.h"
#include "UserData/UserData.h"
#include "Native/CallToOtherLanguage.h"
#include "Helper/GameUtils.h"

#if USE_AUDIO_ENGINE
#include "AudioEngine.h"
#endif

USING_NS_CC;
#if USE_AUDIO_ENGINE
#else
using namespace CocosDenshion;
#endif
using namespace std;

#define MUSIC_CHANGE_INTERVAL 10

#if USE_AUDIO_ENGINE
int backgroundId = -1;
#endif

static SoundManager* _instance = nullptr;

SoundManager::~SoundManager()
{
#if USE_AUDIO_ENGINE
    /*experimental::AudioEngine::stopAll();*/
#else
    _simpleAudioEngine->end();
#endif
}

SoundManager::SoundManager()
{
#if USE_AUDIO_ENGINE
#else
    _simpleAudioEngine = SimpleAudioEngine::getInstance();
#endif
    _loopBG              = false;
    _currentIndexBGMusic = -1;

    _listBackgroundMusics.push_back("sound/background/bensound-jazzcomedy.mp3");
    _listBackgroundMusics.push_back("sound/background/bensound-retrosoul.mp3");
    _listBackgroundMusics.push_back("sound/background/bensound-theelevatorbossanova.mp3");

    _deltaTime = 0;
}

SoundManager* SoundManager::getInstance()
{
    if (!_instance)
    {
        _instance = SoundManager::create();
        _instance->retain();
    }
    return _instance;
}

void SoundManager::destroyInstance()
{
    if (_instance)
        CC_SAFE_RELEASE_NULL(_instance);
}

bool SoundManager::init()
{
    if (!Node::init())
        return false;
    initMaps();
    return true;
}

void SoundManager::initMaps()
{
    this->_maps[SoundEvent::CLICK_NORMAL_BUTTON] = "sound/effect/button_click2.mp3";
	this->_maps[SoundEvent::DEAL_CARD] = "sound/effect/gin/boss_deal_card.mp3";
	this->_maps[SoundEvent::TAKE_OFF] = "sound/effect/boss_take_off.mp3";
	this->_maps[SoundEvent::DOUBLE_MONEY] = "sound/effect/boss_higher_point_1_comp.mp3";
	this->_maps[SoundEvent::REWARD_MONEY] = "sound/effect/gin/boss_score.mp3";

	this->_maps[SoundEvent::TL_THROW_CARD] = "sound/effect/gin/boss_draw_card.mp3";
	this->_maps[SoundEvent::TL_DRAW_CARD] = "sound/effect/gin/boss_draw_card.mp3";

	this->_maps[SoundEvent::TL_HITPOT_1] = "sound/effect/gin/boss_hitpot_1.mp3";
	this->_maps[SoundEvent::TL_HITPOT_2] = "sound/effect/gin/boss_hitpot_2.mp3";
	this->_maps[SoundEvent::TL_SHOW_FINAL_RESULT] = "sound/effect/boss_score_box.mp3";

    this->_maps[SoundEvent::TL_TAKE_CARD] = "sound/effect/gin/boss_draw_card.mp3";
	this->_maps[SoundEvent::TL_WIN] = "sound/effect/win_comp.mp3";
    this->_maps[SoundEvent::TL_WIN_JACKPOT] = "sound/effect/gin/boss_jackpot.mp3";
    this->_maps[SoundEvent::TL_WIN_GAME] = "sound/effect/boss_win_comp.mp3";
    this->_maps[SoundEvent::TL_WIN_MULTIPLY] = "sound/effect/gin/boss_score.mp3";

	this->_maps[SoundEvent::VIDEO_REWARD_LOTTERY] = "sound/effect/win_comp.mp3";

	this->_maps[SoundEvent::TL_GIVE_CARD] = "sound/effect/gin/boss_give_card.mp3";
	this->_maps[SoundEvent::TL_DROP_CARD] = "sound/effect/gin/boss_drop_card.mp3";

	this->_maps[SoundEvent::TL_FIGHT] = "sound/effect/gin/boss_fight.mp3";
	this->_maps[SoundEvent::TL_CHALLENGE] = "sound/effect/gin/boss_fight.mp3";
	this->_maps[SoundEvent::TL_FOLD] = "sound/effect/gin/boss_fold.mp3";
	this->_maps[SoundEvent::TL_BURNED] = "sound/effect/gin/boss_fold.mp3";
	this->_maps[SoundEvent::TL_SCORE] = "sound/effect/gin/boss_score.mp3";

	this->_maps[SoundEvent::TL_READY] = "sound/effect/gin/boss_ready.mp3";
	this->_maps[SoundEvent::TL_BONUS_WIN] = "sound/effect/gin/boss_bonus_win.mp3";
	this->_maps[SoundEvent::TL_GIN] = "sound/effect/gin/boss_hitpot_2.mp3";
	this->_maps[SoundEvent::TL_TURN] = "sound/effect/boss_turn.mp3";
	this->_maps[SoundEvent::TL_CLICK_CARD] = "sound/effect/boss_click_card.mp3";
	this->_maps[SoundEvent::TL_COMPLETE] = "sound/effect/gin/boss_complete.mp3";
	this->_maps[SoundEvent::TL_SECRET_MELD] = "sound/effect/gin/boss_secret_meld.mp3";

    this->_maps[SoundEvent::DAILY_BONUS]  = "sound/effect/gin/boss_score.mp3";
    this->_maps[SoundEvent::CLAIM_MONEY]  = "sound/effect/boss_score_box.mp3";
    this->_maps[SoundEvent::UNLOCK_TABLE] = "sound/effect/gin/boss_score.mp3";

    this->_maps[SoundEvent::TL_UNDERCUT_25]  = "sound/effect/gin/boss_tlmn_chatheo1.mp3";
    this->_maps[SoundEvent::TL_GIN_25]  = "sound/effect/gin/boss_tlmn_chatheo2.mp3";
    this->_maps[SoundEvent::TL_BIGGIN_50] = "sound/effect/gin/boss_tlmn_chatheo2.mp3";

    this->_maps[SoundEvent::TL_MULTIPLY] = "sound/effect/gin/boss_multiply.mp3";
}

bool SoundManager::getLoopBG()
{
    return _loopBG;
}

void SoundManager::setLoopBG(bool loopBG)
{
    _loopBG = loopBG;
}

void SoundManager::preloadBackgroundMusic(const char* filePath)
{
#if USE_AUDIO_ENGINE
    experimental::AudioEngine::preload(filePath);
#else
    _simpleAudioEngine->preloadBackgroundMusic(filePath);
#endif
}

void SoundManager::playBackgroundMusic(const char* filePath, bool loop)
{
    this->setLoopBG(loop);
#if USE_AUDIO_ENGINE
    backgroundId = experimental::AudioEngine::play2d(filePath, _loopBG, 1);
#else
    _simpleAudioEngine->playBackgroundMusic(filePath, _loopBG);
#endif
}

void SoundManager::stopBackgroundMusic()
{
#if USE_AUDIO_ENGINE
    if (backgroundId != -1)
        experimental::AudioEngine::stop(backgroundId);
#else
    _simpleAudioEngine->stopBackgroundMusic();
#endif
}

void SoundManager::pauseBackgroundMusic()
{
#if USE_AUDIO_ENGINE
    if (backgroundId != -1)
        experimental::AudioEngine::pause(backgroundId);
#else
    _simpleAudioEngine->pauseBackgroundMusic();
#endif
}

void SoundManager::resumeBackgroundMusic()
{
#if USE_AUDIO_ENGINE
    if (backgroundId != -1)
        experimental::AudioEngine::resume(backgroundId);
#else
    _simpleAudioEngine->resumeBackgroundMusic();
#endif
}

void SoundManager::rewindBackgroundMusic()
{
#if USE_AUDIO_ENGINE

#else
    _simpleAudioEngine->rewindBackgroundMusic();
#endif
}

bool SoundManager::isPlayingBackgroundMusic()
{
#if USE_AUDIO_ENGINE
    return false;
#else
    return _simpleAudioEngine->isBackgroundMusicPlaying();
#endif
}

float SoundManager::getBackgroundMusicVolume()
{
#if USE_AUDIO_ENGINE
    return experimental::AudioEngine::getVolume(backgroundId);
#else
    return _simpleAudioEngine->getBackgroundMusicVolume();
#endif

}

void SoundManager::setBackgroundMusicVolume(float volume)
{
#if USE_AUDIO_ENGINE
    experimental::AudioEngine::setVolume(backgroundId, volume);
#else
    _simpleAudioEngine->setBackgroundMusicVolume(volume);
#endif
}

void SoundManager::preloadEffect(const char* filePath)
{
#if USE_AUDIO_ENGINE
    experimental::AudioEngine::preload(filePath);
#else
    _simpleAudioEngine->preloadEffect(filePath);
#endif
}

void SoundManager::unloadEffect(const char* filePath)
{
#if USE_AUDIO_ENGINE
    experimental::AudioEngine::uncache(filePath);
#else
    _simpleAudioEngine->unloadEffect(filePath);
#endif
}

int SoundManager::playEffect(const char* filePath, bool loop, float gain /*= 1.0f*/)
{
#if USE_AUDIO_ENGINE
    return experimental::AudioEngine::play2d(filePath, loop, gain);
#else
    return _simpleAudioEngine->playEffect(filePath, loop, 1.0f, 0, gain);
#endif
}

void SoundManager::pauseEffect(unsigned int soundId)
{
#if USE_AUDIO_ENGINE
    experimental::AudioEngine::pause(soundId);
#else
    _simpleAudioEngine->pauseEffect(soundId);
#endif
}

void SoundManager::pauseAllEffects()
{
#if USE_AUDIO_ENGINE
    experimental::AudioEngine::pauseAll();
#else
    _simpleAudioEngine->pauseAllEffects();
#endif
}

void SoundManager::resumeEffect(int soundId)
{
#if USE_AUDIO_ENGINE

#else
    _simpleAudioEngine->resumeEffect(soundId);
#endif

}

void SoundManager::resumeAllEffects()
{
#if USE_AUDIO_ENGINE
    experimental::AudioEngine::resumeAll();
#else
    _simpleAudioEngine->resumeAllEffects();
#endif

}

void SoundManager::stopEffect(unsigned int soundId)
{
#if USE_AUDIO_ENGINE
    experimental::AudioEngine::stop(soundId);
#else
    _simpleAudioEngine->stopEffect(soundId);
#endif
}

void SoundManager::stopAllEffects()
{
#if USE_AUDIO_ENGINE
    experimental::AudioEngine::stopAll();
#else
    _simpleAudioEngine->stopAllEffects();
#endif
}

float SoundManager::getEffectsVolume()
{
#if USE_AUDIO_ENGINE
    return 0;
#else
    return _simpleAudioEngine->getEffectsVolume();
#endif
}

void SoundManager::setEffectsVolume(float volume)
{
#if USE_AUDIO_ENGINE
    //experimental::AudioEngine::setVolume(volume);
#else
    _simpleAudioEngine->setEffectsVolume(volume);
#endif
}

int SoundManager::playSoundEffect(SoundEvent soundEvent, bool loop /*= false*/, float gain/* = 1.0f*/)
{
    if (UserData::getInstance()->getSoundEffect())
    {
        const map<SoundEvent, string>& maps = getInstance()->getMaps();
        if (maps.find(soundEvent) != maps.end())
            return _instance->playEffect(maps.at(soundEvent).c_str(), loop, gain);
    }
    return -1;
}

void SoundManager::stopSoundEffect(int soundId)
{
    if (soundId > -1)
        _instance->stopEffect(soundId);
}

int SoundManager::playSoundEffectRandom(const std::vector<SoundEvent>& listRandom)
{
    if (listRandom.size() > 0)
        SoundManager::playSoundEffect(listRandom[GameUtils::randomIntInRange(0, listRandom.size() - 1)]);
    return 0;
}

void SoundManager::playBackgroundMusicAuto()
{
    update(MUSIC_CHANGE_INTERVAL);
    Director::getInstance()->getScheduler()->schedule(schedule_selector(SoundManager::update), this, 1, false);
}

void SoundManager::stopBackgroundMusicAuto()
{
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(SoundManager::update), this);
    SoundManager::getInstance()->stopBackgroundMusic();
}

void SoundManager::update(float dt)
{
    _deltaTime += dt;

#if USE_AUDIO_ENGINE
    if (_deltaTime >= MUSIC_CHANGE_INTERVAL && experimental::AudioEngine::getState(backgroundId) != experimental::AudioEngine::AudioState::PLAYING)
    {
#else
    if (_deltaTime >= MUSIC_CHANGE_INTERVAL && _simpleAudioEngine->isBackgroundMusicPlaying() == false)
    {
#endif
        if (_listBackgroundMusics.size() > 0)
        {
            int index            = 0;
            do
            {
                index = cocos2d::RandomHelper::random_int((unsigned int) 0, (unsigned int) _listBackgroundMusics.size() - 1);
            } while (_listBackgroundMusics.size() >= 2 && _currentIndexBGMusic == index);
            _currentIndexBGMusic = index;
            SoundManager::getInstance()->stopBackgroundMusic();
            SoundManager::getInstance()->playBackgroundMusic(_listBackgroundMusics[_currentIndexBGMusic].c_str(), false);
            _deltaTime = 0;
        }
    }
}

const map<SoundEvent, string>& SoundManager::getMaps()
{
    return _maps;
}

bool SoundManager::isMusicActive()
{
#if USE_AUDIO_ENGINE
    return !CallToOtherLanguage::getInstance()->willPlayBackgroundMusic();
#else
    return !CallToOtherLanguage::getInstance()->willPlayBackgroundMusic();
#endif
}
