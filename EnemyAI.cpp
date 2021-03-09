#include "EnemyAI.h"
#include "BattleScene.h"
#include "Referee.h"
EnemyAI::EnemyAI(BattleScene* scene, Referee* referee, DeskTop* desktop)
{
	this->scene = scene;
	this->referee = referee;
	this->desktop = desktop;
}

void EnemyAI::startPlayingCards()
{
	flagGetCard = 1; flagDisCard = 0;//更改标志位，开始出牌
	keepPlayingCards();
}
void EnemyAI::keepPlayingCards() {
	if (referee->getGameState() != ONGOING) return;
	auto usableCards = getUseableCards();
	auto delay = DelayTime::create(0.3);
	auto fun = CallFunc::create([=]() {
			if (usableCards->size() > 0) {
				scene->enemyUseCard(getOneCardToUse(usableCards));
				keepPlayingCards();
			}
			else {
				referee->roundDiscardCard();
			}
		});
	auto seq = Sequence::create(delay, fun, NULL);
	scene->runAction(seq);
}
void EnemyAI::startDiscardingCards()
{
	keepDiscardingCards();
}
void EnemyAI::keepDiscardingCards()
{
	if (referee->getGameState() != ONGOING) return;
	if (referee->getDiscardNum(desktop->getP2())>0) {
		auto delay = DelayTime::create(0.3);
		auto fun = CallFunc::create([=]() {
			scene->enemyDiscardCard(getOneCardToDiscard(desktop->getP2()->getHandPile()));
			keepDiscardingCards();
			});
		auto seq = Sequence::create(delay, fun, NULL);
		scene->runAction(seq);
	}
	else {
		referee->roundEnds();
	}
}
Card* EnemyAI::getOneCardToUse(LinkedList<Card*>* usableCards) {
	if (usableCards->size() > 0) {
		//return usableCards->get(0);
		return chooseBestCard(usableCards);
	}
	return nullptr;
}

Card* EnemyAI::getOneCardToDiscard(LinkedList<Card*>* cards) {
	if (cards->size() > 0) return cards->get(0);
	return nullptr;
}


LinkedList<Card*>* EnemyAI::getUseableCards()
{
	auto result = new LinkedList<Card*>;
	auto handp = desktop->getP2()->getHandPile();
	for (int i = 0; i < handp->size(); i++) {
		auto c = handp->get(i);
		if (referee->canIUseThisCard(c, desktop->getP2(), desktop->getP1()) ){
			result->add(c);

		}
	}
	return result;

}


/*分析选出最佳牌*/
Card* EnemyAI::chooseBestCard(LinkedList<Card*>* usableCards)
{
	//investigate();
	auto playerHP = desktop->getP1()->getHP();
	auto playerMaxHP = desktop->getP1()->getMaxHP();
	auto playerEffect = desktop->getP1()->getEffectList();
	auto AIHP = desktop->getP2()->getHP();
	auto AIMaxHP = desktop->getP2()->getMaxHP();
	//auto AIMANA1 = desktop->getP2()->getMANA1();
	//auto AIMANA2 = desktop->getP2()->getMANA2();
	auto AIEffect = desktop->getP2()->getEffectList();
	//auto AIHandPile = usableCards;

	float AIHPProportion, playerHPProportion;//记录血量比例
	AIHPProportion = (float)AIHP / (float)AIMaxHP;
	playerHPProportion = (float)playerHP / (float)playerMaxHP;
	//log("player:%f,AI:%f", playerHPProportion, AIHPProportion);

	for (int i = 0; i < AIEffect->size(); i++) {
		if (AIEffect->get(i)->getId() == 20 || AIEffect->get(i)->getId() == 18) {
			log("策略四（发现不利延时效果）");
			return cleanEffect(usableCards);//特例：发现不利延时效果，调用策略四
		}
	}
	for (int i = 0; i < playerEffect->size(); i++) {
		if (playerEffect->get(i)->getId() == 14) {
			log("策略三（敌方有反甲）");
			return minInjure(usableCards);//特例：敌方有反甲，无法对其伤害，调用策略三
		}
	}
	if (playerHPProportion <= 0.2 && AIHPProportion <= 0.2) {		//双方同时低血量，拼命状态
		if (playerHP > AIHP) {
			if ((float)(playerHP - AIHP) / (float)AIMaxHP > 0.05) {	//保命要紧
				log("策略三（同时低血量，AI血量更少,且血量差值较大）");
				return minInjure(usableCards);
			}
			else if ((float)(playerHP - AIHP) / (float)AIMaxHP <= 0.05) {
				log("策略二（同时低血量，AI血量更少,且血量差值较小）");
				return maxHurt(usableCards);	//拼了！！！
			}
		}
		else if (playerHP <= AIHP) {
			if ((float)(AIHP - playerHP) / (float)AIMaxHP > 0.05) {
				log("策略二（同时低血量，玩家血量更少,且血量差值较大）");
				return maxHurt(usableCards);	//弄死他！！！
			}
			else {
				log("策略一（同时低血量，玩家血量更少,且血量差值较小）");
				return bestToWin(usableCards);	//稳住，我们能赢
			}
		}
	}
	else if (playerHPProportion <= 0.2 && AIHPProportion > 0.2) {
		log("策略二（玩家低血量）");
		return maxHurt(usableCards);	//弄死他！！！
	}
	else if (playerHPProportion > 0.2 && AIHPProportion <= 0.2) {
		log("策略三（AI低血量）");
		return minInjure(usableCards);		//保命要紧
	}
	else {
		log("策略一（常规策略）");
		return bestToWin(usableCards);
	}
}

