#pragma once
#include "DeskTop.h"

class BattleScene;
class Referee;
/*敌人AI类*/
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

	Card* chooseBestCard(LinkedList<Card*>* usableCards);//分析选出最佳牌
	Card* bestToWin(LinkedList<Card*>* usableCards);//策略1：我要赢
	Card* maxHurt(LinkedList<Card*>* usableCards);//策略2：最大伤害策略
	Card* minInjure(LinkedList<Card*>* usableCards);//策略3：最小受伤（回复）
	Card* cleanEffect(LinkedList<Card*>* usableCards);//策略4：减小不利延时效果
public:
	int flagGetCard;
	int flagDisCard;
private:
	BattleScene* scene;
	Referee* referee;
	DeskTop* desktop;
	LinkedList<Card*>* getUseableCards();
};

