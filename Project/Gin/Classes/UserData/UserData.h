#ifndef __USER_DATA_H__
#define __USER_DATA_H__

#include "cocos2d.h"
#include <string>
#include "json/rapidjson.h"
#include "json/document.h"
#include "Component/DeviceInfo.h"

#define USERDEFAULT_FILEEXIST "FileExist"
#define USERDEFAULT_RESTORE_SOUND_BG "RestoreSoundBG"
#define USERDEFAULT_SOUNDEFFECT "SoundEffect"
#define USERDEFAULT_SOUNDBACKGROUND "SoundBackground"
#define USERDEFAULT_VIBRATON "Vibration"
#define USERDEFAULT_IDPLACESELECTED "IdPlaceSelected"
#define USERDEFAULT_IDTABLESELECTED "IdTableSelected"
#define USERDEFAULT_WIN_NUMBER "wnn"
#define USERDEFAULT_WIN_HITPOT_NUMBER "whpnn"
#define USERDEFAULT_LOSE_NUMBER "lnn"
#define USERDEFAULT_WV_NUMBER "wvnn"
#define USERDEFAULT_BET "Bet"
#define USERDEFAULT_TARGET_SCORE "target_score"
#define USERDEFAULT_PLAYGAME_SIGNOUT "pgsignout"
#define USERDEFAULT_DAILY_NEWS_SHOW_COUNT "dailynewsshowcount"
#define USERDEFAULT_DAILY_NEWS_SHOW_DAY_COUNT "dailynewsshowdaycount"
#define USERDEFAULT_DAILY_NEWS_VERSION "dailynewsversion"
#define USERDEFAULT_GAME_CENTER_VERSION "GameCenterVersion"
#define USERDEFAULT_SUGGEST_UPDATE_VERSION_CODE "SuggestUpdateVersionCode"
#define MAILBOX_READ_FILENAME "mrd"

class UserData
{
private:
    std::string              _userName;
    std::string              _avatarLink;
    std::vector<std::string> _mailReadlist;
    double                   _userMoney;
    DeviceInfo* _deviceInfo;
    float  _luckyValue;
    double _bet;
    int    _targetScore;
    int    _level;
    int    _levelexp;
    int    _nextlevelexp;

    bool _restoreSoundBackground;
    bool _soundEffect;
    bool _soundBackground;
    bool _vibration;
    int  _idPlaceSelected;
    int  _idTableSelected;
    int  _idRuleSelected;
    int  _gameCenterVersion;
    bool _enableInterferingAds;
    UserData();
protected:
public:
    ~UserData();

    static UserData* getInstance();
    static void destroyInstance();

    DeviceInfo* getDeviceInfo();

    void setMoney(double money);
    double getMoney();

    void setUserName(const std::string& userName);
    std::string getUserName();

    void setLevel(int level);
    int getLevel();

    void setLevelExp(int levelexp);
    int getLevelExp();

    void setNextLevelExp(int nextlevelexp);
    int getNextLevelExp();

    void setAvatarLink(const std::string& avatarLink);
    std::string getAvatarLink();

    void setLuckyValue(float luckyVal);
    float getLuckyValue();


    void obtainDeviceInfo();

    void setRestoreSoundBG(bool shouldRestore);
    bool getRestoreSoundBG();

    void setSoundEffect(bool sound);
    bool getSoundEffect();

    void setSoundBackground(bool sound);
    bool getSoundBackground();

    void setBet(double bet);
    double getBet();

    void setTargetScore(int targetScore);
    double getTargetScore();

    void setVibration(bool vibration);
    bool getVibration();

    void setIdPlaceSelected(int idPlaceSelected);
    int getIdPlaceSelected();

    void setIdTableSelected(int idTableSelected);
    int getIdTableSelected();

    void saveWinOrLoseNumber();
    void saveWatchVideoNumber();

    void updateNewsFunctionVersion();
    bool shouldShowDailyNews(int versionCode);
    void resetNewsShowCounter();
    void updateNewsShowCounter(int versionCode);

    bool isEnableButtonRating();
    bool isEnableShowRatingPopup();
    bool isEnableGameCenterPopup();
    bool isEnableSuggestUpdate();
    bool isEnableLocalNotification();

    int getGameCenterVersion();
    void setGameCenterVersion(int version);

    int getSuggestUpdateVersionCode();
    void setSuggestUpdateVersionCode(int version);

    void setMailRead(const std::vector<std::string>& ids);
    std::vector<std::string> getMailRead();

    void setInterferingAdsEnable(bool enable);
    bool isEnableInterferingAds();

    bool isEventDecorEnabled();
    void parseLocalNotificationData(const rapidjson::Value& listNotificationData);
};

#endif //__USER_DATA_H__
