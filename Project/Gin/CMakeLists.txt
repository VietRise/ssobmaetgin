#/****************************************************************************
# Copyright (c) 2013-2014 cocos2d-x.org
# Copyright (c) 2015-2017 Chukong Technologies Inc.
#
# http://www.cocos2d-x.org
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# ****************************************************************************/

cmake_minimum_required(VERSION 3.6)

set(APP_NAME Gin)

project(${APP_NAME})
if (APPLE)
    set(CMAKE_OSX_ARCHITECTURES x86_64)
    add_compile_options(-Wall -Wextra -pedantic)
endif ()

set(COCOS2DX_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cocos2d)
set(CMAKE_MODULE_PATH ${COCOS2DX_ROOT_PATH}/cmake/Modules/)

include(CocosBuildSet)
add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos ${ENGINE_BINARY_PATH}/cocos/core)

if (APPLE)
    execute_process(COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/make_data.sh
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/)
endif ()

# record sources, headers, resources...
set(GAME_SOURCE)
set(GAME_HEADER)

set(GAME_RES_FOLDER
        "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
        )
if (APPLE)
    cocos_mark_multi_resources(common_res_files RES_TO "Resources" FOLDERS ${GAME_RES_FOLDER})
endif ()

# add cross-platforms source files and header files 
list(APPEND GAME_SOURCE
        # Classes/AdapterClientServer/*
        Classes/AdapterClientServer/AdapterClientServer.cpp
        Classes/AdapterClientServer/ClientManager.cpp
        Classes/AdapterClientServer/ServerManager.cpp

        # Classes/Boards/*
        Classes/Boards/ContinueToPlayConfirmPopup.cpp
        Classes/Boards/DailyBonusPopup.cpp
        Classes/Boards/GetMoneyToPlayPopup.cpp
        Classes/Boards/InterfacePopupBoard.cpp
        Classes/Boards/LobbyInfoUserBoard.cpp
        Classes/Boards/LobbySettingBoard.cpp
        Classes/Boards/LobbyShopTableBoard.cpp
        Classes/Boards/LobbyTicketFlightBoard.cpp
        Classes/Boards/NewsPopup.cpp
        Classes/Boards/NotifyPopup.cpp
        Classes/Boards/YesNoConfirmPopup.cpp
        Classes/Boards/LeaderboardPopup.cpp
        Classes/Boards/RatingPopup.cpp
        Classes/Boards/RatingPopupThank.cpp
        Classes/Boards/RewardPopup.cpp
        Classes/Boards/GameCenterPopup.cpp
        Classes/Boards/SuggestUpdatePopup.cpp
        Classes/Boards/VideoRewardsPopup.cpp
        Classes/Boards/MailboxPopup.cpp
        Classes/Boards/MailboxContentPopup.cpp
        Classes/Boards/RoundPointPopup.cpp
        Classes/Boards/ChangeAvatarPopup.cpp
        Classes/Boards/ActionConfirmPopup.cpp
        Classes/Boards/FightChallengerPopup.cpp
        Classes/Boards/GameResultPopup.cpp
        Classes/Boards/ThrowCardDetailPopup.cpp

        # Classes/Component/*
        Classes/Component/GinBaseCard.cpp
        Classes/Component/GinCardStore.cpp
        Classes/Component/Avatar.cpp
        Classes/Component/DeviceInfo.cpp
        Classes/Component/LobbyPlace.cpp
        Classes/Component/LobbyTableNew.cpp
        Classes/Component/RuleDetails.cpp
        Classes/Component/RuleScore.cpp
        Classes/Component/TutorialView.cpp
        Classes/Component/MoneyBoxNode.cpp
        Classes/Component/MailBoxItem.cpp
        Classes/Component/ScoreBoxNode.cpp
        Classes/Component/JackpotIconNode.cpp
        Classes/Component/GinUserHandNode.cpp
        Classes/Component/DeadwoodBoxNode.cpp
        Classes/Component/GinScoreTable.cpp
        Classes/Component/ResultScoreBoxNode.cpp
        Classes/Component/EndGameResultNode.cpp

        # Classes/Define/*
        Classes/Define/GameDefine.cpp
        Classes/Define/InGameDefine.cpp

        # Classes/FirebaseGG/*
        Classes/FirebaseGG/RemoteConfig.cpp

        # Classes/Helper/*
        Classes/Helper/CardIndexConverter.cpp
        Classes/Helper/ConsoleTable.cpp
        Classes/Helper/DebugHelper.cpp
        Classes/Helper/format.cc
        Classes/Helper/GameUtils.cpp
        Classes/Helper/JsonHelper.cpp
        Classes/Helper/LogHelper.cpp
        Classes/Helper/StringHelper.cpp
        Classes/Helper/VMD5.cpp

        # Classes/Manager/*
        Classes/Manager/BoardManager.cpp
        Classes/Manager/DownloadManager.cpp
        Classes/Manager/MyActionsManager.cpp
        Classes/Manager/NotificationManager.cpp
        Classes/Manager/SceneManager.cpp
        Classes/Manager/SoundManager.cpp
        Classes/Manager/TextManager.cpp
        Classes/Manager/AdsManager.cpp
        Classes/Manager/ResourcesManager.cpp

        # Classes/Native/*
        Classes/Native/CallToOtherLanguage.cpp
        Classes/Native/NativeCallBack.cpp
        Classes/Native/NativeCallBackJava.cpp

        # Classes/Network/*
        Classes/Network/MessageHandler.cpp
        Classes/Network/MessageSender.cpp

        # Classes/Scenes/*
        Classes/Scenes/Gin/GPGinControlCardLayer.cpp
        Classes/Scenes/Gin/GPGinDealCardLayer.cpp
        Classes/Scenes/Gin/GPGinEffect.cpp
        Classes/Scenes/Gin/GPGinInterfaceLayer.cpp
        Classes/Scenes/Gin/GPGinScene.cpp
        Classes/Scenes/Gin/GPGinSlotManager.cpp
        Classes/Scenes/Gin/GPGinUserSlotNode.cpp
        Classes/Scenes/Gin/GinCard.cpp
        Classes/Scenes/Gin/GinSlotInfo.cpp
        Classes/Scenes/DebugLayer.cpp
        Classes/Scenes/LoadingLayer.cpp
        Classes/Scenes/InterfaceScene.cpp
        Classes/Scenes/LobbyScene.cpp
        Classes/Scenes/SplashScene.cpp
        Classes/Scenes/CheatCardLayer.cpp

        # Classes/ServerOffline/*
        Classes/ServerOffline/Bot/BotConfig.cpp
        Classes/ServerOffline/Bot/BotHelper.cpp
        Classes/ServerOffline/Bot/BotLogger.cpp
        Classes/ServerOffline/Bot/BotOpponentHandEstimation.cpp
        Classes/ServerOffline/Rules/RuleCommon.cpp
        Classes/ServerOffline/Rules/RuleInstantWin.cpp
        Classes/ServerOffline/Rules/RuleShowCard.cpp
        Classes/ServerOffline/Rules/RuleTakeCard.cpp
        Classes/ServerOffline/Rules/RuleThrowCard.cpp
        Classes/ServerOffline/Rules/RuleLayoffCard.cpp
        Classes/ServerOffline/Rules/RuleKnocking.cpp
        Classes/ServerOffline/ServerOffline.cpp
        Classes/ServerOffline/SvrBotHelper.cpp
        Classes/ServerOffline/SvrCard.cpp
        Classes/ServerOffline/SvrHelper.cpp
        Classes/ServerOffline/SvrUserProfile.cpp
        Classes/ServerOffline/GinGameMatchManager.cpp
        Classes/ServerOffline/HandCard.cpp
        Classes/ServerOffline/Feature/SvrLocalNotification.cpp
        Classes/ServerOffline/Feature/SvrTimeBonus.cpp
        Classes/ServerOffline/Feature/SvrHourlyBonus.cpp
        Classes/ServerOffline/Feature/SvrMailbox.cpp
        Classes/ServerOffline/Feature/SvrWelcomeBackBonus.cpp
        Classes/ServerOffline/Feature/SvrRating.cpp
        Classes/ServerOffline/Feature/SvrLevel.cpp
        Classes/ServerOffline/Feature/SvrDailyBonus.cpp
        Classes/ServerOffline/Feature/SvrVideoAds.cpp
        Classes/ServerOffline/Feature/SvrLog.cpp
        Classes/ServerOffline/Feature/SvrShop.cpp

        # Classes/UserData/*
        Classes/UserData/UserData.cpp

        # Classes/*
        Classes/AppDelegate.cpp
        Classes/Instance.cpp
        )

