//
// Created by Vo Minh Nhut on 25/10/2022.
//

#ifndef GIN_GINSCORETABLE_H
#define GIN_GINSCORETABLE_H

class GinScoreTable;

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <functional>
#include "Define/GameDefine.h"
#include "Define/InGameDefine.h"
#include "Component/JackpotIconNode.h"
#include "Component/GinUserHandNode.h"
#include "Avatar.h"
#include "Manager/TextManager.h"

typedef std::function<void()> ActionCallback;

class GinScoreTable : public cocos2d::Layer
{
private:
    cocos2d::ui::Layout   * _root;
    cocos2d::ui::Layout   * _layoutTargetScoreTitle;
    cocos2d::ui::Text * _textTargetScoreTitle;
    cocos2d::ui::Layout   * _layoutScoreUser;
    cocos2d::ui::ImageView * _imageAvatarUser;
    Avatar                 * _avatarUser;
    cocos2d::ui::Text * _textScoreUser;
    cocos2d::ui::Text * _textFinalScoreUser;
    cocos2d::ui::Layout   * _layoutScoreBot;
    cocos2d::ui::ImageView * _imageAvatarBot;
    Avatar                 * _avatarBot;
    cocos2d::ui::Text * _textScoreBot;
    cocos2d::ui::Text * _textFinalScoreBot;

    std::vector<cocos2d::ui::Layout*> _listPlayerScoreLayout;
    std::vector<cocos2d::Point> _listPlayerScoreLayoutPosition;

    std::vector<BetData> _listBetData;
    int    _targetScore;

    bool   _isShown;

    std::vector<cocos2d::Point> _listPosition;
protected:
public:
    GinScoreTable();
    ~GinScoreTable();

    virtual bool init() override;
    void changeLanguage(Language language);

    void initLayout(cocos2d::ui::Layout* root, cocos2d::ui::Layout* parent);

    void resetLayoutTargetScore(int targetScore, const std::vector<BetData>& listBetData);
    void updateLayoutTargetScore(int targetScore, const std::vector<BetData>& listBetData);

    void setAvatarLinks(const std::string& linkUser, const std::string& linkBot);
    void setUserAvatarLink(const std::string& linkUser);
    void setBotAvatarLink(const std::string& linkBot);

    void runShowAnimation(float delay, ActionCallback callback);
    void runHideAnimation(float delay, ActionCallback callback);

    void setShow(bool visible);
    bool getShow();

    cocos2d::Point getUserScoreGlobalPos();
    cocos2d::Point getBotScoreGlobalPos();

    CREATE_FUNC(GinScoreTable);
};

#endif //GIN_GINSCORETABLE_H
