#include "CardInfoWindow.h"
#pragma execution_character_set("utf-8")

CardInfoWindow::CardInfoWindow(Card* c,int w,int h)
{
	width = w;
	height = h;
	card = c;
	
}

CardInfoWindow* CardInfoWindow::create(Card* c,int width,int height)
{
	CardInfoWindow* bs = new CardInfoWindow(c,width,height);
	if (bs && bs->init()) {
		bs->autorelease();
		return bs;
	}
	else {
		delete bs;
		bs = nullptr;
		return nullptr;
	}
	return nullptr;
}

CardInfoWindow::CardInfoWindow(Effect* e, int width, int height)
{
	width = width;
	height = height;
	effect = e;
}

CardInfoWindow* CardInfoWindow::create(Effect* e, int width, int height)
{
	CardInfoWindow* bs = new CardInfoWindow(e, width, height);
	if (bs && bs->init()) {
		bs->autorelease();
		return bs;
	}
	else {
		delete bs;
		bs = nullptr;
		return nullptr;
	}
	return nullptr;
}

bool CardInfoWindow::init()
{
	if (card == nullptr&&effect==nullptr) return false;
	setContentSize(Size(width, height));
	auto linear = ui::Layout::create();
	linear->setLayoutType(ui::Layout::Type::VERTICAL);
	linear->setContentSize(Size(width, height));
	//linear->setPosition(Point(width / 2, height / 2));
	auto lp1 = ui::LinearLayoutParameter::create();
	auto lp0 = ui::LinearLayoutParameter::create();
	lp1->setMargin(ui::Margin(24,4,0,0));
	lp0->setMargin(ui::Margin(24, 24, 0, 0));
	auto name =ui::Text::create();
	name->setContentSize(Size(width,48));
	name->setFontSize(36);
	auto description = ui::Text::create();
	//description->setContentSize(Size(width, 108));
	description->setFontSize(28);
	description->setContentSize(Size(width-32, height - 48));
	description->ignoreContentAdaptWithSize(false);
	auto consume = ui::Text::create();
	consume->setContentSize(Size(width, 48));
	consume->setFontSize(28);

	auto bg = Sprite::create("card_info_window_bg.png");
	bg->setContentSize(Size(width, height));
	bg->setPosition(Point(width / 2, height / 2));
	linear->addChild(bg);

	mString nameS = card != nullptr ? card->getName() : effect->getName();
	mString descriptionS = card != nullptr ? card->getDescription() : effect->getDescription();
	name->setText(nameS.getStrWithMaxEms(width /18));
	if (card != nullptr)consume->setText((card->getConsume_type()==0?"San值消耗:":"肝值消耗:") + mString::parseInt(card->getConsume()));
	description->setText(descriptionS);
	name->ignoreContentAdaptWithSize(false);
	consume->ignoreContentAdaptWithSize(false);
	//description->ignoreContentAdaptWithSize(false);
	name->setLayoutParameter(lp0);
	description->setLayoutParameter(lp1);
	consume->setLayoutParameter(lp1);
	linear->addChild(name);
	if(card!=nullptr)linear->addChild(consume);
	linear->addChild(description);
	addChild(linear);
	return true;
}
