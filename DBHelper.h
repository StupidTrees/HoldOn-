
#pragma once
#include "cocos2d.h"
#include "sqlite3.h"
#include "Card.h"
#include <string>

class Checkpoint;
using namespace std;
/*���ݿ������*/
class DBHelper
{
public:
	DBHelper();
	~DBHelper();
	bool createTable(string tableName, string paramStr); //���������������в���������

	//��ȡ���п���
	LinkedList<Card*>* getAllCards();
	//��ID��ȡ���ƶ����б�
	LinkedList<Card*>* getCardsWithId(LinkedList<int>* ids);
	//��ID��ȡЧ�������б�
	LinkedList<Effect*>* getEffectsWithId(LinkedList<int>* ids);
	//��ID�Ļ�ȡһ�ſ�
	Card* getCardById(int id);
	//��ID��ȡһ��Ч��
	Effect * getEffect(int id);
	//��ID��ȡһ���ؿ�
	Checkpoint* getCheckpointWithId(int id);
private:
	sqlite3* pdb; //���ݿ����ָ��
	void openDB(string dbName); //�����ݿ����
	void closeDB(); //�ر����ݿ�
};

