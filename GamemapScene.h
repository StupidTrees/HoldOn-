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
/*ѡ�ؽ���*/
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
	LinkedList<UnitView*>* unitViewList;//Ҫ��ʾ�Ĺؿ���Ƭ�б�
	ui::Layout* mask; //�ײ���͸������
	Sprite* bg; //����ͼƬ
};