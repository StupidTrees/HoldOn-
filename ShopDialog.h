#pragma once
#include "cocos2d.h"
#include "Player.h"
#include "DBHelper.h"
#include "CardView.h"
#include "ui/UIButton.h"
using namespace cocos2d;
using namespace ui;
class MainScene;

/*����ʥ��������*/
class ShopDialog :
	public Layer
{
public:
	ShopDialog();
	static ShopDialog* create(MainScene* ms,DBHelper* dbh, Player* pl);
	void refreshState(); //ˢ�°�ť״̬���������Ҫ��ʱ�޷������
	void onEnter() override;
	void drawCard(); //������г鿨
private:
	Player* player;
	DBHelper* dbhelper;
	Button* button;
	ui::Layout* mask;
	MainScene* parent;
	bool init(MainScene* ms,DBHelper* dbh,Player* pl);
};

