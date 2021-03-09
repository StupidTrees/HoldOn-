#include "UnitView.h"
#include "mString.h"
#include "GamemapScene.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;
UnitView::UnitView() {

}

UnitView::~UnitView() {

}


UnitView* UnitView::create(Checkpoint* u, Player* p,DBHelper* db)
{
	if (u == nullptr) return nullptr;
	UnitView* uv = new UnitView;
	if (uv && uv->init(u,p,db)) {
		uv->autorelease();
		return uv;
	}
	else {
		delete uv;
		uv = nullptr;
		return nullptr;
	}
}

void UnitView::runActionWithChildren(Action* a)
{
	if (this == nullptr || name == nullptr || description == nullptr || cardFrame == nullptr || button == nullptr) return;
	auto a1 = a->clone();
	auto a2 = a->clone();
	auto a3 = a2->clone();
	auto a4 = a3->clone();
	auto a5 = a4->clone();
	runAction(a);
	content->runAction(a);
	name->runAction(a2);
	description->runAction(a3);
	cardFrame->runAction(a4);
	button->runAction(a5);
}


bool UnitView::init(Checkpoint* u, Player* p,DBHelper* db) {
	if (u == nullptr||p==nullptr) {
		return false;
	}
	else {
		this->U = u;
		this->player = p;
		this->dbhelper = db;
		Sprite::init();
	    cardFrame = Sprite::create("enemy_board_bg.png");
		auto testRead = Sprite::create("roles/enemy" + mString::parseInt(u->getId()) + ".png");
		if (testRead != nullptr) content = testRead;
		else content = Sprite::create("roles/enemy0.png");
		setContentSize(Size(UNITVIEW_WIDTH, UNIVIEW_HEIGHT));
		cardFrame->setContentSize(getContentSize());
		cardFrame->setPosition(Point(UNITVIEW_WIDTH / 2, UNIVIEW_HEIGHT / 2));
		button = ui::Button::create("buttons/enter_unit_normal.png", "buttons/enter_unit_pressed.png", "buttons/enter_unit_pressed.png");
		button->setPosition(Point(UNITVIEW_WIDTH / 2, -60));
		content->setContentSize(Size(ROLE_CARD_WIDTH * 51 / 70, ROLE_CARD_HEIGHT * 38 / 70));
		content->setPosition(Vec2(ROLE_CARD_WIDTH / 2+12, ROLE_CARD_HEIGHT*3/4));
		addChild(content);
		addChild(cardFrame);
		addChild(button);
		button->addClickEventListener([this](Ref* r) {
			SimpleAudioEngine::getInstance()->playEffect("audio/enter.wav");
			SimpleAudioEngine::getInstance()->playBackgroundMusic("audio/BattleScene.wav",true);
			auto sc = BattleScene::create(player,dbhelper,U);
			Director::getInstance()->pushScene(sc);
			});
		name = LabelTTF::create(u->getName() + " LV." +mString::parseInt( u->getBossLevel()),"",36 );

		name->setPosition(Point(UNITVIEW_WIDTH / 2, UNIVIEW_HEIGHT * 3 / 7));
		name->setColor(Color3B::BLACK);
	    description = LabelTTF::create(u->getDescription(), "", 24);
		description->setPosition(Point(UNITVIEW_WIDTH / 2, 30));
		description->setColor(Color3B::BLACK);
		description->setDimensions(Size(UNITVIEW_WIDTH - 30, UNIVIEW_HEIGHT/2));
		addChild(name);
		addChild(description);
		//initWithFile("unit"+(mString::parseInt(u->getid()))+".png");
		setAnchorPoint(Vec2(0.5, 0.5));
	}
	return true;
}

Checkpoint* UnitView::getCheckpoint() {
	return U;
}


