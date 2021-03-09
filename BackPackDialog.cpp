#include "BackPackDialog.h"
#include "Math.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

BackPackDialog::BackPackDialog()
{

}


BackPackDialog::~BackPackDialog()
{
}

BackPackDialog * BackPackDialog::create(DBHelper* dbhelper,Player* player)
{
	BackPackDialog* bs = new BackPackDialog;
	if (bs&&bs->init(dbhelper,player)) {
		bs->autorelease();
		return bs;
	}
	else {
		delete bs;
		bs = nullptr;
		return nullptr;
	}
}

bool BackPackDialog::init(DBHelper* dbh,Player* player)
{
	this->player = player;
	dbhelper = dbh;
	auto cache = TextureCache::sharedTextureCache();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Size s(visibleSize.width - DIALOG_MARGIN_WIDTH-64 , visibleSize.height - DIALOG_MARGIN_HEIGHT);
	setContentSize(visibleSize);
	itemBeans = new LinkedList<Card*>;
	block_list = new LinkedList<BackPackGridItem*>;
	sv = ScrollView::create();
	sv->setPosition(Vec2(DIALOG_MARGIN_WIDTH, 0));
	sv->setContentSize(Size(s.width-EXHIBI_WIDTH,s.height));
	//sv->addEventListenerScrollView(sv, nullptr);
	//sv->enableDpadNavigation(true);
	sv->addEventListener(CC_CALLBACK_2(BackPackDialog::onSVCallback, this));
	sv->addTouchEventListener(CC_CALLBACK_2(BackPackDialog::onSVTouch, this));
	mask = ui::Layout::create();
	mask->setContentSize(getContentSize());
	mask->setPosition(Point::ZERO);
	mask->setOpacity(0);
	mask->setBackGroundColor(Color3B::BLACK);
	mask->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	addChild(mask);
	auto bg = Sprite::createWithTexture(cache->getTextureForKey("backpack_bg"));
	bg->setPosition( Vec2(visibleSize.width / 2, visibleSize.height / 2));
	addChild(bg);
	outerLayout = Layout::create();
	outerLayout->setLayoutType(Layout::Type::HORIZONTAL);
	outerLayout->setContentSize(s);
	outerLayout->setPosition(Vec2(DIALOG_MARGIN_WIDTH/2, 0));
	exhibi_layout = Layout::create();
	exhibi_layout->setLayoutType(Layout::Type::VERTICAL);
	auto  lp0 = LinearLayoutParameter::create();
	lp0->setMargin(Margin(12,0,24,0));
	exhibi_layout->setContentSize(Size(EXHIBI_WIDTH, s.height));
	exhibi_layout->setLayoutParameter(lp0);
	auto exhibi_bg = Sprite::createWithTexture(cache->getTextureForKey("exhibi_bg"));
	exhibi_bg->setContentSize(Size(EXHIBI_WIDTH, s.height));
	exhibi_bg->setPosition(Point(EXHIBI_WIDTH/2, s.height/2));
	//exhibi_layout->setBackGroundImage("card_exhibi_bg.png",Widget::TextureResType::LOCAL);
	exhibi_layout->addChild(exhibi_bg, 0);
	exhibi_image = ImageView::create();
	exhibi_image->ignoreContentAdaptWithSize(false);
	exhibi_image->loadTexture("cardbg.png");
	exhibi_image->setContentSize(Size(CARD_WIDTH,CARD_HEIGHT));
	auto  lp = LinearLayoutParameter::create();
	lp->setMargin(Margin(16,24,2,2));
	auto lp2 = LinearLayoutParameter::create();
	lp2->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
	auto lp3 = LinearLayoutParameter::create();
	lp3->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
	lp3->setMargin(Margin(16,32,16,2));
	exhibi_image->setLayoutParameter(lp2);
	exhibi_name = Text::create();
	exhibi_name->setFontSize(39);
	exhibi_name->setContentSize(Size(EXHIBI_WIDTH, 48));
	exhibi_name->setLayoutParameter(lp3);
	exhibi_name->ignoreContentAdaptWithSize(false);
	exhibi_consume = Text::create();
	exhibi_consume->setTextColor(Color4B::YELLOW);
	exhibi_consume->setFontSize(28);
	exhibi_consume->setContentSize(Size(EXHIBI_WIDTH, 28));
	exhibi_consume->setLayoutParameter(lp);
	exhibi_consume->ignoreContentAdaptWithSize(false);
	exhibi_description = Text::create();
	exhibi_description->setFontSize(28);
	exhibi_description->setContentSize(Size(EXHIBI_WIDTH-48,108));
	exhibi_description->setLayoutParameter(lp);
	exhibi_description->ignoreContentAdaptWithSize(false);
	exhibi_layout->addChild(exhibi_name);
//	exhibi_layout->addChild(exhibi_image);
	exhibi_layout->addChild(exhibi_consume);
	exhibi_layout->addChild(exhibi_description);


	outerLayout->addChild(sv);
	outerLayout->addChild(exhibi_layout);



	//auto backBt = Button::create("close_normal.png",
	//	"close_selected.png", "close_normal.png",)

	auto close = Button::create("buttons/return_normal.png", "buttons/return_pressed.png", "buttons/return_pressed.png");
	close->setPosition(Point(s.width+90,s.height+100));
	close->addClickEventListener([=](Ref* r) {
		SimpleAudioEngine::getInstance()->playEffect("audio/ButtonBack.mp3");
		removeFromParentAndCleanup(true); });
	addChild(outerLayout, 1);
	addChild(close);
	initBlocks();


	return true;
}