/*******************************************
策略方法：
	弃牌：
		1、选取权值最小的牌
	出牌：
		1、扫描各自效果区，确定出牌
		2、根据不同策略决定最优出牌
********************************************/


/*策略一：我要赢*/
Card* EnemyAI::bestToWin(LinkedList<Card*>* usableCards)
{
	int level = 0;
	int minWeightCard = 0;
	int cardWeight[22] = { -1,
						  1,2,3,//伤害牌
						  0,0,//回复
						  2,//吸血
						  1,3,level,1,1,1,//摸牌，扔牌伤害，等级伤害，交换手牌弃牌，获得弃牌，一张手牌一张弃牌
						  4,-1,2,4,-1,
						  1,5,5,6 };
	if (flagDisCard == 1 && flagGetCard == 0) {		//弃牌阶段，返回权值最小
		for (int i = 0; i < usableCards->size() - 1; i++) {
			if (cardWeight[usableCards->get(i)->getId()] < cardWeight[usableCards->get(i + 1)->getId()]) {
				minWeightCard = i;
			}
		}
		log("No.%d", minWeightCard);
		return usableCards->get(minWeightCard);
	}

	else if (flagDisCard == 0 && flagGetCard == 1) {	//出牌阶段
		//人物等级不同，权值不同
		if (desktop->getP1()->getLevel() == 0) {
			level = 0;
		}
		else if (desktop->getP1()->getLevel() <= 1 && desktop->getP1()->getLevel() <= 2) {
			level = 1;
		}
		else if (desktop->getP1()->getLevel() > 2 && desktop->getP1()->getLevel() <= 4) {
			level = 2;
		}
		else if (desktop->getP1()->getLevel() > 4 && desktop->getP1()->getLevel() <= 10) {
			level = 3;
		}
		else if (desktop->getP1()->getLevel() > 10) {
			level = 4;
		}

		for (int i = 0; i < desktop->getP1()->getEffectList()->size(); i++) {	//如果发现爆炸伤害去加倍
			if ((desktop->getP1()->getEffectList()->get(i)->getId() == 20 || desktop->getP1()->getEffectList()->get(i)->getId() == 18)
				&& desktop->getP1()->getEffectList()->get(i)->getCountdown() <= 2) {

				for (int j = 0; j < usableCards->size(); j++) {
					if (usableCards->get(j)->getId() == 16) {
						log("No.%d  ID:16", j);
						return usableCards->get(j);
					}
				}
			}
		}
		for (int i = 0; i < desktop->getP1()->getEffectList()->size(); i++) {
			if (desktop->getP1()->getEffectList()->get(i)->getId() == 15) {
				for (int j = 0; j < usableCards->size(); j++) {
					if (usableCards->get(j)->getId() == 1) {
						log("No.%d  ID:1", j);
						return usableCards->get(j);
					}
				}
				for (int j = 0; j < usableCards->size(); j++) {
					if (usableCards->get(j)->getId() == 9) {
						log("No.%d  ID:9", j);
						return usableCards->get(j);
					}
				}
				for (int j = 0; j < usableCards->size(); j++) {
					if (usableCards->get(j)->getId() == 6
						|| usableCards->get(j)->getId() == 2) {
						log("No.%d  ID:6||2", j);
						return usableCards->get(j);
					}
				}
			}
		}

		//executionStrategy(cardWeight);
		for (int i = 0; i < usableCards->size() - 1; i++) {
			if (cardWeight[usableCards->get(i)->getId()] >= cardWeight[usableCards->get(i + 1)->getId()]) {
				cardWeight[0] = i;
			}
		}
		if (cardWeight[0] == -1) {
			log("No.1");
			return usableCards->get(0);
		}
		else {
			log("No.%d", cardWeight[0]);
			return usableCards->get(cardWeight[0]);
		}
	}
	else {
		log("WRONG!!!");
		return nullptr;
	}
}

