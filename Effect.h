#pragma once
#include "mString.h"
#include "cocos2d.h"

class Role;
using namespace std;
/*Ч����*/
class Effect
{
public:
	Effect();
	Effect(char ** row);
	Effect(Effect* e);
	~Effect();
	int getId();
	mString getName();
	mString getDescription();
	int getType();
	int getTrigger();
	mString getCode();
    int getAfterwards();
	int getLife();
	bool isMergeAble();
	int getCountdown();
	int getCDTrigger();
	void decCountDown();
	mString getRawCode();
	void updateCode(mString newCode);
	void resetCode();
	void setOwner(Role* r);
	Role* getOwner();
	Effect* setParam(mString param);
	Effect* setCDParam(mString param);
	Effect* clone();
private:
	/* �̶����� */
	int id; //Ψһ��ʶ��
	mString name; //����
	mString description; //������
	int type; //���
	int trigger; //��������
	mString code; //ִ�в�������
	mString rawCode;//��ʼ����
	int afterwards; //ִ�к���
	bool mergeAble; //�Ƿ�֧�ֵ���
	int life; //����������/����/��ʱ
	int countDown; //��Ϊ��ʱ�ƣ�����ʱ
	int cdTrigger;

	/*��Ϸ�в���*/
	Role* owner;//��˭ʹ�õ�
};

