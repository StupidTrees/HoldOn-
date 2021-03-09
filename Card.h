#pragma once
#include "Effect.h"
#include "LinkedList.h"
#include "mString.h"

class DBHelper;
using namespace std;
/*卡牌类*/
class Card {
public:
	Card();
	Card(DBHelper * dbH, char ** row); //使用数据库指针进行初始化
	~Card();
	mString getName();
	mString getDescription();
	int getType();
	int getConsume();
	int getConsume_type();
	int getId();
	LinkedList<Effect*>* getEffects();
	LinkedList<int>* getEffectsDir();
	/*将字符串转化为整数型列表*/
	void str2EffectList(DBHelper * dbH, mString effectStr);
	void str2EffectDirList(DBHelper* dbH, mString effectStr);


private:
	int id;
	mString name; //卡牌名
	mString description; //卡牌描述
	int type; //卡牌类别
	int consume; //消耗
	int consume_type; //消耗类别
	LinkedList<Effect*>* effects; //效果表
	LinkedList<int>* effectsDir; //效果的“去向”表
};

