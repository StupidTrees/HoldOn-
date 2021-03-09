#pragma once
#include "LinkedList.h"
#include "Card.h"
#include "mString.h"
#include <time.h>
using namespace std;
class Role //模拟桌面中的角色类
{
public:
	/*这个构造函数别直接用*/
	Role(mString name, mString description, int maxHP, int maxMANA, int maxMANA2);
	~Role();

	/*用这个静态函数获得Role对象*/
	static Role* create(mString name,mString description,int maxHP, int maxMANA, int maxMANA2);

	/*各种get和set*/
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
	/*抽n张牌*/
	void disCards(int n);
	/*洗牌*/
	void shuffle(LinkedList<Card*>* pile);
private:
	/*初始化函数*/
	bool init();
	mString name; //名字
	mString description; //描述
	int HP; //生命值
	int MANA1; //法力值1
	int MANA2; //法力值2
	int maxHP; //生命上限
	int maxMANA1; //法力1上限
	int maxMANA2; //法力2上限
	int level = 0; //角色等级
	LinkedList<Card*>* handPile; // 手牌区
	LinkedList<Card*>* drawPile; //抽牌区
	LinkedList<Card*>* discardPile; //弃牌区
	LinkedList<Effect*>* effectList;//效果区
	LinkedList<Effect*>* settlementQueue; //结算队列
};