/*策略二：最大伤害策略*/
Card* EnemyAI::maxHurt(LinkedList<Card*>* usableCards)
{
	int level = 0;
	int minWeightCard = 0;
	int cardWeight[22] = { -1,
						  4,5,6,//伤害牌
						  0,0,//回复
						  5,//吸血
						  3,6,level,3,3,3,//摸牌，扔牌伤害，等级伤害，交换手牌弃牌，获得弃牌，一张手牌一张弃牌
						  7,-1,2,7,-1,
						  3,4,3,3 };

	if (flagDisCard == 1 && flagGetCard == 0) {		//弃牌阶段，返回权值最小
		for (int i = 0; i < usableCards->size() - 1; i++) {
			if (cardWeight[usableCards->get(i)->getId()] < cardWeight[usableCards->get(i + 1)->getId()]) {
				minWeightCard = i;
			}
		}
		log("No.%d", minWeightCard);
		return usableCards->get(minWeightCard);
	}

	else if (flagDisCard == 0 && flagGetCard == 1) {
		if (desktop->getP1()->getLevel() == 0) {
			level = 0;
		}
		else if (desktop->getP1()->getLevel() <= 1 && desktop->getP1()->getLevel() <= 2) {
			level = 3;
		}
		else if (desktop->getP1()->getLevel() > 2 && desktop->getP1()->getLevel() <= 4) {
			level = 5;
		}
		else if (desktop->getP1()->getLevel() > 4 && desktop->getP1()->getLevel() <= 10) {
			level = 6;
		}
		else if (desktop->getP1()->getLevel() > 10) {
			level = 7;
		}

		for (int i = 0; i < desktop->getP1()->getEffectList()->size(); i++) {	//如果发现爆炸伤害去加倍
			if ((desktop->getP1()->getEffectList()->get(i)->getId() == 20 || desktop->getP1()->getEffectList()->get(i)->getId() == 18)
				&& desktop->getP1()->getEffectList()->get(i)->getCountdown() <= 2) {

				for (int j = 0; j < usableCards->size(); j++) {
					if (usableCards->get(j)->getId() == 16) {
						log("No.%d  ID:16", j);
						return usableCards->get(j);
					}
				}
			}
		}
		for (int i = 0; i < desktop->getP1()->getEffectList()->size(); i++) {
			if (desktop->getP1()->getEffectList()->get(i)->getId() == 15) {
				for (int j = 0; j < usableCards->size(); j++) {
					if (usableCards->get(j)->getId() == 1) {
						log("No.%d  ID:1", j);
						return usableCards->get(j);
					}
				}
				for (int j = 0; j < usableCards->size(); j++) {
					if (usableCards->get(j)->getId() == 9) {
						log("No.%d  ID:9", j);
						return usableCards->get(j);
					}
				}
				for (int j = 0; j < usableCards->size(); j++) {
					if (usableCards->get(j)->getId() == 6
						|| usableCards->get(j)->getId() == 2) {
						log("No.%d  ID:6||2", j);
						return usableCards->get(j);
					}
				}
			}
		}

		//executionStrategy(cardWeight);
		for (int i = 0; i < usableCards->size() - 1; i++) {
			if (cardWeight[usableCards->get(i)->getId()] >=
				cardWeight[usableCards->get(i + 1)->getId()]) {
				cardWeight[0] = i;
			}
		}
		if (cardWeight[0] == -1) {
			log("No.1");
			return usableCards->get(0);
		}
		else {
			log("No.%d", cardWeight[0]);
			return usableCards->get(cardWeight[0]);
		}
	}
	else {
		log("WRONG!!!");
		return nullptr;
	}
}