void BackPackDialog::notifyDatasetChanged()
{
	std::thread t1(&BackPackDialog::loadItemsBackInground, this, 10, "load");
	t1.detach();
}

void BackPackDialog::loadItemsBackInground(int x,std::string y) {
	itemBeans->addAll(dbhelper->getCardsWithId(player->getPile()));
	auto cf = CallFunc::create([=]() {
		for (int i = 0; i < block_list->size(); i++) {
			BackPackGridItem* bgi = block_list->get(i);
			if (i < itemBeans->size()) {
				Card* cardRes = itemBeans->get(i);
				bgi->setUpWithCardRes(cardRes);
			}
			else bgi->setUpWithNone();
		}
		selectItem(0);
		});
	runAction(cf);
}



void BackPackDialog::onSVCallback(Ref *, ui::ScrollView::EventType type)
{
}

void BackPackDialog::onSVTouch(Ref * object, ui::Widget::TouchEventType type)
{
}

void BackPackDialog::selectItem(int index)
{
	if (itemBeans->size()==0) return;
	selectedIndex = index;
	for (int i = 0; i < block_list->size(); i++) {
		block_list->get(i)->setChecked(i==index);
	}
	refreshExhibi();
}

void BackPackDialog::selectItem(BackPackGridItem * block)
{
	int index = 0;
	int size = block_list->size();
	for (int i = 0; i <size; i++ ) {
		BackPackGridItem* bpg = block_list->get(i);
		if (bpg == block) {
			index = i;
			bpg->setChecked(true);
		}
		else bpg->setChecked(false);
	}
	selectedIndex = index;
	refreshExhibi();
}
void BackPackDialog::selectItem(CardView* cv)
{
	int index = 0;
	int size = block_list->size();
	for (int i = 0; i < size; i++) {
		BackPackGridItem* bpg = block_list->get(i);
		if (bpg->getCardView()==cv) {
			index = i;
			bpg->setChecked(true);
		}
		else bpg->setChecked(false);
	}
	selectedIndex = index;
	refreshExhibi();
}
void BackPackDialog::refreshExhibi()
{
	Card* selected = itemBeans->size() == 0?nullptr:itemBeans->get(selectedIndex);
	if (selected != nullptr) {
		exhibi_name->setText(selected->getName());
		exhibi_description->setText(selected->getDescription());
		exhibi_consume->setText("消耗"+ selected->getConsume()==0?"行动点：":"法力：" +mString::parseInt(selected->getConsume()));

		auto effs = selected->getEffects();

		auto children = exhibi_layout->getChildren();
		for (int i = 0; i < children.size(); i++) {
			auto x = children.at(i);
			if (x->getTag() == 5) exhibi_layout->removeChild(x);
		}
		for (int i = 0; i <effs ->size(); i++) {
			mString x = mString::parseInt(i + 1) + "." + effs->get(i)->getName();
			auto lt = Text::create();
			LinearLayoutParameter* lp = LinearLayoutParameter::create();
			lp->setMargin(Margin(16, 1, 1, 1));
			lt->setFontSize(28);
			lt->setTextColor(Color4B::GREEN);
			lt->setText(x);
			lt->setContentSize(Size(EXHIBI_WIDTH-32, 32));
			lt->setLayoutParameter(lp);
			lt->ignoreContentAdaptWithSize(false);
			lt->setTag(5);
			exhibi_layout->addChild(lt);
			//log("add_eff:%d", effs->get(i)->getId());
		}
	}
}
void BackPackDialog::onEnter()
{
	Layer::onEnter();
	mask->runAction(FadeTo::create(0.3, 180));
	notifyDatasetChanged();
}
void BackPackDialog::initBlocks()
{
	int capacity = capacity_limit;
	int size = itemBeans->size();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	int width = visibleSize.width;//滚动层的宽
	int height = visibleSize.height;//滚动层的高
	int padding = 6;
	int count_per_row = 4;//每行的个数
	float  row_heigth = BLOCK_HEIGHT+padding;
	float column_width = BLOCK_WIDTH + 2 * padding;
	//计算行数
	float total_row = capacity / count_per_row+(capacity%count_per_row>0?1:0);
	//计算内容区的高度
	int container_hight = total_row * row_heigth;
	container_hight = (container_hight > height) ? container_hight : height;
	sv->setInnerContainerSize(Size(width, container_hight));
	for (int idx = 0; idx < capacity; idx++)
	{
		int column = idx % count_per_row;
		int row = idx / count_per_row;
		BackPackGridItem* item = BackPackGridItem::create(this);
		item->setContentSize(Size(column_width - padding / 2, row_heigth));
		//计算并设置位置
		int x = column_width * (column)+ (column_width - padding / 2)/4;
		int y = container_hight - (row+1)*(row_heigth)+row_heigth/2;
		//cocos2d::log("x:%d,y%d", x, y);
		item->setPosition(x, y);
		sv->addChild(item);
		block_list->add(item);
	}
}


