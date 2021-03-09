#pragma once
#include "cocos2d.h"
#include "Card.h"
#include "ui\UIImageView.h"
#include "ui\UILayout.h"
USING_NS_CC;
using namespace ui;

/*��װ�Ŀ��ƾ���*/
class CardView :
	public Sprite
{
public:
	CardView();
	~CardView();
	static CardView* create(Card* c, bool direction, bool forExhibi);
	Card* getCard();
	void setFocused(bool f); //�Ƿ����ڱ���ק
	void setOpacityWithChildren(float opa); //�����ӽ��ͬʱ����͸����
	void runActionWithChildren(Action* action); //�����ӽ��ͬʱ���ж���
	void setWillRelease(bool will, bool changeOpacity,bool isEnemy); //�������ͷš��Ŀ��Ŷ���Ч��
	void discard(); //����Ч��
	void release(); //ʹ��Ч��
	void setUseable(bool u); //�����Ƿ�ɱ�ʹ��
	void flipTo(bool front); //����
	Sprite* topTexture;
	bool isFocused();
	bool isUseable();
private:
	bool isFront;
	bool init(Card* c, bool isFront,bool forExhibi);
	Card* card = nullptr;
	/*��UI�ɷ�*/
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

