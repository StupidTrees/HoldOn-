#pragma once
#include "Effect.h"
#include "LinkedList.h"
#include "mString.h"

class DBHelper;
using namespace std;
/*������*/
class Card {
public:
	Card();
	Card(DBHelper * dbH, char ** row); //ʹ�����ݿ�ָ����г�ʼ��
	~Card();
	mString getName();
	mString getDescription();
	int getType();
	int getConsume();
	int getConsume_type();
	int getId();
	LinkedList<Effect*>* getEffects();
	LinkedList<int>* getEffectsDir();
	/*���ַ���ת��Ϊ�������б�*/
	void str2EffectList(DBHelper * dbH, mString effectStr);
	void str2EffectDirList(DBHelper* dbH, mString effectStr);


private:
	int id;
	mString name; //������
	mString description; //��������
	int type; //�������
	int consume; //����
	int consume_type; //�������
	LinkedList<Effect*>* effects; //Ч����
	LinkedList<int>* effectsDir; //Ч���ġ�ȥ�򡱱�
};

