#include "BattleScene.h"
#include"MainScene.h"
#include "CountGameDialog.h"
#include "CardInfoWindow.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "Checkpoint.h"
#include <thread>

#pragma execution_character_set("utf-8")

using namespace CocosDenshion;
BattleScene::BattleScene(Player* player,DBHelper* dbh,Checkpoint* u)
{
	dbhelper = dbh;
	this->checkpoint = u;
	this->player = player;
}


BattleScene::~BattleScene()
{
}


BattleScene* BattleScene::create(Player* player,DBHelper* dbh,Checkpoint* u)
{
	BattleScene* bs = new BattleScene(player,dbh,u);
	if (bs && bs->init()) {
		bs->autorelease();
		return bs;
	}
	else {
		delete bs;
		bs = nullptr;
		return nullptr;
	}
	return nullptr;
}

bool BattleScene::init()
{
	if (!Scene::init()) return false;
	initDesktopInfos();
	//loadResources();
	mHandCardList = new LinkedList<CardView*>;
	eHandCardList = new LinkedList<CardView*>;
	effectEList = new LinkedList<EffectView*>;
	effectMList = new LinkedList<EffectView*>;
	auto cache = TextureCache::getInstance();
	auto bg = Sprite::createWithTexture(cache->getTextureForKey("battle_bg"));
	bg->setPosition(Point(getContentSize().width / 2, getContentSize().height / 2));
	bg->setContentSize(getContentSize());
	auto itemy = MenuItemFont::create("退出战斗", CC_CALLBACK_1(BattleScene::backToMainScene, this));
	auto itemz = MenuItemFont::create("printf", CC_CALLBACK_1(BattleScene::testOutput, this));
	itemy->setFontSizeObj(38);
	itemz->setFontSizeObj(38);
	auto menu = Menu::create(itemy, itemz,NULL);
	menu->alignItemsVertically();
	menu->setPosition(Point(140, getContentSize().height - 100));
	addChild(bg);
	this->addChild(menu);
	auto lp1 = LinearLayoutParameter::create();
	lp1->setMargin(Margin(24, 5, 5, 5));
	enemyRoleView = new RoleView(desktop->getP2(),  ROLE_RED,checkpoint->getId());
	playerRoleView = new RoleView(desktop->getP1(),  ROLE_BLUE,1);
	enemyRoleView->setPosition(Vec2(getContentSize().width/2-ROLE_CARD_WIDTH/2, getContentSize().height - ROLE_CARD_HEIGHT));
	playerRoleView->setPosition(Vec2(0, 16));
	effectMLayer = Layer::create();
	effectELayer = Layer::create();
	effectMLayer->setContentSize(Size(EFFECTS_WIDTH, EFFECTS_HEIGHT));
	effectELayer->setContentSize(Size(EFFECTS_WIDTH, EFFECTS_HEIGHT));
	effectMLayer->setPosition(Vec2(30,ROLE_CARD_HEIGHT+48));
	effectELayer->setPosition(Vec2(enemyRoleView->getPositionX()-EFFECTS_WIDTH+ROLE_CARD_WIDTH/3*2,enemyRoleView->getPositionY()-EFFECTS_HEIGHT*2/3 ));

	mHandCardLayer = Layer::create();
	mHandCardLayer->setContentSize(Size(getContentSize().width - MDASHBOARD_WIDTH * 2 - 100, MHANDPILE_HEIGHT));
	mHandCardLayer->setPosition(Point(ROLE_CARD_WIDTH + 56, 0));
	eHandCardLayer = Layer::create();
	eHandCardLayer->setContentSize(Size(EHANDPILE_WIDTH, EHANDPILE_HEIGHT));
	eHandCardLayer->setPosition(Point(enemyRoleView->getPositionX()-EHANDPILE_WIDTH, getContentSize().height - ROLE_CARD_HEIGHT));
	auto handCardBG = Sprite::createWithTexture(cache->getTextureForKey("handp_bg"));
	handCardBG->setContentSize(Size(getContentSize().width, MHANDPILE_HEIGHT));
	handCardBG->setPosition(Point(getContentSize().width / 2, MHANDPILE_HEIGHT / 2));
	auto listener = EventListenerMouse::create();
	listener->onMouseMove = CC_CALLBACK_1(BattleScene::onMouseMovedOnHandP, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,mHandCardLayer);

	auto listenerEE = EventListenerMouse::create();
	listenerEE->onMouseMove = CC_CALLBACK_1(BattleScene::onMouseMovedOnEEQ,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerEE, effectELayer);


	auto listenerME = EventListenerMouse::create();
	listenerME->onMouseMove = CC_CALLBACK_1(BattleScene::onMouseMovedOnMEQ, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerME, effectMLayer);

	playcardFinishButton = Button::create("bt_playcard_finish_normal.png", "bt_playcard_finish_clicked.png", "bt_playcard_finish_disabled.png");
	playcardFinishButton->setContentSize(Size(240, 80));
	playcardFinishButton->ignoreContentAdaptWithSize(false);
	playcardFinishButton->setPosition(Vec2(getContentSize().width - 180, MHANDPILE_HEIGHT / 3));
	playcardFinishButton->addClickEventListener([this](Ref* r) {
		SimpleAudioEngine::getInstance()->playEffect("audio/ButtonBack.mp3");
		referee->roundDiscardCard();
		});
	auto roundBG = Sprite::create("round.png");
	roundBG->setContentSize(Size(592, 112));
	roundBG->setPosition(Point(getContentSize().width -296, getContentSize().height-56));
	auto hintBG = Sprite::create("hint_bg.png");
	hintBG->setPosition(Point(getContentSize().width, MHANDPILE_HEIGHT-110));
	hintText = Text::create();
	hintText->setFontSize(36);
	hintText->setText("loading....");
	hintText->setColor(Color3B::BLACK);
	hintText->setPosition(Point(hintBG->getContentSize().width / 4, hintBG->getContentSize().height / 2+6));
	hintBG->addChild(hintText);
	roundText = Text::create();
	roundText->setFontSize(36);
	roundText->setContentSize(Size(100, 48));
	roundText->setPosition(Point(226,90));
	roundBG->addChild(roundText);
	EventListenerTouchOneByOne* listener2 = EventListenerTouchOneByOne::create();
	listener2->onTouchBegan = CC_CALLBACK_2(BattleScene::onTouchBegan, this);
	listener2->onTouchMoved = CC_CALLBACK_2(BattleScene::onTouchMoved, this);
	listener2->onTouchEnded = CC_CALLBACK_2(BattleScene::onTouchEnd, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener2, this);
	addChild(hintBG);
	addChild(roundBG);
	addChild(handCardBG);
	addChild(effectELayer);
	addChild(effectMLayer);
	addChild(enemyRoleView);
	addChild(playcardFinishButton);
	addChild(playerRoleView);
	addChild(eHandCardLayer);
	addChild(mHandCardLayer);

	return true;
}

