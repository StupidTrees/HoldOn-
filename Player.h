#pragma once
#include "../proj.win32/mString.h"
#include "LinkedList.h"
using namespace std;
class Player
{
public:
	Player();
	~Player();
	static Player* getInstanceFromDisk(); //从本地读取数据，初始化一个玩家对象
	void saveObjectToDisk(); //将玩家数据保存到本地
	/*各种get和set*/
	mString getName();
	void setName(mString n);
	int getLevel();
	int getCurExp();
	int getCp1();
	int getCp2();
	int getCp3();
	void setCp1(int number);
	void setCp2(int number);
	void setCp3(int number);
	int getTotExp();
	int getCoins();
	void setLevel(int newLevel);
	bool gainExp(int exp);
	void gainCoins(int c);
	void setCoins(int c);
	void spendCoins(int c);
	LinkedList<int>* getPile();

private :
	mString name;
	int level;
	int exp_current;
	int exp_total;
	int coins;
	int cp1, cp2, cp3; //记录关卡进度的三个整数
	LinkedList<int>* pile; //牌库id列表
};


