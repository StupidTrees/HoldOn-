#pragma once
#include "Role.h"
#include "LinkedList.h"
/*模拟桌面类*/
class DeskTop
{
public:
	DeskTop();
	~DeskTop();
	Role* getP1();
	Role* getP2();
	/*用这个静态函数获得桌面对象*/
	static DeskTop* create();
	/*装载P1和P2的初始抽牌堆*/
	void loadCards(DBHelper * dbHelper, LinkedList<int>* p1Cards, LinkedList<int>* p2Cards);
	void loadEffects(DBHelper* dbHelper, LinkedList<int>* p1effct, LinkedList<int>* p2effcts);
	void loadEffects();
private:
	bool init();
	/*敌我*/
	Role* P1; 
	Role* P2;
};