/*拖拽处理1：点击开始*/
bool BattleScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event) {
	bool hasTarget = false;
	int focusedIndex;
	LinkedList<CardView*>::Iterator itr(mHandCardList);
	int i = 0;
	/*循环找到被点击的卡牌，进行初始参数保存和状态设置*/
	while(itr.hasNext()) {
		CardView* target = itr.next();
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);
		if (!hasTarget && rect.containsPoint(locationInNode))
		{

			hasTarget = true;
			hasFocus = true;
			target->setFocused(true);
			focusedCard = target;
			focusedBeginPos = target->getPosition();
			focusedBeginZOrder = target->getZOrder();
			focusedLastPos = target->getPosition();
			touchLastPos = touch->getLocation();
			focusedBeginIndex = i;
			focusedIndex = i;
			if (focusedCard->getPositionZ() == 0 && focusedCard->isUseable() && referee->getSceneState() == ALLOW_USECARD) {
				auto moveTo = MoveTo::create(0.1, Vec3(target->getPositionX(), CARD_HEIGHT / 2 + 60, 100));
				moveTo->setTag(ACTIONS_MOVE);
				target->runAction(moveTo);
				target->setLocalZOrder(mHandCardList->size() - 1);
			}
		}
		else {
			target->setFocused(false);
		}
		i++;
	}
	if (hasFocus) {
		notifyMHandCardFloated(focusedIndex);
	}
	return true;
	//}
	//return false;

}

/*拖拽处理2：鼠标拖动*/
void BattleScene::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unused_event) {
	if (!hasFocus) return;
	EventMouse* e = (EventMouse*)unused_event;
	if (focusedCard != nullptr) { //让被选中的卡牌跟随鼠标作增量移动
		Vec2 newPos = touch->getLocation();
		float deltaX = newPos.x - touchLastPos.x;
		float deltaY = newPos.y - touchLastPos.y;
		Vect focusedNewPos = Vec2(focusedLastPos.x + deltaX, focusedLastPos.y + deltaY);
		focusedCard->setPosition(focusedNewPos);
		focusedLastPos = focusedNewPos;
		touchLastPos = newPos;
		/*移动到一定高度时，执行酷炫的待释放动画*/
		if (referee->getSceneState() == ALLOW_USECARD && focusedCard->getPositionY() > MHANDPILE_HEIGHT + CARD_HEIGHT / 3) {
			if (focusedCard->isUseable())focusedCard->setWillRelease(true, true,false);
		}
		else {
			if (focusedCard->isUseable())focusedCard->setWillRelease(false, true,false);
		}
	}
}

