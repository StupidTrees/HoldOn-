#include "pch.h"
#include "Referee.h"
#include "BattleScene.h"
#include "dimens.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#pragma execution_character_set("utf-8")
#define maxHandPileSize 1

using namespace CocosDenshion;
class useCardAsyncask {
public:
	useCardAsyncask(Referee* ref, Role* user, Role* target, EffectExecutor* reader, BattleScene* bs, Card* c) {
		this->user = user;
		this->target = target;
		this->reader = reader;
		this->c = c;
		referee = ref;
		scene = bs;
	}

	void run() {
		std::thread t1(&useCardAsyncask::runInBackground,this,10,"haha");
		t1.detach();
	}
private:
	Role* user;
	Referee* referee;
	Role* target;
	EffectExecutor* reader;
	Card* c;
	BattleScene* scene;
	void runInBackground(int first, std::string second) {
		SimpleAudioEngine::getInstance()->playEffect("audio/useCard.wav");
		WHO whose = user == referee->player ? PLAYER : ENEMY;
		auto userRoleView = user == referee->player ? scene->playerRoleView : scene->enemyRoleView;
		auto targetRoleView = user == referee->enemy ? scene->playerRoleView : scene->enemyRoleView;
		auto effectsList = c->getEffects();
		auto effectsDir = c->getEffectsDir();
		Effect* effect;
		user->getHandPile()->remove(c, c);
		//log("consume%d:%d", c->getConsume_type(), c->getConsume());
		if (c->getConsume_type() == CONSUME_MANA1) user->setMANA1(user->getMANA1() - c->getConsume());
		else if (c->getConsume_type() == CONSUME_MANA2) 	user->setMANA2(user->getMANA2() - c->getConsume());

		//auto targetQueue = user == player ? settleQueue_player : settleQueue_enemy;


		/*触发 打出一张牌时可能的效果*/
		auto throw_toRemove_m = new LinkedList<Effect*>;
		auto virus_toRemove_m = new LinkedList<Effect*>;
		auto virusStack_m = new LinkedList<Effect*>;
		auto throw_toRemove_e = new LinkedList<Effect*>;
		auto virus_toRemove_e = new LinkedList<Effect*>;
		auto virusStack_e = new LinkedList<Effect*>;

		/*遍历使用者的效果区*/
		LinkedList<Effect*>::Iterator itEU(user->getEffectList());
		while (itEU.hasNext()) {
			auto e = itEU.next();
			//WHO owner = e->getOwner() == referee->player ? PLAYER : ENEMY;
			if (e->getTrigger() == TRIGGER_M_A_CARD_USED || e->getTrigger() == TRIGGER_M_A_CARD_USED + 900 + c->getType() * 10 || e->getTrigger() == TRIGGER_M_A_CARD_USED + c->getId()) {
				/*若该效果为叠加效果，入栈*/
				if (e->getType() == EFFECT_TYPE_VIRUS) {
					virusStack_m->push(e); //对于病毒而言，传入本身
				}
				/*否则，加入到结算队列*/
				else {
					/*如果被触发的效果满足移除条件，则从原列表中移除*/
					if (e->getAfterwards() == EFFECT_AFTERWARDS_GO) throw_toRemove_m->add(e);
					else if (e->getAfterwards() == EFFECT_AFTERWARDS_COUNTDOWN) {
						if (e->getCDTrigger() == CD_TRIGGER_AFTEREFFECT && e->getCountdown() > 0) e->decCountDown();
						if (e->getCountdown() == 0) throw_toRemove_m->add(e);
					}
					mString seg = e->getCountdown() > 0 ? "COUNTDOWN" : "EXECUTE";
					referee->settleQueue->add(Trine<Effect*, WHO, mString>(e->clone(), whose, seg));
					//targetQueue->add(e->clone());

				}

			}
		}
		user->getEffectList()->removeAll_delete(throw_toRemove_m); //删掉被移除的效果

		/*遍历对方的效果区，同理*/
		LinkedList<Effect*>::Iterator itEE(target->getEffectList());
		while (itEE.hasNext()) {
			auto e = itEE.next();
			//WHO owner = e->getOwner() == referee->player ? PLAYER : ENEMY;
			if (e->getTrigger() == TRIGGER_E_A_CARD_USED || e->getTrigger() == TRIGGER_E_A_CARD_USED + 900 + c->getType() * 10 || e->getTrigger() == TRIGGER_E_A_CARD_USED + c->getId()) {
				if (e->getType() == EFFECT_TYPE_VIRUS) {
					virusStack_e->add(e); //对于病毒而言，传入本身
				}
				else {
					if (e->getAfterwards() == EFFECT_AFTERWARDS_GO) throw_toRemove_e->add(e);
					else if (e->getAfterwards() == EFFECT_AFTERWARDS_COUNTDOWN) {
						if (e->getCDTrigger() == CD_TRIGGER_AFTEREFFECT && e->getCountdown() > 0) e->decCountDown();
						if (e->getCountdown() == 0) throw_toRemove_e->add(e);
					}
					mString seg = e->getCountdown() > 0 ? "COUNTDOWN" : "EXECUTE";
					referee->settleQueue->add(Trine<Effect*, WHO, mString>(e->clone(),whose==PLAYER?ENEMY:PLAYER, seg));
				}

			}
		}
		target->getEffectList()->removeAll_delete(throw_toRemove_e);


		/*对这张牌的效果进行清算*/
		LinkedList<Effect*>::Iterator itCE(effectsList);
		LinkedList<int>::Iterator itCER(effectsDir);
		while (itCE.hasNext()) {
			effect = itCE.next();
			int effectDir = itCER.next(); //效果去向

			effect->setOwner(user); //将效果的归属设为使用者
			if (effectDir == EFFECTDIR_TO_SETTLE) { //如果去向为“结算“，则直接加入到队列中清算
				auto e = effect->clone(); //加入的是替身
				referee->settleQueue->add(Trine<Effect*, WHO, mString>(e, whose, "EXECUTE"));
				//targetQueue->add(e);//一定别把原对象放入结算区（会被delete）
				if (e->isMergeAble()) { //如果可被叠加，则将栈中的内容出栈进行叠加
					while (virusStack_m->size() > 0) {
						auto virus = virusStack_m->pop();
						if (virus->getAfterwards() == EFFECT_AFTERWARDS_GO)if (!virus_toRemove_m->contains(virus)) virus_toRemove_m->add(virus);
						else if (virus->getAfterwards() == EFFECT_AFTERWARDS_COUNTDOWN) if (virus->getCountdown() == 0)if (!virus_toRemove_m->contains(virus)) virus_toRemove_m->add(virus);
						reader->insertVirusInto(e, virus);
					}
					while (virusStack_e->size() > 0) {
						auto virus = virusStack_e->pop();
						if (virus->getAfterwards() == EFFECT_AFTERWARDS_GO)if (!virus_toRemove_e->contains(virus)) virus_toRemove_e->add(virus);
						else if (virus->getAfterwards() == EFFECT_AFTERWARDS_COUNTDOWN) if (virus->getCountdown() == 0)if (!virus_toRemove_e->contains(virus)) virus_toRemove_e->add(virus);
						reader->insertVirusInto(e, virus);
					}
				}
			}
			else if (effectDir == EFFECTDIR_TO_M_EFFECTLIST) { //如果去向为“我方效果区”，则加入到我方效果区
				userRoleView->getEffect();
				user->getEffectList()->add(effect->clone());
			}
			else if (effectDir == EFFECTDIR_TO_E_EFFECTLIST) { //如果去向为“对方效果区”，则加入到我方效果区
				targetRoleView->getEffect();
				target->getEffectList()->add(effect->clone());
			}
		}
		user->getEffectList()->removeAll_delete(virus_toRemove_m); // 移除的效果 必要释放
		target->getEffectList()->removeAll_delete(virus_toRemove_e);
		referee->settleEffectsInQueue();
		user->getDiscardPile()->add(c);

		auto callF = CallFunc::create([this] {
			scene->refreshBoardInfo();
			scene->notifyEffectChanges(); });
		scene->runAction(callF);
	}
};


