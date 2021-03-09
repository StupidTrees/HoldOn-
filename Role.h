#pragma once
#include "LinkedList.h"
#include "Card.h"
#include "mString.h"
#include <time.h>
using namespace std;
class Role //ģ�������еĽ�ɫ��
{
public:
	/*������캯����ֱ����*/
	Role(mString name, mString description, int maxHP, int maxMANA, int maxMANA2);
	~Role();

	/*�������̬�������Role����*/
	static Role* create(mString name,mString description,int maxHP, int maxMANA, int maxMANA2);

	/*����get��set*/
	LinkedList<Card*>* getHandPile();
	LinkedList<Card*>* getDrawPile();
	LinkedList<Card*>* getDiscardPile();
	LinkedList<Effect*>* getEffectList();
	LinkedList<Effect*>* getSettlementQueue();
	mString getName();
	mString getDescription();
	int getHP();
	void setHP(int newHp);
	void setMANA1(int newMana1);
	void setMANA2(int newMana2);
	int getMANA1();
	int getMANA2();
	int getMaxHP();
	int getMaxMANA1();
	int getMaxMANA2();
	int getLevel();
	void setMaxHP(int m);
	void setMaxMANA1(int m);
	void setMaxMANA2(int m);
	void setLevel(int l);
	void setName(mString name);
	void setDescription(mString description);
	LinkedList<Card*>* drawCards(int n);
	/*��n����*/
	void disCards(int n);
	/*ϴ��*/
	void shuffle(LinkedList<Card*>* pile);
private:
	/*��ʼ������*/
	bool init();
	mString name; //����
	mString description; //����
	int HP; //����ֵ
	int MANA1; //����ֵ1
	int MANA2; //����ֵ2
	int maxHP; //��������
	int maxMANA1; //����1����
	int maxMANA2; //����2����
	int level = 0; //��ɫ�ȼ�
	LinkedList<Card*>* handPile; // ������
	LinkedList<Card*>* drawPile; //������
	LinkedList<Card*>* discardPile; //������
	LinkedList<Effect*>* effectList;//Ч����
	LinkedList<Effect*>* settlementQueue; //�������
};