/*拖拽处理3：鼠标放开*/
void BattleScene::onTouchEnd(cocos2d::Touch* touch, cocos2d::Event* unused_event) {
	EventMouse* e = (EventMouse*)unused_event;
	hasFocus = false;
	if (focusedCard != nullptr) {
		if (focusedBeginPos.y > CARD_HEIGHT / 2) focusedBeginPos.y = CARD_HEIGHT / 2;
		/*若为出牌阶段，且达到一定高度，则进行出牌处理*/
		if (referee->getSceneState() == ALLOW_USECARD && focusedCard->getPositionY() > MHANDPILE_HEIGHT + CARD_HEIGHT / 3 && focusedCard->isUseable()) {
			//desktop->getP1()->getHandPile()->remove(focusedCard->getCard(), focusedCard->getCard());
			auto delay = DelayTime::create(0.22);
			auto fuc = CallFuncN::create([=](Node* n) {
				auto cv = static_cast<CardView*>(n);
				cv->release();
				
				referee->useCard(desktop->getP1(), desktop->getP2(), cv->getCard());
				});
			focusedCard->runAction(Sequence::create(delay, fuc, NULL));


		}
		/*若为弃牌阶段，且达到一定高度，则进行弃牌处理*/
		else if (referee->getSceneState() == DISCARD_CARDS) {
			referee->discardCard(desktop->getP1(), focusedCard->getCard());
			focusedCard->discard();
		}
		/*否则，回弹回原位*/
		else {
			focusedCard->setFocused(false);
			mHandCardList->add(focusedBeginIndex, focusedCard);
			int padding;
			focusedCard->setZOrder(focusedBeginZOrder);
			if (CARD_WIDTH * mHandCardList->size() < mHandCardLayer->getContentSize().width - 64) padding = MHANDPILE_PADDING;
			else padding = (mHandCardLayer->getContentSize().width - CARD_WIDTH * mHandCardList->size()) / (mHandCardList->size() - 1);
			for (int i = 0; i < mHandCardList->size(); i++) {
				CardView* target = mHandCardList->get(i);
				if (target != focusedCard) {
					target->stopAllActionsByTag(ACTIONS_MOVE);
					target->stopAllActionsByTag(ACTIONS_SCALE);
				}
				target->setZOrder(i);
				focusedCard->setWillRelease(false, referee->getSceneState() == ALLOW_USECARD && focusedCard->isUseable(),false);
				auto resize = ResizeTo::create(0.08, Size(CARD_WIDTH, CARD_HEIGHT));
				resize->setTag(ACTIONS_SCALE);
				auto move = MoveTo::create(0.1, Point(CARD_WIDTH / 2 + (CARD_WIDTH + padding) * i, CARD_HEIGHT / 2));
				move->setTag(ACTIONS_MOVE);
				target->runAction(Spawn::create(resize, move, NULL));
				//	log("x_pos:%d", CARD_WIDTH / 2 + (CARD_WIDTH + padding) * i);
			}


		}

	}
	focusedCard = nullptr;
}


/*当鼠标在卡牌、效果区移动时的监听*/
void BattleScene::onMouseMovedOnHandP(Event* event) {
	EventMouse* e = (EventMouse*)event;
	if (hasFocus)return;
	if (referee->getGameState() == COUNTING) return;
	LinkedList<CardView*>::Iterator itrMHC(mHandCardList);
	bool hasHighlight = false;
	/*遍历手牌，找到被悬停的那张*/
	while(itrMHC.hasNext()) {
		CardView* target = itrMHC.next();
		Point locationInNode = target->convertToNodeSpace(e->getLocationInView());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);
		if (rect.containsPoint(locationInNode))
		{
			auto ciw = CardInfoWindow::create(target->getCard(), 400, 240);
			ciw->setPosition(Point(20, 480));
			removeChildByName("card_info_window");
			addChild(ciw, getChildrenCount() - 1, "card_info_window");
			highlightedCard = target;
			hasHighlight = true;
		}
	}

	if (!hasHighlight) {
		highlightedCard = nullptr;
		removeChildByName("card_info_window");
	}
	itrMHC.reset();
	int i = 0;
	/*被悬停：执行上浮动画，否则回归原位*/
	while (itrMHC.hasNext()) {
		CardView* target = itrMHC.next();
		if (target == highlightedCard) {
			if (!target->isFocused() && referee->getSceneState() == ALLOW_USECARD) {
				if (target->getNumberOfRunningActions() > 0 || target->getPositionY() > CARD_HEIGHT / 2) {
					i++;
					continue;
				}
				if (target->isUseable()) {
					auto moveTo = MoveTo::create(0.15, Vec3(target->getPositionX(), CARD_HEIGHT / 2 + 60, 0));
					moveTo->setTag(ACTIONS_MOVE);
					target->stopAllActions();
					target->runAction(moveTo);
				}
			}
		}
		else if (!target->isFocused()) {
			if (target->getNumberOfRunningActions() > 0) {
				i++;
				continue;
			}
			auto resize = ResizeTo::create(0.08, Size(CARD_WIDTH, CARD_HEIGHT));
			auto move = MoveTo::create(0.05, Vec3(target->getPositionX(), CARD_HEIGHT / 2, 0));
			resize->setTag(ACTIONS_SCALE);
			move->setTag(ACTIONS_MOVE);
			target->runAction(Spawn::create(move, resize, NULL));
		 	target->setLocalZOrder(i);
		}
		i++;
	}
}

void BattleScene::onMouseMovedOnEEQ(Event* event) {
	EventMouse* e = (EventMouse*)event;

	for (int i = 0; i < effectEList->size(); i++) {
		auto target = effectEList->get(i);
		Point locationInNode = target->convertToNodeSpace(e->getLocationInView());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);
		if (rect.containsPoint(locationInNode))
		{
			auto ciw = CardInfoWindow::create(target->getEffect(), 400, 240);
			ciw->setPosition(Point(20, 480));
			removeChildByName("card_info_window");
			addChild(ciw, getChildrenCount() - 1, "card_info_window"); //加入窗口
			return;
		}
	}
}

