#pragma once
#include "cocos2d.h"
#include "Effect.h"
#pragma execution_character_set("utf-8")
USING_NS_CC;
/*·â×°µÄbuff¾«Áé*/
class EffectView :
	public Sprite
{
public:
	EffectView();
	~EffectView();
	Effect* getEffect(void);
	bool init(Effect* e);
	bool update(void);
	static EffectView* create(Effect* e);
private:
	LabelTTF* count = nullptr;
	Effect* effect = nullptr;
};

