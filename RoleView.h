#pragma once
#include "Role.h"
#include "ui/UIText.h"
#include "mString.h"
#include "ui/UILayout.h"
#include "E:\softwares\cocos2d-x-3.17.2\cocos2d-x-3.17.2\codes\HelloCpp\cocos2d\cocos\2d\CCSprite.h"
#include "uidimens.h"

#define ROLE_RED 0
#define ROLE_BLUE 1
using namespace cocos2d;
using namespace ui;
class RoleView :
	public Sprite
{
public:
	RoleView(Role* role,int color,int checkId);

	void refreshDashBoard();
	void getHurt(int howMuch);
	void getRecover(int howMuch);
	void getEffect();
private:
	bool init(int checkId);
	void initAnimations();

	int color;
//	mString avatarFile;
	Role* role;
	ui::Text* NameText;
	ui::Text* HPText;
	ui::Text* MANA1Text;
	ui::Text* MANA2Text;
	ui::Text* HandCdNumText;

	Sprite* anim_hurt_sprite;
	Sprite* anim_heal_sprite;
	Sprite* anim_effect_sprite;
};
