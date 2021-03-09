#include "EffectView.h"
EffectView::EffectView() {
}

EffectView::~EffectView() {
}

EffectView* EffectView::create(Effect* e) {
	auto ev = new EffectView();
	if (ev && ev->init(e)) {
		ev->autorelease();
		return ev;
	}
	else {
		delete ev;
		ev = nullptr;
		return nullptr;
	}
}
Effect* EffectView::getEffect() {
	return effect;
}
bool EffectView::init(Effect* e)
{
	if (e == nullptr) {
		return false;
	}
	else {
		this->effect = e;
		Sprite::initWithFile("effect_00.png");
		setContentSize(Size(64, 64));
		/*cocos2d::Animate* my_ani = Animate::create(AnimationCache::getInstance()->getAnimation("magic_buff"));
		auto anim = Sprite::create();
		anim->setContentSize(getContentSize());
		anim->setPosition(Point(getContentSize().width / 2, getContentSize().height / 2));
		addChild(anim);
		anim->runAction(RepeatForever::create(my_ani));*/
	    setVisible(true);
		count = LabelTTF::create();
		count->setFontSize(28);
		count->setVisible(effect->getCountdown() >= 0);
		count->setString(mString::parseInt(effect->getCountdown()));
		count->setPosition(Point(getContentSize().width / 2+6, getContentSize().height / 2-20));
		addChild(count);
	}
	return true;
}
bool EffectView::update() {
	int flag = this->effect->getCountdown();
	if (flag > 0) {
		count->setString(mString::parseInt(flag));
		return true;
	}
		return false;
}
