#pragma once
#include "cocos2d.h"
#include "Referee.h"
#include "Player.h"
#include "ui/UIButton.h"
#include "ui/UILayout.h"
#include "Checkpoint.h"
#include "DBHelper.h"
using namespace cocos2d;
class CountGameDialog :
	public Layer
{
public:
	static CountGameDialog* create(bool win, Referee* r, Player* player,Checkpoint* checkpoint,DBHelper*dbhelper);
	CountGameDialog(bool win, Referee* r, Player* p,Checkpoint* u, DBHelper* dbhelper);
	bool init();
	void fadeIn();

private:
	Referee* referee;
	Player* player;
	Checkpoint* checkpoint;
	bool win;
	ui::Layout* bg;
	Sprite* scrollBG;
	ui::Button* bt;
	LabelTTF* text;
	DBHelper* dbhelper;

};

