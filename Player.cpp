#include "Player.h"
#include "AppDelegate.h"
#define UserDefault UserDefault::getInstance()
using namespace std;

Player::Player()
{
}


Player::~Player()
{
}

Player * Player::getInstanceFromDisk()
{
	Player* p = new Player;
	p->level = UserDefault->getIntegerForKey("player_level",1);
	p->exp_total = UserDefault->getIntegerForKey("player_exp_total",10);
	p->exp_current = UserDefault->getIntegerForKey("player_exp_current", 0);
	p->coins = UserDefault->getIntegerForKey("player_coins", 0);
	p->name = UserDefault->getStringForKey("player_name", "user0 user1 user2");
	p->cp1 = UserDefault->getIntegerForKey("cp1", 0);
	p->cp2 = UserDefault->getIntegerForKey("cp2", 1);
	p->cp3 = UserDefault->getIntegerForKey("cp3", 2);
	p->pile = new LinkedList<int>;
	mString pileStr = UserDefault->getStringForKey("player_pile", "26,11,27,27,27,5,28,4,2,6");
	auto x = pileStr.supersplit(",");
	for (int i = 0; i < x->size(); i++) {
		p->pile->add(x->get(i).asInt());
	}
	return p;
}

void Player::saveObjectToDisk()
{
	UserDefault->setIntegerForKey("player_level", level);
	UserDefault->setIntegerForKey("player_exp_total", exp_total);
	UserDefault->setIntegerForKey("player_exp_current", exp_current);
	UserDefault->setIntegerForKey("player_coins", coins);
	UserDefault->setStringForKey("player_name", name);
	UserDefault->setIntegerForKey("cp1", cp1);
	UserDefault->setIntegerForKey("cp2", cp2);
	UserDefault->setIntegerForKey("cp3", cp3);
	mString res = "";
	for (int i = 0; i < pile->size(); i++) {
		mString sl = i == pile->size() - 1 ? "":",";
		res.append(mString::parseInt(pile->get(i))).append(sl);
	}
	UserDefault->setStringForKey("player_pile", res);
	UserDefault->flush();
}

mString Player::getName()
{
	return name;
}

void Player::setName(mString n) {
	name = n;
}
int Player::getLevel()
{
	return level;
}

int Player::getCurExp()
{
	return exp_current;
}

int Player::getCp1()
{
	return cp1;
}

int Player::getCp2()
{
	return cp2;
}

int Player::getCp3()
{
	return cp3;
}

void Player::setCp1(int number)
{
	this->cp1 = number;
}

void Player::setCp2(int number)
{
	this->cp2 = number;
}

void Player::setCp3(int number)
{
	this->cp3 = number;
}

int Player::getTotExp()
{
	return exp_total;
}

int Player::getCoins()
{
	return coins;
}

void Player::setLevel(int newLevel)
{
	level = newLevel;
	exp_total = 10 + 8 * (newLevel - 1);
	exp_current = 0;
}

bool Player::gainExp(int exp)
{
	this->exp_current+=exp;
	if (exp_current > exp_total) {
		exp_current -= exp_total;
		exp_total += 8;
		setLevel(level + 1);
		return true;
	}
	return false;

}
void Player::gainCoins(int c)
{
	coins += c;
}
void Player::setCoins(int c)
{
	coins = c;
}
void Player::spendCoins(int c)
{
	coins -= c;
	if (coins < 0) coins = 0;
}
LinkedList<int>* Player::getPile()
{
	return pile;
}



