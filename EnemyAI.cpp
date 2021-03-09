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
	flagGetCard = 1; flagDisCard = 0;//���ı�־λ����ʼ����
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


/*����ѡ�������*/
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

	float AIHPProportion, playerHPProportion;//��¼Ѫ������
	AIHPProportion = (float)AIHP / (float)AIMaxHP;
	playerHPProportion = (float)playerHP / (float)playerMaxHP;
	//log("player:%f,AI:%f", playerHPProportion, AIHPProportion);

	for (int i = 0; i < AIEffect->size(); i++) {
		if (AIEffect->get(i)->getId() == 20 || AIEffect->get(i)->getId() == 18) {
			log("�����ģ����ֲ�����ʱЧ����");
			return cleanEffect(usableCards);//���������ֲ�����ʱЧ�������ò�����
		}
	}
	for (int i = 0; i < playerEffect->size(); i++) {
		if (playerEffect->get(i)->getId() == 14) {
			log("���������з��з��ף�");
			return minInjure(usableCards);//�������з��з��ף��޷������˺������ò�����
		}
	}
	if (playerHPProportion <= 0.2 && AIHPProportion <= 0.2) {		//˫��ͬʱ��Ѫ����ƴ��״̬
		if (playerHP > AIHP) {
			if ((float)(playerHP - AIHP) / (float)AIMaxHP > 0.05) {	//����Ҫ��
				log("��������ͬʱ��Ѫ����AIѪ������,��Ѫ����ֵ�ϴ�");
				return minInjure(usableCards);
			}
			else if ((float)(playerHP - AIHP) / (float)AIMaxHP <= 0.05) {
				log("���Զ���ͬʱ��Ѫ����AIѪ������,��Ѫ����ֵ��С��");
				return maxHurt(usableCards);	//ƴ�ˣ�����
			}
		}
		else if (playerHP <= AIHP) {
			if ((float)(AIHP - playerHP) / (float)AIMaxHP > 0.05) {
				log("���Զ���ͬʱ��Ѫ�������Ѫ������,��Ѫ����ֵ�ϴ�");
				return maxHurt(usableCards);	//Ū����������
			}
			else {
				log("����һ��ͬʱ��Ѫ�������Ѫ������,��Ѫ����ֵ��С��");
				return bestToWin(usableCards);	//��ס��������Ӯ
			}
		}
	}
	else if (playerHPProportion <= 0.2 && AIHPProportion > 0.2) {
		log("���Զ�����ҵ�Ѫ����");
		return maxHurt(usableCards);	//Ū����������
	}
	else if (playerHPProportion > 0.2 && AIHPProportion <= 0.2) {
		log("��������AI��Ѫ����");
		return minInjure(usableCards);		//����Ҫ��
	}
	else {
		log("����һ��������ԣ�");
		return bestToWin(usableCards);
	}
}

/*******************************************
���Է�����
	���ƣ�
		1��ѡȡȨֵ��С����
	���ƣ�
		1��ɨ�����Ч������ȷ������
		2�����ݲ�ͬ���Ծ������ų���
********************************************/


/*����һ����ҪӮ*/
Card* EnemyAI::bestToWin(LinkedList<Card*>* usableCards)
{
	int level = 0;
	int minWeightCard = 0;
	int cardWeight[22] = { -1,
						  1,2,3,//�˺���
						  0,0,//�ظ�
						  2,//��Ѫ
						  1,3,level,1,1,1,//���ƣ������˺����ȼ��˺��������������ƣ�������ƣ�һ������һ������
						  4,-1,2,4,-1,
						  1,5,5,6 };
	if (flagDisCard == 1 && flagGetCard == 0) {		//���ƽ׶Σ�����Ȩֵ��С
		for (int i = 0; i < usableCards->size() - 1; i++) {
			if (cardWeight[usableCards->get(i)->getId()] < cardWeight[usableCards->get(i + 1)->getId()]) {
				minWeightCard = i;
			}
		}
		log("No.%d", minWeightCard);
		return usableCards->get(minWeightCard);
	}

	else if (flagDisCard == 0 && flagGetCard == 1) {	//���ƽ׶�
		//����ȼ���ͬ��Ȩֵ��ͬ
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

		for (int i = 0; i < desktop->getP1()->getEffectList()->size(); i++) {	//������ֱ�ը�˺�ȥ�ӱ�
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

/*���Զ�������˺�����*/
Card* EnemyAI::maxHurt(LinkedList<Card*>* usableCards)
{
	int level = 0;
	int minWeightCard = 0;
	int cardWeight[22] = { -1,
						  4,5,6,//�˺���
						  0,0,//�ظ�
						  5,//��Ѫ
						  3,6,level,3,3,3,//���ƣ������˺����ȼ��˺��������������ƣ�������ƣ�һ������һ������
						  7,-1,2,7,-1,
						  3,4,3,3 };

	if (flagDisCard == 1 && flagGetCard == 0) {		//���ƽ׶Σ�����Ȩֵ��С
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

		for (int i = 0; i < desktop->getP1()->getEffectList()->size(); i++) {	//������ֱ�ը�˺�ȥ�ӱ�
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

/*����������С���ˣ��ظ���*/
Card* EnemyAI::minInjure(LinkedList<Card*>* usableCards)
{
	int level = 1;
	int minWeightCard = 0;
	int cardWeight[22] = { -1,
						  1,1,1,//�˺���
						  5,6,//�ظ�
						  6,//��Ѫ
						  3,1,level,3,3,3,//���ƣ������˺����ȼ��˺��������������ƣ�������ƣ�һ������һ������
						  7,-1,2,7,-1,
						  3,4,5,4 };

	if (flagDisCard == 1 && flagGetCard == 0) {		//���ƽ׶Σ�����Ȩֵ��С
		for (int i = 0; i < usableCards->size() - 1; i++) {
			if (cardWeight[usableCards->get(i)->getId()] < cardWeight[usableCards->get(i + 1)->getId()]) {
				minWeightCard = i;
			}
		}
		log("No.%d", minWeightCard);
		return usableCards->get(minWeightCard);
	}

	else if (flagDisCard == 0 && flagGetCard == 1) {
		for (int i = 0; i < desktop->getP1()->getEffectList()->size(); i++) {	//������ֱ�ը�˺�ȥ�ӱ�
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

/*�����ģ����������ʱЧ��*/
Card* EnemyAI::cleanEffect(LinkedList<Card*>* usableCards)
{
	int level = 1;
	int minWeightCard = 0;
	int cardWeight[22] = { -1,
						  1,2,3,//�˺���
						  1,2,//�ظ�
						  3,//��Ѫ
						  3,3,level,3,3,3,//���ƣ������˺����ȼ��˺��������������ƣ�������ƣ�һ������һ������
						  7,-1,6,7,-1,
						  3,4,5,4 };

	if (flagDisCard == 1 && flagGetCard == 0) {		//���ƽ׶Σ�����Ȩֵ��С
		for (int i = 0; i < usableCards->size() - 1; i++) {
			if (cardWeight[usableCards->get(i)->getId()] < cardWeight[usableCards->get(i + 1)->getId()]) {
				minWeightCard = i;
			}
		}
		log("No.%d", minWeightCard);
		return usableCards->get(minWeightCard);
	}

	else if (flagDisCard == 0 && flagGetCard == 1) {
		for (int i = 0; i < desktop->getP1()->getEffectList()->size(); i++) {	//������ֱ�ը�˺�ȥ�ӱ�
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