#include "DBHelper.h"
#include "Checkpoint.h"
DBHelper::DBHelper() //初始化过程中，打开或创建表
{
	openDB("cards");
	createTable("cards", "id integer primary key , type integer, name text, description text, consume integer, consume_type integer, effects text,effectsDir text");
	createTable("effects", "id integer primary key , name text, description text, type integer, trigger integer, code text,afterwards integer,mergeAble integer,life integer");
	createTable("checkpoint", "id integer primary key , type integet, bossLevel integer, hasComplete integer, visitAble integer, name text, description text, maxHP integer, maxMANA1 integer, maxMANA2 integer, Pile text, effect text, nextUnitId text,reward_exp integer,reward_coins integer,reward_cards text");

	//createTable("unit", "id integer primary key");
}


DBHelper::~DBHelper()
{
	closeDB(); 
}

bool DBHelper::createTable(string tableName,string paramStr)
{
	string sql = "create table if not exists " + tableName + "(" + paramStr + ")"; 
	// 执行sql语句：sqlite3_exec
	int ret = sqlite3_exec(pdb, sql.c_str(), nullptr, nullptr, nullptr);
	if (ret != SQLITE_OK) {
	//	CCLOG("create table failed");
		return false;
	}
	return true;

}


/*从数据库中获取所有卡牌*/
LinkedList<Card*>* DBHelper::getAllCards()
{
	LinkedList<Card*> * res= new LinkedList<Card*>;
	char** table; // 查询结果
	int r, c;     // 行数、列数

	string sql = "select * from cards"; 
	sqlite3_get_table(pdb, sql.c_str(), &table, &r, &c, nullptr);//执行查询语句

	cocos2d::log("行数 is %d , 列数 is %d", r, c);

	// 第0行（0 ~ c-1），为字段名
	// 第1行（c ~ 2*c-1），第一条记录
	// ......
	for (int i = 1; i <= r; i++) {

		char** row = table+i*c;
		Card* cd = new Card(this,row); //把row指针作为Card的构造函数参数传入，以该行各列的值初始化一个卡片对象
	//	CCLOG("%s", table[i * c + j]);
		//for (int j = 0; j < c; j++) {
		//	cocos2d::log("j:%d,%s",j,row[j]);
		//}
		res->add(cd);
		cocos2d::log("------------------------------");
	
	}
	sqlite3_free_table(table);
	return res;
}



/*由一串卡牌id获取卡牌*/
LinkedList<Card*>* DBHelper::getCardsWithId(LinkedList<int>* ids)
{
	LinkedList<Card*> * res = new LinkedList<Card*>;
	if (ids == nullptr || ids->size() == 0) return res;
	char** table; // 查询结果
	int r, c;     // 行数、列数

	for (int i = 0; i < ids->size(); i++) {
		string sql = "select * from cards where id="+mString::parseInt(ids->get(i));
		sqlite3_get_table(pdb, sql.c_str(), &table, &r, &c, nullptr);
		if(r>0&&c>0) {
			Card* cd = new Card(this, table+c);
			if(cd!=nullptr)	res->add(cd);
		}

	}
	sqlite3_free_table(table);
	return res;
}

/*由一串效果id获取效果*/
LinkedList<Effect*>* DBHelper::getEffectsWithId(LinkedList<int>* ids)
{
	LinkedList<Effect*>* res = new LinkedList<Effect*>;
	if (ids == nullptr || ids->size() == 0) return res;
	char** table; // 查询结果
	int r, c;     // 行数、列数

	for (int i = 0; i < ids->size(); i++) {
		string sql = "select * from effects where id=" + mString::parseInt(ids->get(i));
		sqlite3_get_table(pdb, sql.c_str(), &table, &r, &c, nullptr);
		if (r > 0 && c > 0) {
			Effect* cd = new Effect(table + c);
			if (cd != nullptr)	res->add(cd);
		}

	}
	sqlite3_free_table(table);
	return res;
}
Card* DBHelper::getCardById(int id)
{
	Card* result = nullptr;
	char** table; // 查询结果
	int r, c;     // 行数、列数
	string sql = "select * from cards where id=" + mString::parseInt(id);
	sqlite3_get_table(pdb, sql.c_str(), &table, &r, &c, nullptr);
		if (r > 0 && c > 0) {
			result = new Card(this, table + c);
	}
	sqlite3_free_table(table);
	return result;
}
/*由效果id获取效果对象*/
Effect* DBHelper::getEffect(int id)
{
	char** table; // 查询结果
	int r, c;     // 行数、列数
	string sql = "select * from effects  where id="+ mString::parseInt(id);
	sqlite3_get_table(pdb, sql.c_str(), &table, &r, &c, nullptr);
	char** row = table;
	if (r > 0 && c > 0) {
		Effect* ef = new Effect(row+c);
		// 记得是否查询表
		sqlite3_free_table(table);
		return ef;
	}
	return nullptr;

}

Checkpoint* DBHelper::getCheckpointWithId(int id)
{
	char** table;
	int r, c;
	string sql = "select * from checkpoint where id=" + mString::parseInt(id);
	sqlite3_get_table(pdb, sql.c_str(), &table, &r, &c, nullptr);
	char** row = table;
	if (r > 0 && c > 0) {
		Checkpoint* u = new Checkpoint(this,row + c);
		sqlite3_free_table(table);
		return u;
	}
	return nullptr;
}

/*打开数据库，打开结果保存在pdb中*/
void DBHelper::openDB(string dbName)
{
	pdb = nullptr;
	string fileName =dbName+".db";
	if (sqlite3_open(fileName.c_str(), &pdb) != SQLITE_OK) {
		sqlite3_close(pdb);
	}
}

void DBHelper::closeDB()
{
	sqlite3_close(pdb);
}


