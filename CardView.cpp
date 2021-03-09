#include "CardView.h"
#include "uidimens.h"
#include "dimens.h"
#pragma execution_character_set("utf-8")
CardView::CardView()
{
}


CardView::~CardView()
{
}


CardView * CardView::create(Card * c, bool direction, bool forExhibi)
{
	CardView* bs = new CardView;
	if (bs&&bs->init(c,direction, forExhibi)) {
		bs->autorelease();
		return bs;
	}
	else {
		delete bs;
		bs = nullptr;
		return nullptr;
	}
}

Card * CardView::getCard()
{
	return card;
}

void CardView::setFocused(bool f)
{
	focused = f;
	if (!f) {
		removeChildByName("selected");
		removeChildByName("selected_magic");
	}
}

void CardView::setOpacityWithChildren(float opa) {
	topTexture->setOpacity(opa);
	setOpacity(opa);
	name->setOpacity(opa);
	description->setOpacity(opa);

}

void CardView::runActionWithChildren(Action* action) {
	auto action1 = action->clone();
	auto action2 = action->clone();
	auto action3 = action->clone();
	auto action4 = action->clone();
	name->runAction(action1);
	description->runAction(action2);
	topTexture->runAction(action3);
	content->runAction(action4);
}

void CardView::setWillRelease(bool will,bool changeOpacity,bool isEnemy) {
	if (will) {
		if (getChildByName("selected") == nullptr&&getChildByName("selected_magic")==nullptr) {
			stopAllActions();
			auto selected = Sprite::create(isEnemy? "cardbg_selected_enemy.png": "cardbg_selected.png" );
			auto selectedMagic = Sprite::create(isEnemy ? "card_release_magic_enemy.png":"card_release_magic.png");
			float width = getContentSize().width * 1.6;
			float height = getContentSize().height * 1.35;
			selected->setContentSize(Size(width, height));
			selected->setPosition(Point(getContentSize().width / 2, getContentSize().height / 2));
			selected->setOpacity(0);
			selectedMagic->setContentSize(Size(width, height));
			selectedMagic->setPosition(Point(getContentSize().width / 2, getContentSize().height / 2));
			selectedMagic->setOpacity(0);
			addChild(selected, getChildrenCount() - 1, "selected");
			addChild(selectedMagic, getChildrenCount() - 1, "selected_magic");

			auto delay = DelayTime::create(isEnemy ? 0.2 : 0);

			selected->runAction(Sequence::create(delay,FadeTo::create(0.4,isEnemy?160:255),NULL));
			selectedMagic->runAction(Sequence::create(delay,FadeIn::create(0.5),NULL));
			auto repeat = RepeatForever::create(RotateBy::create(1.5, 360));
			selectedMagic->runAction(repeat);
			if (changeOpacity) {
				topTexture->runAction(FadeTo::create(0.4, 120));
			}
		}

	}
	else {
		auto child = getChildByName("selected");
		if (child != nullptr) {
			CallFuncN* func = CallFuncN::create([](Node* target) {
				target->removeFromParentAndCleanup(true);
				});
			auto fadeout = FadeOut::create(0.4);
			child->runAction(Sequence::create(fadeout, func,NULL));
			
		}
		auto child2 = getChildByName("selected_magic");
		if (child2 != nullptr) {
			auto fadeout = FadeOut::create(0.4);
			CallFuncN* func = CallFuncN::create([](Node* target) {
				target->removeFromParentAndCleanup(true);
				});
			child2->runAction(Sequence::create(fadeout, func, NULL));
		}
		if (changeOpacity)topTexture->runAction(FadeTo::create(0.4, 255));
	}
}

void CardView::discard() {
	auto fade = FadeOut::create(0.2);
	CallFuncN* func = CallFuncN::create([](Node* target) {
		CardView* cv = static_cast<CardView*>(target);
		cv->removeFromParentAndCleanup(true);
		});
	runActionWithChildren(Sequence::create(fade, func, NULL));
}

