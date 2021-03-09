#pragma once
#include "Role.h"
#include "mString.h"
class DBHelper;
using namespace std;
/*关卡类*/
class Checkpoint
{
public:
	Checkpoint();
	Checkpoint(DBHelper* dbH, char** row); //使用数据库指针初始化
	~Checkpoint();
	/*各种get和set*/
	int getId();
	int getBossLevel();
	int getType();
	int getMaxHP();
	int getMaxMANA1();
	int getMaxMANA2();
	LinkedList<int>* getPile();
	LinkedList<int>* getEffect();
	bool getHasComplete();
	bool getvisitAble();
	mString getName();
	mString getDescription();
	LinkedList<int>* getRewardCards();
	LinkedList<int>* getNextUnitId();
	int getRewardCoins();
	int getRewardExp();
	void strInt(DBHelper* dbH, mString intStr, LinkedList<int>* intS);
private:
	int id;
	int type;
	int bossLevel;
	int reward_coins;
	int reward_exp;
	bool hasComplete;
	bool visitAble;
	string name; //名字
	string description; //描述
	int maxHP; //生命上限
	int maxMANA1; //法力1上限
	int maxMANA2; //法力2上限
	LinkedList<int>* Pile;//牌堆
	LinkedList<int>* effect;//效果
	LinkedList<int>* rewardCards;//卡牌奖励
	LinkedList<int>* nextUnitId;//接下来
};

