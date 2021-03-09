#include "RoleView.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;
RoleView::RoleView(Role* role,int color,int checkpointId)
{
	this->role = role;
	///this->avatarFile = avatar;
	this->color = color;
	init(checkpointId);
}

void RoleView::refreshDashBoard() {
	mString hpName = color == ROLE_RED ? "HP:" : "发量(HP):";

	NameText->setText(role->getName() + " lv." + mString::parseInt(role->getLevel()));
	HPText->setText(hpName + mString::parseInt(role->getHP()) + "/" + mString::parseInt(role->getMaxHP()));
	MANA1Text->setText("San值:" + mString::parseInt(role->getMANA1()) + "/" + mString::parseInt(role->getMaxMANA1()));
	MANA2Text->setText("肝值:" + mString::parseInt(role->getMANA2()) + "/" + mString::parseInt(role->getMaxMANA2()));
}

bool RoleView::init(int checkpoint)
{
	setContentSize(Size(ROLE_CARD_WIDTH, ROLE_CARD_HEIGHT));
	auto lp2 = LinearLayoutParameter::create();
	lp2->setMargin(Margin(24, 2, 24, 2));
	NameText = Text::create();
	HPText = Text::create();
	MANA1Text = Text::create();
	MANA2Text = Text::create();
	NameText->setFontSize(24);
	HPText->setFontSize(18);
	MANA1Text->setFontSize(18);
	MANA2Text->setFontSize(18);
	NameText->ignoreContentAdaptWithSize(false);
	HPText->ignoreContentAdaptWithSize(false);
	MANA1Text->ignoreContentAdaptWithSize(false);
	MANA2Text->ignoreContentAdaptWithSize(false);
	HPText->setLayoutParameter(lp2);
	MANA1Text->setLayoutParameter(lp2);
	MANA2Text->setLayoutParameter(lp2);

	NameText->setContentSize(Size(MDASHBOARD_WIDTH, 26));
	NameText->setTextColor(Color4B::BLACK);
	HPText->setContentSize(Size(MDASHBOARD_WIDTH, 26));
	HPText->setTextColor(Color4B::BLACK);
	MANA1Text->setContentSize(Size(MDASHBOARD_WIDTH, 26));
	MANA1Text->setTextColor(Color4B::BLACK);
	MANA2Text->setContentSize(Size(MDASHBOARD_WIDTH, 26));
	MANA2Text->setTextColor(Color4B::BLACK);

	auto eDashBoard = Layout::create();
	eDashBoard->setLayoutType(Layout::Type::VERTICAL);
	eDashBoard->setContentSize(Size(ROLE_CARD_WIDTH, ROLE_CARD_HEIGHT/3));
	//eDashBoard->setPosition(Point(getContentSize().width / 2 - MDASHBOARD_WIDTH / 2, getContentSize().height - MHANDPILE_HEIGHT));
	eDashBoard->addChild(HPText);
	eDashBoard->addChild(MANA1Text);
	eDashBoard->addChild(MANA2Text);

	mString frame_name;
	if (color == ROLE_RED) frame_name = "enemy_board_bg.png";
	else if (color == ROLE_BLUE) frame_name = "player_board_bg.png";
	auto eDashBG = Sprite::create(frame_name);

	Sprite* eAvatar = nullptr;
	if (color == ROLE_BLUE) {
		eAvatar = Sprite::create("player.jpg");
	}
	else {
		auto test = Sprite::create("roles/enemy" + mString::parseInt(checkpoint) + ".png");
		if (test != nullptr) eAvatar = test;
		else eAvatar = Sprite::create("roles/enemy0.png");;
	}
	

	eDashBG->setContentSize(Size(ROLE_CARD_WIDTH , ROLE_CARD_HEIGHT ));
	eDashBG->setPosition(Vec2(ROLE_CARD_WIDTH / 2, ROLE_CARD_HEIGHT / 2));
	eAvatar->setContentSize(Size(ROLE_CARD_WIDTH*51/70, ROLE_CARD_HEIGHT*38/70));
	eAvatar->setPosition(Vec2(ROLE_CARD_WIDTH / 2+5, ROLE_CARD_HEIGHT*2/3));

	NameText->setPosition(Vec2(ROLE_CARD_WIDTH / 2+ROLE_CARD_WIDTH/4, ROLE_CARD_HEIGHT*3/ 7));
	eDashBoard->setPosition(Vec2(ROLE_CARD_WIDTH / 4, 0));
	addChild(eAvatar);
	addChild(eDashBG);
	addChild(NameText);
	addChild(eDashBoard);
	initAnimations();
}

