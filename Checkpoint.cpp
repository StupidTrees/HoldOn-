#include "Checkpoint.h"
#include "DBHelper.h"
#include <sstream>
Checkpoint::Checkpoint() {
}
Checkpoint::Checkpoint(DBHelper* dbH, char** row)
{
	id = mString(row[0]).asInt();
	type = mString(row[1]).asInt();
	bossLevel = mString(row[2]).asInt();
	hasComplete = mString(row[3]).asInt();
	visitAble = mString(row[4]).asInt();
	name = row[5];
	description = row[6];
	maxHP = mString(row[7]).asInt();
	maxMANA1 = mString(row[8]).asInt();
	maxMANA2 = mString(row[9]).asInt();
	Pile = new LinkedList<int>;
	effect = new LinkedList<int>;
	nextUnitId = new LinkedList<int>;
	rewardCards = new LinkedList<int>;
	//nextUnit = new LinkedList<unit*>;
	strInt(dbH, row[10], Pile);
	strInt(dbH, row[11], effect);
	strInt(dbH, row[12], nextUnitId);
	reward_exp = mString(row[13]).asInt();
	reward_coins = mString(row[14]).asInt();
	strInt(dbH, row[13], rewardCards);

}
void Checkpoint::strInt(DBHelper* dbH, mString intStr,LinkedList<int>* intS)
{
	auto idS = intStr.supersplit(",");
	string s = intStr;
	for (int i = 0; i < idS->size(); i++) {
		intS->add(idS->get(i).asInt());
	}
}

Checkpoint::~Checkpoint() {

}

int Checkpoint::getId() {
	return id;
}

int Checkpoint::getBossLevel() {
	return bossLevel;
}

int Checkpoint::getType()
{
	return type;
}

int Checkpoint::getMaxHP()
{
	return maxHP;
}

int Checkpoint::getMaxMANA1()
{
	return maxMANA1;
}

int Checkpoint::getMaxMANA2()
{
	return maxMANA2;
}

LinkedList<int>* Checkpoint::getPile()
{
	return Pile;
}

LinkedList<int>* Checkpoint::getEffect()
{
	return effect;
}

bool Checkpoint::getHasComplete()
{
	return hasComplete;
}

bool Checkpoint::getvisitAble()
{
	return visitAble;
}

mString Checkpoint::getName()
{
	return name;
}

mString Checkpoint::getDescription()
{
	return description;
}


LinkedList<int>* Checkpoint::getRewardCards()
{
	return rewardCards;
}
LinkedList<int>* Checkpoint::getNextUnitId()
{
	return nextUnitId;
}

int Checkpoint::getRewardCoins()
{
	return reward_coins;
}

int Checkpoint::getRewardExp()
{
	return reward_exp;
}

/*LinkedList<unit*>* unit::getNextUnit()
{
	return nextUnit;
}*/