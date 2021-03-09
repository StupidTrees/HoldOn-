#pragma once
#include "cocos2d.h"
#include "DeskTop.h"
#include "Referee.h"
#include"CardView.h"
#include "ui/UIText.h"
#include "ui/UIButton.h"
#include "uidimens.h"
#include "DBHelper.h"
#include "RoleView.h"
#include "EnemyAI.h"
#include "EffectView.h"
#include "Player.h"
USING_NS_CC;
using namespace ui;
class Checkpoint;

/*����ս������*/
class BattleScene:
	public cocos2d::Scene
{
public:
	BattleScene();
	BattleScene(Player* player,DBHelper* dbhelper,Checkpoint* n);
	~BattleScene();
	/*��ʼ����������ҹؿ���Ϣ�����ݿ��������*/
	static BattleScene* create(Player* player,DBHelper* dbhelper,Checkpoint* u);
	bool init(); //��ʼ��


	void setHint(mString hint); //������ʾ�ı�
	void refreshHandpState(); //ˢ��������״̬���ɷ����
	void refreshBoardInfo(); //ˢ�¸���ֵui
	void notifyMHandCardChanged();//�������Ƶ�Card�б�����ui�е�����CardView�б���ִ�ж���
	void notifyEHandCardChanged();
	void moveAllMHandCardsToSuitablePlace(); //��UI��������������ƶ����ʵ���λ��
	void moveAllEHandCardsToSuitablePlace();  //��UI�е������������ƶ����ʵ���λ��
	void enemyDiscardCard(Card* card);  //��������Ч��
	void enemyUseCard(Card* card); //���˳���Ч��
	void notifyEffectChanges(); //ˢ��buff��״̬
	void popUpCountGameDialog(bool win); //��Ϸ�������������㴰��

	Text* hintText; //��ʾ�ı�
	Text* roundText; //��ʾ�غ������ı�
	Button* playcardFinishButton;
	DeskTop* desktop; //�������
	Referee* referee; //���ж���
	EnemyAI* enemyAI; //����AI����
	RoleView* enemyRoleView; //���˿�Ƭ
	RoleView* playerRoleView; //��ҿ�Ƭ


private:
	void initDesktopInfos();
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event); //���Ʊ�ץס
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unused_event);//�����϶�
	void onTouchEnd(cocos2d::Touch* touch, cocos2d::Event* unused_event); //�����϶�����
	void notifyMHandCardFloated(int pos); //�����Ʊ�ץסʱ������������ռ�������Ƶ�λ��
	void onMouseMovedOnHandP(Event* event); //��������������Ϸ��ƶ�ʱ����
	void onMouseMovedOnEEQ(Event* event);
	void onMouseMovedOnMEQ(Event* event);
	void backToMainScene(Ref* sender); //�ص����˵�
	void testOutput(Ref* sender); //*�������
	void onEnter() override;

	Player* player; //��ң��������������
	DBHelper* dbhelper;
	Checkpoint* checkpoint;
	Layer* mHandCardLayer; //���ƶѵ�ͼ��
	Layer* eHandCardLayer; //�з����ƶѵ�ͼ��
	Layer* effectMLayer;
	Layer* effectELayer;
	LinkedList<CardView*>* mHandCardList; //ui�е�����CardView�б�
	LinkedList<CardView*>* eHandCardList; //ui�еĵз�����CardView�б�
	LinkedList<EffectView*>* effectMList;
	LinkedList<EffectView*>* effectEList; 
	/*�����϶���ظ�������*/
	bool hasFocus = false; //�Ƿ���ץס������
	CardView* focusedCard = nullptr; //��ץס������
	CardView* highlightedCard = nullptr; //�����ͣ�Ŀ���
	Point focusedLastPos; //�����϶������У���¼��һʱ�̿��Ƶ�λ��
	Point touchLastPos; //�����϶������У���¼��һʱ������λ��
	Point focusedBeginPos;//��¼�����ڱ�ץסʱ�ĳ�ʼ��Ļλ��
	int focusedBeginZOrder; //��¼�����ڱ�ץסʱ�ĳ�ʼz˳���ƶ��е�����˳��
	int focusedBeginIndex;//��¼�����ڱ�ץסʱ�ĳ�ʼ�±꣨�ƶ����λ�ã�

};