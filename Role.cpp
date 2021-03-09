#include"pch.h"
#include "Role.h"




Role::Role(mString name, mString description, int maxHP, int maxMANA1, int maxMANA2)
{
	this->name = name;
	this->description = description;
    this ->maxHP = maxHP;
	this->maxMANA1 = maxMANA1;
	this->maxMANA2 = maxMANA2;
}

Role::~Role()
{
}

int Role::getHP()
{
	return HP;
}
void Role::setHP(int newHp)
{
	if (newHp < 0) HP = 0;
	else if (newHp > maxHP) HP = maxHP;
	else HP = newHp;
}

void Role::setMANA1(int newMana1) {
	if (newMana1 < 0) this->MANA1 = 0;
	else if (newMana1 > maxMANA1) this->MANA1 = maxMANA1;
	else this->MANA1 = newMana1;
}

void Role::setMANA2(int newMana2) {
	if (newMana2 < 0) this->MANA2 = 0;
	else if (newMana2 > maxMANA2) this->MANA2 = maxMANA2;
	else this->MANA2 = newMana2;
}
int Role::getMANA1()
{
	return MANA1;
}

int Role::getMANA2()
{
	return MANA2;
}

int Role::getMaxHP()
{
	return maxHP;
}

int Role::getMaxMANA1()
{
	return maxMANA1;
}

int Role::getMaxMANA2()
{
	return maxMANA2;
}

int Role::getLevel()
{
	return level;
}

void Role::setMaxHP(int m)
{
	maxHP = m;
}

void Role::setMaxMANA1(int m)
{
	maxMANA1 = m;
}

void Role::setMaxMANA2(int m)
{
	maxMANA2 = m;
}

void Role::setLevel(int l)
{
	level = l;
}

void Role::setName(mString name)
{
	this->name = name;
}

void Role::setDescription(mString description)
{
	this->description = description;
}

LinkedList<Card*>* Role::drawCards(int n)
{
	auto res = new LinkedList<Card*>;
	if (drawPile->size()<n) { //Èô³éÅÆ¶ÑÒÑ±»³éÍê£¬Ôò½«ÆúÅÆ¶ÑÏ´ÅÆºó¼ÓÈë³éÅÆ¶Ñ
		shuffle(discardPile);
		drawPile->transportAll(discardPile);
	}
	int dred = 0;
	for (int i = 0; i < n; i++) {
		if (drawPile->size() > 0) {
			Card* c = drawPile->remove(0);
			if (c != nullptr) {
				handPile->add(c);
				res->add(c);
				dred++;
			}

		}
	}
//	cocos2d::log("drawed:%d,drp:%d,dsp:%d,hdp:%d", dred, drawPile->size(), discardPile->size(), handPile->size());
	return res;
}
void Role::disCards(int n){
	for (int i = 0; i < n; i++) {
		if (handPile->size() > 0) discardPile->add(handPile->remove(0));
	}
}

void Role::shuffle(LinkedList<Card*>* pile)
{
	if (pile -> size() == 0) return;
	srand(time(NULL));
	for (int i = 0; i < pile->size(); i++) {
		int index = rand() % pile->size();
		pile->swap(i,index);
	}

}

Role * Role::create(mString name, mString description, int maxHP, int maxMANA, int maxMANA2)
{
	Role* r = new Role(name,description,maxHP,maxMANA,maxMANA2);
	if (r&&r->init()) {
		return r;
	}
	else {
		delete r;
		r = nullptr;
		return nullptr;
	}
	return nullptr;
}



LinkedList<Card*>* Role::getHandPile()
{
	return handPile;
}

LinkedList<Card*>* Role::getDrawPile()
{
	return drawPile;
}

LinkedList<Card*>* Role::getDiscardPile()
{
	return discardPile;
}

LinkedList<Effect*>* Role::getEffectList()
{
	return effectList;
}

LinkedList<Effect*>* Role::getSettlementQueue()
{
	return settlementQueue;
}

mString Role::getName()
{
	return name;
}

mString Role::getDescription()
{
	return description;
}

bool Role::init()
{
	handPile = new LinkedList<Card*>;
	discardPile = new LinkedList<Card*>;
	drawPile = new LinkedList<Card*>;
	effectList = new LinkedList<Effect*>;
	settlementQueue = new LinkedList<Effect*>;
	HP = maxHP;
	MANA1 = maxMANA1;
	MANA2 = maxMANA2;
	return handPile != nullptr&&discardPile != nullptr&&drawPile != nullptr&&effectList != nullptr&&settlementQueue != nullptr;
}
