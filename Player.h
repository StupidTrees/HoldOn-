#pragma once
#include "../proj.win32/mString.h"
#include "LinkedList.h"
using namespace std;
class Player
{
public:
	Player();
	~Player();
	static Player* getInstanceFromDisk(); //�ӱ��ض�ȡ���ݣ���ʼ��һ����Ҷ���
	void saveObjectToDisk(); //��������ݱ��浽����
	/*����get��set*/
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
	int cp1, cp2, cp3; //��¼�ؿ����ȵ���������
	LinkedList<int>* pile; //�ƿ�id�б�
};