void RoleView::initAnimations() {
	anim_hurt_sprite= Sprite::create();
	anim_hurt_sprite->setContentSize(Size(500, 500));
	anim_hurt_sprite->setPosition(getContentSize().width/2,getContentSize().height/2);
	addChild(anim_hurt_sprite);
	anim_heal_sprite = Sprite::create();
	anim_heal_sprite->setPosition(getContentSize().width / 2, getContentSize().height *4 / 9);
	addChild(anim_heal_sprite);
	anim_effect_sprite = Sprite::create();
	anim_effect_sprite->setPosition(getContentSize().width / 2, getContentSize().width * 5/ 9);
	addChild(anim_effect_sprite);

}
void RoleView::getHurt(int howMuch) {
	SimpleAudioEngine::getInstance()->playEffect("audio/hurt.wav");
	int number = 0;
	for (Node* x : getChildren()) if (x->getTag() == 99||x->getTag()==88) number++;
	auto anim_hurt_text = LabelBMFont::create("0", "fonts/atlas_damage.fnt");
	anim_hurt_text->setPosition(Point(getContentSize().width - 80+number*48, getContentSize().height - 200));
	anim_hurt_text->setString("-" + mString::parseInt(howMuch));
	anim_hurt_text->setOpacity(0);
	anim_hurt_text->setScale(howMuch<10?1:2);
	anim_hurt_text->setTag(99);
	addChild(anim_hurt_text);
	auto textUp = MoveTo::create(1, Vec3(anim_hurt_text->getPositionX(), anim_hurt_text->getPositionY() + 50, anim_hurt_text->getPositionZ()));
	auto remove = CallFuncN::create([](Node* n) {
		n->removeFromParentAndCleanup(true);
		});
	auto fadeSeq = Sequence::create(FadeIn::create(0.1), DelayTime::create(0.7), FadeOut::create(0.2), remove, NULL);
	auto sp = Spawn::create(fadeSeq, textUp, NULL);
	auto finalSeq = Sequence::create(DelayTime::create(number * 0.16), sp, NULL);
	anim_hurt_text->runAction(finalSeq);


	if (getActionByTag(13) != nullptr) return;
	cocos2d::Animate* my_ani = Animate::create(AnimationCache::getInstance()->getAnimation("hurt"));
	auto delay = DelayTime::create(0.01);
	anim_hurt_sprite->runAction(Sequence::create(delay,my_ani, Hide::create(), nullptr));
	anim_hurt_sprite->setVisible(true);
	int oldX = getPositionX(), oldY = getPositionY();
	int newX = oldX - 20, newY = oldY - 20;
	auto down = MoveTo::create(0.1, Vec2(newX,newY));
	auto up = MoveTo::create(0.1, Vec2(oldX,oldY));
	auto fadeTo = FadeTo::create(0.01, 100);
	auto fadeBack = FadeTo::create(0.01, 255);
	auto seq = Sequence::create(fadeTo, down, up, fadeBack, NULL);
	seq->setTag(13);
	runAction(seq);




}

void RoleView::getRecover(int howMuch) {
	int number = 0;
	for (Node* x : getChildren()) if (x->getTag() == 88||x->getTag()==99) number++;
	auto anim_hurt_text = LabelBMFont::create("0", "fonts/atlas_heal.fnt");
	anim_hurt_text->setPosition(Point(getContentSize().width - 80 + number * 48, getContentSize().height - 200));
	anim_hurt_text->setString("+" + mString::parseInt(howMuch));
	anim_hurt_text->setOpacity(0);
	anim_hurt_text->setScale(howMuch < 10 ? 1 : 2);
	anim_hurt_text->setTag(88);
	addChild(anim_hurt_text);
	auto textUp = MoveTo::create(1, Vec3(anim_hurt_text->getPositionX(), anim_hurt_text->getPositionY() + 50, anim_hurt_text->getPositionZ()));
	auto remove = CallFuncN::create([](Node* n) {
		n->removeFromParentAndCleanup(true);
		});
	auto fadeSeq = Sequence::create(FadeIn::create(0.1), DelayTime::create(0.7), FadeOut::create(0.2), remove, NULL);
	auto sp = Spawn::create(fadeSeq, textUp, NULL);
	auto finalSeq = Sequence::create(DelayTime::create(number * 0.16), sp, NULL);
	anim_hurt_text->runAction(finalSeq);

	cocos2d::Animate* my_ani = Animate::create(AnimationCache::getInstance()->getAnimation("heal"));
	anim_heal_sprite->runAction(Sequence::create(my_ani, Hide::create(), nullptr));
	anim_heal_sprite->setVisible(true);

}

void RoleView::getEffect() {

	//cocos2d::Animate* my_ani_back= Animate::create(AnimationCache::getInstance()->getAnimation("effect_back"));
	cocos2d::Animate* my_ani_front = Animate::create(AnimationCache::getInstance()->getAnimation("effect_front"));
	anim_effect_sprite->runAction(Sequence::create(my_ani_front, Hide::create(), nullptr));
	//anim_effect_sprite->runAction(Sequence::create(my_ani_back, Hide::create(), nullptr));
	anim_effect_sprite->setVisible(true);

}
