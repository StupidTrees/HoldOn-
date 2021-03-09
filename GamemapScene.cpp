#include "GamemapScene.h"
#include "BattleScene.h"
#include "MainScene.h"
#include "mString.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "cocos2d/cocos/ui/UIText.h"
#include "ui/UILayout.h"

using namespace CocosDenshion;
GamemapScene::GamemapScene() {
}

GamemapScene::~GamemapScene() {
}


GamemapScene* GamemapScene::create(DBHelper* dbhelper, Player* p)
{
	GamemapScene* gs = new GamemapScene;
	if (gs && gs->init(dbhelper,p)) {
		gs->autorelease();
		return gs;
	}
	else {
		delete gs;
		gs = nullptr;
		return nullptr;
	}
	return nullptr;
	return nullptr;
}

bool GamemapScene::init(DBHelper* dbhelper, Player* p) {
	this->dbhelper = dbhelper;
	this->player = p;
	if (!Layer::init()) {
		return false;
	}
	auto cache = TextureCache::sharedTextureCache();
	setContentSize(Size(WINDOW_WIDTH, WINDOW_HEIGHT));
	bg = Sprite::createWithTexture(cache->getTextureForKey("gamemap_bg"));
    mask = ui::Layout::create();
	mask->setContentSize(getContentSize());
	mask->setPosition(Point::ZERO);
	mask->setOpacity(0);
	mask->setBackGroundColor(Color3B::BLACK);
	mask->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	addChild(mask);
	unitViewList = new LinkedList<UnitView*>;
	bg->setPosition(Vec2(getContentSize().width / 2, getContentSize().height / 2));
	bg->setContentSize(getContentSize());
	addChild(bg);
	auto bt_back = ui::Button::create("buttons/return_normal.png", "buttons/return_pressed.png", "buttons/return_pressed.png");
	bt_back->setPosition(Point(WINDOW_WIDTH *4/5, WINDOW_HEIGHT *4 / 5));

	bt_back->addClickEventListener([=](Ref* r) {
		SimpleAudioEngine::getInstance()->playEffect("audio/ButtonBack.mp3");
		removeFromParentAndCleanup(true);
		});
	addChild(bt_back);
	return true;
}

void GamemapScene::onEnter() {
	Layer::onEnter();


}

