#pragma once
#include "cocos2d.h"
#include "Card.h"
#include "cocos2d/cocos/ui/UIText.h"
#include "ui/UILayout.h"

using namespace cocos2d;
/*封装的“详情”窗口*/
class CardInfoWindow :
	public Sprite
{
public:
	/*使用效果或卡牌初始化*/
	CardInfoWindow(Card* c, int width, int height);
	static CardInfoWindow* create(Card* c, int width, int height);
	CardInfoWindow(Effect* c, int width, int height);
	static CardInfoWindow* create(Effect* c, int width, int height);



private:
	Card* card = nullptr;
	Effect* effect = nullptr;
	int width, height;
	bool init();
};