/*策略三：最小受伤（回复）*/
Card* EnemyAI::minInjure(LinkedList<Card*>* usableCards)
{
	int level = 1;
	int minWeightCard = 0;
	int cardWeight[22] = { -1,
						  1,1,1,//伤害牌
						  5,6,//回复
						  6,//吸血
						  3,1,level,3,3,3,//摸牌，扔牌伤害，等级伤害，交换手牌弃牌，获得弃牌，一张手牌一张弃牌
						  7,-1,2,7,-1,
						  3,4,5,4 };

	if (flagDisCard == 1 && flagGetCard == 0) {		//弃牌阶段，返回权值最小
		for (int i = 0; i < usableCards->size() - 1; i++) {
			if (cardWeight[usableCards->get(i)->getId()] < cardWeight[usableCards->get(i + 1)->getId()]) {
				minWeightCard = i;
			}
		}
		log("No.%d", minWeightCard);
		return usableCards->get(minWeightCard);
	}

	else if (flagDisCard == 0 && flagGetCard == 1) {
		for (int i = 0; i < desktop->getP1()->getEffectList()->size(); i++) {	//如果发现爆炸伤害去加倍
			if ((desktop->getP1()->getEffectList()->get(i)->getId() == 20 || desktop->getP1()->getEffectList()->get(i)->getId() == 18)
				&& desktop->getP1()->getEffectList()->get(i)->getCountdown() <= 2) {

				for (int j = 0; j < usableCards->size(); j++) {
					if (usableCards->get(j)->getId() == 16) {
						log("No.%d  ID:16", j);
						return usableCards->get(j);
					}
				}
			}
		}

		//executionStrategy(cardWeight);
		for (int i = 0; i < usableCards->size() - 1; i++) {
			if (cardWeight[usableCards->get(i)->getId()] >= cardWeight[usableCards->get(i + 1)->getId()]) {
				cardWeight[0] = i;
			}
		}
		if (cardWeight[0] == -1) {
			log("No.1");
			return usableCards->get(0);
		}
		else {
			log("No.%d", cardWeight[0]);
			return usableCards->get(cardWeight[0]);
		}
	}
	else {
		log("WRONG!!!");
		return nullptr;
	}
}

/*策略四：清除不利延时效果*/
Card* EnemyAI::cleanEffect(LinkedList<Card*>* usableCards)
{
	int level = 1;
	int minWeightCard = 0;
	int cardWeight[22] = { -1,
						  1,2,3,//伤害牌
						  1,2,//回复
						  3,//吸血
						  3,3,level,3,3,3,//摸牌，扔牌伤害，等级伤害，交换手牌弃牌，获得弃牌，一张手牌一张弃牌
						  7,-1,6,7,-1,
						  3,4,5,4 };

	if (flagDisCard == 1 && flagGetCard == 0) {		//弃牌阶段，返回权值最小
		for (int i = 0; i < usableCards->size() - 1; i++) {
			if (cardWeight[usableCards->get(i)->getId()] < cardWeight[usableCards->get(i + 1)->getId()]) {
				minWeightCard = i;
			}
		}
		log("No.%d", minWeightCard);
		return usableCards->get(minWeightCard);
	}

	else if (flagDisCard == 0 && flagGetCard == 1) {
		for (int i = 0; i < desktop->getP1()->getEffectList()->size(); i++) {	//如果发现爆炸伤害去加倍
			if ((desktop->getP1()->getEffectList()->get(i)->getId() == 20 || desktop->getP1()->getEffectList()->get(i)->getId() == 18)
				&& desktop->getP1()->getEffectList()->get(i)->getCountdown() <= 2) {

				for (int j = 0; j < usableCards->size(); j++) {
					if (usableCards->get(j)->getId() == 16) {
						log("No.%d  ID:16", j);
						return usableCards->get(j);
					}
				}
			}
		}

		//executionStrategy(cardWeight);
		for (int i = 0; i < usableCards->size() - 1; i++) {
			if (cardWeight[usableCards->get(i)->getId()] >= cardWeight[usableCards->get(i + 1)->getId()]) {
				cardWeight[0] = i;
			}
		}
		if (cardWeight[0] == -1) {
			log("No.1");
			return usableCards->get(0);
		}
		else {
			log("No.%d", cardWeight[0]);
			return usableCards->get(cardWeight[0]);
		}
	}
	else {
		log("WRONG!!!");
		return nullptr;
	}
}