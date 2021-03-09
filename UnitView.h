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

/*封装的关卡卡片精灵*/
class UnitView :
	public cocos2d::Sprite{
public:
	UnitView();
	~UnitView();
	static UnitView* create(Checkpoint* u, Player* p,DBHelper* dbhelper); //使用关卡对象完成初始化
	void runActionWithChildren(Action* a);
	Checkpoint* getCheckpoint();
private:
	Player* player;
	DBHelper* dbhelper;
	bool init(Checkpoint* u,Player* p,DBHelper* dbhelper);
	Checkpoint* U;
	Button* button; //进入游戏的按钮
	Sprite* cardFrame;
	Sprite* content;
	LabelTTF* name;
	LabelTTF* description;
};