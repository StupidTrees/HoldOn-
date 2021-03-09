
#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__
#include "cocos2d.h"
#include "Player.h"
#include "ui\UILayout.h"
#include "ui\UIText.h"
#include "DBHelper.h"
USING_NS_CC;
using namespace ui;

/*主菜单界面*/
class MainScene :
	public cocos2d::Scene
{
public:
	MainScene();
	~MainScene();
	static MainScene* create();
	bool init();

	void loadResources();

	void initTopDashBoard(Size visibleSize); //初始化顶部玩家信息UI
	void refreshDashBoard(); //刷新顶部玩家信息
	/*四个按钮的监听事件*/
	void onPushSceneOne(Ref * pSender);
	void onPushSceneTwo(Ref * pSender);
	void onPushSceneThree(Ref* pSender);
	void menuCloseCallback(Ref* pSender);



	void onEnter() override; //进入时调用
	void onEnterTransitionDidFinish() override;
private:
	Menu* menu_oper;
	Text* player_level;
	Text* player_exp;
	Text* player_name;
	Text* player_coins;
	Player* player;
	DBHelper* dbhelper; //全局使用的数据库操作对象


};
#endif  __MAIN_SCENE_H__


