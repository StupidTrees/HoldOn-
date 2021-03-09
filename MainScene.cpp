#include "MainScene.h"
#include "BattleScene.h"
#include "BackPackDialog.h"
#include "Card.h"
#include "String.h"
#include "ShopDialog.h"
#include "GamemapScene.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;
MainScene::MainScene()
{

}


MainScene::~MainScene()
{
}

MainScene * MainScene::create()
{
	MainScene* ms = new MainScene;
	if (ms&&ms->init()) {
		ms->autorelease();
		return ms;
	}
	else {
		delete ms;
		ms = nullptr;
		return nullptr;
	}
}


bool MainScene::init()
{
	//loadResources();
	if (!Scene::init()) return false;
	player = Player::getInstanceFromDisk();
	dbhelper = new DBHelper();
	//获取坐标系界面的屏幕可见区域  
	Director *director = Director::getInstance();
	Size visibleSize = director->getVisibleSize();
	//原点位置  
	auto bg = Sprite::create("main_bg.jpg");
	bg->setContentSize(visibleSize);
	bg->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	addChild(bg);

	Point origin = director->getVisibleOrigin();
	auto item1 = MenuItemImage::create("buttons/startgame_normal.png", "buttons/startgame_pressed.png", "buttons/startgame_pressed.png", CC_CALLBACK_1(MainScene::onPushSceneOne, this));
	auto item2 = MenuItemImage::create("buttons/backpack_normal.png", "buttons/backpack_pressed.png", "buttons/backpack_pressed.png", CC_CALLBACK_1(MainScene::onPushSceneTwo, this));
	auto item3 = MenuItemImage::create("buttons/shop_normal.png", "buttons/shop_pressed.png", "buttons/shop_pressed.png", CC_CALLBACK_1(MainScene::onPushSceneThree, this));
	auto item4 = MenuItemImage::create("buttons/quitgame_normal.png", "buttons/quitgame_pressed.png", "buttons/quitgame_pressed.png", CC_CALLBACK_1(MainScene::menuCloseCallback, this));
    menu_oper = Menu::create(item1, item2,item3,item4,NULL);
	menu_oper->alignItemsVerticallyWithPadding(24);
	menu_oper->setPositionX(1280);
	menu_oper->setVisible(false);
	addChild(menu_oper, 1);
	initTopDashBoard(visibleSize);
	refreshDashBoard();

	return true;
}

