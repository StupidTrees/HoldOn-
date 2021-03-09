#pragma once
#include "DeskTop.h"

class BattleScene;
class Referee;
/*����AI��*/
class EnemyAI
{
public:
	EnemyAI(BattleScene* scene, Referee* referee, DeskTop* desktop);
	void startPlayingCards();
	void keepPlayingCards();
	void startDiscardingCards();
	void keepDiscardingCards();
	Card* getOneCardToUse(LinkedList<Card*>* usableCards);
	Card* getOneCardToDiscard(LinkedList<Card*>* cards);

	Card* chooseBestCard(LinkedList<Card*>* usableCards);//����ѡ�������
	Card* bestToWin(LinkedList<Card*>* usableCards);//����1����ҪӮ
	Card* maxHurt(LinkedList<Card*>* usableCards);//����2������˺�����
	Card* minInjure(LinkedList<Card*>* usableCards);//����3����С���ˣ��ظ���
	Card* cleanEffect(LinkedList<Card*>* usableCards);//����4����С������ʱЧ��
public:
	int flagGetCard;
	int flagDisCard;
private:
	BattleScene* scene;
	Referee* referee;
	DeskTop* desktop;
	LinkedList<Card*>* getUseableCards();
};