void Referee::executeWithDelay(cocos2d::Node* scene, function<void(void)> f, float delayTime) {
	auto delay = DelayTime::create(delayTime);
	//function<void(void)> f(funct);
	auto fun = CallFunc::create(f);
	auto seq = Sequence::create(delay, fun, NULL);
	scene->runAction(seq);
}
Referee::~Referee()
{
}

Referee::Referee(BattleScene* sc, DeskTop* d, Role* player, Role* enemy)
{
	this->d = d;
	this->player = player;
	this->enemy = enemy;
	this->scene = sc;
	settleQueue = new LinkedList<Trine<Effect*, WHO,mString>>;
	reader = new EffectExecutor(d);
	game_state = INIT;
}

/*绑定敌人AI*/
void Referee::setAI(EnemyAI* ai) {
	this->enemyAI = ai;
}

int Referee::getRound()
{
	return round;
}

ROUND_STAGE Referee::getRoundStage()
{
	return round_stage;
}

SCENE_STATE Referee::getSceneState()
{
	return scene_state;
}

GAME_STATE Referee::getGameState() {
	return game_state;
}

void Referee::getReady()
{
	round = 1;
	host = player;
	guest = enemy;
	game_state = READY;
	scene_state = CARD_DISABLED;
}

void Referee::start()
{

	round_stage = START;
	game_state = ONGOING;
	scene_state = CARD_DISABLED;
	d->getP1()->shuffle(d->getP1()->getDrawPile());
	d->getP2()->shuffle(d->getP2()->getDrawPile());
	d->getP1()->drawCards(4);
	scene->notifyMHandCardChanged();
	d->getP2()->drawCards(4);
	scene->notifyEHandCardChanged();
	scene->setHint("游戏开始");

	executeWithDelay(scene, function<void(void)>([this]() {
		scene->refreshHandpState();
		executeWithDelay(scene, function<void(void)>([this]() {
			roundDrawCard(); }), 0.6); }), 0.5);
	//Director::getInstance()->getScheduler()->scheduleScriptFunc([](void) {},2.0, false);
}

/*某个角色使用一张卡*/
Card* Referee::useCard(Role* user, Role* target, Card* c)
{
	auto uca = new useCardAsyncask(this, user, target, reader, scene, c);
	uca->run();
	return c;
}


