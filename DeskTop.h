#pragma once
#include "Role.h"
#include "LinkedList.h"
/*ģ��������*/
class DeskTop
{
public:
	DeskTop();
	~DeskTop();
	Role* getP1();
	Role* getP2();
	/*�������̬��������������*/
	static DeskTop* create();
	/*װ��P1��P2�ĳ�ʼ���ƶ�*/
	void loadCards(DBHelper * dbHelper, LinkedList<int>* p1Cards, LinkedList<int>* p2Cards);
	void loadEffects(DBHelper* dbHelper, LinkedList<int>* p1effct, LinkedList<int>* p2effcts);
	void loadEffects();
private:
	bool init();
	/*����*/
	Role* P1; 
	Role* P2;
};

