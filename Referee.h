#pragma once
#include "EffectExecutor.h"
#include "EnemyAI.h"
#include "Trine.h"
class BattleScene;
typedef enum game_state {
	INIT,//��ʼ��֮ǰ 
	READY,//׼�����
	ONGOING, //���ڽ���
	PAUSED, //��ͣ
	COUNTING,//������
	GAME_END//��Ϸ����
}GAME_STATE;//��Ϸ״̬
typedef enum round_stage {
	START, //�غϿ�ʼ
	DRAW_CARD, //���ƽ׶�
	PLAY_CARD, //���ƽ׶�
	DIS_CARD, //���ƽ׶�
	END //�غϽ���
}ROUND_STAGE;

typedef enum scene_state { //����״̬
	ALLOW_USECARD, //����ʹ����
	DISCARD_CARDS, //��������
	CARD_DISABLED //�����϶���
}SCENE_STATE;

typedef enum who { 
	PLAYER,
	ENEMY
}WHO;

/*������*/
class Referee
{
public:

	void executeWithDelay(cocos2d::Node* scene, function<void(void)> f, float delayTime); //�ӳٵ���ĳ����
	~Referee();
	Referee(BattleScene* sc,DeskTop* d,Role* player,Role* enemy); //��ʼ������
	void setAI(EnemyAI* ai); //װ�ص���AI
	int getRound();
	ROUND_STAGE getRoundStage();
	SCENE_STATE getSceneState();
	GAME_STATE getGameState();

	void getReady();//ս����ʼǰ��׼��
	void start();//��ʼս��
	Card* useCard(Role* user,Role* target,Card* c); //ĳ��ɫʹ��һ�ſ���
	void discardCard(Role* user, Card* c); //ĳ��ɫ����һ����
	void settleEffectsInQueue(); //�����㷨����������е�Ч��
	void triggerEffects(WHO whose, int PARAM, int TRIGGER); //TRIGGER��Ϊʱ������Ч��
	void triggerEffectsDuringSettle(Effect* effect, WHO whose, int PARAM, int TRIGGER); //��������еĹ�����ȴ���
	bool canIUseThisCard(Card* c, Role* me, Role* theOtherGuy); //�ж�ĳ�����ܷ�ʹ��
	int getDiscardNum(Role* user); //����ĳ�����Ҫ���Ƶ�����


	/*�غϿ��ƺ�����ÿ���׶ζ�ִ��ɶ*/
	void roundStarts();
	void roundStartsBackground(int x, std::string y);
	void roundEndsBackground(int x, std::string y);
	void roundDrawCard();
	void roundPlayCard();
	void roundDiscardCard();
	void roundEnds();
	void playCardEnds(); //�û�ѡ���������
	Role* player; //�̶������
	Role* enemy; //�̶�������

	LinkedList<Trine<Effect*, WHO, mString>>* settleQueue; //�������
	void countGame(WHO winner);//��Ϸ����������������Ϸ
private:
	unsigned int round;
	DeskTop* d;
	EnemyAI* enemyAI;
	BattleScene* scene;
	GAME_STATE game_state;//��Ϸ״̬
	SCENE_STATE scene_state;//����״̬

	ROUND_STAGE round_stage; //�غϽ׶�
	Role* host; //��˭�Ļغ�
	Role* guest; //��һ����
	EffectExecutor* reader;//������




};