//void Referee::settleEffectsInQueue(WHO whose) {
//	//log("execute:who=%s", whose == PLAYER ? "player" : "enemy");
//	WHO ME = whose;
//	WHO YOU = whose == PLAYER ? ENEMY : PLAYER;
//	auto user = whose == PLAYER ? player : enemy;
//	auto target = whose == PLAYER ? enemy : player;
//	auto queue = whose == PLAYER ? settleQueue_player : settleQueue_enemy;
//	auto m_roleView = whose == PLAYER ? scene->playerRoleView : scene->enemyRoleView;
//	auto e_roleView = whose == ENEMY ? scene->playerRoleView : scene->enemyRoleView;
//	while (queue->size() > 0) {
//		auto effect = queue->remove(0);
//		/*每当一个效果出队列，调用双方效果区中“效果生效”时可能触发的效果*/
//		auto realUser = effect->getOwner() == user ? user : target;
//		auto realTarget = realUser == player ? enemy : player;
//		WHO realME = effect->getOwner() == user ? ME : YOU;
//		WHO realYOU = realME == PLAYER ? ENEMY : PLAYER;
//		auto virusStack_m = new LinkedList<Effect*>;
//		auto virusStack_e = new LinkedList<Effect*>;
//		auto virus_toRemove_m = new LinkedList<Effect*>;
//		auto virus_toRemove_e = new LinkedList<Effect*>;
//		auto throw_toRemove_e = new LinkedList<Effect*>;
//		auto throw_toRemove_m = new LinkedList<Effect*>;
//		for (int i = 0; i < realUser->getEffectList()->size(); i++) {
//			auto e = realUser->getEffectList()->get(i);
//			if (e->getTrigger() == TRIGGER_M_EFFECT_SETTLE || e->getTrigger() == TRIGGER_M_EFFECT_SETTLE + 900 + effect->getType() * 10 || e->getTrigger() == TRIGGER_M_EFFECT_SETTLE + effect->getId()) {
//				if (e->getType() == EFFECT_TYPE_VIRUS) virusStack_m->add(e); //如果触发且为叠加牌，入栈
//				else { //否则直接加入本方结算区
//					queue->add(e->clone());
//					if (e->getAfterwards() == EFFECT_AFTERWARDS_GO) throw_toRemove_m->add(e);
//					else if (e->getAfterwards() == EFFECT_AFTERWARDS_COUNTDOWN) {
//						if (e->getCountdown() == 0) throw_toRemove_m->add(e);
//					}
//				}
//			}
//		}
//		realUser->getEffectList()->removeAll_delete(throw_toRemove_m);
//		for (int i = 0; i < realTarget->getEffectList()->size(); i++) {
//			auto e = realTarget->getEffectList()->get(i);
//			if (e->getTrigger() == TRIGGER_E_EFFECT_SETTLE || e->getTrigger() == TRIGGER_E_EFFECT_SETTLE + 900 + effect->getType() * 10 || e->getTrigger() == TRIGGER_E_EFFECT_SETTLE + effect->getId()) {
//				if (e->getType() == EFFECT_TYPE_VIRUS) virusStack_e->add(e); //如果触发且为叠加牌，入栈
//				//else { //否则直接加入本方结算区
//				//	queue->add(e->clone());
//				//	if (e->getAfterwards() == EFFECT_AFTERWARDS_GO) throw_toRemove_e->add(e);
//				//	else if (e->getAfterwards() == EFFECT_AFTERWARDS_COUNTDOWN) {
//				//		if (e->getCountdown() == 0) throw_toRemove_e->add(e);
//				//	}
//				//}
//			}
//		}
//		realTarget->getEffectList()->removeAll_delete(throw_toRemove_e);
//		if (effect->isMergeAble()) {
//			for (int i = virusStack_m->size() - 1; i >= 0; i--) {
//				auto virus = virusStack_m->get(i);
//				reader->insertVirusInto(effect, virus);
//				if (virus->getAfterwards() == EFFECT_AFTERWARDS_GO)if (virus_toRemove_m->contains(virus) == -1) virus_toRemove_m->add(virus);
//				else if (virus->getAfterwards() == EFFECT_AFTERWARDS_COUNTDOWN) if (virus->getCountdown() == 0)if (virus_toRemove_m->contains(virus) == -1) virus_toRemove_m->add(virus);
//			}
//			for (int i = virusStack_e->size() - 1; i >= 0; i--) {
//				auto virus = virusStack_e->get(i);
//				reader->insertVirusInto(effect, virus);
//				if (virus->getAfterwards() == EFFECT_AFTERWARDS_GO)if (virus_toRemove_e->contains(virus) == -1) virus_toRemove_e->add(virus);
//				else if (virus->getAfterwards() == EFFECT_AFTERWARDS_COUNTDOWN) if (virus->getCountdown() == 0)if (virus_toRemove_e->contains(virus) == -1) virus_toRemove_e->add(virus);
//			}
//		}
//		realUser->getEffectList()->removeAll_delete(virus_toRemove_m);
//		realTarget->getEffectList()->removeAll_delete(virus_toRemove_e);
//
//		/*然后再执行这个效果*/
//		LinkedList<Pair<int, int>>* exeResult;
//		if (effect->getCountdown() < 0) {
//			exeResult = reader->execute(effect->getCode(), "EXECUTE", user, target);
//		}
//		else {
//			if (effect->getCountdown() > 0) exeResult = reader->execute(effect->getCode(), "COUNTDOWN", user, target);
//			else  exeResult = reader->execute(effect->getCode(), "EXECUTE", user, target);
//		}
//
//		scene->refreshBoardInfo();
//		scene->notifyMHandCardChanged();
//		scene->notifyEHandCardChanged();
//		scene->refreshHandpState();
//		scene->notifyEffectChanges();
//
//		cocos2d::log("exeResult:");
//		for (int i = 0; i < exeResult->size(); i++) {
//			Pair<int, int> x = exeResult->get(i);
//			log("<%d:%d>", exeResult->get(i).getX(), exeResult->get(i).getY());
//		}
//
//		for (int i = 0; i < exeResult->size(); i++) {
//			Pair<int, int> x = exeResult->get(i);
//			if (x.getX() == EXE_NOTE_E_HURT) {
//				e_roleView->getHurt(x.getY());
//				triggerEffects(effect, YOU, x.getY(), TRIGGER_M_GOT_HURT);
//				triggerEffects(effect, ME,x.getY(), TRIGGER_M_HURT_SOMEONE);
//			}
//			else if (x.getX() == EXE_NOTE_M_HURT) {
//				m_roleView->getHurt(x.getY());
//				triggerEffects(effect, ME, x.getY(), TRIGGER_M_GOT_HURT);
//			}
//			else if (x.getX() == EXE_NOTE_M_RECOVER) {
//				m_roleView->getRecover(x.getY());
//				triggerEffects(effect, ME, x.getY(), TRIGGER_M_GOT_RECOVER);
//				triggerEffects(effect, YOU, x.getY(), TRIGGER_E_GOT_RECOVER);
//			}
//			else if (x.getX() == EXE_NOTE_E_RECOVER) {
//				e_roleView->getRecover(x.getY());
//				triggerEffects(effect, YOU, x.getY(), TRIGGER_M_GOT_RECOVER);
//				triggerEffects(effect, ME, x.getY(), TRIGGER_E_GOT_RECOVER);
//			}
//			else if (x.getX() == EXE_NOTE_M_MANA1_UP) {
//				triggerEffects(effect, ME, x.getY(), TRIGGER_M_GOT_MANA1_UP);
//				triggerEffects(effect, YOU, x.getY(), TRIGGER_E_GOT_MANA1_UP);
//			}
//			else if (x.getX() == EXE_NOTE_M_MANA2_UP) {
//				triggerEffects(effect, ME, x.getY(), TRIGGER_M_GOT_MANA2_UP);
//				triggerEffects(effect, YOU, x.getY(), TRIGGER_E_GOT_MANA2_UP);
//			}
//			else if (x.getX() == EXE_NOTE_E_MANA1_UP) {
//				triggerEffects(effect, YOU, x.getY(), TRIGGER_M_GOT_MANA1_UP);
//				triggerEffects(effect, ME, x.getY(), TRIGGER_E_GOT_MANA1_UP);
//			}
//			else if (x.getX() == EXE_NOTE_E_MANA2_UP) {
//				triggerEffects(effect, YOU, x.getY(), TRIGGER_M_GOT_MANA2_UP);
//				triggerEffects(effect, ME, x.getY(), TRIGGER_E_GOT_MANA2_UP);
//			}
//			else if (x.getX() == EXE_NOTE_M_HANDP_INSERT) {
//				triggerEffects(effect,ME, x.getY(), TRIGGER_M_HANDP_INSERT);
//				triggerEffects(effect, YOU, x.getY(), TRIGGER_E_HANDP_INSERT);
//			}
//			else if (x.getX() == EXE_NOTE_M_HANDP_REMOVE) {
//				triggerEffects(effect, ME, x.getY(), TRIGGER_M_HANDP_REMOVE);
//				triggerEffects(effect, YOU, x.getY(), TRIGGER_E_HANDP_REMOVE);
//			}
//			else if (x.getX() == EXE_NOTE_M_DSCDP_INSERT) {
//				triggerEffects(effect, ME, x.getY(), TRIGGER_M_DSCDP_INSERT);
//				triggerEffects(effect, YOU, x.getY(), TRIGGER_E_DSCDP_INSERT);
//			}
//			else if (x.getX() == EXE_NOTE_M_DSCDP_REMOVE) {
//				triggerEffects(effect, ME, x.getY(), TRIGGER_M_DSCDP_REMOVE);
//				triggerEffects(effect, YOU, x.getY(), TRIGGER_E_DSCDP_REMOVE);
//			}
//			else if (x.getX() == EXE_NOTE_E_HANDP_INSERT) {
//				triggerEffects(effect, YOU, x.getY(), TRIGGER_M_HANDP_INSERT);
//				triggerEffects(effect, ME, x.getY(), TRIGGER_E_HANDP_INSERT);
//			}
//			else if (x.getX() == EXE_NOTE_E_HANDP_REMOVE) {
//				triggerEffects(effect, YOU, x.getY(), TRIGGER_M_HANDP_REMOVE);
//				triggerEffects(effect, ME, x.getY(), TRIGGER_E_HANDP_REMOVE);
//			}
//			else if (x.getX() == EXE_NOTE_E_DSCDP_INSERT) {
//				triggerEffects(effect, YOU, x.getY(), TRIGGER_M_DSCDP_INSERT);
//				triggerEffects(effect, ME, x.getY(), TRIGGER_E_DSCDP_INSERT);
//			}
//			else if (x.getX() == EXE_NOTE_E_DSCDP_REMOVE) {
//				triggerEffects(effect, YOU, x.getY(), TRIGGER_M_DSCDP_REMOVE);
//				triggerEffects(effect, ME, x.getY(), TRIGGER_E_DSCDP_REMOVE);
//			}
//		}
//
//		delete effect;//结算后 销毁
//	}
//}

