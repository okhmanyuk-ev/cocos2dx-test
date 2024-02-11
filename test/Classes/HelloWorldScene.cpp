#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "ui/UIButton.h"

USING_NS_CC;

HelloWorld::HelloWorld() :
	playerMoveTarget(0.0f, 0.0f),
	playerMovingToTarget(false),
	state(State::None)
{
}

Scene* HelloWorld::createScene()
{
	return HelloWorld::create();
}

cocos2d::Node* HelloWorld::createButton(const std::string& title, std::function<void()> onClick)
{
	auto button = ui::Button::create("assets/button.png");
	assert(button);
	button->addClickEventListener([onClick](auto ref) {
		onClick();
	});
	button->setTitleFontName("fonts/Marker Felt.ttf");
	button->setTitleText(title);
	button->setTitleFontSize(32.0f);
	return button;
}

bool HelloWorld::init()
{
	if (!Scene::init())
		return false;

	/*auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
										   "CloseNormal.png",
										   "CloseSelected.png",
										   CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

	if (closeItem == nullptr ||
		closeItem->getContentSize().width <= 0 ||
		closeItem->getContentSize().height <= 0)
	{
		problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
	}
	else
	{
		float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
		float y = origin.y + closeItem->getContentSize().height/2;
		closeItem->setPosition(Vec2(x,y));
	}*/

	// create menu, it's an autorelease object
	/*auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
	*/
	/////////////////////////////
	// 3. add your codes below...

	// add a label shows "Hello World"
	// create and initialize a label

	/*auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
	if (label == nullptr)
	{
		problemLoading("'fonts/Marker Felt.ttf'");
	}
	else
	{
		// position the label on the center of the screen
		label->setPosition(Vec2(origin.x + visibleSize.width/2,
								origin.y + visibleSize.height - label->getContentSize().height));

		// add the label as a child to this layer
		this->addChild(label, 1);
	}*/

	// add "HelloWorld" splash screen"
	/*auto sprite = Sprite::create("HelloWorld.png");
	if (sprite == nullptr)
	{
		problemLoading("'HelloWorld.png'");
	}
	else
	{
		// position the sprite on the center of the screen
		sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

		// add the sprite as a child to this layer
		this->addChild(sprite, 0);
	}*/

	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseDown = [this](EventMouse* event) {
		if (!playerMovingToTarget)
			playerSpine->setAnimation(0, "move", true);
	
		auto x = event->getCursorX();
		auto y = event->getCursorY();
		playerMoveTarget = convertToNodeSpace({ x, y });
		playerMovingToTarget = true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

	auto attackButton = createButton("attack", [this] {
		playerMovingToTarget = false;
		playerSpine->setAnimation(0, "attack", true);
	});
	attackButton->setNormalizedPosition({ 0.75f, 0.75f });
	addChild(attackButton);

	auto stopButton = createButton("stop", [this] {
		playerMovingToTarget = false;
		playerSpine->setAnimation(0, "idle", true);
	});
	stopButton->setNormalizedPosition({ 0.75f, 0.6f });
	addChild(stopButton);

	playerSpine = spine::SkeletonAnimation::createWithJsonFile("assets/hero_2.json", "assets/hero_2.atlas", 3.0f);
	playerSpine->setNormalizedPosition({ 0.5f, 0.5f });
	
	auto animsCount = playerSpine->getSkeleton()->data->animationsCount;
	std::vector<std::string> animNames;

	for (int i = 0; i < animsCount; i++)
	{
		auto name = playerSpine->getSkeleton()->data->animations[i]->name;
		animNames.push_back(name);
	}

	addChild(playerSpine);

	playerSpine->setAnimation(0, "attack", true);

	playerSpine->setMix("idle", "move", 0.25f);
	playerSpine->setMix("move", "idle", 0.25f);

	scheduleUpdate();

	return true;
}

void HelloWorld::update(float delta)
{
	cocos2d::Scene::update(delta);

	constexpr float playerSpeed = 200.0f;

	if (playerMovingToTarget)
	{
		auto pos = playerSpine->getPosition();

		auto direction = playerMoveTarget;
		direction.subtract(pos);
		direction.normalize();

		auto nextPos = pos + (direction * delta * playerSpeed);

		playerSpine->setScaleX(direction.x > 0.0f ? -1.0f : 1.0f);

		if (pos.distance(nextPos) < pos.distance(playerMoveTarget))
		{
			playerSpine->setPosition(nextPos);
		}
		else
		{
			playerSpine->setPosition(playerMoveTarget);
			playerMovingToTarget = false;
			playerSpine->setAnimation(0, "idle", true);
		}
	}
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);


}
