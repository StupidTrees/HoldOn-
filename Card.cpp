#include "pch.h"
#include "Card.h"
#include "DBHelper.h"
#include <sstream>


mString list2Str(LinkedList<Effect*>* lst) {
	mString res = "";
	stringstream ss;
	for (int i = 0; i < lst->size(); i++) {
		ss << lst->get(i)->getId();
		if (i == lst->size() - 1) res = res + ss.str();
		else  res = res + ss.str() + ",";
	}
	return res;

}




Card::~Card()
{
}

Card::Card(DBHelper* dbH,char ** row)
{
	istringstream idS(row[0]);
	idS >> id;
	istringstream typeS(row[1]);
	typeS>> type;
	name = row[2];
	description = row[3];
	istringstream consumeS(row[4]);
	consumeS >> consume;
	istringstream consume_typeS(row[5]);
	consume_typeS >> consume_type;
	effects = new LinkedList<Effect*>;
	effectsDir = new LinkedList<int>;
	str2EffectList(dbH, row[6]);
	str2EffectDirList(dbH, row[7]);
}

mString Card::getName()
{
	return name;
}
mString Card::getDescription()
{
	return description;
}


void Card::str2EffectList(DBHelper* dbH, mString effectStr) {
	auto idS = effectStr.supersplit(",");
	string s = effectStr;
	//mString test = "1 2 3 4";
	//auto testS = test.supersplit(" ");
	//cocos2d::log("effect:%s,size=%d", s.c_str(), testS->size());
	for (int i = 0; i < idS->size(); i++) {
		effects->add(dbH->getEffect(idS->get(i).asInt()));
	}
}
void Card::str2EffectDirList(DBHelper* dbH, mString effectStr)
{
	auto idS = effectStr.supersplit(",");
	string s = effectStr;
	for (int i = 0; i < idS->size(); i++) {
		effectsDir->add(idS->get(i).asInt());
	}
}


int Card::getType()
{
	return type;
}

int Card::getConsume()
{
	return consume;
}

int Card::getConsume_type()
{
	return consume_type;
}

int Card::getId()
{
	return id;
}

LinkedList<Effect*>* Card::getEffects()
{
	return effects;
}

LinkedList<int>* Card::getEffectsDir()
{
	return effectsDir;
}
