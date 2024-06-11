#ifndef __CHEAT_CARDS_LAYER_H__
#define __CHEAT_CARDS_LAYER_H__

#include "cocos2d.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "ui/CocosGUI.h"
#include "Define/GameDefine.h"
#include "Define/InGameDefine.h"

enum class CardButtonState
{
    STATE_NORMAL,
    STATE_CHEATED,
    STATE_DISABLE
};

struct CardButtonStruct
{
public:
    CardButtonState _state;
    cocos2d::ui::Button* _cardButton;

    CardButtonStruct(CardButtonState state, cocos2d::ui::Button* cardButton)
    {
        _state      = state;
        _cardButton = cardButton;
    }

    CardButtonStruct()
    {
        _state      = CardButtonState::STATE_NORMAL;
        _cardButton = nullptr;
    }
};

class CheatCardLayer : public cocos2d::Layer
{
private:
    int _userId;
    cocos2d::ui::Layout   * _layoutInterfaceRoot;
    cocos2d::ui::Layout   * _layoutCheatCards;
    cocos2d::ui::Layout   * _layoutCheatCombine;
    cocos2d::ui::Button   * _closeButton;
    cocos2d::ui::Button   * _cardButton;
    cocos2d::ui::Text     * _textCheat;
    cocos2d::ui::Text     * _textUserId;
    cocos2d::ui::TextField* _textField;
    cocos2d::ui::Button   * _buttonApply;
    cocos2d::ui::CheckBox * _cbFirstMove;
    cocos2d::ui::Button   * _buttonCheatCard;
    CardButtonStruct _arrCardButtonStruct[52];
protected:
public:
    CheatCardLayer();
    ~CheatCardLayer();

    static CheatCardLayer* getInstance();
    static void destroyInstance();

    void setCardButtonState(int userid, CardButtonStruct* cardButtonStruct, CardButtonState state, int order = -1);

    virtual bool init() override;
    void initCardsButton();
    void onCloseButton(cocos2d::Ref* sender);
    void onCheatCardButton(cocos2d::Ref* sender);
    void onClearCheatCardButton(cocos2d::Ref* sender);
    void onCardButtonClick(cocos2d::Ref* sender);
    void onApplyButton(cocos2d::Ref* sender);
    void onCheatCardResponse(const rapidjson::Document& cheatCardsResponse);
    void setCheatedCard(int userId, std::vector<int> listCheatedCardIndex, std::vector<int> listDisableCardIndex);
    void attach(cocos2d::Node* node, int sceneId);
    void requestCheatCard(int cardIdx);
    void requestClearCheatCard();
    void requestCancelCheatCard(int cardIdx);
    void showCheatLayer(int userId);
    void onCheckBoxClick(cocos2d::Ref* ref, cocos2d::ui::CheckBox::EventType type);
    virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
    CREATE_FUNC(CheatCardLayer);
};

#endif //__CHEAT_CARDS_LAYER_H__