list(APPEND GAME_HEADER
        # Classes/AdapterClientServer/*
        Classes/AdapterClientServer/AdapterClientServer.h
        Classes/AdapterClientServer/ClientManager.h
        Classes/AdapterClientServer/ServerManager.h

        # Classes/Boards/*
        Classes/Boards/ContinueToPlayConfirmPopup.h
        Classes/Boards/DailyBonusPopup.h
        Classes/Boards/GetMoneyToPlayPopup.h
        Classes/Boards/InterfacePopupBoard.h
        Classes/Boards/LobbyInfoUserBoard.h
        Classes/Boards/LobbySettingBoard.h
        Classes/Boards/LobbyShopTableBoard.h
        Classes/Boards/LobbyTicketFlightBoard.h
        Classes/Boards/NewsPopup.h
        Classes/Boards/NotifyPopup.h
        Classes/Boards/YesNoConfirmPopup.h
        Classes/Boards/LeaderboardPopup.h
        Classes/Boards/RatingPopup.h
        Classes/Boards/RatingPopupThank.h
        Classes/Boards/RewardPopup.h
        Classes/Boards/GameCenterPopup.h
        Classes/Boards/SuggestUpdatePopup.h
        Classes/Boards/VideoRewardsPopup.h
        Classes/Boards/MailboxPopup.h
        Classes/Boards/MailboxContentPopup.h
        Classes/Boards/RoundPointPopup.h
        Classes/Boards/ChangeAvatarPopup.h
        Classes/Boards/ActionConfirmPopup.h
        Classes/Boards/FightChallengerPopup.h
        Classes/Boards/GameResultPopup.h
        Classes/Boards/ThrowCardDetailPopup.h

        # Classes/Component/*
        Classes/Component/GinBaseCard.h
        Classes/Component/GinCardStore.h
        Classes/Component/Avatar.h
        Classes/Component/DeviceInfo.h
        Classes/Component/LobbyPlace.h
        Classes/Component/LobbyTableNew.h
        Classes/Component/RuleDetails.h
        Classes/Component/RuleScore.h
        Classes/Component/TutorialView.h
        Classes/Component/MoneyBoxNode.h
        Classes/Component/MailBoxItem.h
        Classes/Component/ScoreBoxNode.h
        Classes/Component/JackpotIconNode.h
        Classes/Component/GinUserHandNode.h
        Classes/Component/DeadwoodBoxNode.h
        Classes/Component/GinScoreTable.h
        Classes/Component/ResultScoreBoxNode.h
        Classes/Component/EndGameResultNode.h

        # Classes/Define/*
        Classes/Define/GameDefine.h
        Classes/Define/InGameDefine.h
        Classes/Define/ServerClientMessageDefine.h

        # Classes/FirebaseGG/*
        Classes/FirebaseGG/RemoteConfig.h
        Classes/FirebaseGG/Firebase.h

        # Classes/Helper/*
        Classes/Helper/CardIndexConverter.h
        Classes/Helper/ConsoleTable.h
        Classes/Helper/DebugHelper.h
        Classes/Helper/format.cc
        Classes/Helper/GameUtils.h
        Classes/Helper/JsonHelper.h
        Classes/Helper/LogHelper.h
        Classes/Helper/StringHelper.h
        Classes/Helper/VMD5.h

        # Classes/Manager/*
        Classes/Manager/BoardManager.h
        Classes/Manager/DownloadManager.h
        Classes/Manager/MyActionsManager.h
        Classes/Manager/NotificationManager.h
        Classes/Manager/SceneManager.h
        Classes/Manager/SoundManager.h
        Classes/Manager/TextManager.h
        Classes/Manager/AdsManager.h
        Classes/Manager/ResourcesManager.h

        # Classes/Native/*
        Classes/Native/CallToOtherLanguage.h
        Classes/Native/NativeCallBack.h
        Classes/Native/NativeCallBackJava.h

        # Classes/Network/*
        Classes/Network/MessageHandler.h
        Classes/Network/MessageSender.h

        # Classes/Scenes/*
        Classes/Scenes/Gin/GPGinControlCardLayer.h
        Classes/Scenes/Gin/GPGinDealCardLayer.h
        Classes/Scenes/Gin/GPGinEffect.h
        Classes/Scenes/Gin/GPGinInterfaceLayer.h
        Classes/Scenes/Gin/GPGinScene.h
        Classes/Scenes/Gin/GPGinSlotManager.h
        Classes/Scenes/Gin/GPGinUserSlotNode.h
        Classes/Scenes/Gin/GinCard.h
        Classes/Scenes/Gin/GinSlotInfo.h
        Classes/Scenes/DebugLayer.h
        Classes/Scenes/LoadingLayer.h
        Classes/Scenes/InterfaceScene.h
        Classes/Scenes/LobbyScene.h
        Classes/Scenes/SplashScene.h
        Classes/Scenes/CheatCardLayer.h

        # Classes/ServerOffline/*
        Classes/ServerOffline/Bot/BotConfig.h
        Classes/ServerOffline/Bot/BotConstant.h
        Classes/ServerOffline/Bot/BotHelper.h
        Classes/ServerOffline/Bot/BotLogger.h
        Classes/ServerOffline/Bot/BotOpponentHandEstimation.h
        Classes/ServerOffline/Rules/RuleCommon.h
        Classes/ServerOffline/Rules/RuleConstant.h
        Classes/ServerOffline/Rules/RuleData.h
        Classes/ServerOffline/Rules/RuleInstantWin.h
        Classes/ServerOffline/Rules/RuleShowCard.h
        Classes/ServerOffline/Rules/RuleTakeCard.h
        Classes/ServerOffline/Rules/RuleThrowCard.h
        Classes/ServerOffline/Rules/RuleLayoffCard.h
        Classes/ServerOffline/Rules/RuleKnocking.h
        Classes/ServerOffline/ServerOffline.h
        Classes/ServerOffline/SvrBotHelper.h
        Classes/ServerOffline/SvrCard.h
        Classes/ServerOffline/SvrHelper.h
        Classes/ServerOffline/SvrUserProfile.h
        Classes/ServerOffline/GinGameMatchManager.h
        Classes/ServerOffline/HandCard.h
        Classes/ServerOffline/Feature/SvrLocalNotification.h
        Classes/ServerOffline/Feature/SvrTimeBonus.h
        Classes/ServerOffline/Feature/SvrHourlyBonus.h
        Classes/ServerOffline/Feature/SvrMailbox.h
        Classes/ServerOffline/Feature/SvrWelcomeBackBonus.h
        Classes/ServerOffline/Feature/SvrRating.h
        Classes/ServerOffline/Feature/SvrLevel.h
        Classes/ServerOffline/Feature/SvrDailyBonus.h
        Classes/ServerOffline/Feature/SvrVideoAds.h
        Classes/ServerOffline/Feature/SvrLog.h
        Classes/ServerOffline/Feature/SvrShop.h

        # Classes/UserData/*
        Classes/UserData/UserData.h

        # Classes/*
        Classes/AppDelegate.h
        Classes/Instance.h
        )