void Referee::settleEffectsInQueue() {
	auto virusStack_m = new LinkedList<Effect*>;
	auto virusStack_e = new LinkedList<Effect*>;
	auto virus_toRemove_m = new LinkedList<Effect*>;
	auto virus_toRemove_e = new LinkedList<Effect*>;
	auto throw_toRemove_e = new LinkedList<Effect*>;
	auto throw_toRemove_m = new LinkedList<Effect*>;
	while (settleQueue->size() > 0) {
		virusStack_m->clear();
		virus_toRemove_m->clear();
		throw_toRemove_m->clear();
		virusStack_e->clear();
		virus_toRemove_e->clear();
		throw_toRemove_e->clear();
		auto trine = settleQueue->dequeue();
		auto effect = trine.getX();
		WHO whose = trine.getY();
		WHO ME = whose;
		WHO YOU = whose == PLAYER ? ENEMY : PLAYER;
		auto user = whose == PLAYER ? player : enemy;
		auto target = whose == PLAYER ? enemy : player;
		auto m_roleView = whose == PLAYER ? scene->playerRoleView : scene->enemyRoleView;
		auto e_roleView = whose == ENEMY ? scene->playerRoleView : scene->enemyRoleView;
		/*每当一个效果出队列，调用双方效果区中“效果生效”时可能触发的效果*/
		auto realUser = effect->getOwner() == user ? user : target;
		auto realTarget = realUser == player ? enemy : player;
		WHO realME = effect->getOwner() == user ? ME : YOU;
		WHO realYOU = realME == PLAYER ? ENEMY : PLAYER;

		LinkedList<Effect*>::Iterator itrM(realUser->getEffectList());
		LinkedList<Effect*>::Iterator itrE(realTarget->getEffectList());
		while (itrM.hasNext()) {
			auto e = itrM.next();
			if (e->getTrigger() == TRIGGER_M_EFFECT_SETTLE || e->getTrigger() == TRIGGER_M_EFFECT_SETTLE + 900 + effect->getType() * 10 || e->getTrigger() == TRIGGER_M_EFFECT_SETTLE + effect->getId()) {
				if (e->getType() == EFFECT_TYPE_VIRUS) virusStack_m->push(e); //如果触发且为叠加牌，入栈
				else { //否则直接加入结算区
					if (e->getAfterwards() == EFFECT_AFTERWARDS_GO) throw_toRemove_m->add(e);
					else if (e->getAfterwards() == EFFECT_AFTERWARDS_COUNTDOWN) {
						if (e->getCDTrigger() == CD_TRIGGER_AFTEREFFECT && e->getCountdown() > 0) e->decCountDown();
						if (e->getCountdown() == 0) throw_toRemove_m->add(e);
					}
					mString seg = e->getCountdown() > 0 ? "COUNTDOWN" : "EXECUTE";
					settleQueue->add(0, Trine<Effect*, WHO, mString>(e->clone(), realME, seg));
				}
			}
		}
		realUser->getEffectList()->removeAll_delete(throw_toRemove_m);
		while (itrE.hasNext()) {
			auto e = itrE.next();
			if (e->getTrigger() == TRIGGER_E_EFFECT_SETTLE || e->getTrigger() == TRIGGER_E_EFFECT_SETTLE + 900 + effect->getType() * 10 || e->getTrigger() == TRIGGER_E_EFFECT_SETTLE + effect->getId()) {
				if (e->getType() == EFFECT_TYPE_VIRUS) virusStack_e->push(e); //如果触发且为叠加牌，入栈
				else { //否则直接加入结算区
					if (e->getAfterwards() == EFFECT_AFTERWARDS_GO) throw_toRemove_m->add(e);
					else if (e->getAfterwards() == EFFECT_AFTERWARDS_COUNTDOWN) {
						if (e->getCDTrigger() == CD_TRIGGER_AFTEREFFECT && e->getCountdown() > 0) e->decCountDown();
						if (e->getCountdown() == 0) throw_toRemove_m->add(e);
					}
					mString seg = e->getCountdown() > 0 ? "COUNTDOWN" : "EXECUTE";
					settleQueue->add(0, Trine<Effect*, WHO, mString>(e->clone(), realYOU, seg));
				}
			}
		}
		realTarget->getEffectList()->removeAll_delete(throw_toRemove_e);
		if (effect->isMergeAble()) {
			while (virusStack_m->size() > 0) {
				auto virus = virusStack_m->pop();
				reader->insertVirusInto(effect, virus);
				if (virus->getAfterwards() == EFFECT_AFTERWARDS_GO)if (!virus_toRemove_m->contains(virus)) virus_toRemove_m->add(virus);
				else if (virus->getAfterwards() == EFFECT_AFTERWARDS_COUNTDOWN) if (virus->getCountdown() == 0)if (!virus_toRemove_m->contains(virus)) virus_toRemove_m->add(virus);
			}
			while (virusStack_e->size() > 0) {
				auto virus = virusStack_e->pop();
				reader->insertVirusInto(effect, virus);
				if (virus->getAfterwards() == EFFECT_AFTERWARDS_GO)if (!virus_toRemove_e->contains(virus)) virus_toRemove_e->add(virus);
				else if (virus->getAfterwards() == EFFECT_AFTERWARDS_COUNTDOWN) if (virus->getCountdown() == 0)if (!virus_toRemove_e->contains(virus)) virus_toRemove_e->add(virus);
			}
		}
		realUser->getEffectList()->removeAll_delete(virus_toRemove_m);
		realTarget->getEffectList()->removeAll_delete(virus_toRemove_e);

		/*然后再执行这个效果*/
		LinkedList<Pair<int, int>>* exeResult;
		exeResult = reader->execute(effect->getCode(), trine.getZ(), user, target);


		auto callf = CallFuncN::create([=](Node* n) {
			auto scene = static_cast<BattleScene*>(n);
			scene->refreshBoardInfo();
			scene->notifyMHandCardChanged();
			scene->notifyEHandCardChanged();
			scene->refreshHandpState();
			scene->notifyEffectChanges();
			/*触发效果动画*/
			LinkedList<Pair<int, int>>::Iterator itreR(exeResult);
			while (itreR.hasNext()) {
				Pair<int, int> x = itreR.next();
				log("<%d:%d>", x.getX(), x.getY());
				if (x.getX() == EXE_NOTE_E_HURT) e_roleView->getHurt(x.getY());
				else if (x.getX() == EXE_NOTE_M_HURT) m_roleView->getHurt(x.getY());
				else if (x.getX() == EXE_NOTE_M_RECOVER)m_roleView->getRecover(x.getY());
				else if (x.getX() == EXE_NOTE_E_RECOVER) e_roleView->getRecover(x.getY());
			}

			});
		scene->runAction(callf);
		/*执行完成，判断胜负情况*/
		if (player->getHP() == 0) {
			countGame(ENEMY);
			break;
		}
		else if (enemy->getHP() == 0) {
			countGame(PLAYER);
			break;
		}


		/*依照这个效果的执行结果，触发可能的效果*/
		LinkedList<Pair<int, int>>::Iterator itreR(exeResult);
		while (itreR.hasNext()) {
			Pair<int, int> x = itreR.next();
			if (x.getX() == EXE_NOTE_E_HURT) {
				//	e_roleView->getHurt(x.getY());
				triggerEffectsDuringSettle(effect, YOU, x.getY(), TRIGGER_M_GOT_HURT);
				triggerEffectsDuringSettle(effect, ME, x.getY(), TRIGGER_M_HURT_SOMEONE);
			}
			else if (x.getX() == EXE_NOTE_M_HURT) {
				//m_roleView->getHurt(x.getY());
				triggerEffectsDuringSettle(effect, ME, x.getY(), TRIGGER_M_GOT_HURT);
			}
			else if (x.getX() == EXE_NOTE_M_RECOVER) {
				//m_roleView->getRecover(x.getY());
				triggerEffectsDuringSettle(effect, ME, x.getY(), TRIGGER_M_GOT_RECOVER);
				triggerEffectsDuringSettle(effect, YOU, x.getY(), TRIGGER_E_GOT_RECOVER);
			}
			else if (x.getX() == EXE_NOTE_E_RECOVER) {
				//	e_roleView->getRecover(x.getY());
				triggerEffectsDuringSettle(effect, YOU, x.getY(), TRIGGER_M_GOT_RECOVER);
				triggerEffectsDuringSettle(effect, ME, x.getY(), TRIGGER_E_GOT_RECOVER);
			}
			else if (x.getX() == EXE_NOTE_M_MANA1_UP) {
				triggerEffectsDuringSettle(effect, ME, x.getY(), TRIGGER_M_GOT_MANA1_UP);
				triggerEffectsDuringSettle(effect, YOU, x.getY(), TRIGGER_E_GOT_MANA1_UP);
			}
			else if (x.getX() == EXE_NOTE_M_MANA2_UP) {
				triggerEffectsDuringSettle(effect, ME, x.getY(), TRIGGER_M_GOT_MANA2_UP);
				triggerEffectsDuringSettle(effect, YOU, x.getY(), TRIGGER_E_GOT_MANA2_UP);
			}
			else if (x.getX() == EXE_NOTE_E_MANA1_UP) {
				triggerEffectsDuringSettle(effect, YOU, x.getY(), TRIGGER_M_GOT_MANA1_UP);
				triggerEffectsDuringSettle(effect, ME, x.getY(), TRIGGER_E_GOT_MANA1_UP);
			}
			else if (x.getX() == EXE_NOTE_E_MANA2_UP) {
				triggerEffectsDuringSettle(effect, YOU, x.getY(), TRIGGER_M_GOT_MANA2_UP);
				triggerEffectsDuringSettle(effect, ME, x.getY(), TRIGGER_E_GOT_MANA2_UP);
			}
			else if (x.getX() == EXE_NOTE_M_HANDP_INSERT) {
				triggerEffectsDuringSettle(effect, ME, x.getY(), TRIGGER_M_HANDP_INSERT);
				triggerEffectsDuringSettle(effect, YOU, x.getY(), TRIGGER_E_HANDP_INSERT);
			}
			else if (x.getX() == EXE_NOTE_M_HANDP_REMOVE) {
				triggerEffectsDuringSettle(effect, ME, x.getY(), TRIGGER_M_HANDP_REMOVE);
				triggerEffectsDuringSettle(effect, YOU, x.getY(), TRIGGER_E_HANDP_REMOVE);
			}
			else if (x.getX() == EXE_NOTE_M_DSCDP_INSERT) {
				triggerEffectsDuringSettle(effect, ME, x.getY(), TRIGGER_M_DSCDP_INSERT);
				triggerEffectsDuringSettle(effect, YOU, x.getY(), TRIGGER_E_DSCDP_INSERT);
			}
			else if (x.getX() == EXE_NOTE_M_DSCDP_REMOVE) {
				triggerEffectsDuringSettle(effect, ME, x.getY(), TRIGGER_M_DSCDP_REMOVE);
				triggerEffectsDuringSettle(effect, YOU, x.getY(), TRIGGER_E_DSCDP_REMOVE);
			}
			else if (x.getX() == EXE_NOTE_E_HANDP_INSERT) {
				triggerEffectsDuringSettle(effect, YOU, x.getY(), TRIGGER_M_HANDP_INSERT);
				triggerEffectsDuringSettle(effect, ME, x.getY(), TRIGGER_E_HANDP_INSERT);
			}
			else if (x.getX() == EXE_NOTE_E_HANDP_REMOVE) {
				triggerEffectsDuringSettle(effect, YOU, x.getY(), TRIGGER_M_HANDP_REMOVE);
				triggerEffectsDuringSettle(effect, ME, x.getY(), TRIGGER_E_HANDP_REMOVE);
			}
			else if (x.getX() == EXE_NOTE_E_DSCDP_INSERT) {
				triggerEffectsDuringSettle(effect, YOU, x.getY(), TRIGGER_M_DSCDP_INSERT);
				triggerEffectsDuringSettle(effect, ME, x.getY(), TRIGGER_E_DSCDP_INSERT);
			}
			else if (x.getX() == EXE_NOTE_E_DSCDP_REMOVE) {
				triggerEffectsDuringSettle(effect, YOU, x.getY(), TRIGGER_M_DSCDP_REMOVE);
				triggerEffectsDuringSettle(effect, ME, x.getY(), TRIGGER_E_DSCDP_REMOVE);
			}
		}

		delete effect;//结算后 销毁
	}
}
void Referee::triggerEffectsDuringSettle(Effect* effect, WHO whose, int PARAM, int TRIGGER) {
	auto toRemove = new LinkedList<Effect*>;
	auto effectList = whose == PLAYER ? player->getEffectList() : enemy->getEffectList();
	for (int i = 0; i < effectList->size(); i++) {
		auto e = effectList->get(i);

		if (e->getTrigger() == TRIGGER || e->getTrigger() == TRIGGER + 900 + effect->getType() * 10 || e->getTrigger() == TRIGGER + effect->getId()) {

			if (e->getAfterwards() == EFFECT_AFTERWARDS_GO) toRemove->add(e);
			else if (e->getAfterwards() == EFFECT_AFTERWARDS_COUNTDOWN) {
				if (e->getCDTrigger() == CD_TRIGGER_AFTEREFFECT && e->getCountdown() > 0) e->decCountDown();
				if (e->getCountdown() == 0) toRemove->add(e);
			}
			mString seg = e->getCountdown() > 0 ? "COUNTDOWN" : "EXECUTE";
			Trine<Effect*, WHO,mString> t(e->clone()->setParam(mString::parseInt(PARAM))->setCDParam(mString::parseInt(e->getCountdown())), whose,seg);
			settleQueue->add(0, t);
		}
	}
	effectList->removeAll_delete(toRemove);
}
void Referee::triggerEffects(WHO whose, int PARAM, int TRIGGER) {
	auto toRemove = new LinkedList<Effect*>;
	auto effectList = whose == PLAYER ? player->getEffectList() : enemy->getEffectList();
	for (int i = 0; i < effectList->size(); i++) {
		auto e = effectList->get(i);
		if (e->getTrigger() == TRIGGER ) {
			if (e->getAfterwards() == EFFECT_AFTERWARDS_GO) toRemove->add(e);
			else if (e->getAfterwards() == EFFECT_AFTERWARDS_COUNTDOWN) {
				if (e->getCDTrigger() == CD_TRIGGER_AFTEREFFECT && e->getCountdown() > 0) e->decCountDown();
				if (e->getCountdown() == 0) toRemove->add(e);
			}
			mString seg = e->getCountdown() > 0 ? "COUNTDOWN" : "EXECUTE";
			Trine<Effect*, WHO, mString> t(e->clone()->setParam(mString::parseInt(PARAM))->setCDParam(mString::parseInt(e->getCountdown())), whose, seg);
			settleQueue->enqueue( t);
		}
		if (e->getCountdown()>0&&e->getCDTrigger() == CD_TRIGGER_ROUNDSTART && TRIGGER == TRIGGER_ROUND_START) {
			e->decCountDown();
			if (e->getCountdown() == 0 && (!toRemove->contains(e) )) {
				Trine<Effect*, WHO,mString> t(e->clone()->setParam(mString::parseInt(PARAM))->setCDParam(mString::parseInt(e->getCountdown())), whose,"EXECUTE");
				settleQueue->enqueue(t);
				toRemove->add(e);
			}
		}
		if (e->getCountdown() > 0 && e->getCDTrigger() == CD_TRIGGER_ROUNDEND && TRIGGER == TRIGGER_ROUND_END) {
			e->decCountDown();
			if (e->getCountdown() == 0 && (!toRemove->contains(e) )) {
				Trine<Effect*, WHO, mString> t(e->clone()->setParam(mString::parseInt(PARAM))->setCDParam(mString::parseInt(e->getCountdown())), whose, "EXECUTE");
				settleQueue->enqueue(t);
				toRemove->add(e);
			}
		}
	}
	effectList->removeAll_delete(toRemove);
}

