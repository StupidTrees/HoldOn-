#pragma once
#include "cocos2d.h"
#include "Player.h"
#include "DBHelper.h"
#include "CardView.h"
#include "ui/UIButton.h"
using namespace cocos2d;
using namespace ui;
class MainScene;

/*“神圣祈福”界面*/
class ShopDialog :
	public Layer
{
public:
	ShopDialog();
	static ShopDialog* create(MainScene* ms,DBHelper* dbh, Player* pl);
	void refreshState(); //刷新按钮状态（金币少于要求时无法点击）
	void onEnter() override;
	void drawCard(); //点击进行抽卡
private:
	Player* player;
	DBHelper* dbhelper;
	Button* button;
	ui::Layout* mask;
	MainScene* parent;
	bool init(MainScene* ms,DBHelper* dbh,Player* pl);
};