if (ANDROID)
    list(APPEND GAME_SOURCE
            Classes/Component/DeviceInfo_android.cpp
            Classes/FirebaseGG/Firebase_android.cpp
            Classes/FirebaseGG/RemoteConfig_android.cpp
            )
elseif (APPLE)
    if (MACOSX)
        list(APPEND GAME_SOURCE
                Classes/Component/DeviceInfo_win32.cpp
                Classes/Native/CallToOtherLanguage_MAC.mm
                Classes/FirebaseGG/Firebase_win32.cpp
                Classes/FirebaseGG/RemoteConfig_win32.cpp
                )
    endif ()
endif ()

if (ANDROID)
    # change APP_NAME to the share library name for Android, it's value depend on AndroidManifest.xml
    set(APP_NAME MyGame)
    list(APPEND GAME_SOURCE
            proj.android/app/jni/hellocpp/main.cpp
            )
elseif (APPLE)
    if (MACOSX)
        set(APP_UI_RES
                proj.ios_mac/mac/Icon.icns
                proj.ios_mac/mac/Info.plist
                )
        list(APPEND GAME_SOURCE
                proj.ios_mac/mac/main.cpp
                proj.ios_mac/mac/Prefix.pch
                ${APP_UI_RES}
                )
    endif ()
    list(APPEND GAME_SOURCE ${common_res_files})
endif ()


# mark app complie info and libs info
set(all_code_files
        ${GAME_HEADER}
        ${GAME_SOURCE}
        )
if (NOT ANDROID)
    add_executable(${APP_NAME} ${all_code_files})
else ()
    add_library(${APP_NAME} SHARED ${all_code_files})
    add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos/platform/android ${ENGINE_BINARY_PATH}/cocos/platform)
    target_link_libraries(${APP_NAME} -Wl,--whole-archive cpp_android_spec -Wl,--no-whole-archive)
endif ()

target_link_libraries(${APP_NAME} cocos2d)
target_include_directories(${APP_NAME}
        PRIVATE Classes
        PRIVATE ${COCOS2DX_ROOT_PATH}/cocos/audio/include/
        )

# mark app resources
setup_cocos_app_config(${APP_NAME})
if (APPLE)
    set_target_properties(${APP_NAME} PROPERTIES RESOURCE "${APP_UI_RES}")
    if (MACOSX)
        set_target_properties(${APP_NAME} PROPERTIES
                MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/mac/Info.plist"
                )
    endif ()
endif ()