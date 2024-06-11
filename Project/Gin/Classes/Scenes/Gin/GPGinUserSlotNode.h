#ifndef __GPGinUserSlotNode__
#define __GPGinUserSlotNode__

#include "cocos2d.h"
#include "Component/Avatar.h"
#include "GinSlotInfo.h"
#include "Manager/TextManager.h"
#include "ui/CocosGUI.h"
#include "Component/ScoreBoxNode.h"
#include "Component/DeadwoodBoxNode.h"

class GPGinInterfaceLayer;

class GPGinUserSlotNode: public cocos2d::Node 
{
private:
    GinSlotInfo* _slotInfo;
    Avatar                 * _avatar;
    cocos2d::ui::Layout    * _layoutRoot;
    cocos2d::ui::Layout    * _layoutInfo;
    cocos2d::ui::Layout    * _layoutAvatar;
    cocos2d::ui::Text      * _textUsername;
    cocos2d::ui::TextBMFont* _textMoney;
    cocos2d::ui::Layout    * _layoutDot;
    cocos2d::ui::TextBMFont* _textUserLevel;
    cocos2d::ui::Slider    * _sliderUserLevel;
    cocos2d::ui::Layout    * _imageviewBGOnTable;
    cocos2d::ui::Layout    * _imageviewBGOnTableAnim;
    cocos2d::ui::Layout    * _layoutNotify;
    ScoreBoxNode           * _scoreBoxNode;
	DeadwoodBoxNode        * _deadwoodBoxNode;
    cocos2d::ui::Layout    * _jackpotIconNode;
    cocos2d::ui::ImageView * _imageTurnBorder;

    bool           _active;
    int            _slotIdx;
    double         _currentMoney;
    double         _currentMoneyForAnim;
    cocos2d::Point _originalPosition;
    int            _countDot;
    float          _scaleOriginal;
    float          _scaleOriginalTextMoney;
    int            _currentLevel;
    int            _currentLevelExp;
    int            _currentLevelTotalExp;
    double         _currentLevelExpForAnimation;
protected:
public:
	CC_SYNTHESIZE(GPGinInterfaceLayer*, _gpGinInterfaceLayer, gpGinInterfaceLayer);
	GPGinUserSlotNode();
	~GPGinUserSlotNode();

    virtual bool init() override;
    void initLayout(int idx, ScoreBoxNode* scoreBoxNode, DeadwoodBoxNode* deadwoodBoxNode, cocos2d::ui::Layout* imageviewBGTable, cocos2d::ui::Layout* imageviewBGTableAnim, cocos2d::ui::Layout* layoutNotify);
    void setUsername(const std::string& username);
    void setMoney(double money);
    void setMoney(double money, bool animation);
    void setAvatarLink(const std::string& link);
    void onAvatarClick(cocos2d::Ref* pSender);

    void setLevel(int level);
    void setLevelExp(int newLevel, int currentExp, int totalExp, bool animation = false);

    ScoreBoxNode* getScoreBoxNode();
    DeadwoodBoxNode* getDeadwoodBoxNode();
    cocos2d::Size getLayoutCardOnTableSize();

    cocos2d::ui::Text* getTextUserName();
    cocos2d::ui::Layout* getLayoutRoot();

    void setPlaying(bool thinking);
    void setActive(bool active);
    bool isActive();

    void setSlotIdx(int slotIdx);
    int getSlotIdx();

    void setSlotInfo(GinSlotInfo* slotInfo);
    GinSlotInfo* getSlotInfo();
    
    void updateInfo();
    void updateMoney();
    void updateLevel();
    void setOriginalPosition(const cocos2d::Point& pos);
    cocos2d::Point getOriginalPosition();

    void reset();
    void setOnTurn(bool onTurn, bool anim = true);

    cocos2d::Vec2 getPositionTextMoneyOnScreen();
    cocos2d::Vec2 getPositionJackpotIconOnScreen();

    void showJackpotIcon(bool visible, bool anim);
    void showNotify(bool visible, const std::string& text);
    void showTextDropArea(bool visible);

	CREATE_FUNC(GPGinUserSlotNode);
};

#endif //__GPGinUserSlotNode__
