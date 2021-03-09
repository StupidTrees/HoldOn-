#include "BackPackGridItem.h"
#include "BackPackDialog.h"




BackPackGridItem::BackPackGridItem(BackPackDialog * parent)
{
	this->parent = parent;
}

BackPackGridItem::~BackPackGridItem()
{
}

bool BackPackGridItem::init()
{
	Node::init();
	bg = Sprite::create("cardspace.png");

	bg->setContentSize(Size(BLOCK_WIDTH,BLOCK_HEIGHT));
	bg->setPosition(Vec2(BLOCK_WIDTH/2,0));
	this->addChild(bg,0,"bg");
	setChecked(false);
	return true;
}

void BackPackGridItem::setUpWithCardRes(Card* c) {
	this->cardView = CardView::create(c,true,true);
	this->cardView->setContentSize(Size(CARD_WIDTH, CARD_HEIGHT));
	cardView->setPosition(Vec2(BLOCK_WIDTH/2, 0));
	auto listener1 = EventListenerTouchOneByOne::create();
	listener1->onTouchBegan = 
		[this](Touch* touch, Event* event) {
		auto target = static_cast<CardView*>(event->getCurrentTarget());
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		 Rect rect = Rect(0, 0, s.width, s.height);
		if (rect.containsPoint(locationInNode))
		 {
			cocos2d::log("touch:%d", target->getCard()->getEffects()->size());
			target->setOpacity(180);
			parent->selectItem(this);
			return true;
		}
		return false;

	};
	listener1->onTouchEnded = [](Touch* touch, Event* event) {
		auto target = static_cast<CardView*>(event->getCurrentTarget());
		target->setOpacity(255);
		return true;

	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, cardView);
	this->addChild(cardView,1,"card");
}
void BackPackGridItem::setUpWithNone() {
	this->cardView = nullptr;
	this->removeChildByName("card");
}
void BackPackGridItem::setChecked(bool check)
{
	removeChildByName("selected");
	checked = check;
	if (checked) {
		auto selected = Sprite::create("cardspace_selected.png");
		selected->setContentSize(Size(BLOCK_WIDTH, BLOCK_HEIGHT));
		selected->setPosition(Vec2(BLOCK_WIDTH/2, 0));
		addChild(selected,0,"selected");
	}
}
bool BackPackGridItem::isChecked()
{
	return checked;
}
CardView * BackPackGridItem::getCardView()
{
	return cardView;
}
BackPackGridItem * BackPackGridItem::create(BackPackDialog* root)
{
	BackPackGridItem* bs = new BackPackGridItem(root);
	if (bs&&bs->init()) {
		bs->autorelease();
		return bs;
	}
	else {
		delete bs;
		bs = nullptr;
		return nullptr;
	}
}
