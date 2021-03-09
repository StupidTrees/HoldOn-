
#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__
#include "cocos2d.h"
#include "Player.h"
#include "ui\UILayout.h"
#include "ui\UIText.h"
#include "DBHelper.h"
USING_NS_CC;
using namespace ui;

/*���˵�����*/
class MainScene :
	public cocos2d::Scene
{
public:
	MainScene();
	~MainScene();
	static MainScene* create();
	bool init();

	void loadResources();

	void initTopDashBoard(Size visibleSize); //��ʼ�����������ϢUI
	void refreshDashBoard(); //ˢ�¶��������Ϣ
	/*�ĸ���ť�ļ����¼�*/
	void onPushSceneOne(Ref * pSender);
	void onPushSceneTwo(Ref * pSender);
	void onPushSceneThree(Ref* pSender);
	void menuCloseCallback(Ref* pSender);



	void onEnter() override; //����ʱ����
	void onEnterTransitionDidFinish() override;
private:
	Menu* menu_oper;
	Text* player_level;
	Text* player_exp;
	Text* player_name;
	Text* player_coins;
	Player* player;
	DBHelper* dbhelper; //ȫ��ʹ�õ����ݿ��������


};
#endif  __MAIN_SCENE_H__


