#pragma once
#include "cocos2d.h"
#include "ui\UIImageView.h"
#include "ui\UIText.h"
#include "CardView.h"


using namespace ui;
USING_NS_CC;

class BackPackDialog;
class BackPackGridItem:
	public Node
{
public:
	BackPackGridItem(BackPackDialog* parent);
	~BackPackGridItem();
	bool init();
	void setUpWithCardRes(Card * c);
	void setUpWithNone();
	void setChecked(bool check);
	bool isChecked();
	CardView* getCardView();
	static BackPackGridItem * create(BackPackDialog* root);
private:
	Sprite* bg;
	CardView* cardView;
	BackPackDialog* parent;
	bool checked;
};