//
//void Referee::triggerEffects(Effect* effect, WHO whose, int PARAM, int TRIGGER) {
//	auto toRemove = new LinkedList<Effect*>;
//	auto effectList = whose == PLAYER ? player->getEffectList() : enemy->getEffectList();
//	auto settleQ = whose == PLAYER ? settleQueue_player : settleQueue_enemy;
//	for (int i = 0; i < effectList->size(); i++) {
//		auto e = effectList->get(i);
//
//		if (e->getTrigger() == TRIGGER || e->getTrigger() == TRIGGER + 900 + effect->getType() * 10 || e->getTrigger() == TRIGGER + effect->getId()) {
//			settleQ->add(e->clone()->setParam(mString::parseInt(PARAM))->setCDParam(mString::parseInt(e->getCountdown())));
//			if (e->getAfterwards() == EFFECT_AFTERWARDS_GO) toRemove->add(e);
//			else if (e->getAfterwards() == EFFECT_AFTERWARDS_COUNTDOWN) if (e->getCountdown() == 1) toRemove->add(e);
//			if (e->getCountdown() > 0) e->decCountDown();
//		}
//	}
//	effectList->removeAll_delete(toRemove);
//	settleEffectsInQueue(whose);
//}
//
//void Referee::triggerEffects(WHO whose, int PARAM, int TRIGGER) {
//	auto toRemove = new LinkedList<Effect*>;
//	auto effectList = whose == PLAYER ? player->getEffectList() : enemy->getEffectList();
//	auto settleQ = whose == PLAYER ? settleQueue_player : settleQueue_enemy;
//	for (int i = 0; i < effectList->size(); i++) {
//		auto e = effectList->get(i);
//		if (e->getTrigger() == TRIGGER) {
//		settleQ->add(e->clone()->setParam(mString::parseInt(PARAM))->setCDParam(mString::parseInt(e->getCountdown())));
//			if (e->getAfterwards() == EFFECT_AFTERWARDS_GO) toRemove->add(e);
//			else if (e->getAfterwards() == EFFECT_AFTERWARDS_COUNTDOWN) if (e->getCountdown() == 1) toRemove->add(e);
//			if (e->getCountdown() > 0) e->decCountDown();
//		}
//	}
//	effectList->removeAll_delete(toRemove);
//	settleEffectsInQueue(whose);
//}
bool Referee::canIUseThisCard(Card* c, Role* me, Role* theOtherGuy)
{
	auto effects = c->getEffects();
	for (int i = 0; i < effects->size(); i++) {
		if (!reader->isEnabled(effects->get(i)->getCode(), me, theOtherGuy)) return false;
	}

	if (c->getConsume_type() == CONSUME_MANA1) {
		if (me->getMANA1() < c->getConsume()) return false;
	}
	else if (c->getConsume_type() == CONSUME_MANA2) {
		if (me->getMANA2() < c->getConsume()) return false;
	}
	return true;

}

