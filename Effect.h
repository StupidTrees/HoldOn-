#pragma once
#include "mString.h"
#include "cocos2d.h"

class Role;
using namespace std;
/*效果类*/
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
	/* 固定属性 */
	int id; //唯一标识符
	mString name; //名字
	mString description; //描述语
	int type; //类别
	int trigger; //触发条件
	mString code; //执行操作代码
	mString rawCode;//初始代码
	int afterwards; //执行后动作
	bool mergeAble; //是否支持叠加
	int life; //生命：永久/单次/延时
	int countDown; //若为延时牌：倒计时
	int cdTrigger;

	/*游戏中参数*/
	Role* owner;//是谁使用的
};

