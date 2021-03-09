#pragma once
#include "Role.h"
#include "mString.h"
class DBHelper;
using namespace std;
/*�ؿ���*/
class Checkpoint
{
public:
	Checkpoint();
	Checkpoint(DBHelper* dbH, char** row); //ʹ�����ݿ�ָ���ʼ��
	~Checkpoint();
	/*����get��set*/
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
	string name; //����
	string description; //����
	int maxHP; //��������
	int maxMANA1; //����1����
	int maxMANA2; //����2����
	LinkedList<int>* Pile;//�ƶ�
	LinkedList<int>* effect;//Ч��
	LinkedList<int>* rewardCards;//���ƽ���
	LinkedList<int>* nextUnitId;//������
};

