// TestDeskTop.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "pch.h"
#include <iostream>
#include "DeskTop.h"
#include <sstream>
#include "EffectExecutor.h"
#include "Referee.h"


void printRoleState(Role* r) {
	cout << "=======" << r->getName() << "========" << endl;
	cout << "HP:" << r->getHP() << "/" << r->getMaxHP() << " MANA:" << r->getMANA1() << "/" << r->getMaxMANA1() << endl;
	cout << "手牌区:";
	for (int i = 0; i < r->getHandPile()->size(); i++) cout << "【" << r->getHandPile()->get(i)->getName() <<"("<< r->getHandPile()->get(i)->getConsume()<<")】";
	cout << endl<<"抽牌堆:";
	for (int i = 0; i < r->getDrawPile()->size(); i++) cout << "【" << r->getDrawPile()->get(i)->getName() + "】 ";
	cout <<endl<< "弃牌堆:";
	for (int i = 0; i < r->getDiscardPile()->size(); i++) cout << "【" << r->getDiscardPile()->get(i)->getName() + "】 ";
	cout << endl << "效果区:";
	for (int i = 0; i < r->getEffectList()->size(); i++) cout << "（" << r->getEffectList()->get(i)->getName() + ") ";
	cout << endl;
}
void printTableState(mString opr,DeskTop* d) {
	cout <<"进行："<< opr <<"之后"<< endl;
	printRoleState(d->getP1());
	printRoleState(d->getP2());
	cout << endl<<endl<<endl;
}


int main()
{
	DeskTop* d = DeskTop::create();
	EffectExecutor* eexe = new EffectExecutor(d);
	Referee*rfr = new Referee(d, d->getP1(), d->getP2());
	Role* player = d->getP1();
	Role* enemy = d->getP2();
	d->loadCards();//为两个角色分配初始牌到各自抽牌堆
	d->loadEffects();//为两个角色分配初始效果
	//player->drawCards(8); //两人分别抽八张牌
	//enemy->drawCards(8); 
	player->shuffle(player->getDrawPile());//两人分别洗牌
	enemy->shuffle(enemy->getDrawPile());
	//enemy->disCards(5);
	printTableState("初始状态",d);
	rfr->getReady();
	rfr->start();
	//mString codes = "ADD M:HP,12<self-recover>\nLEA PTRC0,E:HANDP\nADD PTRC0,4\nMOV M:HANDP,PTRC0\nSUB E:HP,5<harm>";
	//eexe->execute(code, d->getP1(), d->getP2());\
	
}