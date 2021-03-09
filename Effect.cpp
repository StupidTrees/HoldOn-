#include"pch.h"
#include "Effect.h"
#include <sstream>
#include "Role.h"

Effect::Effect()
{
}

Effect::Effect(char ** row)
{
	//stringstream ss;
	//ss << row[0];
	//ss >> id;
	id = mString(row[0]).asInt();
	name = row[1];
	//cocos2d::log("name:%s", row[1]);
	description = row[2];
	mString typeS = row[3];
	type = typeS.asInt();
	trigger = mString(row[4]).asInt();
	code = row[5];
	rawCode = code;
	afterwards = mString(row[6]).asInt();
	mString s = row[7];
	mergeAble = s.asInt() == 1;
	life = mString(row[8]).asInt();
	countDown = mString(row[9]).asInt();
	cdTrigger = mString(row[10]).asInt();
}

Effect::Effect(Effect* e)
{
	id = e->getId();
	name = e->getName();
	//cocos2d::log("name:%s", row[1]);
	description = e->getDescription();
	type = e->getType();
	trigger = e->getTrigger();
	code = e->getCode();
	rawCode = e->getRawCode();
	afterwards = e->getAfterwards();
	mergeAble = e->isMergeAble();
	life =e->getLife();
	countDown = e->getCountdown();
	owner = e->getOwner();
	cdTrigger = e->getCDTrigger();
}



Effect::~Effect()
{
}

int Effect::getId()
{
	return id;
}

mString Effect::getName()
{
	return name;
}

mString Effect::getDescription()
{
	return description;
}

int Effect::getType()
{
	return type;
}

int Effect::getTrigger()
{
	return trigger;
}

mString Effect::getCode()
{
	return code;
}

int Effect::getAfterwards()
{
	return afterwards;
}

int Effect::getLife()
{
	return life;
}

bool Effect::isMergeAble()
{
	return mergeAble;
}

int Effect::getCountdown()
{
	return countDown;
}
int Effect::getCDTrigger()
{
	return cdTrigger;
}
void Effect::decCountDown()
{
	countDown--;
}
mString Effect::getRawCode()
{
	return rawCode;
}

void Effect::updateCode(mString newCode)
{
	this->code = newCode;
	string s = newCode;
	//cocos2d::log("updatecode:%s", s.c_str());
}

void Effect::resetCode()
{
	code = rawCode;
}

void Effect::setOwner(Role* r)
{
	this->owner = r;
}

Role* Effect::getOwner()
{
	return owner;
}

Effect* Effect::setParam(mString param)
{
	code.replaceAll("<PARA>", param);
	return this;
}

Effect* Effect::setCDParam(mString param)
{
	code.replaceAll("<CDPARA>", param);
	return this;
}

Effect* Effect::clone()
{
	Effect* e = new Effect(this);
	return e;
}


