#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "ui/UIButton.h"

USING_NS_CC;

bool Character::init() {
	spine = spine::SkeletonAnimation::createWithJsonFile("assets/hero_2.json", "assets/hero_2.atlas", 3.0f);
	spine->setNormalizedPosition({ 0.5f, 0.5f });
	spine->setAnimation(0, "idle", true);
		
	addChild(spine);

	return true;
}

void Character::setState(State value) {
	if (state == value)
		return;
	
	state = value;

	if (state == State::None)
		return;

	static const std::unordered_map<State, std::string> StateAnims = {
		{ State::Attack, "attack" },
		{ State::Idle, "idle" },
		{ State::Move, "move" },
	};

	auto anim = spine->setAnimation(0, StateAnims.at(state), true);
	anim->mixDuration = 0.25f;
}

Character::State Character::getState() const {
	return state;
}

void Character::setMoveTarget(const cocos2d::Vec2 value) {
	moveTarget = value;
}

const cocos2d::Vec2& Character::getMoveTarget() const {
	return moveTarget;
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
		player->setMoveTarget(touch->getLocation());
		player->setState(Character::State::Move);
		return true; 
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	auto attackButton = createButton("attack", [this] {
		player->setState(Character::State::Attack);
	});
	attackButton->setAnchorPoint({ 0.0f, 0.0f });
	attackButton->setPosition({ 8.0f, 8.0f });
	addChild(attackButton);

	auto stopButton = createButton("stop", [this] {
		player->setState(Character::State::Idle);
	});
	stopButton->setAnchorPoint({ 0.0f, 0.0f });
	stopButton->setPosition({ 8.0f, 108.0f });
	addChild(stopButton);

	player = Character::create();
	player->setNormalizedPosition({ 0.5f, 0.5f });
	addChild(player);

	scheduleUpdate();

	return true;
}

void HelloWorld::update(float delta)
{
	cocos2d::Scene::update(delta);

	constexpr float playerSpeed = 300.0f;

	if (player->getState() == Character::State::Move)
	{
		auto pos = player->getPosition();

		auto direction = player->getMoveTarget();
		direction.subtract(pos);
		direction.normalize();

		auto nextPos = pos + (direction * delta * playerSpeed);

		player->setScaleX(direction.x > 0.0f ? -1.0f : 1.0f);

		if (pos.distance(nextPos) < pos.distance(player->getMoveTarget()))
		{
			player->setPosition(nextPos);
		}
		else
		{
			player->setPosition(player->getMoveTarget());
			player->setState(Character::State::Idle);
		}
	}
}