void GamemapScene::onEnterTransitionDidFinish() {
	mask->runAction(FadeTo::create(0.3, 160));
	bg->runAction(FadeIn::create(0.3));

	if (unitViewList->size() <3 ) { //首次进入
		UnitView* uNew1, * uNew2, * uNew3;
		unitViewList->clear();
		uNew1 = UnitView::create(dbhelper->getCheckpointWithId(player->getCp1()), player, dbhelper);
		uNew2 = UnitView::create(dbhelper->getCheckpointWithId(player->getCp2()), player, dbhelper);
		uNew3 = UnitView::create(dbhelper->getCheckpointWithId(player->getCp3()), player, dbhelper);
		if (uNew1== nullptr && uNew2 == nullptr && uNew3 == nullptr) {
			player->setCp1(1);
			player->setCp2(2);
			player->setCp3(3);
			player->saveObjectToDisk();
			uNew1 = UnitView::create(dbhelper->getCheckpointWithId(player->getCp1()), player, dbhelper);
			uNew2 = UnitView::create(dbhelper->getCheckpointWithId(player->getCp2()), player, dbhelper);
			uNew3 = UnitView::create(dbhelper->getCheckpointWithId(player->getCp3()), player, dbhelper);
		}
		if(uNew1!=nullptr)uNew1->setPosition(Vec2(500 - 56, -200));
		if (uNew2!= nullptr)uNew2->setPosition(Vec2(1000 - 56, -200));
		if (uNew3 != nullptr)uNew3->setPosition(Vec2(1500 - 56, -200));
		unitViewList->add(uNew1);
		unitViewList->add(uNew2);
		unitViewList->add(uNew3);
		for (int i = 0; i < 3; i++) {
			auto target = unitViewList->get(i);
			if (target != nullptr) {
				addChild(target);
				target->runAction(Spawn::create(FadeIn::create(0.3),EaseCircleActionOut::create(MoveTo::create(0.3, Vec2((i + 1) * 500 - 56, getContentSize().height / 2 - 30))),NULL));
			}
		}
	}
	else {
		if (unitViewList->get(0) == nullptr && unitViewList->get(1) == nullptr && unitViewList->get(2) == nullptr) {
			player->setCp1(1);
			player->setCp2(2);
			player->setCp3(3);
			player->saveObjectToDisk();
			removeFromParent();
			return;
		}
		if (unitViewList->get(0) != nullptr && unitViewList->get(0)->getCheckpoint()!= nullptr && unitViewList->get(0)->getCheckpoint()->getId() != player->getCp1()) {
			unitViewList->get(0)->runActionWithChildren(FadeOut::create(0.4)) ;
			unitViewList->get(0)->runAction(Sequence::create(FadeOut::create(0.4), CallFuncN::create([=](Node* n) {
				n->removeFromParentAndCleanup(true);
				unitViewList->remove(0);
			auto uNew1 = UnitView::create(dbhelper->getCheckpointWithId(player->getCp1()), player, dbhelper);
			unitViewList->add(0, uNew1);
			if (uNew1 != nullptr) {
				uNew1->setPosition(Vec2(500 - 56, -200));
				addChild(uNew1);
				uNew1->runAction(Spawn::create(FadeIn::create(0.3), EaseCircleActionOut::create(MoveTo::create(0.3, Vec2(500 - 56, getContentSize().height / 2 - 30))), NULL));
			}
				}), NULL));

		}

		if (unitViewList->get(1) != nullptr && unitViewList->get(1)->getCheckpoint()!=nullptr && unitViewList->get(1)->getCheckpoint()->getId() != player->getCp2()) {
			unitViewList->get(1)->runActionWithChildren(FadeOut::create(0.4));
			unitViewList->get(1)->runAction(Sequence::create(FadeOut::create(0.4), CallFuncN::create([=](Node* n) {n->removeFromParentAndCleanup(true);
			unitViewList->remove(1);
			auto uNew1 = UnitView::create(dbhelper->getCheckpointWithId(player->getCp2()), player, dbhelper);
			unitViewList->add(1, uNew1);
			if (uNew1 != nullptr) {
				uNew1->setPosition(Vec2(1000 - 56, -200));
				addChild(uNew1);
				uNew1->runAction(Spawn::create(FadeIn::create(0.3), EaseCircleActionOut::create(MoveTo::create(0.3, Vec2(1000 - 56, getContentSize().height / 2 - 30))), NULL));
			}}), NULL));

		}
		if (unitViewList->get(2) != nullptr && unitViewList->get(2)->getCheckpoint()!= nullptr && unitViewList->get(2)->getCheckpoint()->getId() != player->getCp3()) {
			unitViewList->get(2)->runActionWithChildren(FadeOut::create(0.4));
			unitViewList->get(2)->runAction(Sequence::create(FadeOut::create(0.4), CallFuncN::create([=](Node* n) {n->removeFromParentAndCleanup(true);
			unitViewList->remove(2);
				auto uNew1 = UnitView::create(dbhelper->getCheckpointWithId(player->getCp3()), player, dbhelper);
				unitViewList->add(2, uNew1);
				if (uNew1 != nullptr) {
					uNew1->setPosition(Vec2(1500 - 56, -200));
					addChild(uNew1);
					uNew1->runAction(Spawn::create(FadeIn::create(0.3), EaseCircleActionOut::create(MoveTo::create(0.3, Vec2(1500 - 56, getContentSize().height / 2 - 30))), NULL));
				}
				}), NULL));

		}

	}
}

