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

	auto listener = cocos2d::EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event) {
		if (!playerMovingToTarget)
			playerSpine->setAnimation(0, "move", true);

		auto pos = touch->getLocation();
		playerMoveTarget = pos;//convertToNodeSpace(pos);
		playerMovingToTarget = true;
		return true; 
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

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
