#pragma once
#include "cocos2d.h"
#include"ui\UIScrollView.h"
#include"ui\UIButton.h"
#include"ui\UIImageView.h"
#include"ui\UIText.h"
#include"Card.h"
#include "uidimens.h"
#include"ui\UIWidget.h"
#include"BackPackGridItem.h"
#include "DBHelper.h"
#include "Player.h"
using namespace ui;
USING_NS_CC;
class BackPackDialog :
	public Layer 
{
public:
	BackPackDialog(); 
	~BackPackDialog();
	static BackPackDialog* create(DBHelper* dbhelper,Player* player);

	bool init(DBHelper* dbh, Player* player);


	void notifyDatasetChanged();

	void loadItemsBackInground(int x, std::string y);

	//ScrollView滚动事件的回调
	void onSVCallback(Ref*, ui::ScrollView::EventType type);

	//ScrollView触摸事件的回调
	void onSVTouch(Ref *object, ui::Widget::TouchEventType type);

	void selectItem(int index);
	void selectItem(BackPackGridItem* block);

	void selectItem(CardView * cv);
	void refreshExhibi();
	void onEnter() override;
private:
	void initBlocks();
	ui::Layout* outerLayout;
	ui::Layout* exhibi_layout;
	ui::ImageView* exhibi_image;
	ui::Text* exhibi_name;
	ui::Text* exhibi_description;
	ui::Text* exhibi_consume;
	ui::ScrollView* sv;//滚动层
	//ui::ScrollView* sv_left;
	LinkedList<BackPackGridItem*>* block_list;
private:
	DBHelper* dbhelper;
	Player* player;
	ui::Layout* mask;
	//背包列表
	LinkedList<Card*>* itemBeans;
	//背包上限
	int capacity_limit = 27;
	int selectedIndex = 0;
	EventListenerCustom* bag_updateEventListener;//用于监听背包内容更新
};