void Referee::playCardEnds()
{

	int discardNum = getDiscardNum(player);
	if (discardNum > 0) {
		scene->setHint("请弃掉" + mString::parseInt(discardNum) + "张手牌");
	}
	else {
		scene_state = CARD_DISABLED;
		round_stage = END;
		roundEnds();
	}
	scene->refreshHandpState();

}

int Referee::getDiscardNum(Role* user) {
	int maxHandp =  user->getLevel()/6 + 4;
	if (user->getHandPile()->size() <= maxHandp) return 0;
	else return user->getHandPile()->size() - maxHandp;
}
void Referee::discardCard(Role* user, Card* c) {
	user->getHandPile()->remove(c, c);
	user->getDiscardPile()->add(c);
	if (user == player) {
		playCardEnds();
	}
}

void Referee::roundStarts()
{
	if (game_state != ONGOING) return;
	scene->setHint((host == player ? "你的" : "对方") + mString("回合开始"));
	scene_state = CARD_DISABLED;
	host->setMANA1(host->getMaxMANA1());
	host->setMANA2(host->getMaxMANA2());
	std::thread t1(&Referee::roundStartsBackground, this, 10, "start");
	t1.detach();
}

void Referee::roundStartsBackground(int x, std::string y) {
	triggerEffects(host == player ? PLAYER : ENEMY, 0, TRIGGER_ROUND_START);
	settleEffectsInQueue();
	auto callF = CallFunc::create([this]() {
		scene->refreshBoardInfo();
		scene->notifyEffectChanges();
		executeWithDelay(scene, function<void(void)>([this]() {roundDrawCard(); }), 0.4);
		});
	scene->runAction(callF);
}

