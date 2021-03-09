#include "CountGameDialog.h"
#include <math.h>
#include "uidimens.h"
#include "Player.h"
#include "MainScene.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
 CountGameDialog* CountGameDialog::create(bool win, Referee* r,Player* player,Checkpoint* u,DBHelper* dbhelper)
{
	CountGameDialog* bs = new CountGameDialog(win,r,player,u,dbhelper);
	if (bs && bs->init()) {
		bs->autorelease();
		return bs;
	}
	else {
		delete bs;
		bs = nullptr;
		return nullptr;
	}
}

CountGameDialog::CountGameDialog(bool win, Referee* r,Player* p,Checkpoint* u,DBHelper* dbhelper)
{
	this->dbhelper = dbhelper;
	this->win = win;
	this->player = p;
	this->referee = r;
	this->checkpoint = u;
}


bool CountGameDialog::init()
{
	setContentSize(Size(WINDOW_WIDTH,WINDOW_HEIGHT));
	bg = ui::Layout::create();
	bg->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	bg->setBackGroundColor(Color3B::BLACK);
	bg->setSwallowTouches(true);
	bg->setOpacity(0);
	bg->setContentSize(getContentSize());
	auto bgTexture = TextureCache::getInstance()->getTextureForKey(win?"win_bg":"lose_bg");
	if (win) {
		SimpleAudioEngine::getInstance()->stopBackgroundMusic("audio/BattleScene.wav");
		SimpleAudioEngine::getInstance()->playEffect("audio/BattleWin.mp3");
	}
	else {
		SimpleAudioEngine::getInstance()->stopBackgroundMusic("audio/BattleScene.wav");
		SimpleAudioEngine::getInstance()->playEffect("audio/fail.wav");
	}
	scrollBG = Sprite::createWithTexture(bgTexture);
	scrollBG->setPosition(Point(getContentSize().width / 2, getContentSize().height / 2));
	scrollBG->setOpacity(0);
	bt = ui::Button::create("bt_backtomain_normal.png", "bt_backtomain_pressed.png", "bt_backtomain_pressed.png");
	bt->setOpacity(0);
	bt->addClickEventListener([](Ref* r) {
		SimpleAudioEngine::getInstance()->playEffect("audio/ButtonBack.mp3");
		Director::getInstance()->popScene();
	});
	bt->setPosition(Point(getContentSize().width / 2, 80));

	addChild(bg);
	addChild(scrollBG);
	addChild(bt);
	if (win) {
		bool levUp = player->gainExp(checkpoint->getRewardExp());
		player->gainCoins(checkpoint->getRewardCoins());
		Card* rewardCard = nullptr;
		if (checkpoint->getRewardCards()->size() > 0) {
			int r = RandomHelper::random_int(0, checkpoint->getRewardCards()->size()-1);
			int id = checkpoint->getRewardCards()->get(r);
			rewardCard = dbhelper->getCardById(id);
		}

		int tool = checkpoint->getId();
		int count;
		int cp1 = player->getCp1();
		int cp2 = player->getCp2();
		int cp3 = player->getCp3();
		if (cp1 > cp2) {
			if (cp1 > cp3) {
				count = cp1;
			}
			else {
				count = cp3;
			}
		}
		else {
			if (cp2 > cp3) {
				count = cp2;
			}
			else {
				count = cp3;
			}
		}
		if (tool == cp1) player->setCp1(count + 1);
		else if (tool == cp2) player->setCp2(count + 1);
		else player->setCp3(count + 1);
		player->saveObjectToDisk();
		mString hint = "获得" + mString::parseInt(checkpoint->getRewardExp())+ "点经验 " + mString::parseInt(player->getCurExp()) + "/" + mString::parseInt(player->getTotExp());
		mString extra = levUp ? "\n角色提升至" + mString::parseInt(player->getLevel()) + "级" : "";
		mString coins = "\n获得金币 x" + mString::parseInt(checkpoint->getRewardCoins());
		mString extra_cards = rewardCard == nullptr ? "" : "\n随机卡牌奖励【" + rewardCard->getName() + "】";
		text = LabelTTF::create(hint + extra+coins+extra_cards, "arial", 42);
		text->setOpacity(0);
		text->setPosition(scrollBG->getContentSize().width / 2, 500);
		scrollBG->addChild(text);
		 if (rewardCard != nullptr) {
			 auto bt_takethat = ui::Button::create("buttons/illtakethat_normal.png", "buttons/illtakethat_pressed.png", "buttons/illtakethat_pressed.png");
			 bt_takethat->addClickEventListener([=](Ref* r) {
				 SimpleAudioEngine::getInstance()->playEffect("audio/ButtonBack.mp3");
				 player->getPile()->add(rewardCard->getId());
				 player->saveObjectToDisk();
				 bt_takethat->removeFromParent();
				 });
			 bt_takethat->setPosition(Point(getContentSize().width / 2, 180));
			 addChild(bt_takethat);
		 }
	}



	return true;
}

void CountGameDialog::fadeIn()
{
	bg->runAction(FadeTo::create(0.2,180));
	scrollBG->runAction(Sequence::create(DelayTime::create(0.2), FadeIn::create(0.2), NULL));
	bt->runAction(FadeIn::create(0.2));
	if (win) text->runAction(Sequence::create(DelayTime::create(0.3), FadeIn::create(0.1), NULL));
}

