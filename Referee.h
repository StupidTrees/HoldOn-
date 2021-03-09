#pragma once
#include "EffectExecutor.h"
#include "EnemyAI.h"
#include "Trine.h"
class BattleScene;
typedef enum game_state {
	INIT,//初始化之前 
	READY,//准备完成
	ONGOING, //正在进行
	PAUSED, //暂停
	COUNTING,//结算中
	GAME_END//游戏结束
}GAME_STATE;//游戏状态
typedef enum round_stage {
	START, //回合开始
	DRAW_CARD, //抽牌阶段
	PLAY_CARD, //出牌阶段
	DIS_CARD, //弃牌阶段
	END //回合结束
}ROUND_STAGE;

typedef enum scene_state { //场景状态
	ALLOW_USECARD, //可以使用牌
	DISCARD_CARDS, //进行弃牌
	CARD_DISABLED //不可拖动牌
}SCENE_STATE;

typedef enum who { 
	PLAYER,
	ENEMY
}WHO;

/*裁判类*/
class Referee
{
public:

	void executeWithDelay(cocos2d::Node* scene, function<void(void)> f, float delayTime); //延迟调用某函数
	~Referee();
	Referee(BattleScene* sc,DeskTop* d,Role* player,Role* enemy); //初始化传入
	void setAI(EnemyAI* ai); //装载敌人AI
	int getRound();
	ROUND_STAGE getRoundStage();
	SCENE_STATE getSceneState();
	GAME_STATE getGameState();

	void getReady();//战斗开始前的准备
	void start();//开始战斗
	Card* useCard(Role* user,Role* target,Card* c); //某角色使用一张卡牌
	void discardCard(Role* user, Card* c); //某角色弃掉一张牌
	void settleEffectsInQueue(); //核心算法：结算队列中的效果
	void triggerEffects(WHO whose, int PARAM, int TRIGGER); //TRIGGER作为时机触发效果
	void triggerEffectsDuringSettle(Effect* effect, WHO whose, int PARAM, int TRIGGER); //结算过程中的广度优先触发
	bool canIUseThisCard(Card* c, Role* me, Role* theOtherGuy); //判断某张牌能否使用
	int getDiscardNum(Role* user); //计算某玩家需要弃牌的数量


	/*回合控制函数：每个阶段都执行啥*/
	void roundStarts();
	void roundStartsBackground(int x, std::string y);
	void roundEndsBackground(int x, std::string y);
	void roundDrawCard();
	void roundPlayCard();
	void roundDiscardCard();
	void roundEnds();
	void playCardEnds(); //用户选择结束出牌
	Role* player; //固定：玩家
	Role* enemy; //固定：敌人

	LinkedList<Trine<Effect*, WHO, mString>>* settleQueue; //结算队列
	void countGame(WHO winner);//游戏结束，结算整个游戏
private:
	unsigned int round;
	DeskTop* d;
	EnemyAI* enemyAI;
	BattleScene* scene;
	GAME_STATE game_state;//游戏状态
	SCENE_STATE scene_state;//场景状态

	ROUND_STAGE round_stage; //回合阶段
	Role* host; //是谁的回合
	Role* guest; //另一个人
	EffectExecutor* reader;//读代码




};

