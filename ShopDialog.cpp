#include "ShopDialog.h"
#include "uidimens.h"
#include "MainScene.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;
ShopDialog::ShopDialog()
{
}

ShopDialog* ShopDialog::create(MainScene* parent,DBHelper* dbh, Player* pl)
{
	ShopDialog* sd = new ShopDialog();
	if (sd && sd->init(parent,dbh,pl)) {
		sd->autorelease();
		return sd;
	}
	else {
		delete sd;
		sd = nullptr;
		return nullptr;
	}
}

void ShopDialog::refreshState() {
	button->setEnabled(player->getCoins() >= 300);
}
void ShopDialog::onEnter()
{
	Layer::onEnter();
	refreshState();
	mask->runAction(FadeTo::create(0.3, 180));
}
void  ShopDialog::drawCard() {
	player->spendCoins(300);
	parent->refreshDashBoard();
	player->saveObjectToDisk();
	auto all = dbhelper->getAllCards();
	auto toRemove = new LinkedList<Card*>;
	for (int i = 0; i < all->size(); i++) {
		auto c = all->get(i);
		if(player->getPile()->contains(c->getId())) toRemove->add(c);
	}
	all->removeAll_delete(toRemove);
	if (all->size() > 0) {
		int index = RandomHelper::random_int(0, all->size() - 1);
		Card* c = all->get(index);
		CardView* cv = CardView::create(c, false, true);
		cv->setPosition(getContentSize().width / 2+10, getContentSize().height / 2 +10);
		cv->setOpacityWithChildren(0);
		addChild(cv);
		cocos2d::Animate* my_ani_front = Animate::create(AnimationCache::getInstance()->getAnimation("effect_front"));
		auto anim = Sprite::create();
		anim->setPosition(getContentSize().width / 2+10, getContentSize().height / 2 - 44);
		addChild(anim);
		anim->runAction(Sequence::create(my_ani_front, Hide::create(), nullptr));
		cv->setWillRelease(true, true, false);
		auto disable = CallFunc::create([this]() {button->setEnabled(false); });
		auto fadeIn = FadeIn::create(1);
		auto flip = CallFuncN::create([=](Node* n) {
			CardView* cv = static_cast<CardView*>(n);
			if (cv != nullptr) {
				cv->flipTo(true);
				cv->setWillRelease(false, true, false);
				auto bt_takethat = ui::Button::create("buttons/illtakethat_normal.png", "buttons/illtakethat_pressed.png", "buttons/illtakethat_pressed.png");
				bt_takethat->addClickEventListener([=](Ref* r) {
					player->getPile()->add(c->getId());
					player->saveObjectToDisk();
					bt_takethat->removeFromParent();
					auto move = MoveBy::create(0.3, Vec3(-500, -700, 0));
					auto func = CallFuncN::create([](Node* n) {n->removeFromParentAndCleanup(true); });
					cv->runAction(Sequence::create(move, func, NULL));
					refreshState();
					});
				bt_takethat->setPosition(Point(getContentSize().width / 2, 180));
				addChild(bt_takethat);

			}
			});
		cv->runActionWithChildren(Sequence::create(disable, fadeIn, NULL));
		cv->runAction(Sequence::create(DelayTime::create(1), flip, NULL));
	}
}
bool ShopDialog::init(MainScene* ms,DBHelper* dbh, Player* pl)
{
	parent = ms;
	setContentSize(Size(WINDOW_WIDTH, WINDOW_HEIGHT));
	setTouchEnabled(true);
	auto cache = TextureCache::sharedTextureCache();
	auto bg = Sprite::createWithTexture(cache->getTextureForKey("shop_dialog_bg"));
	bg->setPosition(Vec2(getContentSize().width / 2, getContentSize().height / 2));
	bg->setContentSize(getContentSize());
	button = Button::create("buttons/draw_normal.png", "buttons/draw_pressed.png", "buttons/draw_disabled.png");
	auto bt_back = ui::Button::create("buttons/return_normal.png", "buttons/return_pressed.png", "buttons/return_pressed.png");
	bt_back->setPosition(Point(WINDOW_WIDTH * 3/ 4-70, WINDOW_HEIGHT * 4 / 5));
	bt_back->addClickEventListener([=](Ref* r) {
		SimpleAudioEngine::getInstance()->playEffect("audio/ButtonBack.mp3");
		removeFromParentAndCleanup(true);
		});
	button->setPosition(Point(WINDOW_WIDTH / 2, 80));
	button->addClickEventListener([=](Ref* r) {
		SimpleAudioEngine::getInstance()->playEffect("audio/ButtonBack.mp3");
		drawCard();
		});
	dbhelper = dbh;
	player = pl;
	mask = ui::Layout::create();
	mask->setContentSize(getContentSize());
	mask->setPosition(Point::ZERO);
	mask->setOpacity(0);
	mask->setBackGroundColor(Color3B::BLACK);
	mask->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	addChild(mask);
	addChild(bg);
	addChild(button);
	addChild(bt_back);
	return true;
}
