#pragma once
#include "cocos2d.h"
#include "DeskTop.h"
#include "Referee.h"
#include"CardView.h"
#include "ui/UIText.h"
#include "ui/UIButton.h"
#include "uidimens.h"
#include "DBHelper.h"
#include "RoleView.h"
#include "EnemyAI.h"
#include "EffectView.h"
#include "Player.h"
USING_NS_CC;
using namespace ui;
class Checkpoint;

/*单场战斗场景*/
class BattleScene:
	public cocos2d::Scene
{
public:
	BattleScene();
	BattleScene(Player* player,DBHelper* dbhelper,Checkpoint* n);
	~BattleScene();
	/*初始化，传入玩家关卡信息及数据库操作对象*/
	static BattleScene* create(Player* player,DBHelper* dbhelper,Checkpoint* u);
	bool init(); //初始化


	void setHint(mString hint); //设置提示文本
	void refreshHandpState(); //刷新手牌区状态：可否出牌
	void refreshBoardInfo(); //刷新各数值ui
	void notifyMHandCardChanged();//根据手牌的Card列表，更新ui中的手牌CardView列表，并执行动画
	void notifyEHandCardChanged();
	void moveAllMHandCardsToSuitablePlace(); //把UI中玩家所有手牌移动到适当的位置
	void moveAllEHandCardsToSuitablePlace();  //把UI中敌人所有手牌移动到适当的位置
	void enemyDiscardCard(Card* card);  //敌人弃牌效果
	void enemyUseCard(Card* card); //敌人出牌效果
	void notifyEffectChanges(); //刷新buff区状态
	void popUpCountGameDialog(bool win); //游戏结束，弹出结算窗口

	Text* hintText; //提示文本
	Text* roundText; //显示回合数的文本
	Button* playcardFinishButton;
	DeskTop* desktop; //桌面对象
	Referee* referee; //裁判对象
	EnemyAI* enemyAI; //敌人AI对象
	RoleView* enemyRoleView; //敌人卡片
	RoleView* playerRoleView; //玩家卡片


private:
	void initDesktopInfos();
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event); //卡牌被抓住
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unused_event);//卡牌拖动
	void onTouchEnd(cocos2d::Touch* touch, cocos2d::Event* unused_event); //卡牌拖动结束
	void notifyMHandCardFloated(int pos); //当手牌被抓住时，提醒其他牌占据那张牌的位置
	void onMouseMovedOnHandP(Event* event); //当鼠标在手牌区上方移动时调用
	void onMouseMovedOnEEQ(Event* event);
	void onMouseMovedOnMEQ(Event* event);
	void backToMainScene(Ref* sender); //回到主菜单
	void testOutput(Ref* sender); //*输出桌面
	void onEnter() override;

	Player* player; //玩家：用于连接与结算
	DBHelper* dbhelper;
	Checkpoint* checkpoint;
	Layer* mHandCardLayer; //手牌堆的图层
	Layer* eHandCardLayer; //敌方手牌堆的图层
	Layer* effectMLayer;
	Layer* effectELayer;
	LinkedList<CardView*>* mHandCardList; //ui中的手牌CardView列表
	LinkedList<CardView*>* eHandCardList; //ui中的敌方手牌CardView列表
	LinkedList<EffectView*>* effectMList;
	LinkedList<EffectView*>* effectEList; 
	/*手牌拖动相关辅助变量*/
	bool hasFocus = false; //是否有抓住的手牌
	CardView* focusedCard = nullptr; //被抓住的手牌
	CardView* highlightedCard = nullptr; //鼠标悬停的卡牌
	Point focusedLastPos; //手牌拖动过程中，记录上一时刻卡牌的位置
	Point touchLastPos; //手牌拖动过程中，记录上一时刻鼠标的位置
	Point focusedBeginPos;//记录卡牌在被抓住时的初始屏幕位置
	int focusedBeginZOrder; //记录卡牌在被抓住时的初始z顺序（牌堆中的上下顺序）
	int focusedBeginIndex;//记录卡牌在被抓住时的初始下标（牌堆里的位置）

};