void Referee::roundEndsBackground(int x, std::string y) {
	triggerEffects(host == player ? PLAYER : ENEMY, 0, TRIGGER_ROUND_END);
	settleEffectsInQueue();
	auto callF = CallFunc::create([=]() {
		scene->refreshBoardInfo();
		scene->notifyEffectChanges();
		Role* t = guest;
		guest = host;
		host = t;
		scene->refreshBoardInfo();
		if (host == enemy) {
			round++;
		}
		executeWithDelay(scene, function<void(void)>([this]() {roundStarts(); }), 0.4);
		});
	scene->runAction(callF);
}
void Referee::roundDrawCard()
{
	if (game_state != ONGOING) return;
	string s = host->getName() + "draw";
	//cocos2d::log("%s",s.c_str());
	host->drawCards(2);
	scene->setHint((host == player ? "轮到你" : "对方") + mString("抽牌"));
	if (host == player)	scene->notifyMHandCardChanged();
	else scene->notifyEHandCardChanged();
	;	scene_state = CARD_DISABLED;
	executeWithDelay(scene, function<void(void)>([this]() {roundPlayCard(); }), 0.2);
	//scene->refreshHandpState();
}

void Referee::roundPlayCard()
{
	if (game_state != ONGOING) return;
	if (host == player) {
		scene_state = ALLOW_USECARD;
		scene->setHint("轮到你出牌了");
	}
	else {
		scene->setHint("对方出牌");
		enemyAI->startPlayingCards();
	}
	scene->refreshHandpState();
}

void Referee::roundDiscardCard() {
	scene_state = DISCARD_CARDS;
	round_stage = DIS_CARD;
	if (host == player) playCardEnds();
	else {
		scene->setHint("敌方弃牌");
		enemyAI->startDiscardingCards();
	}
}


void Referee::roundEnds()
{
	if (game_state != ONGOING) return;
	scene->setHint((host == player ? "你的" : "对方") + mString("回合结束"));
	std::thread t1(&Referee::roundEndsBackground, this,10, "ends");
	t1.detach();
}

void Referee::countGame(WHO winner)
{
	game_state = COUNTING;
	scene_state = CARD_DISABLED;
	auto call = CallFunc::create([=]() {
		scene->popUpCountGameDialog(winner == PLAYER);
		});

	scene->runAction(call);
}





