#include "pch.h"
#include "DeskTop.h"
#include <sstream> 
#include "DBHelper.h"
DeskTop::DeskTop()
{
}


DeskTop::~DeskTop()
{
}

Role * DeskTop::getP1()
{
	return P1;
}
Role * DeskTop::getP2()
{
	return P2;
}

DeskTop * DeskTop::create()
{
	DeskTop* dt = new DeskTop;
	if (dt&&dt->init()) {
		return dt;
	}
	else {
		delete dt;
		dt = nullptr;
		return nullptr;
	}
	return nullptr;
}

void DeskTop::loadCards(DBHelper* dbHelper,LinkedList<int>* p1Cards, LinkedList<int>* p2Cards)
{
	LinkedList<Card*>* l1 = P1->getDrawPile();
	LinkedList<Card*>* l2 = P2->getDrawPile();
	l1->transportAll(dbHelper->getCardsWithId(p1Cards));
	l2->transportAll(dbHelper->getCardsWithId(p2Cards));
}
void DeskTop::loadEffects(DBHelper* dbHelper, LinkedList<int>* p1effct, LinkedList<int>* p2effcts)
{
	LinkedList<Effect*>* l1 = P1->getEffectList();
	LinkedList<Effect*>* l2 = P2->getEffectList();
	l1->transportAll(dbHelper->getEffectsWithId(p1effct));
	l2->transportAll(dbHelper->getEffectsWithId(p2effcts));
}
void DeskTop::loadEffects()
{
	//LinkedList<Effect*>* l1 = P1->getEffectList();
	//for (int i = 0; i < 4; i++) {
	//	stringstream ss;
	//	ss << i;
	//	l1->add(new Effect("敌人带的效果" + ss.str(), "描述"));
	//}
	//LinkedList<Effect*>* l2 = P2->getEffectList();
	//for (int i = 0; i < 4; i++) {
	//	stringstream ss;
	//	ss << i;
	//	l2->add(new Effect("玩家带的效果" + ss.str(), "描述"));
	//}
}


bool DeskTop::init()
{
	//创建角色区
	P2 = Role::create("Bad Guy", "I'm the bad guy", 100,4, 4);
	P1 = Role::create("The HERO", "ooooh", 100, 4, 4);
	return true;
}
