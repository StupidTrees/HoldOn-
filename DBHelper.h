
#pragma once
#include "cocos2d.h"
#include "sqlite3.h"
#include "Card.h"
#include <string>

class Checkpoint;
using namespace std;
/*数据库操作类*/
class DBHelper
{
public:
	DBHelper();
	~DBHelper();
	bool createTable(string tableName, string paramStr); //创建表（表名，各列参数声明）

	//获取所有卡牌
	LinkedList<Card*>* getAllCards();
	//按ID获取卡牌对象列表
	LinkedList<Card*>* getCardsWithId(LinkedList<int>* ids);
	//按ID获取效果对象列表
	LinkedList<Effect*>* getEffectsWithId(LinkedList<int>* ids);
	//按ID的获取一张卡
	Card* getCardById(int id);
	//按ID获取一个效果
	Effect * getEffect(int id);
	//按ID获取一个关卡
	Checkpoint* getCheckpointWithId(int id);
private:
	sqlite3* pdb; //数据库控制指针
	void openDB(string dbName); //打开数据库操作
	void closeDB(); //关闭数据库
};