void BattleScene::onMouseMovedOnMEQ(Event* event) {
	EventMouse* e = (EventMouse*)event;

	for (int i = 0; i < effectMList->size(); i++) {
		auto target = effectMList->get(i);
		Point locationInNode = target->convertToNodeSpace(e->getLocationInView());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);
		if (rect.containsPoint(locationInNode))
		{
			auto ciw = CardInfoWindow::create(target->getEffect(), 400, 240);
			ciw->setPosition(Point(20, 480));
			removeChildByName("card_info_window");
			addChild(ciw, getChildrenCount() - 1, "card_info_window");
			return;
		}
	}
}

/*返回主菜单*/
void BattleScene::backToMainScene(Ref* sender) {
//	auto scene = MainScene::createScene();
	Director::getInstance()->popScene();
}

void BattleScene::testOutput(Ref* sender) {

	//	enemyRoleView->getRecover(10);
	cocos2d::log("e:drcdp====");
	for (int i = 0; i < desktop->getP2()->getDrawPile()->size(); i++) {
		Card* c = desktop->getP2()->getDrawPile()->get(i);
		string s = c->getName();
		cocos2d::log("[%d]%s", c->getId(), s.c_str());
	}
	cocos2d::log("e:handp====");
	for (int i = 0; i < desktop->getP2()->getHandPile()->size(); i++) {
		Card* c = desktop->getP2()->getHandPile()->get(i);
		string s = c->getName();
		cocos2d::log("[%d]%s", c->getId(), s.c_str());
	}
	cocos2d::log("e:dscdp====");
	for (int i = 0; i < desktop->getP2()->getDiscardPile()->size(); i++) {
		Card* c = desktop->getP2()->getDiscardPile()->get(i);
		string s = c->getName();
		cocos2d::log("[%d]%s", c->getId(), s.c_str());
	}

	cocos2d::log("e:effect====");
	for (int i = 0; i < desktop->getP2()->getEffectList()->size(); i++) {
		auto e = desktop->getP2()->getEffectList()->get(i);
		string s = e->getName();
		cocos2d::log("[%d]%s", e->getId(), s.c_str());
	}


	cocos2d::log("m:drcdp====");
	for (int i = 0; i < desktop->getP1()->getDrawPile()->size(); i++) {
		Card* c = desktop->getP1()->getDrawPile()->get(i);
		string s = c->getName();
		cocos2d::log("[%d]%s", c->getId(), s.c_str());
	}
	cocos2d::log("m:handp====");
	for (int i = 0; i < desktop->getP1()->getHandPile()->size(); i++) {
		Card* c = desktop->getP1()->getHandPile()->get(i);
		string s = c->getName();
		cocos2d::log("[%d]%s", c->getId(), s.c_str());
	}
	cocos2d::log("m:dscdp====");
	for (int i = 0; i < desktop->getP1()->getDiscardPile()->size(); i++) {
		Card* c = desktop->getP1()->getDiscardPile()->get(i);
		string s = c->getName();
		cocos2d::log("[%d]%s", c->getId(), s.c_str());
	}
	cocos2d::log("m:effect====");
	for (int i = 0; i < desktop->getP1()->getEffectList()->size(); i++) {
		auto e = desktop->getP1()->getEffectList()->get(i);
		string s = e->getName();
		cocos2d::log("[%d]%s", e->getId(), s.c_str());
	}
}

void BattleScene::onEnter()
{
	auto cache = TextureCache::getInstance();
	Scene::onEnter();
	refreshBoardInfo();
	refreshHandpState();

	auto mask = ui::Layout::create();
	mask->setContentSize(getContentSize());
	mask->setPosition(Point(0, 0));
	mask->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	mask->setBackGroundColor(Color3B::BLACK);
	mask->setOpacity(0);
	addChild(mask);
	auto text = Sprite::createWithTexture(cache->getTextureForKey("battle_start_text"));
	text->setPosition(Point(getContentSize().width / 2, -300));
	addChild(text);
	mask->runAction(Sequence::create(DelayTime::create(0.5),FadeTo::create(0.4,180), DelayTime::create(0.6), FadeOut::create(0.3), CallFuncN::create([=](Node* n) {
		n->removeFromParentAndCleanup(true);
		}),NULL));
	auto easeIn = EaseCircleActionOut::create(MoveTo::create(0.4, Vec3(getContentSize().width / 2, getContentSize().height / 2, 0)));
	auto easeOut = EaseCircleActionIn::create(MoveTo::create(0.4, Vec3(getContentSize().width / 2, getContentSize().height+300, 0)));
	text->runAction(Sequence::create(DelayTime::create(0.5), easeIn, DelayTime::create(0.5),easeOut, CallFuncN::create([=](Node* n) {
		n->removeFromParentAndCleanup(true);
		referee->start();
		}), NULL));
	refreshBoardInfo();
	refreshHandpState();
	notifyEffectChanges();
}

