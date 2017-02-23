//
//  MemberEditLayer.cpp
//  CrashProject
//
//  Created by kiyotaka-saitou on 2016/08/03.
//
//

#include "MemberEditLayer.hpp"
#include "MemberManager.hpp"
#include "GameHistoryItemNode.hpp"

USING_NS_CC;
using namespace cocostudio;

MemberEditLayer::MemberEditLayer()
:nameEditBox(nullptr)
,nameFuriganaEditBox(nullptr)
,nicknameEditBox(nullptr)
,maleButton(nullptr)
,femaleButton(nullptr)
,trainingButton(nullptr)
,beginnerButton(nullptr)
,middleButton(nullptr)
,higherButton(nullptr)
,countEditBox(nullptr)
,lastTimeEditBox(nullptr)
//,countTextField(nullptr)
//,lastTimeTextField(nullptr)
,deleteButton(nullptr)
,saveButton(nullptr)
,backButton(nullptr)
{
}

MemberEditLayer::~MemberEditLayer()
{
}





#pragma mark - 初期化
MemberEditLayer *MemberEditLayer::create(UserInfo *userInfo)
{
    MemberEditLayer *ret = new MemberEditLayer();
    if (ret->init(userInfo)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool MemberEditLayer::init(UserInfo *userInfo)
{
    std::string csbName = "MemberEditLayer.csb";
    if ( !CsbLayerBase::init(csbName) ) return false;
    
    this->userInfo = userInfo;
    
    auto panel = mainLayer->getChildByName<ui::Layout *>("Panel");
    auto headerPanel = panel->getChildByName<ui::Layout *>("HeaderPanel");
    
    float x = 295.0f;
    float y = 937.0f;
    nameEditBox = this->makeEditBox(Vec2(x, y), "[必須]氏名");
    y -= 80.0f;
    nameFuriganaEditBox = this->makeEditBox(Vec2(x, y), "[必須]ふりがな");
    y -= 80.0f;
    nicknameEditBox = this->makeEditBox(Vec2(x, y), "[任意]ニックネーム");

    auto contentPanel = panel->getChildByName<ui::Layout *>("PanelContent");
    maleButton     = contentPanel->getChildByName<ui::Button *>("ButtonGenderMale");
    femaleButton   = contentPanel->getChildByName<ui::Button *>("ButtonGenderFemale");
    trainingButton = contentPanel->getChildByName<ui::Button *>("ButtonLevelTraining");
    beginnerButton = contentPanel->getChildByName<ui::Button *>("ButtonLevelBeginner");
    middleButton   = contentPanel->getChildByName<ui::Button *>("ButtonLevelMiddle");
    higherButton   = contentPanel->getChildByName<ui::Button *>("ButtonLevelHigher");
    
    y -= 80.0f;
    y -= 80.0f;
    y -= 80.0f;
    countEditBox = this->makeEditBox(Vec2(x, y), "参加回数(編集非推奨)");
    
    y -= 80.0f;
    lastTimeEditBox = this->makeEditBox(Vec2(x, y), "最終参加日(編集非推奨)");
    
//    countTextField    = panel->getChildByName<ui::TextField *>("TextFieldCount");
//    lastTimeTextField = panel->getChildByName<ui::TextField *>("TextFieldLastTime");
    
    deleteButton  = headerPanel->getChildByName<ui::Button *>("ButtonDelete");
    saveButton    = headerPanel->getChildByName<ui::Button *>("ButtonSave");
    backButton    = headerPanel->getChildByName<ui::Button *>("ButtonBack");
   
    this->addButtonEvent(maleButton,     ButtonTag::Male);
    this->addButtonEvent(femaleButton,   ButtonTag::Female);
    this->addButtonEvent(trainingButton, ButtonTag::Training);
    this->addButtonEvent(beginnerButton, ButtonTag::Beginner);
    this->addButtonEvent(middleButton,   ButtonTag::Middle);
    this->addButtonEvent(higherButton,   ButtonTag::Higher);
    this->addButtonEvent(deleteButton,   ButtonTag::Delete);
    this->addButtonEvent(saveButton,     ButtonTag::Save);
    this->addButtonEvent(backButton,        ButtonTag::Back);
    
    // 新規
    if (!userInfo)
    {
        headerPanel->getChildByName<ui::Text *>("TextTitle")->setString("新規");
        deleteButton->setVisible(false);
    }
    // 編集
    else
    {
        headerPanel->getChildByName<ui::Text *>("TextTitle")->setString("編集");
        name         = userInfo->name;
        nameFurigana = userInfo->nameFurigana;
        nickname     = userInfo->nickname;
        gender       = userInfo->gender;
        level        = userInfo->level;
        countStr     = StringUtils::toString(userInfo->count);
        lastTime     = userInfo->lastTime;
    }
    
    // テキスト更新
    nameEditBox->setText(name.c_str());
    nameFuriganaEditBox->setText(nameFurigana.c_str());
    nicknameEditBox->setText(nickname.c_str());
    
    countEditBox->setText(countStr.c_str());
    lastTimeEditBox->setText(lastTime.c_str());
//    countTextField->setString(countStr);
//    lastTimeTextField->setString(lastTime);
    
    
    
    // ボタン表示更新
    this->updateGenderButton();
    this->updateLevelButton();
    
    return true;
}


void MemberEditLayer::updateGenderButton()
{
    bool enabledMale   = true;
    bool enabledFemale = true;
    switch (gender) {
        case Gender::Male:   enabledMale = false;   break;
        case Gender::Female: enabledFemale = false; break;
        default: break;
    }
    
    this->setButtonEnabled(maleButton,   enabledMale);
    this->setButtonEnabled(femaleButton, enabledFemale);
}

void MemberEditLayer::updateLevelButton()
{
    bool enabledTraining = true;
    bool enabledBeginner = true;
    bool enabledMiddle   = true;
    bool enabledHigher   = true;
    switch (level) {
        case Level::Training: enabledTraining = false;  break;
        case Level::Beginner: enabledBeginner = false;  break;
        case Level::Middle:   enabledMiddle   = false;  break;
        case Level::Higher:   enabledHigher   = false;  break;
        default:    break;
    }
    
    this->setButtonEnabled(trainingButton, enabledTraining);
    this->setButtonEnabled(beginnerButton, enabledBeginner);
    this->setButtonEnabled(middleButton, enabledMiddle);
    this->setButtonEnabled(higherButton, enabledHigher);
}



void MemberEditLayer::pushedButton(Ref *pSender, ui::Widget::TouchEventType type)
{
    if (type != ui::Widget::TouchEventType::ENDED) return;
    
    auto button = dynamic_cast<ui::Button *>(pSender);
    ButtonTag tag = (ButtonTag)button->getTag();
    switch (tag) {
        case ButtonTag::Male:       gender = Gender::Male;      break;
        case ButtonTag::Female:     gender = Gender::Female;    break;
        case ButtonTag::Training:   level  = Level::Training;   break;
        case ButtonTag::Beginner:   level  = Level::Beginner;   break;
        case ButtonTag::Middle:     level  = Level::Middle;     break;
        case ButtonTag::Higher:     level  = Level::Higher;     break;
        case ButtonTag::Delete:     this->showDeleteConfirm();  return;
        case ButtonTag::Save:       this->saveUserInfo();       return;
        case ButtonTag::Back:      this->close();              return;
    }
    
    this->updateGenderButton();
    this->updateLevelButton();
}

void MemberEditLayer::showDeleteConfirm()
{
    std::string text = StringUtils::format("%sのデータを削除します\nよろしいですか？", name.c_str());
    std::function<void()> yesCallback = [this](){this->deleteUserInfo();};  // Yesなら削除
    std::function<void()> noCallback  = [](){};                             // Noなら何もしない
    
    ConfirmInfo *info = new ConfirmInfo(text, yesCallback, noCallback);
    Kyarochon::Event::showConfirmView(info);
}


void MemberEditLayer::deleteUserInfo()
{
    // 削除
    Manager::Member::getInstance()->removeMemberAt(userInfo->id);
    
    this->close();
}

void MemberEditLayer::saveUserInfo()
{
    // 入力情報取得
    name         = std::string(nameEditBox->getText());
    nameFurigana = std::string(nameFuriganaEditBox->getText());
    nickname     = std::string(nicknameEditBox->getText());
    
    countStr     = std::string(countEditBox->getText());
    lastTime     = std::string(lastTimeEditBox->getText());
//    countStr     = countTextField->getString();
//    lastTime     = lastTimeTextField->getString();
    
    int count  = std::all_of(countStr.cbegin(), countStr.cend(), isdigit) ? std::atoi(countStr.c_str()) : -1;
    
    
    // 入力チェック
    std::string errorText = "";
    if (name.empty())
    {
        errorText += "氏名が入力されていません\n";
    }
    if (nameFurigana.empty())
    {
        errorText += "ふりがなが入力されていません\n";
    }
    if (gender == Gender::Unknown)
    {
        errorText += "性別が入力されていません\n";
    }
    if (level == Level::Unknown)
    {
        errorText += "レベルが入力されていません\n";
    }
    if (count < 0)
    {
        errorText += "参加回数は0以上で入力してください\n";
    }
    if (!lastTime.empty() && lastTime != "NULL" && lastTime.length() != 19)
    {
        errorText += "最終参加日は`YYYY/MM/DD hh:mm:ss`\nの形式で入力してください\n";
    }
    
    
    if (!errorText.empty())
    {
        errorText.pop_back();
        Kyarochon::Event::showAlertView(errorText);
        return;
    }
        
        
    // 新規
    if (!userInfo)
    {
        // 新規追加
        Manager::Member::getInstance()->addMember(name, nameFurigana, nickname, gender, "", level, count, 0, lastTime);
    }
    // 編集
    else
    {
        // 更新
        userInfo->name         = name;
        userInfo->nameFurigana = nameFurigana;
        userInfo->nickname     = nickname;
        userInfo->gender       = gender;
        userInfo->level        = level;
        userInfo->count        = std::atoi(countStr.c_str());
        userInfo->lastTime     = lastTime;
        
        // マネージャに通知
        Manager::Member::getInstance()->updateMember(userInfo);
    }
    
    this->close();
}

void MemberEditLayer::close()
{
    Kyarochon::Event::sendCustomEvent(EVENT_UPDATE_MEMBER_LIST);
    this->removeFromParent();
}




