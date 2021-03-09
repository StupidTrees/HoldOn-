#pragma once
#include "cocos2d.h"
#include "Card.h"
#include "cocos2d/cocos/ui/UIText.h"
#include "ui/UILayout.h"

using namespace cocos2d;
/*��װ�ġ����顱����*/
class CardInfoWindow :
	public Sprite
{
public:
	/*ʹ��Ч�����Ƴ�ʼ��*/
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

