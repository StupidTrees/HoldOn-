#pragma once
#include "cocos2d.h"
#include "MainScene.h"
#include "DeskTop.h"
#include "Referee.h"
#include "ui/UIText.h"
#include "ui/UILayout.h"
#include "uidimens.h"
#include "DBHelper.h"
#include "RoleView.h"
#include "UnitView.h"
USING_NS_CC;
using namespace ui;
/*选关界面*/
class GamemapScene:
	public cocos2d::Layer {
public:
	GamemapScene();
	~GamemapScene();
	static GamemapScene* create(DBHelper* dbhelper, Player* p);
	bool init(DBHelper* dbhelper, Player* p);
	void onEnter();
	void onEnterTransitionDidFinish();

private:
	DBHelper* dbhelper;
	Player* player;
	LinkedList<UnitView*>* unitViewList;//要显示的关卡卡片列表
	ui::Layout* mask; //底部半透明遮罩
	Sprite* bg; //背景图片
};