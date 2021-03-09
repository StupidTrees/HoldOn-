#pragma once
#include "cocos2d.h"
#include "Card.h"
#include "ui\UIImageView.h"
#include "ui\UILayout.h"
USING_NS_CC;
using namespace ui;

/*封装的卡牌精灵*/
class CardView :
	public Sprite
{
public:
	CardView();
	~CardView();
	static CardView* create(Card* c, bool direction, bool forExhibi);
	Card* getCard();
	void setFocused(bool f); //是否正在被拖拽
	void setOpacityWithChildren(float opa); //自身及子结点同时设置透明度
	void runActionWithChildren(Action* action); //自身及子结点同时进行动画
	void setWillRelease(bool will, bool changeOpacity,bool isEnemy); //“即将释放”的酷炫动画效果
	void discard(); //弃牌效果
	void release(); //使用效果
	void setUseable(bool u); //设置是否可被使用
	void flipTo(bool front); //翻面
	Sprite* topTexture;
	bool isFocused();
	bool isUseable();
private:
	bool isFront;
	bool init(Card* c, bool isFront,bool forExhibi);
	Card* card = nullptr;
	/*各UI成分*/
	LabelTTF* consume;
	LabelTTF* name;
	LabelTTF* description;
	bool focused = false;
	bool useAble = false;
	Sprite* releaseMagic;
	Sprite* content;
	Texture2D* backTexture;
	Texture2D* frontTexture;
};