void BattleScene::setHint(mString hint) {
	hintText->setText(hint);
}
void BattleScene::refreshHandpState() {
	if (referee->getSceneState() == ALLOW_USECARD) playcardFinishButton->setEnabled(true);
	else playcardFinishButton->setEnabled(false);

	for (int i = 0; i < mHandCardList->size(); i++) {
		CardView* cv = mHandCardList->get(i);
		if (referee->getSceneState() == CARD_DISABLED) {
			cv->setUseable(false);
			if (cv->getPositionY() > CARD_HEIGHT / 2) {
				auto mov = MoveTo::create(0.1, Point(cv->getPositionX(), CARD_HEIGHT / 2));
				mov->setTag(ACTIONS_MOVE);
				cv->runAction(mov);
			}
		}
		else if (referee->getSceneState() == DISCARD_CARDS) {
			cv->setUseable(true);
			//cv->setOpacity(160);
			if (cv->getPositionY() > CARD_HEIGHT / 2) {
				auto mov = MoveTo::create(0.1, Point(cv->getPositionX(), CARD_HEIGHT / 2));
				mov->setTag(ACTIONS_MOVE);
				cv->runAction(mov);
			}
		}
		else {
			if (referee->canIUseThisCard(cv->getCard(), desktop->getP1(), desktop->getP2())) {
				cv->setUseable(true);
			}
			else {
				cv->setUseable(false);
			}

		}
	}
	//moveAllHandCardsToSuitablePlace();
}

void BattleScene::refreshBoardInfo()
{
	//log("dashboard:%d,%d", desktop->getP1()->getHP(), desktop->getP2()->getHP());
	playerRoleView->refreshDashBoard();
	enemyRoleView->refreshDashBoard();
	roundText->setText("第" + mString::parseInt(referee->getRound())+"回合");
}

void BattleScene::initDesktopInfos()
{
	desktop = DeskTop::create();
	referee = new Referee(this, desktop, desktop->getP1(), desktop->getP2());
	enemyAI = new EnemyAI(this, referee, desktop);
	referee->setAI(enemyAI);
	referee->getReady();
	desktop->getP2()->setMaxHP(checkpoint->getMaxHP());
	desktop->getP2()->setHP(checkpoint->getMaxHP());
	desktop->getP2()->setLevel(checkpoint->getBossLevel());
	desktop->getP2()->setMaxMANA1(checkpoint->getMaxMANA1()+1);
	desktop->getP2()->setMaxMANA2(checkpoint->getMaxMANA2()+1);
	desktop->getP2()->setMANA1(checkpoint->getMaxMANA1()+1);
	desktop->getP2()->setMANA2(checkpoint->getMaxMANA2()+1);
	desktop->getP2()->setName(checkpoint->getName());
	desktop->getP2()->setDescription(checkpoint->getDescription());
	desktop->getP1()->setName(player->getName());
	desktop->getP1()->setLevel(player->getLevel());
	desktop->getP1()->setMaxHP(15+player->getLevel());
	desktop->getP1()->setHP(15+player->getLevel());
	desktop->getP1()->setMaxMANA1(player->getLevel()+1);
	desktop->getP1()->setMaxMANA2(player->getLevel()+1);
	desktop->getP1()->setMANA1(player->getLevel()+1);
	desktop->getP1()->setMANA2(player->getLevel()+1);
	desktop->loadCards(dbhelper, player->getPile(), checkpoint->getPile());
	desktop->loadEffects(dbhelper, new LinkedList<int>, checkpoint->getEffect());

}

/*某张卡牌被点击时，认为其悬浮于手牌区之上*/
void BattleScene::notifyMHandCardFloated(int pos)
{
	mHandCardList->remove(pos);
	moveAllMHandCardsToSuitablePlace();
}

