#pragma once
#include "cocos2d.h"
#include "MainScene.h"
#include "Checkpoint.h"
#include "Card.h"
#include "ui\UIImageView.h"
#include "ui\UILayout.h"
#include "ui\UIButton.h"
#include "BattleScene.h"
USING_NS_CC;
using namespace ui;

/*��װ�Ĺؿ���Ƭ����*/
class UnitView :
	public cocos2d::Sprite{
public:
	UnitView();
	~UnitView();
	static UnitView* create(Checkpoint* u, Player* p,DBHelper* dbhelper); //ʹ�ùؿ�������ɳ�ʼ��
	void runActionWithChildren(Action* a);
	Checkpoint* getCheckpoint();
private:
	Player* player;
	DBHelper* dbhelper;
	bool init(Checkpoint* u,Player* p,DBHelper* dbhelper);
	Checkpoint* U;
	Button* button; //������Ϸ�İ�ť
	Sprite* cardFrame;
	Sprite* content;
	LabelTTF* name;
	LabelTTF* description;
};