void CardView::release() {
	auto fade = FadeOut::create(0.2);
	runActionWithChildren(fade);

	CallFuncN* func = CallFuncN::create([](Node* target) {
		target->removeFromParentAndCleanup(true);
		});
	CallFuncN* func2 = CallFuncN::create([](Node* target) {
		target->removeFromParentAndCleanup(true);
		});
	CallFuncN* func3 = CallFuncN::create([](Node* target) {
		target->removeFromParentAndCleanup(true);
		});
	auto selected = getChildByName("selected");
	if (selected != nullptr) {
		selected->runAction(Sequence::create(Spawn::create(FadeOut::create(0.4), ScaleTo::create(0.4,1.5),NULL),func,NULL));
	}
	auto selectedMagic = getChildByName("selected_magic");
	if (selectedMagic != nullptr) {
		selectedMagic->runAction(Sequence::create(Spawn::create(FadeOut::create(0.4), ScaleTo::create(0.4, 1.2), NULL), func2, NULL));
	}

	auto delay = DelayTime::create(0.6);
	runAction(Sequence::create(delay, func3, NULL));
}
bool CardView::isFocused()
{
	return focused;
}

bool CardView::isUseable()
{
	return useAble;
}

void CardView::setUseable(bool u)
{
	useAble = u;
	if(!u)runActionWithChildren(FadeTo::create(0.2, 160));
	else runActionWithChildren(FadeTo::create(0.2, 255));
}

void CardView::flipTo(bool front)
{
	auto orbit1 = OrbitCamera::create(0.1, 1, 0, 0,-90, 0, 0);
	auto changeTexture = CallFuncN::create([=](Node* n) {
		auto cv = static_cast<CardView*>(n);
		cv->topTexture->setTexture(front ? frontTexture : backTexture);
		name->setVisible(isFront);
		content->setVisible(isFront);
		description->setVisible(isFront);
		});
	auto orbit2 = OrbitCamera::create(0.1, 1, 0, 90,-90, 0, 0);

	auto seq = Sequence::create(orbit1, changeTexture, orbit2, NULL);
	seq->setTag(ACTIONS_FLIP);
	runAction(seq);
	this->isFront = front;
}


bool CardView::init(Card* c,bool isFront, bool forExhibi)
{
	if (c == nullptr) return false;
	Sprite::init();
	this->isFront = isFront;
	//this->initWithFile("cardbg.png");
	this->card = c;
	setContentSize(Size(CARD_WIDTH, CARD_HEIGHT));
	auto texture_cache = TextureCache::sharedTextureCache();
	mString key = "cardframe_function" ;
	switch (c->getType()) {
	case CARD_TYPE_ATTACK: key = "cardframe_attack"; break;
	case CARD_TYPE_RECOVER: key = "cardframe_recover"; break;
	case CARD_TYPE_FUNCTION: key = "cardframe_function"; break;
	case CARD_TYPE_MAGIC: key = "cardframe_magic"; break;
	}
	frontTexture = texture_cache->getTextureForKey(key);
	backTexture = texture_cache->getTextureForKey("cardbg_back");
	name = LabelTTF::create("汉字", "",24);
	description = LabelTTF::create();
	description->setFontSize(16);
	auto test = Sprite::create("cards/cd"+mString::parseInt(c->getId())+".png");
	if (test != nullptr) content = test;
	else content = Sprite::create("cards/cd8.png");
	content->setContentSize(Size(CARD_WIDTH * 4 / 5-6, CARD_HEIGHT / 2));
	content->setPosition(Point(CARD_WIDTH / 2+2, CARD_HEIGHT * 2 / 3-10));
	name->setString(c->getName());
	name->setColor(Color3B::BLACK);
	description->setColor(Color3B::BLACK);
	description->setDimensions(Size(CARD_WIDTH*3 /5, CARD_HEIGHT / 2));
	description->setString(c->getDescription());
	name->setPosition(CARD_WIDTH / 2, CARD_HEIGHT*4/5 );
	description->setPosition(CARD_WIDTH / 2, CARD_HEIGHT/6);
	//name->setCascadeOpacityEnabled(false);
	topTexture = Sprite::createWithTexture(isFront ? frontTexture : backTexture);
	topTexture->setCascadeOpacityEnabled(true);
	if(!forExhibi)topTexture->setContentSize(Size(0,0));
	else topTexture->setContentSize(Size(CARD_WIDTH,CARD_HEIGHT));
	topTexture->setPosition(Point(CARD_WIDTH / 2, CARD_HEIGHT / 2));
	addChild(content);
	addChild(topTexture);
	addChild(name);
	addChild(description);
	name->setVisible(isFront);
	description->setVisible(isFront);
	content->setVisible(isFront);
	return true;
}