/*执行手牌区改变动画*/
void BattleScene::notifyMHandCardChanged() {
	auto toInsert = new LinkedList<CardView*>;
	auto toRemove = new LinkedList<CardView*>;
	auto temp = new LinkedList<CardView*>;
	auto tempCards = new LinkedList<Card*>;
	temp->transportAll(mHandCardList);
	LinkedList<CardView*>::Iterator itrT(temp);
	while(itrT.hasNext()) tempCards->add(itrT.next()->getCard());

	LinkedList<Card*>::Iterator itrHP(desktop->getP1()->getHandPile());
	while(itrHP.hasNext()) {
		Card* c = itrHP.next();
		int targetIndex = tempCards->indexOf(c);
		if (targetIndex == -1) {
			CardView* cvNew = CardView::create(c, false,false);
			cvNew->setContentSize(Size(0, 0));
			cvNew->setPosition(getContentSize().width / 2, getContentSize().height / 2);
			toInsert->add(cvNew);
			mHandCardList->add(cvNew);
		}
		else {
			CardView* existed = temp->remove(targetIndex);
			tempCards->remove(targetIndex);
			mHandCardList->add(existed);
		}

	}
	toRemove->transportAll(temp);

	/*要移除的卡牌，执行移除动画并释放内存*/
	LinkedList<CardView*>::Iterator itrTR(toRemove);
	while(itrTR.hasNext()) {
		CardView* target = itrTR.next();
		if (target == nullptr) continue;
		target->stopAllActionsByTag(ACTIONS_MOVE);
		target->stopAllActionsByTag(ACTIONS_SCALE);
		auto move = MoveTo::create(0.15, Point(target->getPositionX(), target->getPositionY() + 320));
		auto fade = FadeOut::create(0.15);
		auto fade2 = FadeOut::create(0.15);
		CallFuncN* func = CallFuncN::create([](Node* target) {
			CardView* cv = static_cast<CardView*>(target);
			cv->removeFromParentAndCleanup(true);
			});
		target->runAction(fade);
		target->topTexture->runAction(Sequence::create(DelayTime::create(0.15), fade2, NULL));
		target->runAction(Sequence::create(move, func, NULL));
	}

	int padding;
	if (CARD_WIDTH * mHandCardList->size() < mHandCardLayer->getContentSize().width - 64) padding = MHANDPILE_PADDING;
	else padding = (mHandCardLayer->getContentSize().width - CARD_WIDTH * mHandCardList->size()) / (mHandCardList->size() - 1);

	/*最后，一次循环，所有卡牌移动到合适的位置*/
	LinkedList<CardView*>::Iterator itrHCL(mHandCardList);
	int i = 0;
	while(itrHCL.hasNext()) {
		CardView* target = itrHCL.next();
		target->stopAllActionsByTag(ACTIONS_MOVE);
		target->stopAllActionsByTag(ACTIONS_SCALE);
		if (toInsert->contains(target) ) { //若为新增卡牌，执行渐变放大动画
			mHandCardLayer->addChild(target, mHandCardLayer->getChildrenCount() - 1, "card");
			auto flipF = CallFuncN::create([](Node* n) {
				auto cv = static_cast<CardView*>(n);
				cv->flipTo(true);
				});
			auto resize = ResizeTo::create(0.2, Size(CARD_WIDTH, CARD_HEIGHT));
			auto resize2 = ResizeTo::create(0.2, Size(CARD_WIDTH, CARD_HEIGHT));
			target->topTexture->runAction(resize2);
			resize->setTag(ACTIONS_SCALE);
			target->runAction(Sequence::create(resize, flipF, NULL));
		}
		target->setZOrder(i);
		auto moveTo = MoveTo::create(0.2, Point(CARD_WIDTH / 2 + (CARD_WIDTH + padding) * i, CARD_HEIGHT / 2));
		moveTo->setTag(ACTIONS_MOVE);
		target->runAction(moveTo);
		i++;
		//log("x_pos:%d", CARD_WIDTH / 2 + (CARD_WIDTH + padding) * i);
	}

}

void BattleScene::notifyEHandCardChanged()
{
	/*和M同理*/
	auto toInsert = new LinkedList<CardView*>;
	auto toRemove = new LinkedList<CardView*>;
	auto temp = new LinkedList<CardView*>;
	auto tempCards = new LinkedList<Card*>;
	temp->transportAll(eHandCardList);

	LinkedList<CardView*>::Iterator itrT(temp);
	while (itrT.hasNext()) tempCards->add(itrT.next()->getCard());

	LinkedList<Card*>::Iterator itrHP(desktop->getP2()->getHandPile());
	while (itrHP.hasNext()){
		Card* c = itrHP.next();
		int targetIndex = tempCards->indexOf(c);
		if (targetIndex == -1) {
			CardView* cvNew = CardView::create(c, false,false);
			cvNew->setContentSize(Size(0, 0));
			cvNew->setPosition(getContentSize().width / 2 - eHandCardLayer->getPositionX(), getContentSize().height / 2 - eHandCardLayer->getPositionY());
			toInsert->add(cvNew);
			eHandCardList->add(cvNew);
		}
		else {
			CardView* existed = temp->remove(targetIndex);
			tempCards->remove(targetIndex);
			eHandCardList->add(existed);
		}

	}
	toRemove->transportAll(temp);

	LinkedList<CardView*>::Iterator itrTR(toRemove);
	while (itrTR.hasNext()) {
		CardView* target = itrTR.next();
		if (target == nullptr) continue;
		target->stopAllActionsByTag(ACTIONS_MOVE);
		target->stopAllActionsByTag(ACTIONS_SCALE);
		auto move = MoveTo::create(0.15, Point(target->getPositionX(), target->getPositionY() - 320));
		auto fade = FadeOut::create(0.15);
		auto fade2 = FadeOut::create(0.15);
		CallFuncN* func = CallFuncN::create([](Node* target) {
			CardView* cv = static_cast<CardView*>(target);
			cv->removeFromParentAndCleanup(true);
			});
		target->runAction(fade);
		target->topTexture->runAction(Sequence::create(DelayTime::create(0.15), fade2, NULL));
		target->runAction(Sequence::create(move, func, NULL));
	}

	int padding;
	if (CARD_WIDTH * eHandCardList->size() < eHandCardLayer->getContentSize().width) padding = EHANDPILE_PADDING;
	else padding = (eHandCardLayer->getContentSize().width - CARD_WIDTH * eHandCardList->size()) / (eHandCardList->size() - 1);

	LinkedList<CardView*>::Iterator itrHCL(eHandCardList);
	int i = 0;
	while(itrHCL.hasNext()) {
		CardView* target = itrHCL.next();
		target->stopAllActionsByTag(ACTIONS_MOVE);
		target->stopAllActionsByTag(ACTIONS_SCALE);
		if (toInsert->contains(target)) {
			eHandCardLayer->addChild(target, eHandCardLayer->getChildrenCount() - 1, "card");
			auto resize = ResizeTo::create(0.2, Size(CARD_WIDTH, CARD_HEIGHT));
			resize->setTag(ACTIONS_SCALE);
			target->runAction(resize);
			auto resize2 = ResizeTo::create(0.2, Size(CARD_WIDTH, CARD_HEIGHT));
			target->topTexture->runAction(resize2);

		}
		target->setZOrder(eHandCardList->size() - i);
		auto moveTo = MoveTo::create(0.2, Point(EHANDPILE_WIDTH - (CARD_WIDTH / 2 + (CARD_WIDTH + padding) * i), CARD_HEIGHT / 2));
		moveTo->setTag(ACTIONS_MOVE);
		target->runAction(moveTo);
		i++;
		//log("x_pos:%d", CARD_WIDTH / 2 + (CARD_WIDTH + padding) * i);
	}

}