void MainScene::loadResources() {
	//auto loading = Sprite::create("main_bg.jpg");
	//loading->setContentSize(getContentSize());
	//loading->setPosition(Point(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));
//	addChild(loading, getChildrenCount() - 1,"loading");
	
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("audio/BattleScene.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("audio/ButtonBack.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("audio/BattleWin.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("audio/enter.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("audio/fail.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("audio/hurt.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("audio/useCard.wav");

	FileUtils::getInstance()->addSearchPath("");
	SpriteFrameCache* frameCache = SpriteFrameCache::sharedSpriteFrameCache();
	frameCache->addSpriteFramesWithFile("status_icons.plist");
	Vector<SpriteFrame*> animations;
	char str[100] = { 0 };
	for (int i = 0; i <= 29; i++)
	{
		sprintf(str, "magicbuff_000%02d.png", i);
		SpriteFrame* frame = frameCache->spriteFrameByName(str);
		animations.pushBack(frame);
	}
	auto anim_warmth = Animation::createWithSpriteFrames(animations, 0.1f, 1);
	AnimationCache::sharedAnimationCache()->addAnimation(anim_warmth, "magic_buff");


	frameCache->addSpriteFramesWithFile("impactFx.plist");
	animations.clear();
	char str1[100] = { 0 };
	for (int i = 1; i <= 7; i++)
	{
		sprintf(str1, "BladeDancer_TD_testImpactFx_000%d.png", i);
		SpriteFrame* frame = frameCache->spriteFrameByName(str1);
		//frame->setOriginalSize(Size(500, 500));
		animations.pushBack(frame);
	}
	auto anim_hurt = Animation::createWithSpriteFrames(animations, 0.1f, 1);
	AnimationCache::sharedAnimationCache()->addAnimation(anim_hurt, "hurt");


	frameCache->addSpriteFramesWithFile("healFx.plist");
	animations.clear();
	char str2[100] = { 0 };
	for (int i = 1; i <= 9; i++)
	{
		sprintf(str2, "fx_heal_0%d.png", i);
		SpriteFrame* frame = frameCache->spriteFrameByName(str2);
		if (frame != nullptr)animations.pushBack(frame);
	}
	auto anim_heal = Animation::createWithSpriteFrames(animations, 0.1f, 1);
	AnimationCache::sharedAnimationCache()->addAnimation(anim_heal, "heal");

	frameCache->addSpriteFramesWithFile("generic_glow.plist");
	animations.clear();
	char str4[100] = { 0 };
	for (int i = 1; i <= 9; i++)
	{
		sprintf(str4, "generic_glow_front_0%d.png", i);
		SpriteFrame* frame = frameCache->spriteFrameByName(str4);
		if (frame != nullptr)animations.pushBack(frame);
	}
	auto anim_effect_front = Animation::createWithSpriteFrames(animations, 0.1f, 1);
	AnimationCache::sharedAnimationCache()->addAnimation(anim_effect_front, "effect_front");

	auto texture_cache = TextureCache::getInstance();
	auto img1 = new Image;
	img1->initWithImageFile("win_bg.png");
	auto img2 = new Image;
	img2->initWithImageFile("lose_bg.png");
	auto img3 = new Image;
	img3->initWithImageFile("cardbg_back.png");
	auto img4 = new Image;
	img4->initWithImageFile("card_frame/card_frame_attack.png");
	auto img5 = new Image;
	img5->initWithImageFile("card_frame/card_frame_recover.png");
	auto img6 = new Image;
	img6->initWithImageFile("card_frame/card_frame_magic.png");
	auto img7 = new Image;
	img7->initWithImageFile("card_frame/card_frame_function.png");
	auto img8 = new Image;
	img8->initWithImageFile("battle_bg.png");
	auto img9 = new Image;
	img9->initWithImageFile("handp_bg.png");
	auto img10 = new Image;
	img10->initWithImageFile("battle_start_text.png");
	auto img11 = new Image;
	img11->initWithImageFile("dialog_bg.png");
	auto img12 = new Image;
	img12->initWithImageFile("backpack_dialog_bg.png");
	auto img13 = new Image;
	img13->initWithImageFile("card_exhibi_bg.png");
	auto img14 = new Image;
	img14->initWithImageFile("shop_dialog_bg.png");
	auto img15 = new Image;
	img15->initWithImageFile("top_board_bg.png");
	texture_cache->addImage(img1, "win_bg");
	texture_cache->addImage(img2, "lose_bg");
	texture_cache->addImage(img3, "cardbg_back");
	texture_cache->addImage(img4, "cardframe_attack");
	texture_cache->addImage(img5, "cardframe_recover");
	texture_cache->addImage(img6, "cardframe_magic");
	texture_cache->addImage(img7, "cardframe_function");
	texture_cache->addImage(img8, "battle_bg");
	texture_cache->addImage(img9, "handp_bg");
	texture_cache->addImage(img10, "battle_start_text");
	texture_cache->addImage(img11, "gamemap_bg");
	texture_cache->addImage(img12, "backpack_bg");
	texture_cache->addImage(img13, "exhibi_bg");
	texture_cache->addImage(img14, "shop_dialog_bg");
	texture_cache->addImage(img15, "top_board");

	auto cf = CallFuncN::create([=](Node* n) {
		menu_oper->setVisible(true);
		});
	runAction(cf);
	//removeChildByName("loading", true);
}

void MainScene::initTopDashBoard(Size visibleSize) {
	auto bg = Sprite::create("top_board_bg.png");
	bg->setPosition(Point(visibleSize.width/2, visibleSize.height-TOP_DASHBOARD_HEIGHT));
	//bg->setContentSize(Size(visibleSize.width, TOP_DASHBOARD_HEIGHT));
	player_exp = Text::create("","",36);
	player_level = Text::create("", "", 36);
	player_name = Text::create("", "", 36);
	player_coins = Text::create("", "", 36);
	auto coinIcon = Sprite::create("coins.png");
	player_name->setPosition(Point(340, visibleSize.height-40));
	player_exp->setPosition(Point(980, visibleSize.height-40));
	player_level ->setPosition(Point(690, visibleSize.height-40));
	player_coins->setPosition(Point(visibleSize.width-200, visibleSize.height -40));
	coinIcon->setPosition(Point(visibleSize.width - 254, visibleSize.height - 40));
	auto playerAvatar = Sprite::create("player.jpg");
	playerAvatar->setContentSize(Size(60, 60));
	playerAvatar->setPosition(Point(80, visibleSize.height - 40));
	addChild(playerAvatar);
	addChild(bg);
	addChild(player_name);
	addChild(player_exp);
	addChild(player_level);
	addChild(player_coins);
	addChild(coinIcon);
}
void MainScene::onPushSceneOne(Ref* pSender) {
	SimpleAudioEngine::getInstance()->playEffect("audio/ButtonBack.mp3");
	auto layer = GamemapScene::create(dbhelper,player);
	layer->setSwallowsTouches(true);
	addChild(layer,getChildrenCount()-1);
}
void MainScene::onPushSceneTwo(Ref* pSender) {
	SimpleAudioEngine::getInstance()->playEffect("audio/ButtonBack.mp3");
	auto layer = BackPackDialog::create(dbhelper,player);
	addChild(layer,getChildrenCount()-1);
}
void MainScene::onPushSceneThree(Ref* pSender) {
	SimpleAudioEngine::getInstance()->playEffect("audio/ButtonBack.mp3");
	auto layer = ShopDialog::create(this,dbhelper, player);
	addChild(layer, getChildrenCount() - 1);
}
void MainScene::refreshDashBoard() {
	stringstream curExp,level,coins,totExp;
	curExp << player->getCurExp();
	level << player->getLevel();
	coins << player->getCoins();
	totExp << player->getTotExp();
	player_exp->setString("经验：" +curExp.str()+"/"+totExp.str());
	player_level->setString("等级："+level.str());
	player_name->setString("玩家："+player->getName());
	player_coins->setString(mString::parseInt(player->getCoins()));
}
 
/**
 * 关闭视窗的方法
 * 下面是官方定义好的一个宏，意思就是，当平台为wp8或者winRT会弹出框
 */
void MainScene::menuCloseCallback(Ref* pSender)
{
	//player->setCoins(500);
	//player->getPile()->clear();
	//player->setCp1(1);
	//player->setCp2(2);
	//player->setCp3(3);
	//player->setLevel(1);
	//player->setName("哈深小白");
	//player->saveObjectToDisk();
	
	SimpleAudioEngine::getInstance()->playEffect("audio/ButtonBack.mp3");
	Director::getInstance()->end();
}

void MainScene::onEnter()
{
	Scene::onEnter();
	SimpleAudioEngine::getInstance()->stopBackgroundMusic("audio/BattleScene.wav");
	refreshDashBoard();
	loadResources();
}


void MainScene::onEnterTransitionDidFinish() {
	Scene::onEnterTransitionDidFinish();

}