void BattleScene::moveAllMHandCardsToSuitablePlace() {

	int padding;
	if (CARD_WIDTH * mHandCardList->size() < mHandCardLayer->getContentSize().width - 64) padding = MHANDPILE_PADDING;
	else padding = (mHandCardLayer->getContentSize().width - CARD_WIDTH * mHandCardList->size()) / (mHandCardList->size() - 1);
	for (int i = 0; i < mHandCardList->size(); i++) {
		CardView* target = mHandCardList->get(i);
		target->stopAllActionsByTag(ACTIONS_MOVE);
		target->stopAllActionsByTag(ACTIONS_SCALE);
		target->setZOrder(i);
		auto moveTo = MoveTo::create(0.2, Point(CARD_WIDTH / 2 + (CARD_WIDTH + padding) * i, CARD_HEIGHT / 2));
		moveTo->setTag(ACTIONS_MOVE);
		target->runAction(moveTo);
		//log("x_pos:%d", CARD_WIDTH / 2 + (CARD_WIDTH + padding) * i);
	}
}
void BattleScene::moveAllEHandCardsToSuitablePlace() {
	int padding;
	if (CARD_WIDTH * eHandCardList->size() < eHandCardLayer->getContentSize().width - 64) padding = EHANDPILE_PADDING;
	else padding = (eHandCardLayer->getContentSize().width - CARD_WIDTH * eHandCardList->size()) / (eHandCardList->size() - 1);
	for (int i = 0; i < eHandCardList->size(); i++) {
		CardView* target = eHandCardList->get(i);
		target->stopAllActionsByTag(ACTIONS_MOVE);
		target->stopAllActionsByTag(ACTIONS_SCALE);

		target->setZOrder(eHandCardList->size() - i);
		auto moveTo = MoveTo::create(0.2, Point(EHANDPILE_WIDTH - (CARD_WIDTH / 2 + (CARD_WIDTH + padding) * i), CARD_HEIGHT / 2));
		target->runAction(moveTo);
		//log("x_pos:%d", CARD_WIDTH / 2 + (CARD_WIDTH + padding) * i);
	}
}

void BattleScene::enemyDiscardCard(Card* card) {
	CardView* usedCard = nullptr;
	for (int i = 0; i < eHandCardList->size(); i++) {
		auto cv = eHandCardList->get(i);
		if (cv->getCard() == card) {
			usedCard = cv;
			break;
		}
	}
	if (usedCard != nullptr) {
		eHandCardList->remove(usedCard, usedCard);
		moveAllEHandCardsToSuitablePlace();
		usedCard->flipTo(true);
		usedCard->setZOrder(99);
		Vec2 pos(getContentSize().width / 2 - eHandCardLayer->getPositionX(), getContentSize().height / 2 - eHandCardLayer->getPositionY() + 48);
		auto mov = MoveTo::create(0.1, pos);
		auto scale = ScaleTo::create(0.1, 1.2);
		auto func = CallFuncN::create([=](Node* n) {
			CardView* cv = static_cast<CardView*>(n);
			cv->discard();
			referee->discardCard(desktop->getP2(), cv->getCard());
			});
		auto delay = DelayTime::create(0.2);
		auto com = Spawn::create(mov, scale, NULL);
		usedCard->runAction(Sequence::create(com, delay, func, NULL));
	}

}

void BattleScene::enemyUseCard(Card* card) {
	CardView* usedCard = nullptr;
	for (int i = 0; i < eHandCardList->size(); i++) {
		auto cv = eHandCardList->get(i);
		if (cv->getCard() == card) {
			usedCard = cv;
			break;
		}
	}
	if (usedCard != nullptr) {
		eHandCardList->remove(usedCard, usedCard);
		moveAllEHandCardsToSuitablePlace();
		usedCard->setWillRelease(true, true, true);
		usedCard->flipTo(true);
		usedCard->setZOrder(99);
		Vec2 pos(getContentSize().width / 2 - eHandCardLayer->getPositionX(), getContentSize().height / 2 - eHandCardLayer->getPositionY() + 48);
		auto mov = MoveTo::create(0.15, pos);
		auto scale = ScaleTo::create(0.15, 1.2);
		auto func = CallFuncN::create([this](Node* n) {
			CardView* cv = static_cast<CardView*>(n);
			cv->release();
			referee->useCard(desktop->getP2(), desktop->getP1(), cv->getCard());
			});
		auto delay = DelayTime::create(0.35);
		auto com = Spawn::create(mov, scale, NULL);
		usedCard->runAction(Sequence::create(com, delay, func, NULL));
	}

}
void BattleScene::notifyEffectChanges() {
	//和手牌的刷新同理
	LinkedList<EffectView*>effectMRemove;
	LinkedList<EffectView*>effectERemove;
	LinkedList<EffectView*>toMInsert;
	LinkedList<EffectView*>toEInsert;
	LinkedList<EffectView*>tempM;
	LinkedList<EffectView*>tempE;
	LinkedList<Effect*>toolE;
	LinkedList<Effect*>toolM;

	tempM.transportAll(this->effectMList);
	tempE.transportAll(this->effectEList);
	for (int i = 0; i < tempM.size(); i++) {
		toolM.add(tempM.get(i)->getEffect());//加入自身效果
	}
	for (int i = 0; i < tempE.size(); i++) {
		toolE.add(tempE.get(i)->getEffect());//加入敌人效果
	}
	for (int i = 0; i < desktop->getP1()->getEffectList()->size(); i++) {
		Effect* e = desktop->getP1()->getEffectList()->get(i);
		int judge = toolM.indexOf(e);
		if (judge == -1) {//实际需要显示却没有显示
			EffectView* eMNew = EffectView::create(e);
			eMNew->setContentSize(Size(64, 64));
			eMNew->setAnchorPoint(Vec2(0.5, 0.5));
			this->effectMList->add(eMNew);
			toMInsert.add(eMNew);
		}
		else {
			EffectView* has = tempM.remove(judge);
			has->update();
			toolM.remove(judge);
			this->effectMList->add(has);
		}
	}
	for (int i = 0; i < desktop->getP2()->getEffectList()->size(); i++) {
		Effect* e = desktop->getP2()->getEffectList()->get(i);
		int judge = toolE.indexOf(e);
		if (judge == -1) {//实际需要显示却没有显示
			//flagE = 1;
			EffectView* eENew = EffectView::create(e);
			eENew->setContentSize(Size(64, 64));
			eENew->setAnchorPoint(Vec2(0.5, 0.5));
			this->effectEList->add(eENew);
			toEInsert.add(eENew);
		//	log("toEInser:%d", e->getId());
		}
		else {
			EffectView* has = tempE.remove(judge);
			has->update();
			toolE.remove(judge);
			effectEList->add(has);
		}
	}
	effectMRemove.transportAll(&tempM);
	effectERemove.transportAll(&tempE);
	//使不需要的消失
	for (int i = 0; i < effectMRemove.size(); i++) {
		EffectView* target = effectMRemove.get(i);
		if (target == nullptr) {
			continue;
		}
		//flagM = 1;
		target->stopAllActions();
		auto act2 = Blink::create(0.4, 3);
		CallFuncN* acf = CallFuncN::create([](Node* it) {
			it->removeFromParentAndCleanup(true);
			});
		target->runAction(Sequence::create(act2, acf, NULL));
	}
	for (int i = 0; i < effectERemove.size(); i++) {
		EffectView* target = effectERemove.get(i);
		if (target == nullptr) {
			continue;
		}
		//flagE = 1;
		target->stopAllActions();
		auto act1 = Blink::create(0.4, 3);
		CallFuncN* acf = CallFuncN::create([](Node* it) {
			it->removeFromParentAndCleanup(true);
			});
		target->runAction(Sequence::create(act1, acf, NULL));
	}
	//使需要的显示
	for (int i = 0; i < effectMList->size(); i++) {
		//log("aaa\n");
		auto target = effectMList->get(i);
		target->stopAllActions();
		if (toMInsert.contains(target)) {
			effectMLayer->addChild(target);
			target->runAction(ResizeTo::create(0.2, Size(64, 64)));
		}
		target->runAction(MoveTo::create(0.3, Vec2((i + 1) * EFFECT_WIDTH, 0)));
	}
	for (int i = 0; i < effectEList->size(); i++) {
		auto target = effectEList->get(i);
		target->stopAllActions();
		if (toEInsert.contains(target)) {
			effectELayer->addChild(target);
			target->runAction(ResizeTo::create(0.2, Size(64, 64)));
		}
		target->runAction(MoveTo::create(0.3, Vec2(EFFECTS_WIDTH-(i - 1) * EFFECT_WIDTH, 0)));
	}

}

void BattleScene::popUpCountGameDialog(bool win) {
	auto dialog = CountGameDialog::create(win, referee,player,checkpoint,dbhelper);
	dialog->setPosition(Point::ZERO);
	addChild(dialog);
	dialog->fadeIn();
}